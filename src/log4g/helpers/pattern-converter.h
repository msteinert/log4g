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
 * \brief Provide formatting functionality for pattern converters.
 * \author Mike Steinert
 * \date 2-15-2010
 *
 * The pattern converter is an abstract class that provides the formatting
 * functionality needed by pattern converters.
 *
 * Conversion specifiers in a conversion pattern are parsed into individual
 * pattern converters. Each pattern converter is responsible for converting
 * a logging event in a converter-specific way.
 *
 * Log4g includes the following pattern converters:
 * - Basic pattern converter
 * - Literal pattern converter
 * - Date pattern converter
 * - MDC (mapped data context) pattern converter
 * - Location pattern converter
 * - Logger category pattern converter
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
 *
 * \see log4g/layout/pattern-layout.h, log4g/helpers/pattern-parser.h,
 *      log4g/mdc.h, log4g/ndc.h, log4g/helpers/date-layout.h
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
    GObject parent_instance;
};

/** \brief Log4gPatternConverterClass definition */
struct _Log4gPatternConverterClass {
    GObjectClass parent_class;

    /**
     * \brief Convert a pattern.
     *
     * Derived converters must override this function in order to convert
     * conversion specifiers in the correct way.
     *
     * \param self [in] A pattern converter object.
     * \param event [in] The log event to convert.
     *
     * \return The converted pattern.
     *
     * \see log4g/logging-event.h
     */
    const gchar *
    (*convert)(Log4gPatternConverter *self, Log4gLoggingEvent *event);

