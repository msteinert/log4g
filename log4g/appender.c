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
 * SECTION: appender
 * @short_description: The log output interface
 *
 * Extend this class to define your own strategy for outputting log
 * statements.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/appender.h"
#include "log4g/helpers/only-once-error-handler.h"

enum _properties_t {
    PROP_O = 0,
    PROP_THRESHOLD,
    PROP_MAX
};

G_DEFINE_ABSTRACT_TYPE(Log4gAppender, log4g_appender, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    Log4gLayout *layout;
    gchar *name;
    Log4gLevel *threshold;
    gpointer error;
    Log4gFilter *head;
    Log4gFilter *tail;
    gboolean closed;
    GMutex *lock;
};

static void
log4g_appender_init(Log4gAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->layout = NULL;
    priv->name = NULL;
    priv->threshold = NULL;
    priv->error = log4g_only_once_error_handler_new();
    priv->head = NULL;
    priv->tail = NULL;
    priv->closed = FALSE;
    if (g_thread_supported()) {
        priv->lock = g_mutex_new();
    } else {
        priv->lock = NULL;
    }
}

static void
dispose(GObject *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->layout) {
        g_object_unref(priv->layout);
        priv->layout = NULL;
    }
    if (priv->threshold) {
        g_object_unref(priv->name);
        priv->threshold = NULL;
    }
    if (priv->error) {
        g_object_unref(priv->error);
        priv->error = NULL;
    }
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = priv->tail = NULL;
    }
    G_OBJECT_CLASS(log4g_appender_parent_class)->dispose(self);
}

static void
finalize(GObject *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_free(priv->name);
    priv->name = NULL;
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    G_OBJECT_CLASS(log4g_appender_parent_class)->finalize(self);
}

static void
set_property(GObject *self, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    switch (id) {
    case PROP_THRESHOLD:
        if (priv->threshold) {
            g_object_unref(priv->threshold);
        }
        const gchar *threshold = g_value_get_string(value);
        if (threshold) {
            priv->threshold = log4g_level_string_to_level(threshold);
            if (priv->threshold) {
                g_object_ref(priv->threshold);
            }
        } else {
            priv->threshold = NULL;
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(self, id, pspec);
        break;
    }
}

static void
add_filter(Log4gAppender *self, Log4gFilter *filter)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->head) {
        g_object_ref(filter);
        priv->head = priv->tail = filter;
    } else {
        log4g_filter_set_next(priv->tail, filter);
        priv->tail = filter;
    }
}

static Log4gFilter *
get_filter(Log4gAppender *self)
{
    return GET_PRIVATE(self)->head;
}

static void
do_append(Log4gAppender *self, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_mutex_lock(priv->lock);
    if (priv->closed) {
        log4g_log_error(Q_("attempted to append to closed appender named [%s]"),
                priv->name);
        goto exit;
    }
    Log4gLevel *level = log4g_logging_event_get_level(event);
    if (!log4g_appender_is_as_severe_as(self, level)) {
        goto exit;
    }
    if (priv->head) {
        Log4gFilter *filter = priv->head;
        while (filter) {
            gint decision = log4g_filter_decide(filter, event);
            if (LOG4G_FILTER_DENY == decision) {
                goto exit;
            } else if (LOG4G_FILTER_ACCEPT == decision) {
                break;
            } else if (LOG4G_FILTER_NEUTRAL == decision) {
                filter = log4g_filter_get_next(filter);
            }
        }
    }
    log4g_appender_append(self, event);
exit:
    g_mutex_unlock(priv->lock);
}

static const gchar *
get_name(Log4gAppender *self)
{
    return GET_PRIVATE(self)->name;
}

static void
set_error_handler(Log4gAppender *self, gpointer error)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_mutex_lock(priv->lock);
    if (G_UNLIKELY(!error)) {
        log4g_log_warn(Q_("you have tried to set a NULL error-handler"));
    } else {
        if (priv->error) {
            g_object_unref(priv->error);
        }
        g_object_ref(error);
        priv->error = error;
    }
    g_mutex_unlock(priv->lock);
}

static gpointer
get_error_handler(Log4gAppender *self)
{
    return GET_PRIVATE(self)->error;
}

