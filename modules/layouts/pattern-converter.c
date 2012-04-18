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
 * SECTION: pattern-converter
 * @short_description: Provide formatting functionality for pattern converters
 * @see_also: #Log4gPatternLayoutClass, #Log4gPatternParserClass,
 *            #Log4gMDCClass, #Log4gNDCClass, #Log4gDateLayoutClass
 *
 * The pattern converter is an abstract class that provides the formatting
 * functionality needed by pattern converters.
 *
 * Conversion specifiers in a conversion pattern are parsed into individual
 * pattern converters. Each pattern converter is responsible for converting
 * a logging event in a converter-specific way.
 *
 * Log4g includes the following pattern converters:
 * <itemizedlist>
 * <listitem><para>Basic pattern converter</para></listitem>
 * <listitem><para>Literal pattern converter</para></listitem>
 * <listitem><para>Date pattern converter</para></listitem>
 * <listitem><para>MDC (mapped data context) pattern converter</para></listitem>
 * <listitem><para>Location pattern converter</para></listitem>
 * <listitem><para>Logger category pattern converter</para></listitem>
 * </itemizedlist>
 *
 * The basic pattern converter handles relative time, thread names,
 * NDC (nested data context) values and log messages.
 *
 * The literal pattern converter is used to print non-pattern text from the
 * conversion pattern.
 *
 * The date pattern converter converts a date into a string.
 *
 * The MDC pattern converter handles converting MDC values.
 *
 * The location pattern converter handles location information.
 *
 * The logger category pattern converter handles logger names.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "helpers/pattern-converter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gPatternConverter, log4g_pattern_converter,
		G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_CONVERTER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gPatternConverter *)instance)->priv)

struct Private {
	Log4gPatternConverter *next;
	gint min;
	gint max;
	gboolean align;
};

static void
log4g_pattern_converter_init(Log4gPatternConverter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->next = NULL;
	priv->min = -1;
	priv->max = 0x7fffffff;
	priv->align = FALSE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->next) {
		g_object_unref(priv->next);
		priv->next = NULL;
	}
	G_OBJECT_CLASS(log4g_pattern_converter_parent_class)->dispose(base);
}

static void
format(Log4gPatternConverter *self, GString *buffer, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(self);
	const gchar *string = log4g_pattern_converter_convert(self, event);
	if (!string) {
		if (0 < priv->min) {
			log4g_pattern_converter_space_pad(self, buffer,
					priv->min);
		}
		return;
	}
	gint len = strlen(string);
	if (len > priv->max) {
		g_string_append(buffer, string + (len - priv->max));
	} else if (len < priv->min) {
		if (priv->align) {
			g_string_append(buffer, string);
			log4g_pattern_converter_space_pad(self, buffer,
					priv->min - len);
		} else {
			log4g_pattern_converter_space_pad(self, buffer,
					priv->min - len);
			g_string_append(buffer, string);
		}
	} else {
		g_string_append(buffer, string);
	}
}

static void
log4g_pattern_converter_class_init(Log4gPatternConverterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	klass->convert = NULL;
	klass->format = format;
	g_type_class_add_private(klass, sizeof(struct Private));
}

static void
log4g_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_pattern_converter_convert:
 * @self: A pattern converter object.
 * @event: The log event to convert.
 *
 * Call the @convert function from the #Log4gPatternConverterClass of @self.
 *
 * Returns: The converted pattern.
 * Since: 0.1
 */
const gchar *
log4g_pattern_converter_convert(Log4gPatternConverter *self,
		Log4gLoggingEvent *event)
{
	g_return_val_if_fail(LOG4G_IS_PATTERN_CONVERTER(self), NULL);
	Log4gPatternConverterClass *klass =
		LOG4G_PATTERN_CONVERTER_GET_CLASS(self);
	return klass->convert(self, event);
}

/**
 * log4g_pattern_converter_format:
 * @self: A pattern converter object.
 * @string: The formatted output is placed here.
 * @event: The log event to format.
 *
 * Call the @format function from the #Log4gPatternConverterClass of @self.
 *
 * Since: 0.1
 */
