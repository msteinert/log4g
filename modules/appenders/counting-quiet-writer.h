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
 * \brief Counts the number of bytes written.
 * \author Mike Steinert
 * \date 2-17-2010
 *
 * A quiet writer that counts the number of bytes it has written.
 *
 * \see log4g/helpers/quiet-writer.h
 */

#ifndef LOG4G_COUNTING_QUIET_WRITER_H
#define LOG4G_COUNTING_QUIET_WRITER_H

#include "quiet-writer.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_COUNTING_QUIET_WRITER \
    (log4g_counting_quiet_writer_get_type())

#define LOG4G_COUNTING_QUIET_WRITER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_COUNTING_QUIET_WRITER, \
            Log4gCountingQuietWriter))

#define LOG4G_IS_COUNTING_QUIET_WRITER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_COUNTING_QUIET_WRITER))

#define LOG4G_COUNTING_QUIET_WRITER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_COUNTING_QUIET_WRITER, \
            Log4gCountingQuietWriterClass))

#define LOG4G_IS_COUNTING_QUIET_WRITER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_COUNTING_QUIET_WRITER))

#define LOG4G_COUNTING_QUIET_WRITER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_COUNTING_QUIET_WRITER, \
            Log4gCountingQuietWriterClass))

/** \brief Log4gCountingQuietWriter object type definition */
typedef struct _Log4gCountingQuietWriter Log4gCountingQuietWriter;

/** \brief Log4gCountingQuietWriter class type definition */
typedef struct _Log4gCountingQuietWriterClass Log4gCountingQuietWriterClass;

/** \brief Log4gCountingQuietWriterClass definition */
struct _Log4gCountingQuietWriter {
    Log4gQuietWriter parent_instance;
};

/** \brief Log4gCountingQuietWriterClass definition */
struct _Log4gCountingQuietWriterClass {
    Log4gQuietWriterClass parent_class;
};

GType
log4g_counting_quiet_writer_get_type(void);

/**
 * \brief Create a new counting quiet writer object.
 *
 * \param file [in] The an open stdio(3) stream to write to.
 * \param error [in] The error handler to use.
 *
 * \return A new counting quiet writer object.
 *
 * \see stdio(3), log4g/interface/error-handler.h
 */
Log4gQuietWriter *
log4g_counting_quiet_writer_new(FILE *file, GObject *error);

/**
 * \brief Retrieve the number of bytes written.
 *
 * \param base [in] A counting quiet writer object.
 *
 * \return The number of bytes written by \e base.
 */
gulong
log4g_counting_quiet_writer_get_count(Log4gQuietWriter *base);

/**
 * \brief Set the number of bytes a counting quiet writer thinks it has
 *        written.
 *
 * This function is mostly useful for resetting the count to zero.
 *
 * \param base [in] A counting quiet writer object.
 * \param count [in] The number of bytes \e self will think it has written.
 */
void
log4g_counting_quiet_writer_set_count(Log4gQuietWriter *base, gulong count);

G_END_DECLS

#endif /* LOG4G_COUNTING_QUIET_WRITER_H */
