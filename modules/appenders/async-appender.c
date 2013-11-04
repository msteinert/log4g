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
 * SECTION: async-appender
 * @short_description: Log events asynchronously
 *
 * The async appender will collect events sent to it and then dispatch them
 * to all appenders that are attached to it. Multiple appenders may be
 * attached to an async appender.
 *
 * The async appender uses a separate thread to serve the events in its
 * buffer. You must call g_thread_init() before attempting to configure an
 * async appender.
 *
 * Async appenders accept the following properties:
 * <orderedlist>
 * <listitem><para>blocking</para></listitem>
 * <listitem><para>buffer-size</para></listitem>
 * </orderedlist>
 *
 * The blocking property determines the behavior of the async appender when
 * its log event buffer is full. If blocking is %TRUE then the client will
 * block until there is room in the buffer. Otherwise the client will not
 * block and the log event will be dropped. In non-blocking mode the async
 * appender will keep a summary of all dropped logging events. The default
 * value is %TRUE.
 *
 * The buffer-size property determines how many messages are allowed in the
 * buffer before the client will block. The default value is 128.
 *
 * <note><para>
 * If blocking is %FALSE then the value of buffer-size has no effect.
 * </para></note>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/async-appender.h"
#include "log4g/helpers/appender-attachable-impl.h"
#include "log4g/interface/error-handler.h"

/**
 * Log4gDiscardSummary:
 * @event: The last event missed.
 * @count: The number of events missed.
 *
 * A discarded event summary object.
 */
typedef struct Log4gDiscardSummary_ {
	Log4gLoggingEvent *event;
	gint count;
} Log4gDiscardSummary;

/**
 * log4g_discard_summary_destroy:
 * @self: A discard summary object.
 *
 * Free dynamic resources used by a discard summary object.
 */
static void
log4g_discard_summary_destroy(Log4gDiscardSummary *self)
{
	if (self) {
		if (self->event) {
			g_object_unref(self->event);
		}
		g_slice_free(Log4gDiscardSummary, self);
	}
}

/**
 * log4g_discard_summary_new:
 * @event: A discarded logging event.
 *
 * Create a new discard summary object.
 *
 * Returns: A new discard summary object.
 */
static Log4gDiscardSummary *
log4g_discard_summary_new(Log4gLoggingEvent *event)
{
	g_return_val_if_fail(event, NULL);
	g_return_val_if_fail(LOG4G_IS_LOGGING_EVENT(event), NULL);
	Log4gDiscardSummary *self = g_slice_new0(Log4gDiscardSummary);
	if (!self) {
		return NULL;
	}
	self->event = g_object_ref(event);
	self->count = 1;
	return self;
}

/**
 * log4g_discard_summary_add:
 * @self: A discard summary object.
 * @event: A discarded logging event.
 *
 * Add a discarded logging event to a discard summary object.
 */
static void
log4g_discard_summary_add(Log4gDiscardSummary *self, Log4gLoggingEvent *event)
{
	if (self->event) {
		g_object_unref(self->event);
	}
	g_object_ref(event);
	self->event = event;
	++self->count;
}

/**
 * log4g_discard_summary_create_event0:
 * @self: A discard summary object.
 * @message: A log message format.
 * @...: Format parameters.
 *
 * Create a discard summary logging event. This function is called by
 * log4g_discard_summary_create_event() to create the logging event.
 *
 * Returns: A new logging event.
 */
static inline Log4gLoggingEvent *
log4g_discard_summary_create_event0(Log4gDiscardSummary *self,
		const gchar *message, ...)
{
	va_list ap;
	va_start(ap, message);
	Log4gLoggingEvent *event = log4g_logging_event_new(
			log4g_logging_event_get_logger_name(self->event),
			log4g_logging_event_get_level(self->event),
			NULL, NULL, NULL, message, ap);
	va_end(ap);
	return event;
}

/**
 * log4g_discard_summary_create_event:
 * @self: A discard summary object.
 *
 * Create a discard summary logging event.
 *
 * Returns: A new logging event.
 */
static Log4gLoggingEvent *
log4g_discard_summary_create_event(Log4gDiscardSummary *self)
{
	return log4g_discard_summary_create_event0(self,
			"Discarded %d messages due to full event buffer: %s",
			self->count,
			log4g_logging_event_get_message(self->event));
}

struct Private {
	Log4gAppenderAttachable *appenders; /* Asynchronous appenders */
	GHashTable *summary; /* Summary of discarded events */
	GThreadPool *pool; /* Worker thread pool */
	gboolean blocking; /* Indicates if logging thread should block */
	gsize size; /* Maximum size of the event queue */
	GMutex lock; /* Synchronizes access to \e appenders */
	GMutex discard; /* Synchronizes access to \e summary */
};

