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

#ifndef LOG4G_WRITER_APPENDER_H
#define LOG4G_WRITER_APPENDER_H

#include "helpers/quiet-writer.h"
#include <log4g/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_WRITER_APPENDER \
    (log4g_writer_appender_get_type())

#define LOG4G_WRITER_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_WRITER_APPENDER, \
            Log4gWriterAppender))

#define LOG4G_IS_WRITER_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_WRITER_APPENDER))

#define LOG4G_WRITER_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_WRITER_APPENDER, \
            Log4gWriterAppenderClass))

#define LOG4G_IS_WRITER_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_WRITER_APPENDER))

#define LOG4G_WRITER_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_WRITER_APPENDER, \
            Log4gWriterAppenderClass))

typedef struct _Log4gWriterAppender Log4gWriterAppender;

typedef struct _Log4gWriterAppenderClass Log4gWriterAppenderClass;

struct _Log4gWriterAppender {
    /*< private >*/
    Log4gAppender parent_instance;
};

/**
 * Log4gWriterAppenderSubAppend:
 * @base: A writer appender object.
 * @event: The log event to append.
 *
 * Actual writing occurs here.
 *
 * Most subclasses of writer appender will need to override this method.
 *
 * Since: 0.1
 */
typedef void
(*Log4gWriterAppenderSubAppend)(Log4gAppender *base, Log4gLoggingEvent *event);

/**
 * Log4gWriterAppenderCloseWriter:
 * @base: A writer appender object.
 *
 * Close the underlying stdio(3) stream.
 *
 * Since: 0.1
 */
typedef void
(*Log4gWriterAppenderCloseWriter)(Log4gAppender *base);

/**
 * Log4gWriterAppenderReset:
 * @base: A writer appender object.
 *
 * Clear internal references to the writer and other variables.
 *
 * Subclasses can override this method for alternate closing behavior.
 *
 * Since: 0.1
 */
typedef void
(*Log4gWriterAppenderReset)(Log4gAppender *base);

/**
 * Log4gWriterAppenderClass:
 * @sub_append: Actual writing occurs here.
 * @close_writer: Close the underlying stream.
 * @reset: Clear internal references and variables.
 */
struct _Log4gWriterAppenderClass {
    /*< private >*/
    Log4gAppenderClass parent_class;
    /*< public >*/
    Log4gWriterAppenderSubAppend sub_append;
    Log4gWriterAppenderCloseWriter close_writer;
    Log4gWriterAppenderReset reset;
};

GType
log4g_writer_appender_get_type(void);

void
log4g_writer_appender_register(GTypeModule *module);

gboolean
log4g_writer_appender_check_entry_conditions(Log4gAppender *base);

void
log4g_writer_appender_set_writer(Log4gAppender *base, FILE *file);

void
log4g_writer_appender_close_writer(Log4gAppender *base);

void
log4g_writer_appender_sub_append(Log4gAppender *base,
        Log4gLoggingEvent *event);

void
log4g_writer_appender_reset(Log4gAppender *base);

void
log4g_writer_appender_write_footer(Log4gAppender *base);

void
log4g_writer_appender_write_header(Log4gAppender *base);

Log4gQuietWriter *
log4g_writer_appender_get_quiet_writer(Log4gAppender *base);

void
log4g_writer_appender_set_quiet_writer(Log4gAppender *base,
        Log4gQuietWriter *writer);

G_END_DECLS

#endif /* LOG4G_WRITER_APPENDER_H */