static void
set_layout(Log4gAppender *self, Log4gLayout *layout)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->layout) {
        g_object_unref(priv->layout);
    }
    if (layout) {
        g_object_ref(layout);
    }
    priv->layout = layout;
}

static Log4gLayout *
get_layout(Log4gAppender *self)
{
    return GET_PRIVATE(self)->layout;
}

static void
set_name(Log4gAppender *self, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_free(priv->name);
    priv->name = g_strdup(name);
}

static void
activate_options(Log4gAppender *self)
{
    /* do nothing */
}

static void
log4g_appender_class_init(Log4gAppenderClass *klass)
{
    /* initialize GObjectClass */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    klass->add_filter = add_filter;
    klass->get_filter = get_filter;
    klass->close = NULL;
    klass->do_append = do_append;
    klass->get_name = get_name;
    klass->set_error_handler = set_error_handler;
    klass->get_error_handler = get_error_handler;
    klass->set_layout = set_layout;
    klass->get_layout = get_layout;
    klass->set_name = set_name;
    klass->requires_layout = NULL;
    klass->activate_options = activate_options;
    /**
     * Log4gAppender:threshold:
     *
     * The level threshold for this appender.
     * @See: #Log4gLevel
     *
     * Since: 0.1
     */
    g_object_class_install_property(gobject_class, PROP_THRESHOLD,
            g_param_spec_string("threshold", Q_("Threshold"),
                    Q_("Threshold for this appender"), NULL,
                    G_PARAM_WRITABLE));
}

/**
 * log4g_appender_clear_filters:
 * @self: A #Log4gAppender object.
 *
 * Remove all filters attached to this appender.
 *
 * Since: 0.1
 */
void
log4g_appender_clear_filters(Log4gAppender *self)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = priv->tail = NULL;
    }
}

/**
 * log4g_appender_add_filter:
 * @self: A #Log4gAppender object.
 * @filter: A filter to add to @self.
 *
 * Calls the @add_filter function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_add_filter(Log4gAppender *self, Log4gFilter *filter)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->add_filter(self, filter);
}

/**
 * log4g_appender_get_filter:
 * @self: A #Log4gAppender object.
 *
 * Calls the @get_filter function from the #Log4gAppenderClass of @self.
 *
 * Returns: The first filter in the filter chain.
 * Since: 0.1
 */
Log4gFilter *
log4g_appender_get_filter(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return LOG4G_APPENDER_GET_CLASS(self)->get_filter(self);
}

/**
 * log4g_appender_close:
 * @self: A #Log4gAppender object.
 *
 * Calls the @close function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_close(Log4gAppender *self)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->close(self);
}

/**
 * log4g_appender_do_append:
 * @self: A #Log4gAppender object.
 * @event: The log event to append.
 *
 * Calls the @do_append function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_do_append(Log4gAppender *self, Log4gLoggingEvent *event)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->do_append(self, event);
}

/**
 * log4g_appender_get_name:
 * @self: A #Log4gAppender object.
 *
 * Calls the @get_name function from the #Log4gAppenderClass of @self.
 *
 * Returns: The name of @self (may be %NULL).
 * Since: 0.1
 */
const gchar *
log4g_appender_get_name(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return LOG4G_APPENDER_GET_CLASS(self)->get_name(self);
}

/**
 * log4g_appender_set_error_handler:
 * @self: A #Log4gAppender object.
 * @handler: The new error handler object for @self.
 *
 * Calls the @set_error_handler function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_set_error_handler(Log4gAppender *self, gpointer handler)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(handler));
    LOG4G_APPENDER_GET_CLASS(self)->set_error_handler(self, handler);
}

/**
 * log4g_appender_get_error_handler:
 * @self: A #Log4gAppender object.
 *
 * Calls the @get_error_handler function from the #Log4gAppenderClass of @self.
 *
 * Returns: The error handler object used by @self.
 * Since: 0.1
 */
gpointer
log4g_appender_get_error_handler(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return LOG4G_APPENDER_GET_CLASS(self)->get_error_handler(self);
}

