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

/**
 * SECTION: counting-quiet-writer
 * @short_description: Counts the number of bytes written
 * @see_also: #Log4gQuietWriterClass
 *
 * A quiet writer that counts the number of bytes it has written.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/error-handler.h"
#include "helpers/counting-quiet-writer.h"

G_DEFINE_DYNAMIC_TYPE(Log4gCountingQuietWriter, log4g_counting_quiet_writer,
		LOG4G_TYPE_QUIET_WRITER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_COUNTING_QUIET_WRITER, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gCountingQuietWriter *)instance)->priv)

struct Private {
	gulong count;
};

static void
log4g_counting_quiet_writer_init(Log4gCountingQuietWriter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
write_(Log4gQuietWriter *self, const gchar *string)
{
	LOG4G_QUIET_WRITER_CLASS(log4g_counting_quiet_writer_parent_class)->
		write(self, string);
	GET_PRIVATE(self)->count += strlen(string);
}

static void
log4g_counting_quiet_writer_class_init(Log4gCountingQuietWriterClass *klass)
{
	Log4gQuietWriterClass *qw_class = LOG4G_QUIET_WRITER_CLASS(klass);
	qw_class->write = write_;
	g_type_class_add_private(klass, sizeof(struct Private));
}

static void
log4g_counting_quiet_writer_class_finalize(
		G_GNUC_UNUSED Log4gCountingQuietWriterClass *klass)
{
	/* do nothing */
}

void
log4g_counting_quiet_writer_register(GTypeModule *module)
{
	log4g_counting_quiet_writer_register_type(module);
}

/**
 * log4g_counting_quiet_writer_new:
 * @file: The an open stdio(3) stream to write to.
 * @error: The error handler to use.
 *
 * Create a new counting quiet writer object.
 *
 * @See: stdio(3), #Log4gErrorHandlerInterface
 *
 * Returns: A new counting quiet writer object.
 * Since: 0.1
 */
Log4gQuietWriter *
log4g_counting_quiet_writer_new(FILE *file, GObject *error)
{
	Log4gQuietWriter *self;
	g_return_val_if_fail(file, NULL);
	g_return_val_if_fail(error, NULL);
	self = g_object_new(LOG4G_TYPE_COUNTING_QUIET_WRITER, NULL);
	if (!self) {
		return NULL;
	}
	log4g_quiet_writer_set_error_handler(self, error);
	log4g_quiet_writer_set_file(self, file);
	return self;
}

/**
 * log4g_counting_quiet_writer_get_count:
 * @base: A counting quiet writer object.
 *
 * Retrieve the number of bytes written.
 *
 * Returns: The number of bytes written by @base.
 * Since: 0.1
 */
gulong
log4g_counting_quiet_writer_get_count(Log4gQuietWriter *base)
{
	g_return_val_if_fail(LOG4G_IS_COUNTING_QUIET_WRITER(base), 0);
	return GET_PRIVATE(base)->count;
}

/**
 * log4g_counting_quiet_writer_set_count:
 * @base: A counting quiet writer object.
 * @count: The number of bytes @base will think it has written.
 *
 * Set the number of bytes a counting quiet writer thinks it has written.
 *
 * This function is mostly useful for resetting the count to zero.
 *
 * Since: 0.1
 */
void
log4g_counting_quiet_writer_set_count(Log4gQuietWriter *base, gulong count)
{
	g_return_if_fail(LOG4G_IS_COUNTING_QUIET_WRITER(base));
	GET_PRIVATE(base)->count = count;
}