static void
appender_attachable_init(Log4gAppenderAttachableInterface *interface)
{
	interface->add_appender =
		(gconstpointer)log4g_async_appender_add_appender;
	interface->get_all_appenders =
		(gconstpointer)log4g_async_appender_get_all_appenders;
	interface->get_appender =
		(gconstpointer)log4g_async_appender_get_appender;
	interface->is_attached =
		(gconstpointer)log4g_async_appender_is_attached;
	interface->remove_all_appenders =
		(gconstpointer)log4g_async_appender_remove_all_appenders;
	interface->remove_appender =
		(gconstpointer)log4g_async_appender_remove_appender;
	interface->remove_appender_name =
		(gconstpointer)log4g_async_appender_remove_appender_name;
}

G_DEFINE_DYNAMIC_TYPE_EXTENDED(Log4gAsyncAppender, log4g_async_appender,
		LOG4G_TYPE_APPENDER, 0,
		G_IMPLEMENT_INTERFACE_DYNAMIC(LOG4G_TYPE_APPENDER_ATTACHABLE,
			appender_attachable_init))

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_ASYNC_APPENDER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gAsyncAppender *)instance)->priv)

static void
discarded_(G_GNUC_UNUSED gpointer key, gpointer value, gpointer user_data)
{
	struct Private *priv = (struct Private *)user_data;
	Log4gLoggingEvent *event = log4g_discard_summary_create_event(value);
	if (!event) {
		return;
	}
	/* priv->lock is held in run_() */
	log4g_appender_attachable_impl_append_loop_on_appenders(
			priv->appenders, event);
	g_object_unref(event);
}

static void
run_(gpointer data, gpointer user_data)
{
	struct Private *priv = GET_PRIVATE(user_data);
	g_mutex_lock(&priv->lock);
	log4g_appender_attachable_impl_append_loop_on_appenders(
			priv->appenders, LOG4G_LOGGING_EVENT(data));
	g_object_unref(data);
	if (g_atomic_int_get(&priv->blocking)) {
		g_mutex_lock(&priv->discard);
		if (priv->summary && g_hash_table_size(priv->summary)) {
			g_hash_table_foreach(priv->summary, discarded_, priv);
			g_hash_table_remove_all(priv->summary);
		}
		g_mutex_unlock(&priv->discard);
	}
	g_mutex_unlock(&priv->lock);
}

