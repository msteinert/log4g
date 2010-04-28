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
 * \brief Log events to a file.
 * \author Mike Steinert
 * \date 2-9-2010
 *
 * The file appender logs events to a regular text file.
 *
 * File appenders accept the following properties:
 * -# file
 * -# append
 * -# buffered-io
 * -# buffer-size
 *
 * The value of file specifies the location of the output. This may be an
 * absolute or relative path.
 *
 * The value of append determines if the file will be truncated when it is
 * opened for writing. The default value is \e TRUE (i.e. do not truncate).
 *
 * The value of buffered-io determines if file I/O will be buffered. Buffered
 * I/O may exhibit improved performance, however messages in the buffer will
 * be lost if your application crashes. Buffering is turned off by default.
 *
 * The buffer-size property controls the size of the I/O buffer. The default
 * value is eight kilobytes (8192 bytes).
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
    Log4gWriterAppender parent_instance;
};

/** \brief Log4gFileAppenderClass definition */
struct _Log4gFileAppenderClass {
    Log4gWriterAppenderClass parent_class;
    /**
     * \brief Set the file and other options.
     *
     * The rolling file appender overloads this function.
     *
     * \param base [in] A file appender object.
     * \param file [in] The new value for the file property.
     * \param append [in] The new value for the append property.
     * \param buffered [in] The new value for the buffered-io property.
     * \param size [in] The new value for the size property.
     *
     * \see log4g/appender/rolling-file-appender.h
     */
    void (*set_file_full)(Log4gAppender *base, const gchar *file,
            gboolean append, gboolean buffered, guint size);
    /**
     * \brief Set the quiet writer used for output.
     *
     * The rolling file appender overloads this function.
     *
     * \param base [in] A file appender object.
     * \param file [in] The open file handle to write to.
     *
     * \see log4g/appender/rolling-file-appender.h,
     *      log4g/helpers/quiet-writer.h, stdio(3)
     */
    void (*set_qw_for_files)(Log4gAppender *base, FILE *file);
};

GType log4g_file_appender_get_type(void);

/**
 * \brief Create a new file appender object.
 *
 * \param layout [in] The layout to use.
 * \param file [in] The name of the file to log to.
 * \param append [in] The value for the append property.
 * \param buffered [in] The value for the buffered-io property.
 *
 * \return A new file appender object.
 */
Log4gAppender *
log4g_file_appender_new(Log4gLayout *layout, const gchar *file,
        gboolean append, gboolean buffered);

/**
 * \brief Close the log file.
 *
 * Once a file appender is closed it is no longer usable.
 *
 * \param base [in] A file appender object.
 */
void
log4g_file_appender_close_file(Log4gAppender *base);

/**
 * \brief Invokes the virtual function _Log4gFileAppender::set_file_full().
 *
 * \param base [in] A file appender object.
 * \param file [in] The new value for the file property.
 * \param append [in] The new value for the append property.
 * \param buffered [in] The new value for the buffered-io property.
 * \param size [in] The new value for the size property.
 */
void
log4g_file_appender_set_file_full(Log4gAppender *base, const gchar *file,
        gboolean append, gboolean buffered, guint size);

/**
 * \brief Set the file property.
 *
 * \param base [in] A file appender object.
 * \param file [in] The new file to append to.
 */
void
log4g_file_appender_set_file(Log4gAppender *base, const gchar *file);

/**
 * \brief Retrieve the file property.
 *
 * \param base [in] A file appender object.
 *
 * \return The name of the file being appended to.
 */
const gchar *
log4g_file_appender_get_file(Log4gAppender *base);

/**
 * \brief Set the append property.
 *
 * \param base [in] A file appender object.
 * \brief append [in] The new append value for \e base.
 */
void
log4g_file_appender_set_append(Log4gAppender *base, gboolean append);

/**
 * \brief Retrieve the append property.
 *
 * \param base [in] A file appender object.
 *
 * \return The append value for \e base.
 */
gboolean
log4g_file_appender_get_append(Log4gAppender *base);

/**
 * \brief Set the buffered-io property.
 *
 * \param base [in] A file appender object.
 * \parma buffered [in] The new buffered-io value for \e base.
 */
void
log4g_file_appender_set_buffered_io(Log4gAppender *base, gboolean buffered);

/**
 * \brief Retrieve the buffered-io property.
 *
 * \param base [in] A file appender object.
 *
 * \return The buffered-io value for \e base.
 */
gboolean
log4g_file_appender_get_buffered_io(Log4gAppender *base);

/**
 * \brief Set the buffer-size property.
 *
 * \param base [in] A file appender object.
 * \param size [in] The new buffer-size value for \e base.
 */
void
log4g_file_appender_set_buffer_size(Log4gAppender *base, guint size);

/**
 * \brief Retrieve the buffer-size property.
 *
 * \param base [in] A file appender object.
 *
 * \return The buffer-size value for \e base.
 */
guint
log4g_file_appender_get_buffer_size(Log4gAppender *base);

/**
 * \brief Invokes the virtual function
 *        _Log4gFileAppenderClass::set_qw_for_files().
 *
 * \param base [in] A file appender object.
 * \param file [in] An open file descriptor.
 */
void
log4g_file_appender_set_qw_for_files(Log4gAppender *base, FILE *file);

G_END_DECLS

#endif /* LOG4G_FILE_APPENDER_H */
