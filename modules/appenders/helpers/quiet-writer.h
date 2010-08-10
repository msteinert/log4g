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

#ifndef LOG4G_QUIET_WRITER_H
#define LOG4G_QUIET_WRITER_H

#include <glib-object.h>
#include <stdio.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_QUIET_WRITER \
    (log4g_quiet_writer_get_type())

#define LOG4G_QUIET_WRITER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_QUIET_WRITER, \
            Log4gQuietWriter))

#define LOG4G_IS_QUIET_WRITER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_QUIET_WRITER))

#define LOG4G_QUIET_WRITER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_QUIET_WRITER, \
            Log4gQuietWriterClass))

#define LOG4G_IS_QUIET_WRITER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_QUIET_WRITER))

#define LOG4G_QUIET_WRITER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_QUIET_WRITER, \
            Log4gQuietWriterClass))

typedef struct _Log4gQuietWriter Log4gQuietWriter;

typedef struct _Log4gQuietWriterClass Log4gQuietWriterClass;

/**
 * Log4gQuietWriter:
 *
 * The <structname>Log4gQuietWriter</structname> structure does not have any
 * public members.
 */
struct _Log4gQuietWriter {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gQuietWriterWrite:
 * @self: A quiet writer object.
 * @string: The string to write.
 *
 * Write a string to a stdio(3) stream.
 *
 * Since: 0.1
 */
typedef void
(*Log4gQuietWriterWrite)(Log4gQuietWriter *self, const gchar *string);

/**
 * Log4gQuietWriterClass:
 * @write: Write to a stdio(3) stream.
 */
struct _Log4gQuietWriterClass {
    /*< private >*/
    GObjectClass parent_class;
    /*< public >*/
    Log4gQuietWriterWrite write;
};

GType
log4g_quiet_writer_get_type(void);

void
log4g_quiet_writer_register(GTypeModule *module);

Log4gQuietWriter *
log4g_quiet_writer_new(FILE *file, gpointer error);

void
log4g_quiet_writer_close(Log4gQuietWriter *self);

void
log4g_quiet_writer_write(Log4gQuietWriter *self, const char *string);

void
log4g_quiet_writer_flush(Log4gQuietWriter *self);

void
log4g_quiet_writer_set_error_handler(Log4gQuietWriter *self, gpointer error);

void
log4g_quiet_writer_set_file(Log4gQuietWriter *self, FILE *file);

G_END_DECLS

#endif /* LOG4G_QUIET_WRITER_H */
