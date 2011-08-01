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
 * SECTION: writer-appender
 * @short_description: Append to a stdio(3) stream.
 * @see_also: stdio(3)
 *
 * This class is a base class for appenders that write to stdio(3) streams.
 *
 * Writer appenders accept one property:
 * <orderedlist>
 * <listitem><para>immediate-flush</para></listitem>
 * </orderedlist>
 *
 * The value of immediate-flush determines if the I/O stream will be flushed
 * after each write. The default value is %TRUE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/writer-appender.h"
#include "log4g/interface/error-handler.h"

G_DEFINE_DYNAMIC_TYPE(Log4gWriterAppender, log4g_writer_appender,
		LOG4G_TYPE_APPENDER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_WRITER_APPENDER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gWriterAppender *)instance)->priv)

struct Private {
	gboolean flush;
	Log4gQuietWriter *writer;
	GMutex *lock;
};

static void
log4g_writer_appender_init(Log4gWriterAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->flush = TRUE;
	priv->writer = NULL;
	priv->lock = NULL;
	if (g_thread_supported()) {
		priv->lock = g_mutex_new();
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->writer) {
		g_object_unref(priv->writer);
		priv->writer = NULL;
	}
	G_OBJECT_CLASS(log4g_writer_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	log4g_appender_close(LOG4G_APPENDER(base));
	if (priv->lock) {
		g_mutex_free(priv->lock);
	}
	G_OBJECT_CLASS(log4g_writer_appender_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_IMMEDIATE_FLUSH,
	PROP_WRITER,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_IMMEDIATE_FLUSH:
		priv->flush = g_value_get_boolean(value);
		break;
	case PROP_WRITER:
		g_mutex_lock(priv->lock);
		log4g_writer_appender_reset(LOG4G_APPENDER(base));
		FILE *file = g_value_get_pointer(value);
		GObject *error = log4g_appender_get_error_handler(
				LOG4G_APPENDER(base));
		priv->writer = log4g_quiet_writer_new(file, error);
		if (priv->writer) {
			log4g_writer_appender_write_header(LOG4G_APPENDER(base));
		}
		g_mutex_unlock(priv->lock);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
	if (!log4g_writer_appender_check_entry_conditions(base)) {
		return;
	}
	log4g_writer_appender_sub_append(base, event);
}

static void
close_(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!log4g_appender_get_closed(base)) {
		g_mutex_lock(priv->lock);
		log4g_appender_set_closed(base, TRUE);
		log4g_writer_appender_write_footer(base);
		log4g_writer_appender_reset(base);
		g_mutex_unlock(priv->lock);
	}
}

static gboolean
requires_layout(Log4gAppender *self)
{
	return TRUE;
}

void
sub_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
	Log4gLayout *layout = log4g_appender_get_layout(base);
	const char *message = log4g_layout_format(layout, event);
	struct Private *priv = GET_PRIVATE(base);
	log4g_quiet_writer_write(priv->writer, message);
	if (priv->flush) {
		log4g_quiet_writer_flush(priv->writer);
	}
}

static void
close_writer(Log4gAppender *base)
{
	log4g_quiet_writer_close(GET_PRIVATE(base)->writer);
}

static void
reset(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->writer) {
		return;
	}
	log4g_writer_appender_close_writer(base);
	if (priv->writer) {
		g_object_unref(priv->writer);
		priv->writer = NULL;
	}
}

