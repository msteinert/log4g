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

#ifndef LOG4G_ROLLING_FILE_APPENDER_H
#define LOG4G_ROLLING_FILE_APPENDER_H

#include "appender/file-appender.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_ROLLING_FILE_APPENDER \
    (log4g_rolling_file_appender_get_type())

#define LOG4G_ROLLING_FILE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_ROLLING_FILE_APPENDER, \
            Log4gRollingFileAppender))

#define LOG4G_IS_ROLLING_FILE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_ROLLING_FILE_APPENDER))

#define LOG4G_ROLLING_FILE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_ROLLING_FILE_APPENDER, \
            Log4gRollingFileAppenderClass))

#define LOG4G_IS_ROLLING_FILE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_ROLLING_FILE_APPENDER))

#define LOG4G_ROLLING_FILE_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_ROLLING_FILE_APPENDER, \
            Log4gRollingFileAppenderClass))

typedef struct _Log4gRollingFileAppender Log4gRollingFileAppender;

typedef struct _Log4gRollingFileAppenderClass Log4gRollingFileAppenderClass;

/**
 * Log4gRollingFileAppender:
 *
 * The <structname>Log4gRollingFileAppender</structname> structure does not
 * have any public members.
 */
struct _Log4gRollingFileAppender {
    /*< private >*/
    Log4gFileAppender parent_instance;
};

/**
 * Log4gRollingFileAppenderRollOver:
 * @base: A rolling file appender object.
 *
 * Subclasses may overload this function to implement custom rollover schemes.
 *
 * Since: 0.1
 */
typedef void
(*Log4gRollingFileAppenderRollOver)(Log4gAppender *base);

/**
 * Log4gRollingFileAppenderClass:
 * @roll_over: Roll the current log file over.
 */
struct _Log4gRollingFileAppenderClass {
    /*< private >*/
    Log4gFileAppenderClass parent_class;
    /*< public >*/
    Log4gRollingFileAppenderRollOver roll_over;
};

GType
log4g_rolling_file_appender_get_type(void);

void
log4g_rolling_file_appender_register(GTypeModule *module);

void
log4g_rolling_file_appender_roll_over(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_ROLLING_FILE_APPENDER_H */
