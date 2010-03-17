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
 * \brief Implements the API in log4g/appender/appender-skeleton.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/appender/appender-skeleton.h"
#include "log4g/helpers/only-once-error-handler.h"
#include "log4g/interface/option-handler.h"

enum _properties_t {
    PROP_O = 0,
    PROP_THRESHOLD,
    PROP_MAX
};

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_APPENDER_SKELETON, \
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
add_filter(Log4gAppender *base, Log4gFilter *filter)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!priv->head) {
        g_object_ref(filter);
        priv->head = priv->tail = filter;
    } else {
        log4g_filter_set_next(priv->tail, filter);
        priv->tail = filter;
    }
}

static Log4gFilter *
get_filter(Log4gAppender *base)
{
    return GET_PRIVATE(base)->head;
}

static void
do_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gLevel *level;
    gint decision;
    g_mutex_lock(priv->lock);
    if (priv->closed) {
        log4g_log_warn(Q_("attempted to append to closed appender named [%s]"),
                priv->name);
        goto exit;
    }
    level = log4g_logging_event_get_level(event);
    if (!log4g_appender_skeleton_is_as_severe_as(base, level)) {
        goto exit;
    }
    if (priv->head) {
        Log4gFilter *filter = priv->head;
        while (filter) {
            decision = log4g_filter_decide(filter, event);
            if (LOG4G_FILTER_DENY == decision) {
                goto exit;
            } else if (LOG4G_FILTER_ACCEPT == decision) {
                break;
            } else if (LOG4G_FILTER_NEUTRAL == decision) {
                filter = log4g_filter_get_next(filter);
            }
        }
    }
    log4g_appender_skeleton_append(base, event);
exit:
    g_mutex_unlock(priv->lock);
}

static const gchar *
get_name(Log4gAppender *base)
{
    return GET_PRIVATE(base)->name;
}

static void
set_error_handler(Log4gAppender *base, gpointer error)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    if (G_UNLIKELY(!error)) {
        g_debug(Q_("you have tried to set a NULL error-handler"));
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
get_error_handler(Log4gAppender *base)
{
    return GET_PRIVATE(base)->error;
}

static void
set_layout(Log4gAppender *base, Log4gLayout *layout)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->layout) {
        g_object_unref(priv->layout);
    }
    if (layout) {
        g_object_ref(layout);
    }
    priv->layout = layout;
}

static Log4gLayout *
get_layout(Log4gAppender *base)
{
    return GET_PRIVATE(base)->layout;
}

static void
set_name(Log4gAppender *base, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->name) {
        g_free(priv->name);
    }
    priv->name = g_strdup(name);
}

static void
appender_init(Log4gAppenderInterface *interface, gpointer data)
{
    interface->add_filter = add_filter;
    interface->get_filter = get_filter;
    interface->close = NULL;
    interface->do_append = do_append;
    interface->get_name = get_name;
    interface->set_error_handler = set_error_handler;
    interface->get_error_handler = get_error_handler;
    interface->set_layout = set_layout;
    interface->get_layout = get_layout;
    interface->set_name = set_name;
    interface->requires_layout = NULL;
}

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = activate_options;
}

G_DEFINE_TYPE_WITH_CODE(Log4gAppenderSkeleton, log4g_appender_skeleton,
        G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init)
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

static void
log4g_appender_skeleton_init(Log4gAppenderSkeleton *self)
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
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->layout) {
        g_object_unref(priv->layout);
    }
    if (priv->name) {
        g_free(priv->name);
        priv->name = NULL;
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
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    G_OBJECT_CLASS(log4g_appender_skeleton_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *threshold;
    switch (id) {
    case PROP_THRESHOLD:
        if (priv->threshold) {
            g_object_unref(priv->threshold);
        }
        threshold = g_value_get_string(value);
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
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
log4g_appender_skeleton_class_init(Log4gAppenderSkeletonClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_THRESHOLD,
            g_param_spec_string("threshold", Q_("Threshold"),
                    Q_("Threshold for this appender"), NULL,
                    G_PARAM_WRITABLE));
}

void
log4g_appender_skeleton_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    Log4gAppenderSkeletonClass *klass;
    g_return_if_fail(LOG4G_IS_APPENDER_SKELETON(base));
    klass = LOG4G_APPENDER_SKELETON_GET_CLASS(base);
    klass->append(base, event);
}

void
log4g_appender_skeleton_clear_filters(Log4gAppender *base)
{
    struct Log4gPrivate *priv;
    g_return_if_fail(LOG4G_IS_APPENDER_SKELETON(base));
    priv = GET_PRIVATE(base);
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = priv->tail = NULL;
    }
}

Log4gFilter *
log4g_appender_skeleton_get_first_filter(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_SKELETON(base), NULL);
    return GET_PRIVATE(base)->head;
}

Log4gLevel *
log4g_appender_skeleton_get_threshold(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_SKELETON(base), NULL);
    return GET_PRIVATE(base)->threshold;
}

gboolean
log4g_appender_skeleton_is_as_severe_as(Log4gAppender *base, Log4gLevel *level)
{
    struct Log4gPrivate *priv;
    g_return_val_if_fail(LOG4G_IS_APPENDER_SKELETON(base), FALSE);
    priv = GET_PRIVATE(base);
    return ((priv->threshold == NULL)
                || (log4g_level_is_greater_or_equal(level, priv->threshold)));
}

void
log4g_appender_skeleton_set_threshold(Log4gAppender *base,
                                      const gchar *threshold)
{
    g_return_if_fail(LOG4G_IS_APPENDER_SKELETON(base));
    g_object_set(base, "threshold", threshold, NULL);
}

gboolean
log4g_appender_skeleton_get_closed(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_SKELETON(base), TRUE);
    return GET_PRIVATE(base)->closed;
}

void
log4g_appender_skeleton_set_closed(Log4gAppender *base, gboolean closed)
{
    g_return_if_fail(LOG4G_IS_APPENDER_SKELETON(base));
    GET_PRIVATE(base)->closed = closed;
}
