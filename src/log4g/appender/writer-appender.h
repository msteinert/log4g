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

#ifndef LOG4G_WRITER_APPENDER_H
#define LOG4G_WRITER_APPENDER_H

#include <log4g/appender/appender-skeleton.h>
#include <log4g/helpers/quiet-writer.h>

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

/** \brief Log4gWriterAppender object type definition */
typedef struct _Log4gWriterAppender Log4gWriterAppender;

/** \brief Log4gWriterAppender class type definition */
typedef struct _Log4gWriterAppenderClass Log4gWriterAppenderClass;

/** \brief Log4gWriterAppenderClass definition */
struct _Log4gWriterAppender {
    Log4gAppenderSkeleton parent_instance; /**< parent instance */
};

/** \brief Log4gWriterAppenderClass definition */
struct _Log4gWriterAppenderClass {
    Log4gAppenderSkeletonClass parent_class; /**< parent class */
    void (*sub_append)(Log4gAppender *base, Log4gLoggingEvent *event);
    void (*close_writer)(Log4gAppender *base);
    void (*reset)(Log4gAppender *base);
};

GType
log4g_writer_appender_get_type(void);

/**
 */
Log4gAppender *
log4g_writer_appender_new(Log4gLayout *layout, FILE *file);

/**
 */
void
log4g_writer_appender_set_immediate_flush(Log4gAppender *base, gboolean flush);

/**
 */
gboolean
log4g_writer_appender_get_immediate_flush(Log4gAppender *base);

/**
 */
gboolean
log4g_writer_appender_check_entry_conditions(Log4gAppender *base);

/**
 */
void
log4g_writer_appender_set_writer(Log4gAppender *base, FILE *file);

/**
 */
void
log4g_writer_appender_close_writer(Log4gAppender *base);

/**
 */
void
log4g_writer_appender_sub_append(Log4gAppender *base, Log4gLoggingEvent *event);

/**
 */
void
log4g_writer_appender_reset(Log4gAppender *base);

/**
 */
void
log4g_writer_appender_write_footer(Log4gAppender *base);

/**
 */
void
log4g_writer_appender_write_header(Log4gAppender *base);

/*< protected >*/

/**
 */
Log4gQuietWriter *
log4g_writer_appender_get_quiet_writer(Log4gAppender *base);

/**
 */
void
log4g_writer_appender_set_quiet_writer(Log4gAppender *base,
        Log4gQuietWriter *writer);

G_END_DECLS

#endif /* LOG4G_WRITER_APPENDER_H */
