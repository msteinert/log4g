/* Copyright 2010, 2011 Michael Steinert
 * This file is part of Log4g.
 *
 * Log4g is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * Log4g is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Log4g. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * SECTION: pattern-parser
 * @short_description: Parse pattern layout conversion patterns
 * @see_also: #Log4gPatternLayoutClass, #Log4gPatternConverterClass
 *
 * This class performs most of the work done by the pattern layout class.
 * The conversion pattern is parsed and a chained list of pattern converters
 * is created.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "helpers/pattern-parser.h"
#include "log4g/layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gPatternParser, log4g_pattern_parser, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_PARSER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gPatternParser *)instance)->priv)

#define ESCAPE_CHAR '%'

struct Private {
	GString *buffer;
	struct Log4gFormattingInfo formatting;
	gchar *pattern;
	gint length;
	gint i;
	Log4gPatternConverter *head;
	Log4gPatternConverter *tail;
	gint state;
};

enum PatternParserState {
	LITERAL_STATE = 0,
	CONVERTER_STATE,
	DOT_STATE,
	MIN_STATE,
	MAX_STATE
};

static void
formatting_info_reset_(struct Log4gFormattingInfo *info)
{
	info->min = -1;
	info->max = 0x7fffffff;
	info->align = FALSE;
}

static void
log4g_pattern_parser_init(Log4gPatternParser *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->buffer = g_string_sized_new(32);
	formatting_info_reset_(&priv->formatting);
	priv->length = 0;
	priv->i = 0;
	priv->head = priv->tail = NULL;
	priv->state = LITERAL_STATE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->head) {
		g_object_unref(priv->head);
		priv->head = priv->tail = NULL;
	}
	G_OBJECT_CLASS(log4g_pattern_parser_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->buffer) {
		g_string_free(priv->buffer, TRUE);
	}
	g_free(priv->pattern);
	G_OBJECT_CLASS(log4g_pattern_parser_parent_class)->finalize(base);
}

static void
log4g_pattern_parser_class_init(Log4gPatternParserClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

static void
log4g_pattern_parser_class_finalize(Log4gPatternParserClass *klass)
{
	/* do nothing */
}

void
log4g_pattern_parser_register(GTypeModule *module)
{
	log4g_pattern_parser_register_type(module);
}
    
/**
 * log4g_pattern_parser_new:
 * @pattern: The conversion pattern to parse.
 *
 * Create a new pattern parser object.
 *
 * Returns: A new pattern parser object.
 * Since: 0.1
 */
Log4gPatternParser *
log4g_pattern_parser_new(const gchar *pattern)
{
	Log4gPatternParser *self =
		g_object_new(LOG4G_TYPE_PATTERN_PARSER, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	priv->pattern = g_strdup(pattern);
	if (!priv->pattern) {
		g_object_unref(self);
		return NULL;
	}
	priv->length = strlen(pattern);
	return self;
}

/**
 * log4g_pattern_parser_add_to_list:
 * @self: A pattern converter object.
 * @pc: The pattern converter to add to @self.
 *
 * Add a pattern converter to the current list.
 *
 * Since: 0.1
 */
void
log4g_pattern_parser_add_to_list(Log4gPatternParser *self,
		Log4gPatternConverter *pc)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->head) {
		priv->head = priv->tail = g_object_ref(pc);
	} else {
		log4g_pattern_converter_set_next(priv->tail, pc);
		priv->tail = pc;
	}
}

/**
 * log4g_pattern_parser_extract_option:
 * @self: A pattern converter object.
 *
 * Extract a braced conversion pattern option.
 *
 * Returns: A conversion pattern option or %NULL if none was found. The
 *          caller must free the returned string with g_free().
 * Since: 0.1
 */
gchar *
log4g_pattern_parser_extract_option(Log4gPatternParser *self)
{
	gchar *option = NULL;
	struct Private *priv = GET_PRIVATE(self);
	if ((priv->i < priv->length) && ('{' == priv->pattern[priv->i])) {
		gint end = 0;
		while (priv->pattern[++end + priv->i]) {
			if ('}' == priv->pattern[end + priv->i]) {
				break;
			}
		}
		if (end) {
			option = g_strndup(&priv->pattern[priv->i + 1],
					end - 1);
		}
		priv->i += (end + 1);
	}
	return option;
}

