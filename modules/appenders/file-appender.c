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
 * SECTION: file-appender
 * @short_description: Log events to a file
 *
 * The file appender logs events to a regular text file.
 *
 * File appenders accept the following properties:
 * <orderedlist>
 * <listitem><para>file</para></listitem>
 * <listitem><para>append</para></listitem>
 * <listitem><para>buffered-io</para></listitem>
 * <listitem><para>buffer-size</para></listitem>
 * </orderedlist>
 *
 * The value of file specifies the location of the output. This may be an
 * absolute or relative path.
 *
 * The value of append determines if the file will be truncated when it is
 * opened for writing. The default value is %TRUE (i.e. do not truncate).
 *
 * The value of buffered-io determines if file I/O will be buffered. Buffered
 * I/O may exhibit improved performance, however messages in the buffer will
 * be lost if your application crashes. Buffering is turned off by default.
 *
 * The buffer-size property controls the size of the I/O buffer. The default
 * value is eight kilobytes (8192 bytes).
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/file-appender.h"
#include <errno.h>

G_DEFINE_DYNAMIC_TYPE(Log4gFileAppender, log4g_file_appender,
        LOG4G_TYPE_WRITER_APPENDER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_FILE_APPENDER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gFileAppender *)instance)->priv)

struct Private {
	gboolean append;
	gchar *file;
	gboolean buffered;
	guint size;
	GMutex *lock;
};

