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
 * \brief Implements the API in log4g/appender-attachable-impl.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#include "config.h"
#include "log4g/helpers/appender-attachable-impl.h"

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, struct Log4gPrivate))

struct Log4gPrivate {
    GArray *list;
};

static void
add_appender(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_return_if_fail(appender);
    if (!priv->list) {
        priv->list =
            g_array_sized_new(TRUE, FALSE, sizeof(Log4gAppender *), 1);
        if (!priv->list) {
            return;
        }
    }
    g_object_ref(appender);
    g_array_append_val(priv->list, appender);
}

static const GArray *
get_all_appenders(Log4gAppenderAttachable *base)
{
    return GET_PRIVATE(base)->list;
}

static Log4gAppender *
get_appender(Log4gAppenderAttachable *base, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gAppender *appender;
    guint i;
    if (!priv->list || !name) {
        return NULL;
    }
    for (i = 0; i < priv->list->len; ++i) {
        appender = g_array_index(priv->list, Log4gAppender *, i);
        if (!appender) {
            continue;
        }
        if (!g_strcmp0(name, log4g_appender_get_name(appender))) {
            g_object_ref(appender);
            return appender;
        }
    }
    return NULL;
}

static gboolean
is_attached(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gAppender *stored;
    guint i;
    if (!priv->list || !appender) {
        return FALSE;
    }
    for (i = 0; i < priv->list->len; ++i) {
        stored = g_array_index(priv->list, Log4gAppender *, i);
        if (!stored) {
            continue;
        }
        if (stored == appender) {
            return TRUE;
        }
    }
    return FALSE;
}

static void
remove_all_appenders(Log4gAppenderAttachable *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gAppender *appender;
    guint i;
    if (!priv->list) {
        return;
    }
    for (i = 0; i < priv->list->len; ++i) {
        appender = g_array_index(priv->list, Log4gAppender *, i);
        if (!appender) {
            continue;
        }
        g_object_unref(appender);
    }
    g_array_free(priv->list, TRUE);
    priv->list = NULL;
}

static void
remove_appender(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gAppender *stored;
    guint i;
    if (!priv->list) {
        return;
    }
    for (i = 0; i < priv->list->len; ++i) {
        stored = g_array_index(priv->list, Log4gAppender *, i);
        if (!stored) {
            continue;
        }
        if (stored == appender) {
            g_object_unref(stored);
            g_array_remove_index(priv->list, i);
            return;
        }
    }
}

static void
remove_appender_name(Log4gAppenderAttachable *base, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gAppender *appender;
    guint i;
    if (!priv->list || !name) {
        return;
    }
    for (i = 0; i < priv->list->len; ++i) {
        appender = g_array_index(priv->list, Log4gAppender *, i);
        if (!appender) {
            continue;
        }
        if (!g_strcmp0(name, log4g_appender_get_name(appender))) {
            g_object_unref(appender);
            g_array_remove_index(priv->list, i);
            return;
        }
    }
}

static void
log4g_appender_attachable_impl_interface_init(
        Log4gAppenderAttachableInterface *interface, gpointer data)
{
    interface->add_appender = add_appender;
    interface->get_all_appenders = get_all_appenders;
    interface->get_appender = get_appender;
    interface->is_attached = is_attached;
    interface->remove_all_appenders = remove_all_appenders;
    interface->remove_appender = remove_appender;
    interface->remove_appender_name = remove_appender_name;
}

G_DEFINE_TYPE_WITH_CODE(Log4gAppenderAttachableImpl,
        log4g_appender_attachable_impl, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER_ATTACHABLE,
                log4g_appender_attachable_impl_interface_init))

static void
log4g_appender_attachable_impl_init(Log4gAppenderAttachableImpl *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->list = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->list) {
        guint i;
        Log4gAppender *appender;
        for (i = 0; i < priv->list->len; ++i) {
            appender = g_array_index(priv->list, Log4gAppender *, i);
            g_object_unref(appender);
        }
        g_array_free(priv->list, TRUE);
    }
    G_OBJECT_CLASS(
            log4g_appender_attachable_impl_parent_class)->finalize(base);
}

static void
log4g_appender_attachable_impl_class_init(
        Log4gAppenderAttachableImplClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gAppenderAttachable *
log4g_appender_attachable_impl_new(void)
{
    return g_object_new(LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, NULL);
}

guint
log4g_appender_attachable_impl_append_loop_on_appenders(
        Log4gAppenderAttachable *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv;
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE_IMPL(base), 0);
    priv = GET_PRIVATE(base);
    Log4gAppender *appender;
    guint size = 0, i;
    if (!priv->list) {
        return 0;
    }
    for (i = 0; i < priv->list->len; ++i) {
        appender = g_array_index(priv->list, Log4gAppender *, i);
        if (!appender) {
            continue;
        }
        log4g_appender_do_append(appender, event);
        ++size;
    }
    return size;
}
