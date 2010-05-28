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
 * \brief Implements the API in async-appender.h
 * \author Mike Steinert
 * \date 2-17-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/async-appender.h"
#include "log4g/helpers/appender-attachable-impl.h"
#include "log4g/interface/error-handler.h"
#include <string.h>
#include <unistd.h>

enum _properties_t {
    PROP_O = 0,
    PROP_BLOCKING,
    PROP_BUFFER_SIZE,
    PROP_MAX
};

/** \brief A discarded event summary object */
typedef struct _Log4gDiscardSummary {
    Log4gLoggingEvent *event; /**< The last event missed */
    gint count; /**< The number of events missed */
} Log4gDiscardSummary;

/**
 * \brief Free dynamic resources used by a discard summary object.
 * \param self [in] A discard summary object.
 */
static void
log4g_discard_summary_destroy(Log4gDiscardSummary *self)
{
    if (self) {
        if (self->event) {
            g_object_unref(self->event);
        }
        g_free(self);
    }
}

/**
 * \brief Create a new discard summary object.
 * \param event [in] A discarded logging event.
 * \return A new discard summary object.
 */
static Log4gDiscardSummary *
log4g_discard_summary_new(Log4gLoggingEvent *event)
{
    g_return_val_if_fail(event, NULL);
    g_return_val_if_fail(LOG4G_IS_LOGGING_EVENT(event), NULL);
    Log4gDiscardSummary *self = g_try_malloc(sizeof *self);
    if (!self) {
        log4g_discard_summary_destroy(self);
        return NULL;
    }
    memset(self, 0, sizeof *self);
    g_object_ref(event);
    self->event = event;
    self->count = 1;
    return self;
}

/**
 * \brief Add a discarded logging event to a discard summary object.
 * \param self [in] A discard summary object.
 * \param event [in] A discarded logging event.
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
 * \brief [private] Create a discard summary logging event.
 * This function is called by log4g_discard_summary_create_event() to
 * create the logging event.
 * \param self [in] A discard summary object.
 * \param message [in] A log message format.
 * \param ... [in] Format parameters.
 * \return A new logging event.
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
 * \brief Create a discard summary logging event.
 * \param self [in] A discard summary object.
 * \return A new logging event.
 */
static Log4gLoggingEvent *
log4g_discard_summary_create_event(Log4gDiscardSummary *self)
{
    return log4g_discard_summary_create_event0(self,
                "Discarded %d messages due to full event buffer including: %s",
                self->count, log4g_logging_event_get_message(self->event));
}

struct Log4gPrivate {
    Log4gAppenderAttachable *appenders; /**< Asynchronous appenders */
    GHashTable *summary; /**< Summary of discarded events */
    GThreadPool *pool; /**< Worker thread pool */
    gboolean blocking; /**< Indicates if logging thread should block */
    gint size; /**< Maximum size of the event queue */
    GMutex *lock; /**< Synchronizes access to \e appenders */
    GMutex *discard; /**< Synchronizes access to \e summary */
};

static void
appender_attachable_init(Log4gAppenderAttachableInterface *interface)
{
    interface->add_appender = (gconstpointer)log4g_async_appender_add_appender;
    interface->get_all_appenders =
        (gconstpointer)log4g_async_appender_get_all_appenders;
    interface->get_appender = (gconstpointer)log4g_async_appender_get_appender;
    interface->is_attached = (gconstpointer)log4g_async_appender_is_attached;
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

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_ASYNC_APPENDER, \
            struct Log4gPrivate))

static void
_discarded(gpointer key, gpointer value, gpointer user_data)
{
    struct Log4gPrivate *priv = (struct Log4gPrivate *)user_data;
    Log4gLoggingEvent *event = log4g_discard_summary_create_event(value);
    if (!event) {
        return;
    }
    /* priv->lock is held in _run() */
    log4g_appender_attachable_impl_append_loop_on_appenders(
            priv->appenders, event);
    g_object_unref(event);
}

static void
_run(gpointer data, gpointer user_data)
{
    struct Log4gPrivate *priv = GET_PRIVATE(user_data);
    g_mutex_lock(priv->lock);
    log4g_appender_attachable_impl_append_loop_on_appenders(
            priv->appenders, LOG4G_LOGGING_EVENT(data));
    g_object_unref(data);
    if (g_atomic_int_get(&priv->blocking)) {
        g_mutex_lock(priv->discard);
        if (priv->summary && g_hash_table_size(priv->summary)) {
            g_hash_table_foreach(priv->summary, _discarded, priv);
            g_hash_table_remove_all(priv->summary);
        }
        g_mutex_unlock(priv->discard);
    }
    g_mutex_unlock(priv->lock);
}