static void
log4g_writer_appender_class_init(Log4gWriterAppenderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->append = append;
	appender_class->close = close_;
	appender_class->requires_layout = requires_layout;
	klass->sub_append = sub_append;
	klass->close_writer = close_writer;
	klass->reset = reset;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_IMMEDIATE_FLUSH,
		g_param_spec_boolean("immediate-flush", Q_("Immediate Flush"),
			Q_("Flush immediately after writing"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_WRITER,
		g_param_spec_pointer("writer", Q_("Writer"),
			Q_("Set the stdio(3) stream to use"),
			G_PARAM_WRITABLE));
}

static void
log4g_writer_appender_class_finalize(Log4gWriterAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_writer_appender_register(GTypeModule *module)
{
	log4g_quiet_writer_register(module);
	log4g_writer_appender_register_type(module);
}

/**
 * log4g_writer_appender_check_entry_conditions:
 * @base: A writer appender object.
 *
 * Determine if it is OK to append.
 *
 * Checks the following conditions:
 * <itemizedlist>
 * <listitem><para>The output target is set</para></listitem>
 * <listitem><para>The layout is set</para></listitem>
 * </itemizedlist>
 * 
 * Returns: %TRUE if all entry conditions pass, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_writer_appender_check_entry_conditions(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_WRITER_APPENDER(base), FALSE);
	if (log4g_appender_get_closed(base)) {
		log4g_log_warn(Q_("not allowed to write to a closed appender"));
		return FALSE;
	}
	if (!GET_PRIVATE(base)->writer) {
		Log4gErrorHandler *error = (Log4gErrorHandler *)
			log4g_appender_get_error_handler(base);
		if (error) {
			log4g_error_handler_error(error, NULL,
					Q_("no output stream or file set for "
					"the appender named [%s]"),
					log4g_appender_get_name(base));
		}
		return FALSE;
	}
	if (!log4g_appender_get_layout(base)) {
		Log4gErrorHandler *error = (Log4gErrorHandler *)
			log4g_appender_get_error_handler(base);
		if (error) {
			log4g_error_handler_error(error, NULL,
					Q_("no layout set for the "
					"appender named [%s]"),
					log4g_appender_get_name(base));
		}
		return FALSE;
	}
	return TRUE;
}

/**
 * log4g_writer_appender_set_writer:
 * @base: A writer appender object.
 * @file: An open stdio(3) stream.
 *
 * Set the stdio(3) stream to use.
 *
 * @See: stdio(3)
 *
 * Since: 0.1
 */
void
log4g_writer_appender_set_writer(Log4gAppender *base, FILE *file)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	g_object_set(base, "writer", file, NULL);
}

/**
 * log4g_writer_appender_close_writer:
 * @base: A writer appender object.
 *
 * Calls the @close_writer function from the #Log4gWriterAppender of @self.
 *
 * Since: 0.1
 */
void
log4g_writer_appender_close_writer(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	LOG4G_WRITER_APPENDER_GET_CLASS(base)->close_writer(base);
}

/**
 * log4g_writer_appender_sub_append:
 * @base: A writer appender object.
 * @event: The log event to append.
 *
 * Calls the @sub_append function from the #Log4gWriterAppender of @self.
 *
 * Since: 0.1
 */
void
log4g_writer_appender_sub_append(Log4gAppender *base,
		Log4gLoggingEvent *event)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	LOG4G_WRITER_APPENDER_GET_CLASS(base)->sub_append(base, event);
}

/**
 * log4g_writer_appender_reset:
 * @base: A writer appender object.
 *
 * Calls the @reset function from the #Log4gWriterAppender of @self.
 *
 * Since: 0.1
 */
void
log4g_writer_appender_reset(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	LOG4G_WRITER_APPENDER_GET_CLASS(base)->reset(base);
}

/**
 * log4g_writer_appender_write_footer:
 * @base: A writer appender object.
 *
 * Write the layout footer.
 *
 * Since: 0.1
 */
void
log4g_writer_appender_write_footer(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	Log4gLayout *layout = log4g_appender_get_layout(base);
	if (layout) {
		struct Private *priv = GET_PRIVATE(base);
		const gchar *footer = log4g_layout_get_footer(layout);
		if (footer && priv->writer) {
			log4g_quiet_writer_write(priv->writer, footer);
			log4g_quiet_writer_flush(priv->writer);
		}
	}
}

/**
 * log4g_writer_appender_write_header:
 * @base: A writer appender object.
 *
 * Write the layout header.
 *
 * Since: 0.1
 */
void
log4g_writer_appender_write_header(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	Log4gLayout *layout = log4g_appender_get_layout(base);
	if (layout) {
		struct Private *priv = GET_PRIVATE(base);
		const gchar *header = log4g_layout_get_header(layout);
		if (header && priv->writer) {
			log4g_quiet_writer_write(priv->writer, header);
		}
	}
}

/**
 * log4g_writer_appender_get_quiet_writer:
 * @base: A writer appender object.
 *
 * Retrieve the current quiet writer object.
 *
 * @See: #Log4gQuietWriterClass
 *
 * Returns: The current quiet writer object used by @base.
 * Since: 0.1
 */
Log4gQuietWriter *
log4g_writer_appender_get_quiet_writer(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_WRITER_APPENDER(base), NULL);
	return GET_PRIVATE(base)->writer;
}

/**
 * log4g_writer_appender_set_quiet_writer:
 * @base: A writer appender object.
 * @writer: A quiet writer.
 *
 * Set the quiet writer to use.
 *
 * @See: #Log4gQuietWriterClass
 *
 * Since: 0.1
 */
void
log4g_writer_appender_set_quiet_writer(Log4gAppender *base,
		Log4gQuietWriter *writer)
{
	g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
	struct Private *priv = GET_PRIVATE(base);
	if (priv->writer) {
		g_object_unref(priv->writer);
	}
	g_object_ref(writer);
	priv->writer = writer;
}