/**
 * log4g_pattern_parser_extract_precision_option:
 * @self: A pattern converter object.
 *
 * Extract a precision option from a conversion pattern.
 *
 * Returns: The precision value or zero if none was found.
 * Since: 0.1
 */
gint
log4g_pattern_parser_extract_precision_option(Log4gPatternParser *self)
{
	glong r = 0;
	gchar *option = log4g_pattern_parser_extract_option(self);
	if (option) {
		errno = 0;
		r = g_ascii_strtoll(option, NULL, 10);
		if (errno) {
			log4g_log_error(Q_("category option \"%s\" is not "
					"a decimal number: %s"),
					option, g_strerror(errno));
		}
		if (r <= 0) {
			log4g_log_error(Q_("precision option (%s) is not "
					"a positive integer"), option);
			r = 0;
		}
		g_free(option);
	}
	return (gint)r;
}

/**
 * log4g_pattern_parser_parse:
 * @self: A pattern converter object.
 *
 * Parse the conversion pattern.
 *
 * Returns: A chained list of pattern converters or %NULL if none were created.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_pattern_parser_parse(Log4gPatternParser *self)
{
	struct Private *priv = GET_PRIVATE(self);
	Log4gPatternConverter *pc;
	priv->i = 0;
	if (priv->head) {
		g_object_unref(priv->head);
		priv->head = NULL;
	}
	while (priv->i < priv->length) {
		gchar c = priv->pattern[priv->i++];
		switch (priv->state) {
		case LITERAL_STATE:
			if (priv->i == priv->length) {
				g_string_append_c(priv->buffer, c);
				continue;
			}
			if (ESCAPE_CHAR == c) {
				switch (priv->pattern[priv->i]) {
				case ESCAPE_CHAR:
					g_string_append_c(priv->buffer, c);
					++priv->i;
					break;
				case 'n':
					g_string_append(priv->buffer,
							LOG4G_LAYOUT_LINE_SEP);
					++priv->i;
					break;
				default:
					if (priv->buffer->len != 0) {
						pc = log4g_literal_pattern_converter_new(
								priv->buffer->str);
						if (pc) {
							log4g_pattern_parser_add_to_list(self, pc);
							g_object_unref(pc);
							pc = NULL;
						}
					}
					g_string_set_size(priv->buffer, 0);
					g_string_append_c(priv->buffer, c);
					priv->state = CONVERTER_STATE;
					formatting_info_reset_(&priv->formatting);
					break;
				}
			} else {
				g_string_append_c(priv->buffer, c);
			}
			break;
		case CONVERTER_STATE:
			g_string_append_c(priv->buffer, c);
			switch (c) {
			case '-':
				priv->formatting.align = TRUE;
				break;
			case '.':
				priv->state = DOT_STATE;
				break;
			default:
				if (c >= '0' && c <= '9') {
					priv->formatting.min = c - '0';
					priv->state = MIN_STATE;
				} else {
					log4g_pattern_parser_finalize_converter(
							self, c);
				}
				break;
			}
			break;
		case MIN_STATE:
			g_string_append_c(priv->buffer, c);
			if (c >= '0' && c <= '9') {
				priv->formatting.min =
					priv->formatting.min * 10 + (c - '0');
			} else if ('.' == c) {
				priv->state = DOT_STATE;
			} else {
				log4g_pattern_parser_finalize_converter(self,
						c);
			}
			break;
		case DOT_STATE:
			g_string_append_c(priv->buffer, c);
			if (c >= '0' && c <= '9') {
				priv->formatting.max = c - '0';
				priv->state = MAX_STATE;
			} else {
				log4g_log_error(Q_("error occurred in "
						"position %d\nwas expecting "
						"digit, instead got char %c"),
						priv->i, c);
				priv->state = LITERAL_STATE;
			}
			break;
		case MAX_STATE:
			g_string_append_c(priv->buffer, c);
			if (c >= '0' && c <= '9') {
				priv->formatting.max =
					priv->formatting.max * 10 + (c - '0');
			} else {
				log4g_pattern_parser_finalize_converter(self,
						c);
				priv->state = LITERAL_STATE;
			}
			break;
		} /* switch */
	} /* while */
	if (priv->buffer->len != 0) {
		pc = log4g_literal_pattern_converter_new(priv->buffer->str);
		if (pc) {
			log4g_pattern_parser_add_to_list(self, pc);
			g_object_unref(pc);
			pc = NULL;
		}
	}
	pc = priv->head;
	priv->head = NULL;
	return pc; /* caller owns return value */
}

