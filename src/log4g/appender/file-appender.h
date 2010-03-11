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
 * \date 2-9-2010
 */

#ifndef LOG4G_FILE_APPENDER_H
#define LOG4G_FILE_APPENDER_H

#include <log4g/appender/writer-appender.h>

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

/** \brief Log4gFileAppender object type definition */
typedef struct _Log4gFileAppender Log4gFileAppender;

/** \brief Log4gFileAppender class type definition */
typedef struct _Log4gFileAppenderClass Log4gFileAppenderClass;

/** \brief Log4gFileAppenderClass definition */
struct _Log4gFileAppender {
    Log4gWriterAppender parent_instance; /**< parent instance */
};

/** \brief Log4gFileAppenderClass definition */
struct _Log4gFileAppenderClass {
    Log4gWriterAppenderClass parent_class; /**< parent class */
    void (*set_file_full)(Log4gAppender *base, const gchar *file,
            gboolean append, gboolean buffered, guint size);
    void (*set_qw_for_files)(Log4gAppender *base, FILE *file);
};

GType log4g_file_appender_get_type(void);

/**
 */
Log4gAppender *
log4g_file_appender_new(Log4gLayout *layout, const gchar *file,
        gboolean append, gboolean buffered);

/**
 */
void
log4g_file_appender_close_file(Log4gAppender *base);

/**
 */
void
log4g_file_appender_set_file_full(Log4gAppender *base, const gchar *file,
        gboolean append, gboolean buffered, guint size);

/**
 */
void
log4g_file_appender_set_file(Log4gAppender *base, const gchar *file);

/**
 */
const gchar *
log4g_file_appender_get_file(Log4gAppender *base);

/**
 */
void
log4g_file_appender_set_append(Log4gAppender *base, gboolean append);

/**
 */
gboolean
log4g_file_appender_get_append(Log4gAppender *base);

/**
 */
void
log4g_file_appender_set_buffered_io(Log4gAppender *base, gboolean buffered);

/**
 */
gboolean
log4g_file_appender_get_buffered_io(Log4gAppender *base);

/**
 */
void
log4g_file_appender_set_buffer_size(Log4gAppender *base, guint size);

/**
 */
guint
log4g_file_appender_get_buffer_size(Log4gAppender *base);

/**
 */
void
log4g_file_appender_set_qw_for_files(Log4gAppender *base, FILE *file);

G_END_DECLS

#endif /* LOG4G_FILE_APPENDER_H */
