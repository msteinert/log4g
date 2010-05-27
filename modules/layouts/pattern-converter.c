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
 * \brief Implements the API in log4g/helpers/pattern-converter.h
 * \author Mike Steinert
 * \date 2-15-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "pattern-converter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gPatternConverter, log4g_pattern_converter,
        G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_CONVERTER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    Log4gPatternConverter *next;
    gint min;
    gint max;
    gboolean align;
};

static void
log4g_pattern_converter_init(Log4gPatternConverter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->min = -1;
    priv->max = 0x7fffffff;
    priv->align = FALSE;
    priv->next = NULL;
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->next) {
        g_object_unref(priv->next);
        priv->next = NULL;
    }
    G_OBJECT_CLASS(log4g_pattern_converter_parent_class)->dispose(base);
}

static void
format(Log4gPatternConverter *self, GString *buffer, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    const gchar *string = log4g_pattern_converter_convert(self, event);
    if (!string) {
        if (0 < priv->min) {
            log4g_pattern_converter_space_pad(self, buffer, priv->min);
        }
        return;
    }
    gint len = strlen(string);
    if (len > priv->max) {
        g_string_append(buffer, string + (len - priv->max));
    } else if (len < priv->min) {
        if (priv->align) {
            g_string_append(buffer, string);
            log4g_pattern_converter_space_pad(self, buffer, priv->min - len);
        } else {
            log4g_pattern_converter_space_pad(self, buffer, priv->min - len);
            g_string_append(buffer, string);
        }
    } else {
        g_string_append(buffer, string);
    }
}

static void
log4g_pattern_converter_class_init(Log4gPatternConverterClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gPatternConverter */
    klass->convert = NULL;
    klass->format = format;
}

static void
log4g_pattern_converter_class_finalize(Log4gPatternConverterClass *klass)
{
    /* do nothing */
}

const gchar *
log4g_pattern_converter_convert(Log4gPatternConverter *self,
        Log4gLoggingEvent *event)
{
    g_return_val_if_fail(LOG4G_IS_PATTERN_CONVERTER(self), NULL);
    Log4gPatternConverterClass *klass =
        LOG4G_PATTERN_CONVERTER_GET_CLASS(self);
    return klass->convert(self, event);
}

void
log4g_pattern_converter_format(Log4gPatternConverter *self,
        GString *string, Log4gLoggingEvent *event)
{
    g_return_if_fail(LOG4G_IS_PATTERN_CONVERTER(self));
    Log4gPatternConverterClass *klass =
        LOG4G_PATTERN_CONVERTER_GET_CLASS(self);
    klass->format(self, string, event);
}

Log4gPatternConverter *
log4g_pattern_converter_get_next(Log4gPatternConverter *self)
{
    g_return_val_if_fail(LOG4G_IS_PATTERN_CONVERTER(self), NULL);
    return GET_PRIVATE(self)->next;
}

void
log4g_pattern_converter_set_next(Log4gPatternConverter *self,
        Log4gPatternConverter *next)
{
    g_return_if_fail(LOG4G_IS_PATTERN_CONVERTER(self));
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->next) {
        g_object_unref(priv->next);
    }
    g_object_ref(next);
    priv->next = next;
}

void
log4g_pattern_converter_space_pad(const Log4gPatternConverter *self,
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

#define GET_BASIC_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_BASIC_PATTERN_CONVERTER, struct BasicPrivate))

struct BasicPrivate {
    Log4gPatternConverterType type;
    gchar buffer[128];
};

static void
log4g_basic_pattern_converter_init(Log4gBasicPatternConverter *self)
{
    GET_BASIC_PRIVATE(self)->type = INVALID_CONVERTER;
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
        struct timeval *tv = log4g_logging_event_get_time_stamp(event);
        glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
        g_snprintf(priv->buffer, sizeof priv->buffer, "%ld", time - start);
        return priv->buffer;
    }
    case THREAD_CONVERTER:
        return log4g_logging_event_get_thread_name(event);
    case LEVEL_CONVERTER: {
        return log4g_level_to_string(log4g_logging_event_get_level(event));
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
log4g_basic_pattern_converter_class_init(
        Log4gBasicPatternConverterClass *klass)
{
    Log4gPatternConverterClass *pc_class =
        LOG4G_PATTERN_CONVERTER_CLASS(klass);
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct BasicPrivate));
    /* initialize Log4gPatternConverter class */
    pc_class->convert = basic_pattern_converter_convert;
}

static void
log4g_basic_pattern_converter_class_finalize(
        Log4gBasicPatternConverterClass *klass)
{
    /* do nothing */
}

Log4gPatternConverter *
log4g_basic_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type)
{
    struct Log4gPrivate *priv;
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

#define GET_LITERAL_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, struct LiteralPrivate))

struct LiteralPrivate {
    gchar *literal;
};

static void
log4g_literal_pattern_converter_init(Log4gLiteralPatternConverter *self)
{
    GET_LITERAL_PRIVATE(self)->literal = NULL;
}

static void
literal_pattern_converter_finalize(GObject *base)
{
    struct LiteralPrivate *priv = GET_LITERAL_PRIVATE(base);
    g_free(priv->literal);
    priv->literal = NULL;
    G_OBJECT_CLASS(log4g_literal_pattern_converter_parent_class)->
        finalize(base);
}

static const gchar *
literal_pattern_converter_convert(Log4gPatternConverter *base,
        Log4gLoggingEvent *event)
{
    return GET_LITERAL_PRIVATE(base)->literal;
}

static void
literal_pattern_converter_format(Log4gPatternConverter *base, GString *string,
        Log4gLoggingEvent *event)
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
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gPatternConverterClass *pc_class =
        LOG4G_PATTERN_CONVERTER_CLASS(klass);
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct LiteralPrivate));
    /* initialize GObject class */
    gobject_class->finalize = literal_pattern_converter_finalize;
    /* initialize Log4gPatternConverter class */
    pc_class->convert = literal_pattern_converter_convert;
    pc_class->format = literal_pattern_converter_format;
}