static void
log4g_file_appender_init(Log4gFileAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->append = TRUE;
	priv->size = 8 * 1024;
	if (g_thread_supported()) {
		priv->lock = g_mutex_new();
	}
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	log4g_appender_close(LOG4G_APPENDER(base));
	g_free(priv->file);
	if (priv->lock) {
		g_mutex_free(priv->lock);
	}
	G_OBJECT_CLASS(log4g_file_appender_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_FILE,
	PROP_APPEND,
	PROP_BUFFERED_IO,
	PROP_BUFFER_SIZE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	gboolean buffered;
	switch (id) {
	case PROP_FILE:
		g_mutex_lock(priv->lock);
		g_free(priv->file);
		priv->file = NULL;
		const gchar *file = g_value_get_string(value);
		if (file) {
			priv->file = g_strdup(file);
			if (priv->file) {
				g_strstrip(priv->file);
			}
		}
		g_mutex_unlock(priv->lock);
		break;
	case PROP_APPEND:
		priv->append = g_value_get_boolean(value);
		break;
	case PROP_BUFFERED_IO:
		buffered = g_value_get_boolean(value);
		g_atomic_int_set(&priv->buffered, buffered);
		if (buffered) {
			g_object_set(base, "immediate-flush", FALSE, NULL);
		}
		break;
	case PROP_BUFFER_SIZE:
		priv->size = g_value_get_uint(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
set_file_full(Log4gAppender *base, const gchar *file, gboolean append,
		gboolean buffered, guint size)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->buffered) {
		g_object_set(base, "immediate-flush", FALSE, NULL);
	}
	log4g_writer_appender_reset(base);
	g_object_set(base, "file", file, NULL);
	g_mutex_lock(priv->lock);
	priv->append = append;
	priv->buffered = buffered;
	priv->size = size;
	g_mutex_unlock(priv->lock);
	log4g_appender_activate_options(base);
}

static void
set_qw_for_files(Log4gAppender *base, FILE *file)
{
	GObject *error = log4g_appender_get_error_handler(base);
	Log4gQuietWriter *writer = log4g_quiet_writer_new(file, error);
	if (writer) {
		log4g_writer_appender_set_quiet_writer(base, writer);
		g_object_unref(writer);
	}
}

static void
reset(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	log4g_file_appender_close_file(base);
	g_free(priv->file);
	priv->file = NULL;
	LOG4G_WRITER_APPENDER_CLASS(log4g_file_appender_parent_class)->
		reset(base);
}

static void
activate_options(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(priv->lock);
	if (G_UNLIKELY(!priv->file)) {
		log4g_log_warn(Q_("file option not set for appender [%s]"),
				log4g_appender_get_name(LOG4G_APPENDER(base)));
		log4g_log_warn(Q_("are you using FileAppender instead "
					"of ConsoleAppender?"));
		goto exit;
	}
	FILE *ostream = fopen(priv->file, (priv->append ? "a" : "w"));
	if (!ostream) {
		log4g_log_error("%s: %s", priv->file, g_strerror(errno));
		goto exit;
	}
	if (priv->buffered) {
		if (setvbuf(ostream, NULL, _IOFBF, priv->size)) {
			log4g_log_error("%s: %s", priv->file,
					g_strerror(errno));
		}
	} else {
		if (setvbuf(ostream, NULL, _IONBF, 0)) {
			log4g_log_error("%s: %s", priv->file,
					g_strerror(errno));
		}
	}
	log4g_file_appender_set_qw_for_files(LOG4G_APPENDER(base), ostream);
	log4g_writer_appender_write_header(LOG4G_APPENDER(base));
exit:
	g_mutex_unlock(priv->lock);
}

static void
log4g_file_appender_class_init(Log4gFileAppenderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->activate_options = activate_options;
	Log4gWriterAppenderClass *writer_class =
		LOG4G_WRITER_APPENDER_CLASS(klass);
	writer_class->reset = reset;
	klass->set_file_full = set_file_full;
	klass->set_qw_for_files = set_qw_for_files;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_FILE,
		g_param_spec_string("file", Q_("File"),
			Q_("Output file name"), NULL, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_APPEND,
		g_param_spec_boolean("append", Q_("Append"),
			Q_("Append or overwrite file"), TRUE,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_BUFFERED_IO,
		g_param_spec_boolean("buffered-io", Q_("Buffered I/O"),
			Q_("Buffer log output"), FALSE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_BUFFER_SIZE,
		g_param_spec_uint("buffer-size", Q_("Buffer Size"),
			Q_("Size of the output buffer"),
			0, G_MAXUINT, 8 * 1024, G_PARAM_WRITABLE));
}

static void
log4g_file_appender_class_finalize(
		G_GNUC_UNUSED Log4gFileAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_file_appender_register(GTypeModule *module)
{
    log4g_quiet_writer_register(module);
    log4g_file_appender_register_type(module);
}

/**
 * log4g_file_appender_set_file_full:
 * @base: A file appender object.
 * @file: The new value for the file property.
 * @append: The new value for the append property.
 * @buffered: The new value for the buffered-io property.
 * @size: The new value for the size property.
 *
 * Calls the @set_file_full function from the #Log4gFileAppender of @self.
 *
 * Since: 0.1
 */
void
log4g_file_appender_set_file_full(Log4gAppender *base, const gchar *file,
		gboolean append, gboolean buffered, guint size)
{
	g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
	LOG4G_FILE_APPENDER_GET_CLASS(base)->
		set_file_full(base, file, append, buffered, size);
}

/**
 * log4g_file_appender_close_file:
 * @base: A file appender object.
 *
 * Close the log file.
 *
 * Once a file appender is closed it is no longer usable.
 *
 * Since: 0.1
 */
void
log4g_file_appender_close_file(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
	Log4gQuietWriter *writer =
		log4g_writer_appender_get_quiet_writer(base);
	if (writer) {
		log4g_quiet_writer_close(writer);
	}
}

/**
 * log4g_file_appender_set_qw_for_files:
 * @base: A file appender object.
 * @file: An open file descriptor.
 *
 * Calls the @set_qw_for_files function from the #Log4gFileAppenderClass
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_file_appender_set_qw_for_files(Log4gAppender *base, FILE *file)
{
	g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
	LOG4G_FILE_APPENDER_GET_CLASS(base)->set_qw_for_files(base, file);
}

/**
 * log4g_file_appender_get_file:
 * @base: A file appender object.
 *
 * Retrieve the file property.
 *
 * Returns: The name of the file being appended to.
 * Since: 0.1
 */
const gchar *
log4g_file_appender_get_file(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), NULL);
	return GET_PRIVATE(base)->file;
}

/**
 * log4g_file_appender_get_buffered_io:
 * @base: A file appender object.
 *
 * Retrieve the buffered-io property.
 *
 * Returns: The buffered-io value for \e base.
 * Since: 0.1
 */
gboolean
log4g_file_appender_get_buffered_io(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), FALSE);
	return GET_PRIVATE(base)->buffered;
}

/**
 * log4g_file_appender_get_buffer_size:
 * @base: A file appender object.
 *
 * Retrieve the buffer-size property.
 *
 * Returns: The buffer-size value for \e base.
 * Since: 0.1
 */
guint
log4g_file_appender_get_buffer_size(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), FALSE);
	return GET_PRIVATE(base)->size;
}