void
log4g_pattern_converter_format(Log4gPatternConverter *self,
		GString *string, Log4gLoggingEvent *event)
{
	g_return_if_fail(LOG4G_IS_PATTERN_CONVERTER(self));
	Log4gPatternConverterClass *klass =
		LOG4G_PATTERN_CONVERTER_GET_CLASS(self);
	klass->format(self, string, event);
}

/**
 * log4g_pattern_converter_get_next:
 * @self: A pattern converter object.
 *
 * Retrieve the next pattern converter in the chain.
 *
 * Returns: The next pattern converter in the chain.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_pattern_converter_get_next(Log4gPatternConverter *self)
{
	g_return_val_if_fail(LOG4G_IS_PATTERN_CONVERTER(self), NULL);
	return GET_PRIVATE(self)->next;
}

/**
 * log4g_pattern_converter_set_next:
 * @self: A pattern converter object.
 * @next: The next pattern converter in the chain.
 *
 * Set the next pattern converter in the chain.
 *
 * Since: 0.1
 */
void
log4g_pattern_converter_set_next(Log4gPatternConverter *self,
		Log4gPatternConverter *next)
{
	g_return_if_fail(LOG4G_IS_PATTERN_CONVERTER(self));
	struct Private *priv = GET_PRIVATE(self);
	if (priv->next) {
		g_object_unref(priv->next);
	}
	priv->next = g_object_ref(next);
}

/**
 * log4g_pattern_converter_space_pad:
 * @self: A pattern converter object.
 * @buffer: The string to pad.
 * @length: The number of spaces to pad.
 *
 * A fast space padding function.
 *
 * Since: 0.1
 */
void
log4g_pattern_converter_space_pad(
		G_GNUC_UNUSED const Log4gPatternConverter *self,
		GString *buffer, gint length)
{
	static gchar *SPACES[] = {
		" ", "  ", "    ", "        ", /* 1, 2, 4, 8 spaces */
		"                ", /* 16 spaces */
		"                                ", /* 32 spaces */
	};
	for (gint i = 4; i >= 0; --i) {
		if (0 != (length & (1 << i))) {
			g_string_append(buffer, SPACES[i]);
		}
	}
}

G_DEFINE_DYNAMIC_TYPE(Log4gBasicPatternConverter,
		log4g_basic_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_BASIC_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_BASIC_PATTERN_CONVERTER, struct BasicPrivate))

#define GET_BASIC_PRIVATE(instance) \
	((struct BasicPrivate *)((Log4gBasicPatternConverter *)instance)->priv)

struct BasicPrivate {
	Log4gPatternConverterType type;
	gchar buffer[128];
};

static void
log4g_basic_pattern_converter_init(Log4gBasicPatternConverter *self)
{
	self->priv = ASSIGN_BASIC_PRIVATE(self);
	struct BasicPrivate *priv = GET_BASIC_PRIVATE(self);
	priv->type = INVALID_CONVERTER;
}

static const gchar *
basic_pattern_converter_convert(Log4gPatternConverter *base,
		Log4gLoggingEvent *event)
{
	Log4gBasicPatternConverter *self = LOG4G_BASIC_PATTERN_CONVERTER(base);
	struct BasicPrivate *priv = GET_BASIC_PRIVATE(self);
	switch (priv->type) {
	case RELATIVE_TIME_CONVERTER: {
		glong start = log4g_logging_event_get_start_time();
		const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
		glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
		g_snprintf(priv->buffer, sizeof priv->buffer, "%ld",
				time - start);
		return priv->buffer;
	}
	case THREAD_CONVERTER:
	      return log4g_logging_event_get_thread_name(event);
	case LEVEL_CONVERTER: {
		return log4g_level_to_string(
				log4g_logging_event_get_level(event));
		return NULL;
	}
	case NDC_CONVERTER:
		return log4g_logging_event_get_ndc(event);
	case MESSAGE_CONVERTER:
		return log4g_logging_event_get_rendered_message(event);
	default:
		return NULL;
	}
}

static void
log4g_basic_pattern_converter_class_init(Log4gBasicPatternConverterClass *klass)
{
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	pc_class->convert = basic_pattern_converter_convert;
	g_type_class_add_private(klass, sizeof(struct BasicPrivate));
}

