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
 * SECTION: mdc
 * @short_description: mapped data context
 *
 * The MDC class provides mapped data contexts. A mapped data context (MDC for
 * short) is an instrument for distinguishing interleaved log output from
 * different sources. An example of interleaved log output may occur when a
 * server handles multiple clients simultaneously.
 *
 * Mapped data context is managed on a per-thread basis. The main difference
 * between Log4g MDCs and Log4j MDCs is that Log4g contexts are
 * <emphasis>not</emphasis> inherited by child threads.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/mdc.h"

G_DEFINE_TYPE(Log4gMDC, log4g_mdc, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_MDC, \
            struct Log4gPrivate))

struct Log4gPrivate {
    GHashTable *table;
};

/* Thread specific data. */
static GPrivate *priv = NULL;

static void
log4g_mdc_init(Log4gMDC *self)
{
    GET_PRIVATE(self)->table =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
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
    GObject *self = g_private_get(priv);
    if (!self) {
        self = G_OBJECT_CLASS(log4g_mdc_parent_class)->
            constructor(type, n, params);
        if (!self) {
            return NULL;
        }
        g_private_set(priv, self);
    } else {
        g_object_ref(self);
    }
    return self;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->table) {
        g_hash_table_destroy(priv->table);
        priv->table = NULL;
    }
    G_OBJECT_CLASS(log4g_mdc_parent_class)->finalize(base);
}

static void
log4g_mdc_class_init(Log4gMDCClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->constructor = constructor;
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

/**
 * log4g_mdc_get_instance:
 *
 * Retrieve the MDC instance object for currently executing thread.
 *
 * Returns: A mapped data context object.
 */
static Log4gMDC *
log4g_mdc_get_instance(void)
{
    Log4gMDC *self = g_private_get(priv);
    if (!self) {
        self = g_object_new(LOG4G_TYPE_MDC, NULL);
    }
    return self;
}

/**
 * log4g_mdc_put:
 * @key: The key to associate with @value.
 * @value: The value to associate with @key (accepts printf formats).
 * @...: Format parameters.
 *
 * Put a context @value as identified by a @key into the current thread's
 * context map.
 *
 * If a context map has not been created for the current thread it will be
 * created as a side-effect.
 *
 * Since: 0.1
 */
void
log4g_mdc_put(const gchar *key, const gchar *value, ...)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return;
    }
    va_list ap;
    va_start(ap, value);
    g_hash_table_insert(GET_PRIVATE(self)->table, g_strdup(key),
            g_strdup_vprintf(value, ap));
    va_end(ap);
}

/**
 * log4g_mdc_get:
 * @key: The key to retrieve.
 *
 * Retrieve a the context value associated with a @key from the current
 * thread's context map.
 *
 * Returns: The context value associated with @key.
 * Since: 0.1
 */
const gchar *
log4g_mdc_get(const gchar *key)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return NULL;
    }
    return g_hash_table_lookup(GET_PRIVATE(self)->table, key);
}

/**
 * log4g_mdc_remove:
 * @key: The key to remove.
 *
 * Remove a context value associated with a @key from the current thread's
 * context map.
 *
 * Since: 0.1
 */
void
log4g_mdc_remove(const gchar *key)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return;
    }
    g_hash_table_remove(GET_PRIVATE(self)->table, key);
}

/**
 * log4g_mdc_get_context:
 *
 * Retrieve the current thread's MDC as a hash table.
 *
 * This function is used internally by appenders that log asynchronously.
 *
 * Returns: The current MDC context as a hash table.
 *
 * Since: 0.1
 */
const GHashTable *
log4g_mdc_get_context(void)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return NULL;
    }
    return GET_PRIVATE(self)->table;
}
