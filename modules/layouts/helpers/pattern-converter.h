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

#ifndef LOG4G_PATTERN_CONVERTER_H
#define LOG4G_PATTERN_CONVERTER_H

#include <log4g/logging-event.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_PATTERN_CONVERTER \
    (log4g_pattern_converter_get_type())

#define LOG4G_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_PATTERN_CONVERTER, \
            Log4gPatternConverter))

#define LOG4G_IS_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_PATTERN_CONVERTER))

#define LOG4G_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_PATTERN_CONVERTER, \
            Log4gPatternConverterClass))

#define LOG4G_IS_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_PATTERN_CONVERTER))

#define LOG4G_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_PATTERN_CONVERTER, \
            Log4gPatternConverterClass))

typedef struct _Log4gPatternConverter Log4gPatternConverter;

typedef struct _Log4gPatternConverterClass Log4gPatternConverterClass;

/**
 * Log4gPatternConverter:
 *
 * The <structname>Log4gPatternConverter</structname> structure does not have
 * any public members.
 */
struct _Log4gPatternConverter {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gPatternConverterConvert:
 * @self: A pattern converter object.
 * @event: The log event to convert.
 *
 * Convert a pattern.
 *
 * Derived converters must override this function in order to convert
 * conversion specifiers in the correct way.
 *
 * @See: log4g/logging-event.h
 *
 * Returns: The converted pattern.
 * Since: 0.1
 */
typedef const gchar *
(*Log4gPatternConverterConvert)(Log4gPatternConverter *self,
        Log4gLoggingEvent *event);

/**
 * Log4gPatternConverterFormat:
 * @self: A pattern converter object.
 * @string: The formatted output is placed here.
 * @event: The log event to format.
 *
 * A template function for formatting in a converter specific way.
 *
 * @See: log4g/logging-event.h
 *
 * Since: 0.1
 */
typedef void
(*Log4gPatternConverterFormat)(Log4gPatternConverter *self, GString *string,
        Log4gLoggingEvent *event);

/**
 * Log4gPatternConverterClass:
 * @convert: Convert a pattern.
 * @format: Format in a converter specific way.
 */
struct _Log4gPatternConverterClass {
    /*< private >*/
    GObjectClass parent_class;
    /*< public >*/
    Log4gPatternConverterConvert convert;
    Log4gPatternConverterFormat format;
};

/**
 * Log4gFormattingInfo:
 * @min: The minimum field width
 * @max: The maximum field width
 * @align: Indicates left alignment
 *
 * Formatting information for pattern converters.
 */
struct Log4gFormattingInfo {
    /*< public >*/
    gint min;
    gint max;
    gboolean align;
};

GType
log4g_pattern_converter_get_type(void);

void
log4g_pattern_converter_register(GTypeModule *module);

const gchar *
log4g_pattern_converter_convert(Log4gPatternConverter *self,
        Log4gLoggingEvent *event);

void
log4g_pattern_converter_format(Log4gPatternConverter *self,
        GString *string, Log4gLoggingEvent *event);

Log4gPatternConverter *
log4g_pattern_converter_get_next(Log4gPatternConverter *self);

void
log4g_pattern_converter_set_next(Log4gPatternConverter *self,
        Log4gPatternConverter *next);

void
log4g_pattern_converter_space_pad(const Log4gPatternConverter *self,
        GString *buffer, gint length);

/**
 * Log4gPatternConverterType:
 * @INVALID_CONVERTER: Sentinel value
 * @RELATIVE_TIME_CONVERTER: Time converter
 * @THREAD_CONVERTER: Thread converter
 * @LEVEL_CONVERTER: Log level converter
 * @NDC_CONVERTER: Nested data context converter
 * @MESSAGE_CONVERTER: Log message converter
 * @FULL_LOCATION_CONVERTER: Log location converter
 * @METHOD_LOCATION_CONVERTER: Function name converter
 * @LINE_LOCATION_CONVERTER: Line number converter
 * @FILE_LOCATION_CONVERTER: File name converter
 * @MAX_CONVERTER: Sentinel value
 */
typedef enum {
    INVALID_CONVERTER = 0,
    RELATIVE_TIME_CONVERTER,
    THREAD_CONVERTER,
    LEVEL_CONVERTER,
    NDC_CONVERTER,
    MESSAGE_CONVERTER,
    FULL_LOCATION_CONVERTER,
    METHOD_LOCATION_CONVERTER,
    LINE_LOCATION_CONVERTER,
    FILE_LOCATION_CONVERTER,
    MAX_CONVERTER
} Log4gPatternConverterType;

#define LOG4G_TYPE_BASIC_PATTERN_CONVERTER \
    (log4g_basic_pattern_converter_get_type())

#define LOG4G_BASIC_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_BASIC_PATTERN_CONVERTER, Log4gBasicPatternConverter))

#define LOG4G_IS_BASIC_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
                                LOG4G_TYPE_BASIC_PATTERN_CONVERTER))