static void
log4g_basic_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gBasicPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_basic_pattern_converter_new:
 * @formatting: Formatting parameters.
 * @type: The type of converter to create.
 *
 * Create a new basic pattern converter object.
 *
 * Returns: A new basic pattern converter object.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_basic_pattern_converter_new(struct Log4gFormattingInfo *formatting,
		Log4gPatternConverterType type)
{
	struct Private *priv;
	Log4gBasicPatternConverter *self =
		g_object_new(LOG4G_TYPE_BASIC_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	priv = GET_PRIVATE(self);
	priv->min = formatting->min;
	priv->max = formatting->max;
	priv->align = formatting->align;
	GET_BASIC_PRIVATE(self)->type = type;
	return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gLiteralPatternConverter,
        log4g_literal_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_LITERAL_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, struct LiteralPrivate))

#define GET_LITERAL_PRIVATE(instance) \
	((struct LiteralPrivate *)((Log4gLiteralPatternConverter *)instance)->priv)

struct LiteralPrivate {
	gchar *literal;
};

static void
log4g_literal_pattern_converter_init(Log4gLiteralPatternConverter *self)
{
	self->priv = ASSIGN_LITERAL_PRIVATE(self);
	struct LiteralPrivate *priv = GET_LITERAL_PRIVATE(self);
	priv->literal = NULL;
}

static void
literal_pattern_converter_finalize(GObject *base)
{
	struct LiteralPrivate *priv = GET_LITERAL_PRIVATE(base);
	g_free(priv->literal);
	G_OBJECT_CLASS(log4g_literal_pattern_converter_parent_class)->
		finalize(base);
}

static const gchar *
literal_pattern_converter_convert(Log4gPatternConverter *base,
		G_GNUC_UNUSED Log4gLoggingEvent *event)
{
	return GET_LITERAL_PRIVATE(base)->literal;
}

static void
literal_pattern_converter_format(Log4gPatternConverter *base, GString *string,
		G_GNUC_UNUSED Log4gLoggingEvent *event)
{
	struct LiteralPrivate *priv = GET_LITERAL_PRIVATE(base);
	if (priv->literal) {
		g_string_append(string, priv->literal);
	}
}

static void
log4g_literal_pattern_converter_class_init(
		Log4gLiteralPatternConverterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	object_class->finalize = literal_pattern_converter_finalize;
	pc_class->convert = literal_pattern_converter_convert;
	pc_class->format = literal_pattern_converter_format;
	g_type_class_add_private(klass, sizeof(struct LiteralPrivate));
}

static void
log4g_literal_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gLiteralPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_literal_pattern_converter_new:
 * @pattern: The literal text to "convert".
 *
 * Create a new literal pattern converter object.
 *
 * Returns: A new literal pattern converter object.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_literal_pattern_converter_new(const gchar *pattern)
{
	Log4gLiteralPatternConverter *self =
		g_object_new(LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	if (pattern) {
		struct LiteralPrivate *priv = GET_LITERAL_PRIVATE(self);
		priv->literal = g_strdup(pattern);
		if (!priv->literal) {
			g_object_unref(self);
			return NULL;
		}
	}
	return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gDatePatternConverter, log4g_date_pattern_converter,
		LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_DATE_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_DATE_PATTERN_CONVERTER, struct DatePrivate))

#define GET_DATE_PRIVATE(instance) \
	((struct DatePrivate *)((Log4gDatePatternConverter*)instance)->priv)

struct DatePrivate {
	gchar *format;
	gchar buffer[128];
};

static void
log4g_date_pattern_converter_init(Log4gDatePatternConverter *self)
{
	self->priv = ASSIGN_DATE_PRIVATE(self);
	struct DatePrivate *priv = GET_DATE_PRIVATE(self);
	priv->format = NULL;
}

static void
date_pattern_converter_finalize(GObject *base)
{
	struct DatePrivate *priv = GET_DATE_PRIVATE(base);
	g_free(priv->format);
	priv->format = NULL;
	G_OBJECT_CLASS(log4g_date_pattern_converter_parent_class)->
		finalize(base);
}

