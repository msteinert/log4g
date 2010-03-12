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
 * \file
 * \brief ...
 * \author Mike Steinert
 * \date 2-15-2010
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

/** \brief Log4gPatternConverter object type definition */
typedef struct _Log4gPatternConverter Log4gPatternConverter;

/** \brief Log4gPatternConverter class type definition */
typedef struct _Log4gPatternConverterClass Log4gPatternConverterClass;

/** \brief Log4gPatternConverterClass definition */
struct _Log4gPatternConverter {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gPatternConverterClass definition */
struct _Log4gPatternConverterClass {
    GObjectClass parent_class; /**< parent class */
    const gchar *(*convert)(Log4gPatternConverter *self,
            Log4gLoggingEvent *event);
    void (*format)(Log4gPatternConverter *self, GString *string,
            Log4gLoggingEvent *event);
};

/** \brief Formatting information for pattern converters */
struct Log4gFormattingInfo {
    gint min; /**< The minimum field width */
    gint max; /**< The maximum field width */
    gboolean align; /**< Indicates left alignment */
};

GType
log4g_pattern_converter_get_type(void);

/**
 */
const gchar *
log4g_pattern_converter_convert(Log4gPatternConverter *self,
        Log4gLoggingEvent *event);

/**
 */
void
log4g_pattern_converter_format(Log4gPatternConverter *self,
        GString *buffer, Log4gLoggingEvent *event);

/**
 */
Log4gPatternConverter *
log4g_pattern_converter_get_next(Log4gPatternConverter *self);

/**
 */
void
log4g_pattern_converter_set_next(Log4gPatternConverter *self,
        Log4gPatternConverter *next);

/**
 * \brief A fast space padding function.
 * \param self [in] A pattern converter object.
 * \param string [out] The string to pad.
 * \param length [in] The number of spaces to pad.
 */
void
log4g_pattern_converter_space_pad(const Log4gPatternConverter *self,
        GString *buffer, gint length);

/* -------------------------------------------------------------------------- */

/** \brief ... */
typedef enum {
    INVALID_CONVERTER = 0, /**< [private] Sentinel value */
    RELATIVE_TIME_CONVERTER, /**< ... */
    THREAD_CONVERTER, /**< ... */
    LEVEL_CONVERTER, /**< ... */
    NDC_CONVERTER, /**< ... */
    MESSAGE_CONVERTER, /**< ... */
    FULL_LOCATION_CONVERTER, /**< ... */
    METHOD_LOCATION_CONVERTER, /**< ... */
    LINE_LOCATION_CONVERTER, /**< ... */
    FILE_LOCATION_CONVERTER, /**< ... */
    MAX_CONVERTER /**< [private] Sentinel value */
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

/** \brief Log4gBasicPatternConverter object type definition */
typedef struct _Log4gBasicPatternConverter Log4gBasicPatternConverter;

/** \brief Log4gBasicPatternConverter class type definition */
typedef struct _Log4gBasicPatternConverterClass
        Log4gBasicPatternConverterClass;

/** \brief Log4gBasicPatternConverterClass definition */
struct _Log4gBasicPatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gBasicPatternConverterClass definition */
struct _Log4gBasicPatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_basic_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_basic_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type);

/* -------------------------------------------------------------------------- */

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

/** \brief Log4gLiteralPatternConverter object type definition */
typedef struct _Log4gLiteralPatternConverter Log4gLiteralPatternConverter;

/** \brief Log4gLiteralPatternConverter class type definition */
typedef struct _Log4gLiteralPatternConverterClass
        Log4gLiteralPatternConverterClass;

/** \brief Log4gLiteralPatternConverterClass definition */
struct _Log4gLiteralPatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gLiteralPatternConverterClass definition */
struct _Log4gLiteralPatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_literal_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_literal_pattern_converter_new(const gchar *pattern);

/* -------------------------------------------------------------------------- */

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

/** \brief Log4gDatePatternConverter object type definition */
typedef struct _Log4gDatePatternConverter Log4gDatePatternConverter;

/** \brief Log4gDatePatternConverter class type definition */
typedef struct _Log4gDatePatternConverterClass Log4gDatePatternConverterClass;

/** \brief Log4gDatePatternConverterClass definition */
struct _Log4gDatePatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gDatePatternConverterClass definition */
struct _Log4gDatePatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_date_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_date_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *format);

/* -------------------------------------------------------------------------- */

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

/** \brief Log4gMDCPatternConverter object type definition */
typedef struct _Log4gMDCPatternConverter Log4gMDCPatternConverter;

/** \brief Log4gMDCPatternConverter class type definition */
typedef struct _Log4gMDCPatternConverterClass Log4gMDCPatternConverterClass;

/** \brief Log4gMDCPatternConverterClass definition */
struct _Log4gMDCPatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gMDCPatternConverterClass definition */
struct _Log4gMDCPatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_mdc_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_mdc_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gchar *key);

/* -------------------------------------------------------------------------- */

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

/** \brief Log4gLocationPatternConverter object type definition */
typedef struct _Log4gLocationPatternConverter Log4gLocationPatternConverter;

/** \brief Log4gLocationPatternConverter class type definition */
typedef struct _Log4gLocationPatternConverterClass
        Log4gLocationPatternConverterClass;

/** \brief Log4gLocationPatternConverterClass definition */
struct _Log4gLocationPatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gLocationPatternConverterClass definition */
struct _Log4gLocationPatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_location_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_location_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        Log4gPatternConverterType type);

/* -------------------------------------------------------------------------- */

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

/** \brief Log4gCategoryPatternConverter object type definition */
typedef struct _Log4gCategoryPatternConverter Log4gCategoryPatternConverter;

/** \brief Log4gCategoryPatternConverter class type definition */
typedef struct _Log4gCategoryPatternConverterClass
        Log4gCategoryPatternConverterClass;

/** \brief Log4gCategoryPatternConverterClass definition */
struct _Log4gCategoryPatternConverter {
    Log4gPatternConverter parent_instance; /**< parent instance */
};

/** \brief Log4gCategoryPatternConverterClass definition */
struct _Log4gCategoryPatternConverterClass {
    Log4gPatternConverterClass parent_class; /**< parent class */
};

GType
log4g_category_pattern_converter_get_type(void);

/**
 */
Log4gPatternConverter *
log4g_category_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gint precision);

G_END_DECLS

#endif /* LOG4G_PATTERN_CONVERTER_H */