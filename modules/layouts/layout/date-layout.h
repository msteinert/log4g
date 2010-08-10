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

/**
 * Log4gDateLayoutType:
 * @INVALID_DATE_FORMAT: Senteinel value
 * @RELATIVE_TIME_DATE_FORMAT: Milliseconds since Log4g was initialized
 * @MAX_DATE_FORMAT: Sentinel value
 */
typedef enum {
    INVALID_DATE_FORMAT = 0,
    RELATIVE_TIME_DATE_FORMAT,
    MAX_DATE_FORMAT
} Log4gDateLayoutType;

typedef struct _Log4gDateLayout Log4gDateLayout;

typedef struct _Log4gDateLayoutClass Log4gDateLayoutClass;

/**
 * Log4gDateLayout:
 *
 * The <structname>Log4gDateLayout</structname> structure does not have any
 * public members.
 */
struct _Log4gDateLayout {
    /*< private >*/
    Log4gLayout parent_instance;
};

/**
 * Log4gDateLayoutClass:
 *
 * The <structname>Log4gDateLayoutClass</structname> structure does not have
 * any public members.
 */
struct _Log4gDateLayoutClass {
    /*< private >*/
    Log4gLayoutClass parent_class;
};

GType
log4g_date_layout_get_type(void);

void
log4g_date_layout_register(GTypeModule *module);

void
log4g_date_layout_date_format(Log4gLayout *base, GString *string,
        Log4gLoggingEvent *event);

G_END_DECLS

#endif /* LOG4G_DATE_LAYOUT_H */
