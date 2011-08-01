/* Copyright 2010, 2011 Michael Steinert
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

#ifndef LOG4G_COUNTING_QUIET_WRITER_H
#define LOG4G_COUNTING_QUIET_WRITER_H

#include "helpers/quiet-writer.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_COUNTING_QUIET_WRITER \
	(log4g_counting_quiet_writer_get_type())

#define LOG4G_COUNTING_QUIET_WRITER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), \
		LOG4G_TYPE_COUNTING_QUIET_WRITER, Log4gCountingQuietWriter))

#define LOG4G_IS_COUNTING_QUIET_WRITER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), \
		LOG4G_TYPE_COUNTING_QUIET_WRITER))

#define LOG4G_COUNTING_QUIET_WRITER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_COUNTING_QUIET_WRITER, \
		Log4gCountingQuietWriterClass))

#define LOG4G_IS_COUNTING_QUIET_WRITER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_COUNTING_QUIET_WRITER))

#define LOG4G_COUNTING_QUIET_WRITER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), \
		LOG4G_TYPE_COUNTING_QUIET_WRITER, \
		Log4gCountingQuietWriterClass))

typedef struct Log4gCountingQuietWriter_ Log4gCountingQuietWriter;

typedef struct Log4gCountingQuietWriterClass_ Log4gCountingQuietWriterClass;

/**
 * Log4gCountingQuietWriter:
 *
 * The <structname>Log4gCountingQuietWriter</structname> structure does not
 * have any public members.
 */
struct Log4gCountingQuietWriter_ {
	/*< private >*/
	Log4gQuietWriter parent_instance;
	gpointer priv;
};

/**
 * Log4gCountingQuietWriterClass:
 *
 * The <structname>Log4gCountingQuietWriterClass</structname> structure does
 * not have any public members.
 */
struct Log4gCountingQuietWriterClass_ {
	/*< private >*/
	Log4gQuietWriterClass parent_class;
};

G_GNUC_INTERNAL GType
log4g_counting_quiet_writer_get_type(void);

G_GNUC_INTERNAL void
log4g_counting_quiet_writer_register(GTypeModule *module);

G_GNUC_INTERNAL Log4gQuietWriter *
log4g_counting_quiet_writer_new(FILE *file, GObject *error);

G_GNUC_INTERNAL gulong
log4g_counting_quiet_writer_get_count(Log4gQuietWriter *base);

G_GNUC_INTERNAL void
log4g_counting_quiet_writer_set_count(Log4gQuietWriter *base, gulong count);

G_END_DECLS

#endif /* LOG4G_COUNTING_QUIET_WRITER_H */