/**
 * log4g_pattern_parser_finalize_converter:
 * @self: A pattern converter object.
 * @c: The current conversion character.
 *
 * Finalize the conversion pattern being parsed.
 *
 * Since: 0.1
 */
void
log4g_pattern_parser_finalize_converter(Log4gPatternParser *self, gchar c)
{
	struct Private *priv = GET_PRIVATE(self);
	Log4gPatternConverter *pc = NULL;
	switch (c) {
	case 'c':
		pc = log4g_category_pattern_converter_new(&priv->formatting,
				log4g_pattern_parser_extract_precision_option(
					self));
		break;
	case 'd': {
		char *format = log4g_pattern_parser_extract_option(self);
		if (!format) {
			format = g_strdup("%c");
			if (!format) {
				break;
			}
		}
		pc = log4g_date_pattern_converter_new(&priv->formatting,
				format);
		if (!pc) {
			g_free(format);
		}
		break;
	}
	case 'F':
		pc = log4g_location_pattern_converter_new(&priv->formatting,
				FILE_LOCATION_CONVERTER);
		break;
	case 'l':
		pc = log4g_location_pattern_converter_new(&priv->formatting,
				FULL_LOCATION_CONVERTER);
		break;
	case 'L':
		pc = log4g_location_pattern_converter_new(&priv->formatting,
				LINE_LOCATION_CONVERTER);
		break;
	case 'm':
		pc = log4g_basic_pattern_converter_new(&priv->formatting,
				MESSAGE_CONVERTER);
		break;
	case 'M':
		pc = log4g_location_pattern_converter_new(&priv->formatting,
				METHOD_LOCATION_CONVERTER);
		break;
	case 'p':
		pc = log4g_basic_pattern_converter_new(&priv->formatting,
				LEVEL_CONVERTER);
		break;
	case 'r':
		pc = log4g_basic_pattern_converter_new(&priv->formatting,
				RELATIVE_TIME_CONVERTER);
		break;
	case 't':
		pc = log4g_basic_pattern_converter_new(&priv->formatting,
				THREAD_CONVERTER);
		break;
	case 'x':
		pc = log4g_basic_pattern_converter_new(&priv->formatting,
				NDC_CONVERTER);
		break;
	case 'X': {
		gchar *opt = log4g_pattern_parser_extract_option(self);
		if (opt) {
			pc = log4g_mdc_pattern_converter_new(&priv->formatting,
					opt);
			if (!pc) {
				g_free(opt);
			}
		}
		break;
	}
	default:
		log4g_log_error(Q_("unexpected char [%c] at position %d in "
				"conversion pattern"), c, priv->i);
		pc = log4g_literal_pattern_converter_new(priv->buffer->str);
		break;
	}
	if (pc) {
		log4g_pattern_parser_add_converter(self, pc);
		g_object_unref(pc);
		pc = NULL;
	}
}

/**
 * log4g_pattern_parser_add_converter:
 * @self: A pattern converter object.
 * @pc: The pattern converter to add to @self.
 *
 * Add a pattern converter to a pattern parser object.
 *
 * Since: 0.1
 */
void
log4g_pattern_parser_add_converter(Log4gPatternParser *self,
		Log4gPatternConverter *pc)
{
	struct Private *priv = GET_PRIVATE(self);
	g_string_set_size(priv->buffer, 0);
	log4g_pattern_parser_add_to_list(self, pc);
	priv->state = LITERAL_STATE;
	formatting_info_reset_(&priv->formatting);
}