static const gchar *
date_pattern_converter_convert(Log4gPatternConverter *base,
		Log4gLoggingEvent *event)
{
	struct DatePrivate *priv = GET_DATE_PRIVATE(base);
	const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
	if (!tv) {
		return NULL;
	}
	struct tm tm;
	time_t time = tv->tv_sec;
	if (!localtime_r(&time, &tm)) {
		log4g_log_error("localtime_r(): %s", g_strerror(errno));
		return NULL;
	}
	if (!strftime(priv->buffer, sizeof priv->buffer, priv->format, &tm)) {
		log4g_log_error(Q_("strftime() returned zero (0)"));
		return NULL;
	}
	return priv->buffer;
}

static void
log4g_date_pattern_converter_class_init(Log4gDatePatternConverterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	object_class->finalize = date_pattern_converter_finalize;
	pc_class->convert = date_pattern_converter_convert;
	g_type_class_add_private(klass, sizeof(struct DatePrivate));
}

static void
log4g_date_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gDatePatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_date_pattern_converter_new:
 * @formatting: Formatting parameters.
 * @format: A strfime(3) date conversion pattern.
 *
 * Create a new date pattern converter.
 *
 * @See: strftime(3)
 *
 * Returns: Create a new date pattern converter.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_date_pattern_converter_new(struct Log4gFormattingInfo *formatting,
		gchar *format)
{
	Log4gDatePatternConverter *self =
		g_object_new(LOG4G_TYPE_DATE_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	priv->min = formatting->min;
	priv->max = formatting->max;
	priv->align = formatting->align;
	GET_DATE_PRIVATE(self)->format = format;
	return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gMDCPatternConverter, log4g_mdc_pattern_converter,
		LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_MDC_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_MDC_PATTERN_CONVERTER, struct MDCPrivate))

#define GET_MDC_PRIVATE(instance) \
	((struct MDCPrivate *)((Log4gMDCPatternConverter *)instance)->priv)

struct MDCPrivate {
	gchar *key;
};

static void
log4g_mdc_pattern_converter_init(Log4gMDCPatternConverter *self)
{
	self->priv = ASSIGN_MDC_PRIVATE(self);
	struct MDCPrivate *priv = GET_MDC_PRIVATE(self);
	priv->key = NULL;
}

static void
mdc_pattern_converter_finalize(GObject *base)
{
	struct MDCPrivate *priv = GET_MDC_PRIVATE(base);
	g_free(priv->key);
	G_OBJECT_CLASS(log4g_mdc_pattern_converter_parent_class)->
		finalize(base);
}

static const gchar *
mdc_pattern_converter_convert(Log4gPatternConverter *base,
		Log4gLoggingEvent *event)
{
	return log4g_logging_event_get_mdc(event, GET_MDC_PRIVATE(base)->key);
}

static void
log4g_mdc_pattern_converter_class_init(Log4gMDCPatternConverterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	object_class->finalize = mdc_pattern_converter_finalize;
	pc_class->convert = mdc_pattern_converter_convert;
	g_type_class_add_private(klass, sizeof(struct MDCPrivate));
}

static void
log4g_mdc_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gMDCPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_mdc_pattern_converter_new:
 * @formatting: Formatting parameters.
 * @key: The MDC key to look up.
 *
 * Create a new MDC (mapped data context) pattern converter object.
 *
 * @See: #Log4gMDCClass
 *
 * Returns: A new MDC pattern converter object.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_mdc_pattern_converter_new(struct Log4gFormattingInfo *formatting,
		gchar *key)
{
	Log4gMDCPatternConverter *self =
		g_object_new(LOG4G_TYPE_MDC_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	priv->min = formatting->min;
	priv->max = formatting->max;
	priv->align = formatting->align;
	GET_MDC_PRIVATE(self)->key = key;
	return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gLocationPatternConverter,
		log4g_location_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_LOCATION_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, struct LocationPrivate))

#define GET_LOCATION_PRIVATE(instance) \
	((struct LocationPrivate *)((Log4gLocationPatternConverter *)instance)->priv)

struct LocationPrivate {
	Log4gPatternConverterType type;
};

static void
log4g_location_pattern_converter_init(Log4gLocationPatternConverter *self)
{
	self->priv = ASSIGN_LOCATION_PRIVATE(self);
}