static void
log4g_async_appender_init(Log4gAsyncAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	GError *error = NULL;
	priv->appenders = log4g_appender_attachable_impl_new();
	priv->blocking = TRUE;
	priv->size = 128;
	priv->pool = g_thread_pool_new(run_, self, 1, TRUE, &error);
	if (error) {
		log4g_log_warn("g_thread_pool_new(): %s", error->message);
		g_error_free(error);
	}
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	log4g_appender_close(LOG4G_APPENDER(base));
	if (priv->summary) {
		g_hash_table_destroy(priv->summary);
		priv->summary = NULL;
	}
	if (priv->appenders) {
		g_object_unref(priv->appenders);
		priv->appenders = NULL;
	}
	G_OBJECT_CLASS(log4g_async_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_clear(&priv->lock);
	g_mutex_clear(&priv->discard);
	G_OBJECT_CLASS(log4g_async_appender_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_BLOCKING,
	PROP_BUFFER_SIZE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	gboolean blocking;
	switch (id) {
	case PROP_BLOCKING:
		blocking = g_value_get_boolean(value);
		g_mutex_lock(&priv->discard);
		if (blocking) {
			if (!priv->summary) {
				priv->summary = g_hash_table_new_full(
						g_str_hash, g_str_equal, NULL,
						(GDestroyNotify)log4g_discard_summary_destroy);
			}
		} else {
			if (priv->summary) {
				g_hash_table_destroy(priv->summary);
				priv->summary = NULL;
			}
		}
		g_atomic_int_set(&priv->blocking, blocking);
		g_mutex_unlock(&priv->discard);
		break;
	case PROP_BUFFER_SIZE:
		priv->size = g_value_get_int(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	gboolean blocking = g_atomic_int_get(&priv->blocking);
	gboolean discard = FALSE;
	if (!g_thread_supported()) {
		log4g_log_warn("Log4gAsyncAppender: threading is not enabled "
				"(message discarded)");
		return;
	}
	log4g_logging_event_get_thread_copy(event);
	log4g_logging_event_get_ndc_copy(event);
	log4g_logging_event_get_mdc_copy(event);
	if (blocking) {
		while (g_thread_pool_unprocessed(priv->pool) >= priv->size) {
			g_usleep(5000); /* sleep 5 milliseconds */
		}
	} else {
		if (G_UNLIKELY(g_thread_pool_unprocessed(priv->pool)
					> priv->size)) {
			discard = TRUE;
		}
	}
	if (!discard) {
		g_object_ref(event);
		GError *error = NULL;
		g_thread_pool_push(priv->pool, event, &error);
		if (error) {
			log4g_log_error("g_thread_pool_push(): %s",
					error->message);
			g_error_free(error);
			discard = TRUE;
		}
	}
	if (G_UNLIKELY(discard)) {
		Log4gDiscardSummary *summary;
		const gchar *name = log4g_logging_event_get_logger_name(event);
		g_mutex_lock(&priv->discard);
		summary = g_hash_table_lookup(priv->summary, name);
		if (!summary) {
			summary = log4g_discard_summary_new(event);
			if (!summary) {
				return;
			}
			g_hash_table_insert(priv->summary, (gpointer)name,
					summary);
		} else {
			log4g_discard_summary_add(summary, event);
		}
		g_mutex_unlock(&priv->discard);
	}
}

static void
close_(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!log4g_appender_get_closed(base)) {
		log4g_appender_set_closed(base, TRUE);
		if (priv->pool) {
			g_thread_pool_free(priv->pool, FALSE, TRUE);
			priv->pool = NULL;
		}
	}
}

static gboolean
requires_layout(G_GNUC_UNUSED Log4gAppender *self)
{
	return FALSE;
}

static void
log4g_async_appender_class_init(Log4gAsyncAppenderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->append = append;
	appender_class->close = close_;
	appender_class->requires_layout = requires_layout;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_BLOCKING,
		g_param_spec_boolean("blocking", Q_("Blocking"),
			Q_("Toggle whether the caller blocks"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_BUFFER_SIZE,
		g_param_spec_int("buffer-size", Q_("Buffer Size"),
			Q_("The size of the logging event queue"),
			0, G_MAXINT, 128, G_PARAM_WRITABLE));
}

static void
log4g_async_appender_class_finalize(
		G_GNUC_UNUSED Log4gAsyncAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_async_appender_register(GTypeModule *module)
{
	log4g_async_appender_register_type(module);
}

/**
 * log4g_async_appender_add_appender:
 * @base: An async appender object.
 * @appender: The appender to add.
 *
 * Add an appender to an async appender.
 *
 * If @appender is already attached to @base then this function does not do
 * anything.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_async_appender_add_appender(Log4gAppender *base, Log4gAppender *appender)
{
	struct Private *priv;
	g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
	priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	log4g_appender_attachable_add_appender(priv->appenders, appender);
	g_mutex_unlock(&priv->lock);
}

/**
 * log4g_async_appender_get_all_appenders:
 * @base: An async appender object.
 *
 * Retrieve an array of appenders attached to an async appender.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Returns: An array of appenders attached to @base, or %NULL if there are
 *          none. The caller is responsible for calling g_array_free() for the
 *          returned value.
 * Since: 0.1
 */
const GArray *
log4g_async_appender_get_all_appenders(Log4gAppender *base)
{
	g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), NULL);
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	const GArray *appenders =
		log4g_appender_attachable_get_all_appenders(priv->appenders);
	g_mutex_unlock(&priv->lock);
	return appenders;
}

/**
 * log4g_async_appender_get_appender:
 * @base: An async appender object.
 * @name: The name of the appender to look up.
 *
 * Retrieve an attached named appender.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Returns: The appender named @name or %NULL if @name is not found.
 * Since: 0.1
 */
Log4gAppender *
log4g_async_appender_get_appender(Log4gAppender *base, const gchar *name)
{
	g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), NULL);
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	Log4gAppender *appender =
		log4g_appender_attachable_get_appender(priv->appenders, name);
	g_mutex_unlock(&priv->lock);
	return appender;
}

/**
 * log4g_async_appender_is_attached:
 * @base: An async appender object.
 * @appender: An appender.
 *
 * Determine if an appender is attached.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Returns: %TRUE is @appender is attached to @base, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_async_appender_is_attached(Log4gAppender *base, Log4gAppender *appender)
{
	g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), FALSE);
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	gboolean attached = log4g_appender_attachable_is_attached(
			priv->appenders, appender);
	g_mutex_unlock(&priv->lock);
	return attached;
}

/**
 * log4g_async_appender_remove_all_appenders:
 * @base: An async appender object.
 *
 * Remove all attached appenders.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_async_appender_remove_all_appenders(Log4gAppender *base)
{
	g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	log4g_appender_attachable_remove_all_appenders(priv->appenders);
	g_mutex_unlock(&priv->lock);
}

/**
 * log4g_async_appender_remove_appender:
 * @base: An async appender object.
 * @appender: The appender to remove.
 *
 * Remove an attached appender.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_async_appender_remove_appender(Log4gAppender *base,
		Log4gAppender *appender)
{
	g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	log4g_appender_attachable_remove_appender(priv->appenders, appender);
	g_mutex_unlock(&priv->lock);
}

/**
 * log4g_async_appender_remove_appender_name:
 * @base: An async appender object.
 * @name: The name of the appender to remove.
 *
 * Remove a named appender.
 *
 * @See: #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_async_appender_remove_appender_name(Log4gAppender *base,
		const gchar *name)
{
	g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
	struct Private *priv = GET_PRIVATE(base);
	g_mutex_lock(&priv->lock);
	log4g_appender_attachable_remove_appender_name(priv->appenders, name);
	g_mutex_unlock(&priv->lock);
}