static void
log4g_literal_pattern_converter_class_finalize(
        Log4gLiteralPatternConverterClass *klass)
{
    /* do nothing */
}

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

#define GET_DATE_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_DATE_PATTERN_CONVERTER, \
            struct DatePrivate))

struct DatePrivate {
    gchar *format;
    gchar buffer[128];
};

static void
log4g_date_pattern_converter_init(Log4gDatePatternConverter *self)
{
    GET_DATE_PRIVATE(self)->format = NULL;
}

static void
date_pattern_converter_finalize(GObject *base)
{
    struct DatePrivate *priv = GET_DATE_PRIVATE(base);
    g_free(priv->format);
    priv->format = NULL;
    G_OBJECT_CLASS(log4g_date_pattern_converter_parent_class)->finalize(base);
}

static const gchar *
date_pattern_converter_convert(Log4gPatternConverter *base,
        Log4gLoggingEvent *event)
{
    struct DatePrivate *priv = GET_DATE_PRIVATE(base);
    struct timeval *tv = log4g_logging_event_get_time_stamp(event);
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
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gPatternConverterClass *pc_class =
        LOG4G_PATTERN_CONVERTER_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = date_pattern_converter_finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct DatePrivate));
    /* initialize Log4gPatternConverter class */
    pc_class->convert = date_pattern_converter_convert;
}

static void
log4g_date_pattern_converter_class_finalize(
        Log4gDatePatternConverterClass *klass)
{
    /* do nothing */
}

Log4gPatternConverter *
log4g_date_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *format)
{
    Log4gDatePatternConverter *self =
        g_object_new(LOG4G_TYPE_DATE_PATTERN_CONVERTER, NULL);
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->min = formatting->min;
    priv->max = formatting->max;
    priv->align = formatting->align;
    GET_DATE_PRIVATE(self)->format = format;
    return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gMDCPatternConverter, log4g_mdc_pattern_converter,
        LOG4G_TYPE_PATTERN_CONVERTER)

#define GET_MDC_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_MDC_PATTERN_CONVERTER, \
            struct MDCPrivate))

struct MDCPrivate {
    gchar *key;
};

static void
log4g_mdc_pattern_converter_init(Log4gMDCPatternConverter *self)
{
    GET_MDC_PRIVATE(self)->key = NULL;
}

static void
mdc_pattern_converter_finalize(GObject *base)
{
    struct MDCPrivate *priv = GET_MDC_PRIVATE(base);
    g_free(priv->key);
    priv->key = NULL;
    G_OBJECT_CLASS(log4g_mdc_pattern_converter_parent_class)->finalize(base);
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
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gPatternConverterClass *pc_class = LOG4G_PATTERN_CONVERTER_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = mdc_pattern_converter_finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct MDCPrivate));
    /* initialize Log4gPatternConverter class */
    pc_class->convert = mdc_pattern_converter_convert;
}

static void
log4g_mdc_pattern_converter_class_finalize(
        Log4gMDCPatternConverterClass *klass)
{
    /* do nothing */
}

Log4gPatternConverter *
log4g_mdc_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *key)
{
    Log4gMDCPatternConverter *self =
        g_object_new(LOG4G_TYPE_MDC_PATTERN_CONVERTER, NULL);
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->min = formatting->min;
    priv->max = formatting->max;
    priv->align = formatting->align;
    GET_MDC_PRIVATE(self)->key = key;
    return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gLocationPatternConverter,
        log4g_location_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define GET_LOCATION_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, struct LocationPrivate))

struct LocationPrivate {
    Log4gPatternConverterType type;
};

static void
log4g_location_pattern_converter_init(Log4gLocationPatternConverter *self)
{
    /* do nothing */
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
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct LocationPrivate));
    /* initialize Log4gPatternConverter class */
    pc_class->convert = location_pattern_converter_convert;
}

static void
log4g_location_pattern_converter_class_finalize(
        Log4gLocationPatternConverterClass *klass)
{
    /* do nothing */
}

Log4gPatternConverter *
log4g_location_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type)
{
    Log4gLocationPatternConverter *self =
        g_object_new(LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, NULL);
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->min = formatting->min;
    priv->max = formatting->max;
    priv->align = formatting->align;
    GET_LOCATION_PRIVATE(self)->type = type;
    return LOG4G_PATTERN_CONVERTER(self);
}

G_DEFINE_DYNAMIC_TYPE(Log4gCategoryPatternConverter,
        log4g_category_pattern_converter, LOG4G_TYPE_PATTERN_CONVERTER)

#define GET_CATEGORY_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, struct CategoryPrivate))

struct CategoryPrivate {
    gint precision;
};

static void
log4g_category_pattern_converter_init(Log4gCategoryPatternConverter *self)
{
    GET_CATEGORY_PRIVATE(self)->precision = 0;
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
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct CategoryPrivate));
    /* initialize Log4gPatternConverter class */
    pc_class->convert = category_pattern_converter_convert;
}

static void
log4g_category_pattern_converter_class_finalize(
        Log4gCategoryPatternConverterClass *klass)
{
    /* do nothing */
}

Log4gPatternConverter *
log4g_category_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gint precision)
{
    Log4gCategoryPatternConverter *self =
        g_object_new(LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, NULL);
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
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