    /**
     * \brief A template function for formatting in a converter specific way.
     *
     * \param self [in] A pattern converter object.
     * \param string [out] The formatted output is placed here.
     * \param event [in] The log event to format.
     *
     * \see log4g/logging-event.h
     */
    void
    (*format)(Log4gPatternConverter *self, GString *string,
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
 * \brief Invokes the virtual function _Log4gPatternConverterClass::convert().
 *
 * \param self [in] A pattern converter object.
 * \param event [in] The log event to convert.
 *
 * \return The converted pattern.
 */
const gchar *
log4g_pattern_converter_convert(Log4gPatternConverter *self,
        Log4gLoggingEvent *event);

/**
 * \brief Invokes the virtual function _Log4gPatternConverterClass::format().
 *
 * \param self [in] A pattern converter object.
 * \param string [out] The formatted output is placed here.
 * \param event [in] The log event to format.
 */
void
log4g_pattern_converter_format(Log4gPatternConverter *self,
        GString *buffer, Log4gLoggingEvent *event);

/**
 * \brief Retrieve the next pattern converter in the chain.
 *
 * \param self [in] A pattern converter object.
 *
 * \return The next pattern converter in the chain.
 */
Log4gPatternConverter *
log4g_pattern_converter_get_next(Log4gPatternConverter *self);

/**
 * \brief Set the next pattern converter in the chain.
 *
 * \param self [in] A pattern converter object.
 * \param next [in] The next pattern converter in the chain.
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

/** \brief Conversion types */
typedef enum {
    INVALID_CONVERTER = 0, /**< Sentinel value */
    RELATIVE_TIME_CONVERTER, /**< Time converter */
    THREAD_CONVERTER, /**< Thread converter */
    LEVEL_CONVERTER, /**< Log level converter */
    NDC_CONVERTER, /**< Nested data context converter */
    MESSAGE_CONVERTER, /**< Log message converter */
    FULL_LOCATION_CONVERTER, /**< Log location converter */
    METHOD_LOCATION_CONVERTER, /**< Function name converter */
    LINE_LOCATION_CONVERTER, /**< Line number converter */
    FILE_LOCATION_CONVERTER, /**< File name converter */
    MAX_CONVERTER /**< Sentinel value */
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
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gBasicPatternConverterClass definition */
struct _Log4gBasicPatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_basic_pattern_converter_get_type(void);

/**
 * \brief Create a new basic pattern converter object.
 *
 * \param formatting [in] Formatting parameters.
 * \param type [in] The type of converter to create.
 *
 * \return A new basic pattern converter object.
 */
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

/** \brief Log4gLiteralPatternConverter object type definition */
typedef struct _Log4gLiteralPatternConverter Log4gLiteralPatternConverter;

/** \brief Log4gLiteralPatternConverter class type definition */
typedef struct _Log4gLiteralPatternConverterClass
        Log4gLiteralPatternConverterClass;

/** \brief Log4gLiteralPatternConverterClass definition */
struct _Log4gLiteralPatternConverter {
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gLiteralPatternConverterClass definition */
struct _Log4gLiteralPatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_literal_pattern_converter_get_type(void);

/**
 * \brief Create a new literal pattern converter object.
 *
 * \param pattern [in] The literal text to "convert".
 *
 * \return A new literal pattern converter object.
 */
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

/** \brief Log4gDatePatternConverter object type definition */
typedef struct _Log4gDatePatternConverter Log4gDatePatternConverter;

/** \brief Log4gDatePatternConverter class type definition */
typedef struct _Log4gDatePatternConverterClass Log4gDatePatternConverterClass;

/** \brief Log4gDatePatternConverterClass definition */
struct _Log4gDatePatternConverter {
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gDatePatternConverterClass definition */
struct _Log4gDatePatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_date_pattern_converter_get_type(void);

/**
 * \brief Create a new date pattern converter.
 *
 * \param formatting [in] Formatting parameters.
 * \param format [in] A strfime(3) date conversion pattern.
 *
 * \return Create a new date pattern converter.
 *
 * \see strftime(3)
 */
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

/** \brief Log4gMDCPatternConverter object type definition */
typedef struct _Log4gMDCPatternConverter Log4gMDCPatternConverter;

/** \brief Log4gMDCPatternConverter class type definition */
typedef struct _Log4gMDCPatternConverterClass Log4gMDCPatternConverterClass;

/** \brief Log4gMDCPatternConverterClass definition */
struct _Log4gMDCPatternConverter {
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gMDCPatternConverterClass definition */
struct _Log4gMDCPatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_mdc_pattern_converter_get_type(void);

/**
 * \brief Create a new MDC (mapped data context) pattern converter object.
 *
 * \param formatting [in] Formatting parameters.
 * \param key [in] The MDC key to look up.
 *
 * \return A new MDC pattern converter object.
 *
 * \see log4g/mdc.h
 */
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

/** \brief Log4gLocationPatternConverter object type definition */
typedef struct _Log4gLocationPatternConverter Log4gLocationPatternConverter;

/** \brief Log4gLocationPatternConverter class type definition */
typedef struct _Log4gLocationPatternConverterClass
        Log4gLocationPatternConverterClass;

/** \brief Log4gLocationPatternConverterClass definition */
struct _Log4gLocationPatternConverter {
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gLocationPatternConverterClass definition */
struct _Log4gLocationPatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_location_pattern_converter_get_type(void);

/**
 * \brief Create a new location pattern converter object.
 *
 * \param formatting [in] Formatting parameters.
 * \param type [in] The type of converter to create.
 *
 * \return A new location pattern converter object.
 */
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

/** \brief Log4gCategoryPatternConverter object type definition */
typedef struct _Log4gCategoryPatternConverter Log4gCategoryPatternConverter;

/** \brief Log4gCategoryPatternConverter class type definition */
typedef struct _Log4gCategoryPatternConverterClass
        Log4gCategoryPatternConverterClass;

/** \brief Log4gCategoryPatternConverterClass definition */
struct _Log4gCategoryPatternConverter {
    Log4gPatternConverter parent_instance;
};

/** \brief Log4gCategoryPatternConverterClass definition */
struct _Log4gCategoryPatternConverterClass {
    Log4gPatternConverterClass parent_class;
};

GType
log4g_category_pattern_converter_get_type(void);

/**
 * \brief Create a new logger category pattern converter object.
 *
 * \param formatting [in] Formatting parameters.
 * \param precision [in] The logger category precision value.
 *
 * \return A new logger category pattern converter object.
 *
 * \see log4g/logger.h, log4g/layout/pattern-layout.h
 */
Log4gPatternConverter *
log4g_category_pattern_converter_new(struct Log4gFormattingInfo *formatting,
        gint precision);

G_END_DECLS

#endif /* LOG4G_PATTERN_CONVERTER_H */
