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
 * \brief Write to a stdio(3) stream.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * The quiet writer class handles errors via an error handler.
 *
 * \see log4g/interface/error-handler.h
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

/** \brief Log4gQuietWriter object type definition */
typedef struct _Log4gQuietWriter Log4gQuietWriter;

/** \brief Log4gQuietWriter class type definition */
typedef struct _Log4gQuietWriterClass Log4gQuietWriterClass;

/** \brief Log4gQuietWriterClass definition */
struct _Log4gQuietWriter {
    GObject parent_instance;
};

/** \brief Log4gQuietWriterClass definition */
struct _Log4gQuietWriterClass {
    GObjectClass parent_class;

    /**
     * \brief Write a string to a stdio(3) stream.
     *
     * \param self [in] A quiet writer object.
     * \param string [in] The string to write.
     */
    void
    (*write)(Log4gQuietWriter *self, const gchar *string);
};

GType
log4g_quiet_writer_get_type(void);

void
log4g_quiet_writer_register(GTypeModule *module);

/**
 * \brief Create a new quiet writer object.
 *
 * \param file [in] An open stdio(3) stream.
 * \param error [in] The error handler to use.
 *
 * \return A new quiet writer object.
 *
 * \see stdio(3), log4g/interface/quiet-writer.h
 */
Log4gQuietWriter *
log4g_quiet_writer_new(FILE *file, gpointer error);

/**
 * \brief Close the stdio(3) stream held by a quiet writer object.
 *
 * \param self [in] A quiet writer object.
 *
 * \see stdio(3)
 */
void
log4g_quiet_writer_close(Log4gQuietWriter *self);

/**
 * \brief Invokes the virtual function _Log4gQuietWriterClass::write().
 *
 * \param self [in] A quiet writer object.
 * \param string [in] The string to write.
 */
void
log4g_quiet_writer_write(Log4gQuietWriter *self, const char *string);

/**
 * \brief Flush the stdio(3) stream.
 *
 * \param self [in] A quiet writer object.
 *
 * \see stdio(3)
 */
void
log4g_quiet_writer_flush(Log4gQuietWriter *self);

/**
 * \brief Set the error handler for a quiet writer.
 *
 * \param self [in] A quiet writer object.
 * \param error [in] A new error handler for \e self.
 *
 * \see log4g/interface/error-handler.h
 */
void
log4g_quiet_writer_set_error_handler(Log4gQuietWriter *self, gpointer error);

/**
 * \brief [protected] Set the stdio(3) stream to write to.
 *
 * \param self [in] A quiet writer object.
 * \param file [in] An open stdio(3) stream.
 *
 * \see stdio(3)
 */
void
log4g_quiet_writer_set_file(Log4gQuietWriter *self, FILE *file);

G_END_DECLS

#endif /* LOG4G_QUIET_WRITER_H */
