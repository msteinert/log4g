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
 * \brief Backup log files when they reach a specified size.
 * \author Mike Steinert
 * \date 2-17-2010
 *
 * This class extends \ref log4g/appender/file-appender.h "file appender" to
 * backup log files when they reach a specified size.
 *
 * Rolling file appenders accept two properties:
 * -# max-backup-index
 * -# maximum-file-size
 *
 * The value of max-backup-index sets the number of backup files that will
 * be kept. For example if the value is ten, then eleven files will be kept
 * (indexes zero through ten). The default value is one.
 *
 * The log files will be rotated when the current log file reaches a size of
 * maximum-file-size or larger. The default value is ten megabytes.
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
    Log4gFileAppender parent_instance;
};

/** \brief Log4gRollingFileAppenderClass definition */
struct _Log4gRollingFileAppenderClass {
    Log4gFileAppenderClass parent_class;

    /**
     * \brief Roll the current log file over.
     *
     * Subclasses may overload this function to implement custom rollover
     * schemes.
     *
     * \param base [in] A rolling file appender object.
     */
    void
    (*roll_over)(Log4gAppender *base);
};

GType
log4g_rolling_file_appender_get_type(void);

/**
 * \brief Create a new rolling file appender object.
 *
 * \param layout [in] The layout to use.
 * \param file [in] The name of the file to write to.
 * \param append [in] The value of the append property.
 * \param buffered [in] The value of the buffered-io property.
 *
 * \return A new rolling file appender object.
 */
Log4gAppender *
log4g_rolling_file_appender_new(Log4gLayout *layout, const char *file,
        gboolean append, gboolean buffered);

/**
 * \brief Retrieve the max-backup-index property.
 *
 * \param base [in] A rolling file appender object.
 *
 * \return The max backup index for \e base.
 */
guint
log4g_rolling_file_appender_get_max_backup_index(Log4gAppender *base);

/**
 * \brief Set the max-backup-index property.
 * 
 * \param base [in] A rolling file appender object.
 * \param backup [in] The new max backup index for \e base.
 */
void
log4g_rolling_file_appender_set_max_backup_index(Log4gAppender *base,
        guint backup);

/**
 * \brief Retrieve the maximum-file-size property.
 *
 * \param base [in] A rolling file appender object.
 *
 * \return The maximum file size for \e base.
 */
gulong
log4g_rolling_file_appender_get_maximum_file_size(Log4gAppender *base);

/**
 * \brief Set the maximum-file-size property.
 *
 * \param base [in] A rolling file appender object.
 * \param max [in] The new maximum file size for \e base.
 */
void
log4g_rolling_file_appender_set_maximum_file_size(Log4gAppender *base,
        gulong max);

/**
 * \brief Invokes the virtual function _Log4gRollingFileAppender::roll_over().
 *
 * \param base [in] A rolling file appender object.
 */
void
log4g_rolling_file_appender_roll_over(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_ROLLING_FILE_APPENDER_H */
