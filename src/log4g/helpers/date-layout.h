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
 * \brief A layout that formats dates.
 * \author Mike Steinert
 * \date 2-5-2010
 *
 * This is an abstract layout class that takes care of all date related
 * options and formatting. This class understands date formats understood
 * by strftime(3).
 *
 * Date layouts accept two properties:
 * -# date-format
 * -# time-zone
 *
 * The date-format property set the strftime(3) confversion pattern that will
 * be used to format the date.
 *
 * The time-zone property can be used to explicitly set the timezone. The
 * default is \e NULL.
 */

#ifndef LOG4G_DATE_LAYOUT_H
#define LOG4G_DATE_LAYOUT_H

#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DATE_LAYOUT \
    (log4g_date_layout_get_type())

#define LOG4G_DATE_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_DATE_LAYOUT, \
            Log4gDateLayout))

#define LOG4G_IS_DATE_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_DATE_LAYOUT))

#define LOG4G_DATE_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DATE_LAYOUT, \
            Log4gDateLayoutClass))

#define LOG4G_IS_DATE_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DATE_LAYOUT))

#define LOG4G_DATE_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_DATE_LAYOUT, \
            Log4gDateLayoutClass))

/** \brief ... */
typedef enum {
    INVALID_DATE_FORMAT = 0, /**< [private] Sentinel value */
    RELATIVE_TIME_DATE_FORMAT, /**< Milliseconds since Log4g was initialized */
    MAX_DATE_FORMAT /**< [private] Sentinel value */
} Log4gDateLayoutType;

/** \brief Log4gDateLayout object type definition */
typedef struct _Log4gDateLayout Log4gDateLayout;

/** \brief Log4gDateLayout class type definition */
typedef struct _Log4gDateLayoutClass Log4gDateLayoutClass;

/** \brief Log4gDateLayoutClass definition */
struct _Log4gDateLayout {
    Log4gLayout parent_instance; /**< parent instance */
    /*< private >*/
    Log4gDateLayoutType type;
    gchar *format;
    gchar *tz;
};

/** \brief Log4gDateLayoutClass definition */
struct _Log4gDateLayoutClass {
    Log4gLayoutClass parent_class; /**< parent class */
};

GType
log4g_date_layout_get_type(void);

/**
 * \brief Set the date-format property.
 *
 * \param self [in] A date format object.
 * \param format [in] The new date format for \e self.
 */
void
log4g_date_layout_set_date_format(Log4gLayout *self, const char *format);

/**
 * \brief Get the date-format property.
 *
 * \param self [in] A date format object.
 *
 * \return The date format for \e self.
 */
const char *
log4g_date_layout_get_date_format(Log4gLayout *self);

/**
 * \brief Set the time-zone property.
 *
 * \param self [in] A date format object.
 * \param tz [in] The new time zone for \e self.
 */
void
log4g_date_layout_set_time_zone(Log4gLayout *self, const char *tz);

/**
 * \brief Get the time-zone property.
 *
 * \param self [in] A date format object.
 *
 * \return The time zone for \e self.
 */
const char *
log4g_date_layout_get_time_zone(Log4gLayout *self);

/**
 * \brief Format a date layout for a log event info a buffer.
 *
 * \param self [in] A date format object.
 * \param buffer [in] The buffer in which to layout the date.
 * \param even [in] The log event to get the date from.
 */
void
log4g_date_layout_date_format(Log4gLayout *self, GString *buffer,
        Log4gLoggingEvent *event);

G_END_DECLS

#endif /* LOG4G_DATE_LAYOUT_H */