#define LOG4G_BASIC_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_BASIC_PATTERN_CONVERTER, \
            Log4gBasicPatternConverterClass))

#define LOG4G_IS_BASIC_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_BASIC_PATTERN_CONVERTER))

#define LOG4G_BASIC_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_BASIC_PATTERN_CONVERTER, \
            Log4gBasicPatternConverterClass))

typedef struct _Log4gBasicPatternConverter Log4gBasicPatternConverter;

typedef struct _Log4gBasicPatternConverterClass
        Log4gBasicPatternConverterClass;

/**
 * Log4gBasicPatternConverter:
 *
 * The <structname>Log4gBasicPatternConverter</structname> structure does not
 * have any public members.
 */
struct _Log4gBasicPatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gBasicPatternConverterClass:
 *
 * The <structname>Log4gBasicPatternConverterClass</structname> structure does
 * not have any public members.
 */
struct _Log4gBasicPatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_basic_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_basic_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type);

#define LOG4G_TYPE_LITERAL_PATTERN_CONVERTER \
    (log4g_literal_pattern_converter_get_type())

#define LOG4G_LITERAL_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, \
            Log4gLiteralPatternConverter))

#define LOG4G_IS_LITERAL_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_LITERAL_PATTERN_CONVERTER))

#define LOG4G_LITERAL_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, \
            Log4gLiteralPatternConverterClass))

#define LOG4G_IS_LITERAL_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LITERAL_PATTERN_CONVERTER))

#define LOG4G_LITERAL_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_LITERAL_PATTERN_CONVERTER, \
            Log4gLiteralPatternConverterClass))

typedef struct _Log4gLiteralPatternConverter Log4gLiteralPatternConverter;

typedef struct _Log4gLiteralPatternConverterClass
        Log4gLiteralPatternConverterClass;

/**
 * Log4gLiteralPatternConverter:
 *
 * The <structname>Log4gLiteralPatternConverter</structname> structure does
 * not have any public members.
 */
struct _Log4gLiteralPatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gLiteralPatternConverterClass:
 *
 * The <structname>Log4gLiteralPatternConverterClass</structname> structure
 * does not have any public members.
 */
struct _Log4gLiteralPatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_literal_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_literal_pattern_converter_new(const gchar *pattern);

#define LOG4G_TYPE_DATE_PATTERN_CONVERTER \
    (log4g_date_pattern_converter_get_type())

#define LOG4G_DATE_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_DATE_PATTERN_CONVERTER, \
            Log4gDatePatternConverter))

#define LOG4G_IS_DATE_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_DATE_PATTERN_CONVERTER))

#define LOG4G_DATE_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DATE_PATTERN_CONVERTER, \
            Log4gDatePatternConverterClass))

#define LOG4G_IS_DATE_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DATE_PATTERN_CONVERTER))

#define LOG4G_DATE_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_DATE_PATTERN_CONVERTER, \
            Log4gDatePatternConverterClass))

typedef struct _Log4gDatePatternConverter Log4gDatePatternConverter;

typedef struct _Log4gDatePatternConverterClass Log4gDatePatternConverterClass;

/**
 * Log4gDatePatternConverter:
 *
 * The <structname>Log4gDatePatternConverter</structname> structure does not
 * have any public members.
 */
struct _Log4gDatePatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gDatePatternConverterClass:
 *
 * The <structname>Log4gDatePatternConverterClass</structname> structure does
 * not have any public members.
 */
