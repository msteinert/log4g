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
 * \brief Append to a stdio(3) stream.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * This class is a base class for appenders that write to stdio(3) streams.
 *
 * Writer appenders accept one property:
 * -# immediate-flush
 *
 * The value of immediate-flush determines if the I/O stream will be flushed
 * after each write. The default value is \e TRUE.
 *
 * \see stdio(3)
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
    Log4gAppenderSkeleton parent_instance;
};

/** \brief Log4gWriterAppenderClass definition */
struct _Log4gWriterAppenderClass {
    Log4gAppenderSkeletonClass parent_class;
    /**
     * \brief Actual writing occurs here.
     *
     * Most subclasses of writer appender will need to override this method.
     *
     * \param base [in] A writer appender object.
     * \param event [in] The log event to append.
     */
    void (*sub_append)(Log4gAppender *base, Log4gLoggingEvent *event);
    /**
     * \brief Close the underlying stdio(3) stream.
     *
     * \param base [in] A writer appender object.
     */
    void (*close_writer)(Log4gAppender *base);
    /**
     * \brief Clear internal references to the writer and other variables.
     *
     * Subclasses can override this method for alternate closing behavior.
     *
     * \param base [in] A writer appender object.
     */
    void (*reset)(Log4gAppender *base);
};

GType
log4g_writer_appender_get_type(void);

/**
 * \brief Create a new writer appender object.
 *
 * \param layout [in] The layout to use.
 * \param file [in] An open stdio(3) file handle.
 *
 * \return A new writer appender object.
 */
Log4gAppender *
log4g_writer_appender_new(Log4gLayout *layout, FILE *file);

/**
 * \brief Set the immediate-flush property.
 *
 * \param base [in] A writer appender object.
 * \param flush [in] The new immediate flush value for \e base.
 */
void
log4g_writer_appender_set_immediate_flush(Log4gAppender *base, gboolean flush);

/**
 * \brief Retrieve the immediate-flush property.
 *
 * \param base [in] A writer appender object.
 *
 * \return The immediate flush value for \e base.
 */
gboolean
log4g_writer_appender_get_immediate_flush(Log4gAppender *base);

/**
 * \brief Determine if it is OK to append.
 *
 * Checks the following conditions:
 * - The output target is set
 * - The layout is set
 * 
 * \param base [in] A writer appender object.
 *
 * \return \e TRUE if all entry conditions pass, \e FALSE otherwise.
 */
gboolean
log4g_writer_appender_check_entry_conditions(Log4gAppender *base);

/**
 * \brief Set the stdio(3) stream to use.
 *
 * \param base [in] A writer appender object.
 * \param file [in] An open stdio(3) stream.
 *
 * \see stdio(3)
 */
void
log4g_writer_appender_set_writer(Log4gAppender *base, FILE *file);

/**
 * \brief Invokes the virtual function _Log4gWriterAppender::close_writer().
 *
 * \param base [in] A writer appender object.
 */
void
log4g_writer_appender_close_writer(Log4gAppender *base);

/**
 * \brief Invokes the virtual function _Log4gWriterAppender::sub_append().
 *
 * \param base [in] A writer appender object.
 * \param event [in] The log event to append.
 */
void
log4g_writer_appender_sub_append(Log4gAppender *base,
        Log4gLoggingEvent *event);

/**
 * \brief Invokes the virtual function _Log4gWriterAppender::reset().
 *
 * \param base [in] A writer appender object.
 */
void
log4g_writer_appender_reset(Log4gAppender *base);

/**
 * \brief Write the layout footer.
 *
 * \param base [in] A writer appender object.
 */
void
log4g_writer_appender_write_footer(Log4gAppender *base);

/**
 * \brief Write the layout header.
 *
 * \param base [in] A writer appender object.
 */
void
log4g_writer_appender_write_header(Log4gAppender *base);

/**
 * \brief [protected] Retrieve the current quiet writer object.
 *
 * \param base [in] A writer appender object.
 *
 * \return The current quiet writer object used by \e base.
 *
 * \see log4g/helpers/quiet-writer.h
 */
Log4gQuietWriter *
log4g_writer_appender_get_quiet_writer(Log4gAppender *base);

/**
 * \brief [protected] Set the quiet writer to use.
 *
 * \param base [in] A writer appender object.
 * \param writer [in] A quiet writer.
 *
 * \see log4g/helpers/quiet-writer.h
 */
void
log4g_writer_appender_set_quiet_writer(Log4gAppender *base,
        Log4gQuietWriter *writer);

G_END_DECLS

#endif /* LOG4G_WRITER_APPENDER_H */