static void
log4g_async_appender_init(Log4gAsyncAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->appenders = log4g_appender_attachable_impl_new();
    priv->summary = NULL;
    priv->pool = NULL;
    priv->lock = NULL;
    priv->discard = NULL;
    priv->blocking = TRUE;
    priv->size = 128;
    if (g_thread_supported()) {
        GError *error = NULL;
        priv->pool = g_thread_pool_new(_run, self, 1, TRUE, &error);
        if (error) {
            log4g_log_warn("g_thread_pool_new(): %s", error->message);
            g_error_free(error);
        }
        priv->lock = g_mutex_new();
        priv->discard = g_mutex_new();
    } else {
        log4g_log_warn("you must call g_thread_init() before log4g_init() "
                "if you plan to use Log4gAsyncAppender");
    }
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
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
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    if (priv->discard) {
        g_mutex_free(priv->discard);
        priv->discard = NULL;
    }
    G_OBJECT_CLASS(log4g_async_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    gboolean blocking;
    switch (id) {
    case PROP_BLOCKING:
        blocking = g_value_get_boolean(value);
        g_mutex_lock(priv->discard);
        if (blocking) {
            if (!priv->summary) {
                priv->summary =
                    g_hash_table_new_full(g_str_hash, g_str_equal, NULL,
                              (GDestroyNotify)log4g_discard_summary_destroy);
            }
        } else {
            if (priv->summary) {
                g_hash_table_destroy(priv->summary);
                priv->summary = NULL;
            }
        }
        g_atomic_int_set(&priv->blocking, blocking);
        g_mutex_unlock(priv->discard);
        break;
    case PROP_BUFFER_SIZE:
        priv->blocking = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    gboolean blocking = g_atomic_int_get(&priv->blocking);
    gint size = g_atomic_int_get(&priv->size);
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
        while (g_thread_pool_unprocessed(priv->pool) >= size) {
            g_usleep(5000); /* sleep 5 milliseconds */
        }
    } else {
        if (G_UNLIKELY(g_thread_pool_unprocessed(priv->pool) > size)) {
            discard = TRUE;
        }
    }
    if (!discard) {
        g_object_ref(event);
        GError *error = NULL;
        g_thread_pool_push(priv->pool, event, &error);
        if (error) {
            log4g_log_error("g_thread_pool_push(): %s", error->message);
            g_error_free(error);
            discard = TRUE;
        }
    }
    if (G_UNLIKELY(discard)) {
        Log4gDiscardSummary *summary;
        const gchar *name = log4g_logging_event_get_logger_name(event);
        g_mutex_lock(priv->discard);
        summary = g_hash_table_lookup(priv->summary, name);
        if (!summary) {
            summary = log4g_discard_summary_new(event);
            if (!summary) {
                return;
            }
            g_hash_table_insert(priv->summary, (gpointer)name, summary);
        } else {
            log4g_discard_summary_add(summary, event);
        }
        g_mutex_unlock(priv->discard);
    }
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_get_closed(base)) {
        log4g_appender_set_closed(base, TRUE);
        if (priv->pool) {
            g_thread_pool_free(priv->pool, FALSE, TRUE);
            priv->pool = NULL;
        }
    }
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return FALSE;
}

static void
log4g_async_appender_class_init(Log4gAsyncAppenderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObjectClass */
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->append = append;
    appender_class->close = _close;
    appender_class->requires_layout = requires_layout;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_BLOCKING,
            g_param_spec_boolean("blocking", Q_("Blocking"),
                    Q_("Toggle whether the caller blocks"),
                    TRUE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_BUFFER_SIZE,
            g_param_spec_int("buffer-size", Q_("Buffer Size"),
                    Q_("The size of the logging event queue"),
                    0, G_MAXINT, 128, G_PARAM_WRITABLE));
}

static void
log4g_async_appender_class_finalize(Log4gAsyncAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_async_appender_register(GTypeModule *module)
{
    log4g_async_appender_register_type(module);
}

void
log4g_async_appender_add_appender(Log4gAppender *base, Log4gAppender *appender)
{
    struct Log4gPrivate *priv;
    g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
    priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    log4g_appender_attachable_add_appender(priv->appenders, appender);
    g_mutex_unlock(priv->lock);
}

const GArray *
log4g_async_appender_get_all_appenders(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), NULL);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    const GArray *appenders =
        log4g_appender_attachable_get_all_appenders(priv->appenders);
    g_mutex_unlock(priv->lock);
    return appenders;
}

Log4gAppender *
log4g_async_appender_get_appender(Log4gAppender *base, const gchar *name)
{
    g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), NULL);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    Log4gAppender *appender =
        log4g_appender_attachable_get_appender(priv->appenders, name);
    g_mutex_unlock(priv->lock);
    return appender;
}

gboolean
log4g_async_appender_is_attached(Log4gAppender *base, Log4gAppender *appender)
{
    g_return_val_if_fail(LOG4G_IS_ASYNC_APPENDER(base), FALSE);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    gboolean attached =
        log4g_appender_attachable_is_attached(priv->appenders, appender);
    g_mutex_unlock(priv->lock);
    return attached;
}

void
log4g_async_appender_remove_all_appenders(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    log4g_appender_attachable_remove_all_appenders(priv->appenders);
    g_mutex_unlock(priv->lock);
}

void
log4g_async_appender_remove_appender(Log4gAppender *base,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    log4g_appender_attachable_remove_appender(priv->appenders, appender);
    g_mutex_unlock(priv->lock);
}

void
log4g_async_appender_remove_appender_name(Log4gAppender *base,
        const gchar *name)
{
    g_return_if_fail(LOG4G_IS_ASYNC_APPENDER(base));
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    log4g_appender_attachable_remove_appender_name(priv->appenders, name);
    g_mutex_unlock(priv->lock);
}
