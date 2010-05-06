/* Copyright 2010 Michael Steinert
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
 * \brief Implements the API in log4g/helpers/pattern-parser.h
 * \author Mike Steinert
 * \date 2-15-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "log4g/helpers/pattern-parser.h"
#include "log4g/layout.h"
#include <string.h>
#include <stdlib.h>

G_DEFINE_TYPE(Log4gPatternParser, log4g_pattern_parser, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_PARSER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    GString *buffer;
    struct Log4gFormattingInfo formatting;
    gchar *pattern;
    gint length;
    gint i;
    Log4gPatternConverter *head;
    Log4gPatternConverter *tail;
    gint state;
};

#define ESCAPE_CHAR '%'

enum _PatternParserState {
    LITERAL_STATE = 0,
    CONVERTER_STATE,
    DOT_STATE,
    MIN_STATE,
    MAX_STATE
};

static void
_formatting_info_reset(struct Log4gFormattingInfo *info)
{
    info->min = -1;
    info->max = 0x7fffffff;
    info->align = FALSE;
}

static void
log4g_pattern_parser_init(Log4gPatternParser *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->buffer = g_string_sized_new(32);
    _formatting_info_reset(&priv->formatting);
    priv->pattern = NULL;
    priv->length = 0;
    priv->i = 0;
    priv->head = priv->tail = NULL;
    priv->state = LITERAL_STATE;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->buffer) {
        g_string_free(priv->buffer, TRUE);
        priv->buffer = NULL;
    }
    if (priv->pattern) {
        g_free(priv->pattern);
        priv->pattern = NULL;
    }
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = priv->tail = NULL;
    }
    G_OBJECT_CLASS(log4g_pattern_parser_parent_class)->finalize(base);
}

static void
log4g_pattern_parser_class_init(Log4gPatternParserClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gPatternParser *
log4g_pattern_parser_new(const gchar *pattern)
{
    struct Log4gPrivate *priv;
    Log4gPatternParser *self = g_object_new(LOG4G_TYPE_PATTERN_PARSER, NULL);
    if (!self) {
        return NULL;
    }
    priv = GET_PRIVATE(self);
    priv->pattern = g_strdup(pattern);
    if (!priv->pattern) {
        g_object_unref(self);
        return NULL;
    }
    priv->length = strlen(pattern);
    return self;
}

void
log4g_pattern_parser_add_to_list(Log4gPatternParser *self,
        Log4gPatternConverter *pc)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->head) {
        g_object_ref(pc);
        priv->head = priv->tail = pc;
    } else {
        log4g_pattern_converter_set_next(priv->tail, pc);
        priv->tail = pc;
    }
}

gchar *
log4g_pattern_parser_extract_option(Log4gPatternParser *self)
{
    gchar *option = NULL;
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if ((priv->i < priv->length) && ('{' == priv->pattern[priv->i])) {
        gint end = 0;
        while (priv->pattern[++end + priv->i]) {
            if ('}' == priv->pattern[end + priv->i]) {
                break;
            }
        }
        if (end) {
            option = g_strndup(&priv->pattern[priv->i + 1], end - 1);
        }
        priv->i += (end + 1);
    }
    return option;
}

gint
log4g_pattern_parser_extract_precision_option(Log4gPatternParser *self)
{
    glong r = 0;
    gchar *option = log4g_pattern_parser_extract_option(self);
    if (option) {
        errno = 0;
        r = strtol(option, NULL, 10);
        if (errno) {
            if (EINVAL) {
                log4g_log_error(Q_("category option \"%s\" is not a decimal "
                        "number: %s"), option, g_strerror(errno));
            }
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

Log4gPatternConverter *
log4g_pattern_parser_parse(Log4gPatternParser *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    Log4gPatternConverter *pc;
    gchar c;
    priv->i = 0;
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = NULL;
    }
    while (priv->i < priv->length) {
        c = priv->pattern[priv->i++];
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
                    g_string_append(priv->buffer, LOG4G_LAYOUT_LINE_SEP);
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
                    _formatting_info_reset(&priv->formatting);
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
                    log4g_pattern_parser_finalize_converter(self, c);
                }
                break;
            }
            break;
        case MIN_STATE:
            g_string_append_c(priv->buffer, c);
            if (c >= '0' && c <= '9') {
                priv->formatting.min = priv->formatting.min * 10 + (c - '0');
            } else if ('.' == c) {
                priv->state = DOT_STATE;
            } else {
                log4g_pattern_parser_finalize_converter(self, c);
            }
            break;
        case DOT_STATE:
            g_string_append_c(priv->buffer, c);
            if (c >= '0' && c <= '9') {
                priv->formatting.max = c - '0';
                priv->state = MAX_STATE;
            } else {
                log4g_log_error(Q_("error occurred in position %d\nwas "
                        "expecting digit, instead got char %c"), priv->i, c);
                priv->state = LITERAL_STATE;
            }
            break;
        case MAX_STATE:
            g_string_append_c(priv->buffer, c);
            if (c >= '0' && c <= '9') {
                priv->formatting.max = priv->formatting.max * 10 + (c - '0');
            } else {
                log4g_pattern_parser_finalize_converter(self, c);
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

void
log4g_pattern_parser_finalize_converter(Log4gPatternParser *self, gchar c)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    Log4gPatternConverter *pc = NULL;
    switch (c) {
    case 'c':
        pc = log4g_category_pattern_converter_new(&priv->formatting,
                    log4g_pattern_parser_extract_precision_option(self));
        break;
    case 'd': {
        char *format = log4g_pattern_parser_extract_option(self);
        if (!format) {
            format = g_strdup("%c");
            if (!format) {
                break;
            }
        }
        pc = log4g_date_pattern_converter_new(&priv->formatting, format);
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
            pc = log4g_mdc_pattern_converter_new(&priv->formatting, opt);
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

void
log4g_pattern_parser_add_converter(Log4gPatternParser *self,
        Log4gPatternConverter *pc)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_string_set_size(priv->buffer, 0);
    log4g_pattern_parser_add_to_list(self, pc);
    priv->state = LITERAL_STATE;
    _formatting_info_reset(&priv->formatting);
}