struct _Log4gDatePatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_date_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_date_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *format);

#define LOG4G_TYPE_MDC_PATTERN_CONVERTER \
    (log4g_mdc_pattern_converter_get_type())

#define LOG4G_MDC_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_MDC_PATTERN_CONVERTER, Log4gMDCPatternConverter))

#define LOG4G_IS_MDC_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_MDC_PATTERN_CONVERTER))

#define LOG4G_MDC_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_MDC_PATTERN_CONVERTER, \
            Log4gMDCPatternConverterClass))

#define LOG4G_IS_MDC_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_MDC_PATTERN_CONVERTER))

#define LOG4G_MDC_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_MDC_PATTERN_CONVERTER, Log4gMDCPatternConverterClass))

typedef struct _Log4gMDCPatternConverter Log4gMDCPatternConverter;

typedef struct _Log4gMDCPatternConverterClass Log4gMDCPatternConverterClass;

/**
 * Log4gMDCPatternConverter:
 *
 * The <structname>Log4gMDCPatternConverter</structname> structure does not
 * have any public members.
 */
struct _Log4gMDCPatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gMDCPatternConverterClass:
 *
 * The <structname>Log4gMDCPatternConverterClass</structname> structure does
 * not have any public members.
 */
struct _Log4gMDCPatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_mdc_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_mdc_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *key);

#define LOG4G_TYPE_LOCATION_PATTERN_CONVERTER \
    (log4g_location_pattern_converter_get_type())

#define LOG4G_LOCATION_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, \
            Log4gLocationPatternConverter))

#define LOG4G_IS_LOCATION_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_LOCATION_PATTERN_CONVERTER))

#define LOG4G_LOCATION_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, \
            Log4gLocationPatternConverterClass))

#define LOG4G_IS_LOCATION_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOCATION_PATTERN_CONVERTER))

#define LOG4G_LOCATION_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_LOCATION_PATTERN_CONVERTER, \
            Log4gLocationPatternConverterClass))

typedef struct _Log4gLocationPatternConverter Log4gLocationPatternConverter;

typedef struct _Log4gLocationPatternConverterClass
        Log4gLocationPatternConverterClass;

/**
 * Log4gLocationPatternConverter:
 *
 * The <structname>Log4gLocationPatternConverter</structname> structure does
 * not have any public members.
 */
struct _Log4gLocationPatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gLocationPatternConverterClass:
 *
 * The <structname>Log4gLocationPatternConverterClass</structname> structure
 * does not have any public members.
 */
struct _Log4gLocationPatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_location_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_location_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type);

#define LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER \
    (log4g_category_pattern_converter_get_type())

#define LOG4G_CATEGORY_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, \
            Log4gCategoryPatternConverter))

#define LOG4G_IS_CATEGORY_PATTERN_CONVERTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER))

#define LOG4G_CATEGORY_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, \
            Log4gCategoryPatternConverterClass))

#define LOG4G_IS_CATEGORY_PATTERN_CONVERTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER))

#define LOG4G_CATEGORY_PATTERN_CONVERTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_CATEGORY_PATTERN_CONVERTER, \
            Log4gCategoryPatternConverterClass))

typedef struct _Log4gCategoryPatternConverter Log4gCategoryPatternConverter;

typedef struct _Log4gCategoryPatternConverterClass
        Log4gCategoryPatternConverterClass;

/**
 * Log4gCategoryPatternConverter:
 *
 * The <structname>Log4gCategoryPatternConverter</structname> structure does
 * not have any public members.
 */
struct _Log4gCategoryPatternConverter {
    /*< private >*/
    Log4gPatternConverter parent_instance;
};

/**
 * Log4gCategoryPatternConverterClass:
 *
 * The <structname>Log4gCategoryPatternConverterClass</structname> structure
 * does not have any public members.
 */
struct _Log4gCategoryPatternConverterClass {
    /*< private >*/
    Log4gPatternConverterClass parent_class;
};

GType
log4g_category_pattern_converter_get_type(void);

Log4gPatternConverter *
log4g_category_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gint precision);

G_END_DECLS

#endif /* LOG4G_PATTERN_CONVERTER_H */
