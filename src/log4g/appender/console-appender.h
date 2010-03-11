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
 * \date 2-8-2010
 */

#ifndef LOG4G_CONSOLE_APPENDER_H
#define LOG4G_CONSOLE_APPENDER_H

#include <log4g/appender/writer-appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_CONSOLE_APPENDER \
    (log4g_console_appender_get_type())

#define LOG4G_CONSOLE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppender))

#define LOG4G_IS_CONSOLE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_CONSOLE_APPENDER))

#define LOG4G_CONSOLE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppenderClass))

#define LOG4G_IS_CONSOLE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_CONSOLE_APPENDER))

#define LOG4G_CONSOLE_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppenderClass))

/** \brief Log4gConsoleAppender object type definition */
typedef struct _Log4gConsoleAppender Log4gConsoleAppender;

/** \brief Log4gConsoleAppender class type definition */
typedef struct _Log4gConsoleAppenderClass Log4gConsoleAppenderClass;

/** \brief Log4gConsoleAppenderClass definition */
struct _Log4gConsoleAppender {
    Log4gWriterAppender parent_instance; /**< parent instance */
};

/** \brief Log4gConsoleAppenderClass definition */
struct _Log4gConsoleAppenderClass {
    Log4gWriterAppenderClass parent_class; /**< parent class */
    /*< private >*/
    gchar *SYSTEM_OUT;
    gchar *SYSTEM_ERR;
};

GType
log4g_console_appender_get_type(void);

/**
 */
Log4gAppender *
log4g_console_appender_new(Log4gLayout *layout, const char *target);

/**
 */
void
log4g_console_appender_set_target(Log4gAppender *self, const gchar *target);

/**
 */
const gchar *
log4g_console_appender_get_target(Log4gAppender *self);

/**
 */
void
log4g_console_appender_set_follow(Log4gAppender *base, gboolean follow);

/**
 */
gboolean
log4g_console_appender_get_follow(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_CONSOLE_APPENDER_H */