static const gchar *
location_pattern_converter_convert(Log4gPatternConverter *base,
		Log4gLoggingEvent *event)
{
	switch (GET_LOCATION_PRIVATE(base)->type) {
	case FULL_LOCATION_CONVERTER:
		return log4g_logging_event_get_full_info(event);
	case METHOD_LOCATION_CONVERTER:
		return log4g_logging_event_get_function_name(event);
	case LINE_LOCATION_CONVERTER:
		return log4g_logging_event_get_line_number(event);
	case FILE_LOCATION_CONVERTER:
		return log4g_logging_event_get_file_name(event);
	default:
		return NULL;
	}
}

static void
log4g_location_pattern_converter_class_init(
		Log4gLocationPatternConverterClass *klass)
{
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	pc_class->convert = location_pattern_converter_convert;
	g_type_class_add_private(klass, sizeof(struct LocationPrivate));
}

static void
log4g_location_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gLocationPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_location_pattern_converter_new:
 * @formatting: Formatting parameters.
 * @type: The type of converter to create.
 *
 * Create a new location pattern converter object.
 *
 * Returns: A new location pattern converter object.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_location_pattern_converter_new(struct Log4gFormattingInfo *formatting,
		Log4gPatternConverterType type)
{
	Log4gLocationPatternConverter *self =
		g_object_new(LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	priv->min = formatting->min;
	priv->max = formatting->max;
	priv->align = formatting->align;
	GET_LOCATION_PRIVATE(self)->type = type;
	return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gCategoryPatternConverter,
		log4g_category_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define ASSIGN_CATEGORY_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, struct CategoryPrivate))

#define GET_CATEGORY_PRIVATE(instance) \
	((struct CategoryPrivate *)((Log4gCategoryPatternConverter *)instance)->priv)

struct CategoryPrivate {
	gint precision;
};

static void
log4g_category_pattern_converter_init(Log4gCategoryPatternConverter *self)
{
	self->priv = ASSIGN_CATEGORY_PRIVATE(self);
}

static const gchar *
category_pattern_converter_convert(Log4gPatternConverter *base,
		Log4gLoggingEvent *event)
{
	const gchar *name = log4g_logging_event_get_logger_name(event);
	if (!name) {
		return NULL;
	}
	struct CategoryPrivate *priv = GET_CATEGORY_PRIVATE(base);
	if (1 > priv->precision) {
		return name;
	}
	gint end = strlen(name);
	for (gint i = priv->precision; i > 0; --i) {
		while (--end > -1) {
			if ('.' == name[end]) {
				break;
			}
		}
		if (-1 == end) {
			break;
		}
	}
	return &name[end + 1];
}

static void
log4g_category_pattern_converter_class_init(
		Log4gCategoryPatternConverterClass *klass)
{
	Log4gPatternConverterClass *pc_class =
		LOG4G_PATTERN_CONVERTER_CLASS(klass);
	pc_class->convert = category_pattern_converter_convert;
	g_type_class_add_private(klass, sizeof(struct CategoryPrivate));
}

static void
log4g_category_pattern_converter_class_finalize(
		G_GNUC_UNUSED Log4gCategoryPatternConverterClass *klass)
{
	/* do nothing */
}

/**
 * log4g_category_pattern_converter_new:
 * @formatting: Formatting parameters.
 * @precision: The logger category precision value.
 *
 * Create a new logger category pattern converter object.
 *
 * @See: #Log4gLoggerClass, #Log4gPatternLayoutClass
 *
 * Returns: A new logger category pattern converter object.
 * Since: 0.1
 */
Log4gPatternConverter *
log4g_category_pattern_converter_new(struct Log4gFormattingInfo *formatting,
		gint precision)
{
	Log4gCategoryPatternConverter *self =
		g_object_new(LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	priv->min = formatting->min;
	priv->max = formatting->max;
	priv->align = formatting->align;
	GET_CATEGORY_PRIVATE(self)->precision = precision;
	return LOG4G_PATTERN_CONVERTER(self);
}

void
log4g_pattern_converter_register(GTypeModule *module)
{
	/* register all pattern converters */
	log4g_pattern_converter_register_type(module);
	log4g_basic_pattern_converter_register_type(module);
	log4g_literal_pattern_converter_register_type(module);
	log4g_date_pattern_converter_register_type(module);
	log4g_mdc_pattern_converter_register_type(module);
	log4g_location_pattern_converter_register_type(module);
	log4g_category_pattern_converter_register_type(module);
}
