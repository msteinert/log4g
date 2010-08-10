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

#ifndef LOG4G_FILE_APPENDER_H
#define LOG4G_FILE_APPENDER_H

#include "appender/writer-appender.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_FILE_APPENDER \
    (log4g_file_appender_get_type())

#define LOG4G_FILE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_FILE_APPENDER, \
            Log4gFileAppender))

#define LOG4G_IS_FILE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_FILE_APPENDER))

#define LOG4G_FILE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_FILE_APPENDER, \
            Log4gFileAppenderClass))

#define LOG4G_IS_FILE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_FILE_APPENDER))

#define LOG4G_FILE_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_FILE_APPENDER, \
            Log4gFileAppenderClass))

typedef struct _Log4gFileAppender Log4gFileAppender;

typedef struct _Log4gFileAppenderClass Log4gFileAppenderClass;

/**
 * Log4gFileAppender:
 *
 * The <structname>Log4gFileAppender</structname> structure does not have any
 * public members.
 */
struct _Log4gFileAppender {
    /*< private >*/
    Log4gWriterAppender parent_instance;
};

/**
 * Log4gFileAppenderSetFileFull:
 * @base: A file appender object.
 * @file: The new value for the file property.
 * @append: The new value for the append property.
 * @buffered: The new value for the buffered-io property.
 * @size: The new value for the size property.
 *
 * The rolling file appender overloads this function.
 *
 * @See: #Log4gRollingFileAppender
 *
 * Since: 0.1
 */
typedef void
(*Log4gFileAppenderSetFileFull)(Log4gAppender *base, const gchar *file,
        gboolean append, gboolean buffered, guint size);

/**
 * Log4gFileAppenderSetQwForFiles:
 * @base: A file appender object.
 * @file: The open file handle to write to.
 *
 * The rolling file appender overloads this function.
 *
 * @See: #Log4gRollingFileAppender, #Log4gQuietWriter, stdio(3)
 *
 * Since: 0.1
 */
typedef void
(*Log4gFileAppenderSetQwForFiles)(Log4gAppender *base, FILE *file);

/**
 * Log4gFileAppenderClass:
 * @set_file_full: Set the file and other options.
 * @set_qw_for_files: Set the quiet writer used for output.
 */
struct _Log4gFileAppenderClass {
    /*< private >*/
    Log4gWriterAppenderClass parent_class;
    /*< public >*/
    Log4gFileAppenderSetFileFull set_file_full;
    Log4gFileAppenderSetQwForFiles set_qw_for_files;
};

GType
log4g_file_appender_get_type(void);

void
log4g_file_appender_register(GTypeModule *module);

void
log4g_file_appender_set_file_full(Log4gAppender *base, const gchar *file,
        gboolean append, gboolean buffered, guint size);

void
log4g_file_appender_close_file(Log4gAppender *base);

void
log4g_file_appender_set_qw_for_files(Log4gAppender *base, FILE *file);

const gchar *
log4g_file_appender_get_file(Log4gAppender *base);

gboolean
log4g_file_appender_get_buffered_io(Log4gAppender *base);

guint
log4g_file_appender_get_buffer_size(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_FILE_APPENDER_H */
