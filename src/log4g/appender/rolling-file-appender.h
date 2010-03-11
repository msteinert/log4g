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
 * \date 2-17-2010
 */

#ifndef LOG4G_ROLLING_FILE_APPENDER_H
#define LOG4G_ROLLING_FILE_APPENDER_H

#include <log4g/appender/file-appender.h>

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

/** \brief Log4gRollingFileAppender object type definition */
typedef struct _Log4gRollingFileAppender Log4gRollingFileAppender;

/** \brief Log4gRollingFileAppender class type definition */
typedef struct _Log4gRollingFileAppenderClass Log4gRollingFileAppenderClass;

/** \brief Log4gRollingFileAppenderClass definition */
struct _Log4gRollingFileAppender {
    Log4gFileAppender parent_instance; /**< parent instance */
};

/** \brief Log4gRollingFileAppenderClass definition */
struct _Log4gRollingFileAppenderClass {
    Log4gFileAppenderClass parent_class; /**< parent class */
    void (*roll_over)(Log4gAppender *base);
};

GType
log4g_rolling_file_appender_get_type(void);

/**
 */
Log4gAppender *
log4g_rolling_file_appender_new(Log4gLayout *layout, const char *file,
        gboolean append, gboolean buffered);

/**
 */
guint
log4g_rolling_file_appender_get_max_backup_index(Log4gAppender *base);

/**
 */
void
log4g_rolling_file_appender_set_max_backup_index(Log4gAppender *base,
        guint backup);

/**
 */
gulong
log4g_rolling_file_appender_get_maximum_file_size(Log4gAppender *base);

/**
 */
void
log4g_rolling_file_appender_set_maximum_file_size(Log4gAppender *base,
        gulong max);

/**
 */
void
log4g_rolling_file_appender_roll_over(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_ROLLING_FILE_APPENDER_H */
