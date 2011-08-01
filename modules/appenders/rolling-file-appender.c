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
 * SECTION: rolling-file-appender
 * @short_description: Backup log files when they reach a specified size
 *
 * This class extends #Log4gFileAppenderClass to backup log files when they
 * reach a specified size.
 *
 * Rolling file appenders accept two properties:
 * <orderedlist>
 * <listitem><para>max-backup-index</para></listitem>
 * <listitem><para>maximum-file-size</para></listitem>
 * </orderedlist>
 *
 * The value of max-backup-index sets the number of backup files that will
 * be kept. For example if the value is ten, then eleven files will be kept
 * (indexes zero through ten). The default value is one.
 *
 * The log files will be rotated when the current log file reaches a size of
 * maximum-file-size or larger. The default value is ten megabytes.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/rolling-file-appender.h"
#include <errno.h>
#include <glib/gstdio.h>
#include "helpers/counting-quiet-writer.h"

G_DEFINE_DYNAMIC_TYPE(Log4gRollingFileAppender, log4g_rolling_file_appender,
        LOG4G_TYPE_FILE_APPENDER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_ROLLING_FILE_APPENDER, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gRollingFileAppender *)instance)->priv)

struct Private {
	guint backup; /* The number of backup indexes to keep */
	gulong max; /* The maximum file size (default is 10MB) */
	gulong next;
};

static void
log4g_rolling_file_appender_init(Log4gRollingFileAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->backup = 1;
	priv->max = 10 * 1024 * 1024; /* 10MB */
}

enum Properties {
	PROP_O = 0,
	PROP_MAX_BACKUP_INDEX,
	PROP_MAXIMUM_FILE_SIZE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_MAX_BACKUP_INDEX:
		priv->backup = g_value_get_uint(value);
		break;
	case PROP_MAXIMUM_FILE_SIZE:
		priv->max = g_value_get_ulong(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
sub_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	Log4gQuietWriter *writer =
		log4g_writer_appender_get_quiet_writer(base);
	LOG4G_WRITER_APPENDER_CLASS(log4g_rolling_file_appender_parent_class)->
		sub_append(base, event);
	if (log4g_file_appender_get_file(base) && writer) {
		gulong size = log4g_counting_quiet_writer_get_count(writer);
		if ((size >= priv->max) && (size >= priv->next)) {
			log4g_rolling_file_appender_roll_over(base);
		}
	}
}

static void
set_file_full(Log4gAppender *base, const gchar *file, gboolean append,
		gboolean buffered, guint size)
{
	LOG4G_FILE_APPENDER_CLASS(log4g_rolling_file_appender_parent_class)->
		set_file_full(base, file, append, buffered, size);
	if (append) {
		struct stat buf;
		Log4gQuietWriter *qw =
			log4g_writer_appender_get_quiet_writer(base);
		if (g_stat(file, &buf)) {
			if (ENOENT != errno) {
				log4g_log_error("g_stat(): %s",
						g_strerror(errno));
				return;
			}
		}
		log4g_counting_quiet_writer_set_count(qw, buf.st_size);
	}
}

static void
set_qw_for_files(Log4gAppender *base, FILE *file)
{
	GObject *error = log4g_appender_get_error_handler(base);
	Log4gQuietWriter *writer = log4g_counting_quiet_writer_new(file, error);
	if (writer) {
		log4g_writer_appender_set_quiet_writer(base, writer);
		g_object_unref(writer);
	}
}

static void
roll_over(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	Log4gQuietWriter *writer = log4g_writer_appender_get_quiet_writer(base);
	if (writer) {
		gulong size = log4g_counting_quiet_writer_get_count(writer);
		priv->next = size + priv->max;
	}
	if (priv->max > 0) {
		GString *source = g_string_sized_new(128);
		if (G_UNLIKELY(!source)) {
			return;
		}
		GString *target = g_string_sized_new(128);
		if (G_UNLIKELY(!target)) {
			g_string_free(source, TRUE);
			return;
		}
		const gchar *file = log4g_file_appender_get_file(base);
		for (guint i = priv->backup - 1; i >= 1; --i) {
			g_string_printf(source, "%s.%u", file, i);
			g_string_printf(target, "%s.%u", file, i + 1);
			g_rename(source->str, target->str);
		}
		g_string_free(source, TRUE);
		g_string_printf(target, "%s.%u", file, 1);
		if (!g_rename(file, target->str)) {
			g_string_printf(target, "%s", file);
			log4g_file_appender_set_file_full(base, target->str, TRUE,
					log4g_file_appender_get_buffered_io(base),
					log4g_file_appender_get_buffer_size(base));
		}
		g_string_free(target, TRUE);
	}
	priv->next = 0;
}

static void
log4g_rolling_file_appender_class_init(Log4gRollingFileAppenderClass *klass)
{
	/* initialize GObject class */
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->set_property = set_property;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
	/* initialize Log4gWriterAppender class */
	Log4gWriterAppenderClass *writer_class =
		LOG4G_WRITER_APPENDER_CLASS(klass);
	writer_class->sub_append = sub_append;
	/* initialize Log4gaFileAppender class */
	Log4gFileAppenderClass *file_class =
		LOG4G_FILE_APPENDER_CLASS(klass);
	file_class->set_file_full = set_file_full;
	file_class->set_qw_for_files = set_qw_for_files;
	/* initialize Log4gRollingFileAppender class */
	klass->roll_over = roll_over;
	/* install properties */
	g_object_class_install_property(object_class, PROP_MAX_BACKUP_INDEX,
		g_param_spec_uint("max-backup-index",
			Q_("Maximum Backup Index"),
			Q_("Maximum number of backup files"),
			0, G_MAXUINT, 1, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_MAXIMUM_FILE_SIZE,
		g_param_spec_ulong("maximum-file-size",
			Q_("Maximum File Size"),
			Q_("Maximum size a log file may grow to"),
			0, G_MAXLONG, 10 * 1024 * 1024, G_PARAM_WRITABLE));
}

static void
log4g_rolling_file_appender_class_finalize(
		Log4gRollingFileAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_rolling_file_appender_register(GTypeModule *module)
{
	log4g_counting_quiet_writer_register(module);
	log4g_rolling_file_appender_register_type(module);
}

/**
 * log4g_rolling_file_appender_roll_over:
 * @base: A rolling file appender object.
 *
 * Calls the @roll_over function from the #Log4gRollingFileAppender of @self.
 *
 * Since: 0.1
 */
void
log4g_rolling_file_appender_roll_over(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base));
	LOG4G_ROLLING_FILE_APPENDER_GET_CLASS(base)->roll_over(base);
}
