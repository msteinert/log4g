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
 * SECTION: quiet-writer
 * @short_description: Write to a stdio(3) stream
 *
 * The quiet writer class handles errors via an error handler.
 *
 * @See: #Log4gErrorHandlerInterface
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "helpers/quiet-writer.h"
#include "log4g/interface/error-handler.h"

G_DEFINE_DYNAMIC_TYPE(Log4gQuietWriter, log4g_quiet_writer, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_QUIET_WRITER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gQuietWriter *)instance)->priv)

struct Private {
	FILE *file;
	gpointer error;
};

static void
log4g_quiet_writer_init(Log4gQuietWriter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->error) {
		g_object_unref(priv->error);
		priv->error = NULL;
	}
	G_OBJECT_CLASS(log4g_quiet_writer_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	log4g_quiet_writer_close(LOG4G_QUIET_WRITER(base));
	G_OBJECT_CLASS(log4g_quiet_writer_parent_class)->finalize(base);
}

void
write_(Log4gQuietWriter *self, const char *string)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->file) {
		return;
	}
	if (EOF == fputs(string, priv->file)) {
		log4g_error_handler_error(priv->error, NULL,
				Q_("failed to write [%s]: %s"),
				string, g_strerror(errno));
	}
}

static void
log4g_quiet_writer_class_init(Log4gQuietWriterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	klass->write = write_;
	g_type_class_add_private(klass, sizeof(struct Private));
}

static void
log4g_quiet_writer_class_finalize(G_GNUC_UNUSED Log4gQuietWriterClass *klass)
{
	/* do nothing */
}

void
log4g_quiet_writer_register(GTypeModule *module)
{
	log4g_quiet_writer_register_type(module);
}

/**
 * log4g_quiet_writer_new:
 * @file: An open stdio(3) stream.
 * @error: The error handler to use.
 *
 * Create a new quiet writer object.
 *
 * @See: stdio(3)
 *
 * Returns: A new quiet writer object.
 * Since: 0.1
 */
Log4gQuietWriter *
log4g_quiet_writer_new(FILE *file, gpointer error)
{
	g_return_val_if_fail(file, NULL);
	g_return_val_if_fail(error, NULL);
	g_return_val_if_fail(LOG4G_IS_ERROR_HANDLER(error), NULL);
	Log4gQuietWriter *self = g_object_new(LOG4G_TYPE_QUIET_WRITER, NULL);
	if (!self) {
		return NULL;
	}
	log4g_quiet_writer_set_error_handler(self, error);
	GET_PRIVATE(self)->file = file;
	return self;
}

/**
 * log4g_quiet_writer_close:
 * @self: A quiet writer object.
 *
 * Close the stdio(3) stream held by a quiet writer object.
 *
 * @See: stdio(3)
 *
 * Since: 0.1
 */
void
log4g_quiet_writer_close(Log4gQuietWriter *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->file) {
		return;
	}
	if (EOF == fclose(priv->file)) {
		log4g_error_handler_error(priv->error, NULL,
				Q_("failed to close writer: %s"),
				g_strerror(errno));
	}
	priv->file = NULL;
}

/**
 * log4g_quiet_writer_write:
 * @self: A quiet writer object.
 * @string: The string to write.
 *
 * Call the @write function from the #Log4gQuietWriterClass of @self.
 *
 * Since: 0.1
 */
void
log4g_quiet_writer_write(Log4gQuietWriter *self, const char *string)
{
	g_return_if_fail(LOG4G_IS_QUIET_WRITER(self));
	LOG4G_QUIET_WRITER_GET_CLASS(self)->write(self, string);
}

/**
 * log4g_quiet_writer_flush:
 * @self: A quiet writer object.
 *
 * Flush the stdio(3) stream.
 *
 * @See: stdio(3)
 *
 * Since: 0.1
 */
void
log4g_quiet_writer_flush(Log4gQuietWriter *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->file) {
		return;
	}
	if (EOF == fflush(priv->file)) {
		log4g_error_handler_error(priv->error, NULL,
				Q_("failed to flush writer: %s"),
				g_strerror(errno));
	}
}

/**
 * log4g_quiet_writer_set_error_handler:
 * @self: A quiet writer object.
 * @error: A new error handler for @self.
 *
 * Set the error handler for a quiet writer.
 *
 * @See: #Log4gErrorHandlerInterface
 *
 * Since: 0.1
 */
void
log4g_quiet_writer_set_error_handler(Log4gQuietWriter *self, gpointer error)
{
	if (!error) {
		log4g_log_warn(Q_("attempted to set NULL error handler"));
	} else {
		struct Private *priv;
		g_return_if_fail(LOG4G_IS_ERROR_HANDLER(error));
		priv = GET_PRIVATE(self);
		if (priv->error) {
			g_object_unref(priv->error);
		}
		g_object_ref(error);
		priv->error = error;
	}
}

/**
 * log4g_quiet_writer_set_file:
 * @self: A quiet writer object.
 * @file: An open stdio(3) stream.
 *
 * Set the stdio(3) stream to write to.
 *
 * @See: stdio(3)
 *
 * Since: 0.1
 */
void
log4g_quiet_writer_set_file(Log4gQuietWriter *self, FILE *file)
{
	GET_PRIVATE(self)->file = file;
}
