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
 * \brief Implements the API in log4g/helpers/thread.h
 * \author Mike Steinert
 * \date 2-14-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/thread.h"

G_DEFINE_TYPE(Log4gThread, log4g_thread, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_THREAD, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gchar *name;
};

/** \brief Thread specific data. */
static GPrivate *priv = NULL;

/** \brief Count the number of thread name requests. */
static gint counter = 0;

static void
log4g_thread_init(Log4gThread *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->name = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->name);
    priv->name = NULL;
    G_OBJECT_CLASS(log4g_thread_parent_class)->finalize(base);
}

static void
log4g_thread_class_init(Log4gThreadClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gThread *
log4g_thread_get_instance(void)
{
    if (g_thread_supported()) {
        static gsize once = 0;
        if (g_once_init_enter(&once)) {
            priv = g_private_new(g_object_unref);
            if (!priv) {
                return NULL;
            }
            g_once_init_leave(&once, 1);
        }
    }
    Log4gThread *self = (Log4gThread *)g_private_get(priv);
    if (!self) {
        self = g_object_new(LOG4G_TYPE_THREAD, NULL);
        if (!self) {
            return NULL;
        }
        g_private_set(priv, self);
    }
    return self;
}

void
log4g_thread_remove_instance(void)
{
    Log4gThread *self = (Log4gThread *)g_private_get(priv);
    if (self) {
        g_object_unref(self);
        g_private_set(priv, NULL);
    }
}

const gchar *
log4g_thread_get_name(void)
{
    Log4gThread *self = log4g_thread_get_instance();
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->name) {
        g_atomic_int_inc(&counter);
        priv->name = g_strdup_printf("thread%d", g_atomic_int_get(&counter));
    }
    return priv->name;
}

void
log4g_thread_set_name(const gchar *name)
{
    Log4gThread *self = log4g_thread_get_instance();
    if (!self) {
        return;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_free(priv->name);
    priv->name = g_strdup(name);
}