/**
 * log4g_appender_set_layout:
 * @self: A #Log4gAppender object.
 * @layout: The new layout for @self.
 *
 * Calls the @set_layout function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_set_layout(Log4gAppender *self, Log4gLayout *layout)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->set_layout(self, layout);
}

/**
 * log4g_appender_get_layout:
 * @self: A #Log4gAppender object.
 *
 * Calls the @get_layout function from the #Log4gAppenderClass of @self.
 *
 * Returns: The layout set for @self.
 * Since: 0.1
 */
Log4gLayout *
log4g_appender_get_layout(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return LOG4G_APPENDER_GET_CLASS(self)->get_layout(self);
}

/**
 * log4g_appender_set_name:
 * @self: A #Log4gAppender object.
 * @name: The new name for this appender.
 *
 * Calls the @set_name function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_set_name(Log4gAppender *self, const gchar *name)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->set_name(self, name);
}

/**
 * log4g_appender_requires_layout:
 * @self: A #Log4gAppender object.
 *
 * Calls the @requires_layout function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
gboolean
log4g_appender_requires_layout(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), FALSE);
    return LOG4G_APPENDER_GET_CLASS(self)->requires_layout(self);
}

/**
 * log4g_appender_activate_options:
 * @self: A #Log4gAppender object.
 *
 * Calls the @activate_options function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_activate_options(Log4gAppender *self)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->activate_options(self);
}
 /**
 * log4g_appender_append:
 * @self: A #Log4gAppender object.
 * @event: A log event.
 *
 * Calls the @append function from the #Log4gAppenderClass of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_append(Log4gAppender *self, Log4gLoggingEvent *event)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    LOG4G_APPENDER_GET_CLASS(self)->append(self, event);
}

/**
 * log4g_appender_get_first_filter:
 * @self: A #Log4gAppender object.
 *
 * Retrieve the first filter in the filter chain.
 *
 * Returns: The first filter in the filter chain, or %NULL if there is none.
 * Since: 0.1
 */
Log4gFilter *
log4g_appender_get_first_filter(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return GET_PRIVATE(self)->head;
}

/**
 * log4g_appender_is_as_severe_as:
 * @self: A #Log4gAppender object.
 * @level: A log level.
 *
 * Determine if a log level is below the appender's threshold.
 *
 * If there is no threshold set then the return value is always %TRUE.
 *
 * Returns: %TRUE if @level is above the level threshold of this appender,
 *          %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_appender_is_as_severe_as(Log4gAppender *self, Log4gLevel *level)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), FALSE);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    return ((priv->threshold == NULL)
                || (log4g_level_is_greater_or_equal(level, priv->threshold)));
}

/**
 * log4g_appender_set_threshold:
 * @self: A #Log4gAppender object.
 * @threshold: A string representation of a log level.
 *
 * Set the threshold property for this appender.
 *
 * Since: 0.1
 */
void
log4g_appender_set_threshold(Log4gAppender *self, const gchar *threshold)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    g_object_set(self, "threshold", threshold, NULL);
}

/**
 * log4g_appender_get_threshold:
 * @self: A #Log4gAppender object.
 *
 * Retrieve the threshold property.
 *
 * Returns: The threshold value for this appender.
 * Since: 0.1
 */
Log4gLevel *
log4g_appender_get_threshold(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    return GET_PRIVATE(self)->threshold;
}

/**
 * log4g_appender_get_closed:
 * @self: A #Log4gAppender object.
 *
 * Determine if an appender has been closed.
 *
 * Returns: %TRUE if this appender is closed, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_appender_get_closed(Log4gAppender *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), TRUE);
    return GET_PRIVATE(self)->closed;
}

/**
 * log4g_appender_set_closed:
 * @self: A #Log4gAppender object.
 * @closed: The new closed value for @self.
 *
 * Set the closed parameter.
 *
 * Appenders should set this value appropriately. The default value is %FALSE.
 * 
 * Returns: %TRUE if this appender is closed, %FALSE otherwise.
 * Since: 0.1
 */
void
log4g_appender_set_closed(Log4gAppender *self, gboolean closed)
{
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    GET_PRIVATE(self)->closed = closed;
}
