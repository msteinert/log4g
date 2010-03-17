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
 * \brief Implements the API in log4g/mdc.h
 * \author Mike Steinert
 * \date 2-3-2010
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

/** \brief Thread specific data. */
static GPrivate *priv = NULL;

static void
log4g_mdc_init(Log4gMDC *self)
{
    GET_PRIVATE(self)->table =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
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
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gMDC *
log4g_mdc_get_instance(void)
{
    Log4gMDC *self;
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
    self = (Log4gMDC *)g_private_get(priv);
    if (!self) {
        self = g_object_new(LOG4G_TYPE_MDC, NULL);
        if (!self) {
            return NULL;
        }
        g_private_set(priv, self);
    }
    return self;
}

void
log4g_mdc_put(const gchar *key, const gchar *value, ...)
{
    va_list ap;
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return;
    }
    va_start(ap, value);
    g_hash_table_insert(GET_PRIVATE(self)->table, g_strdup(key),
            g_strdup_vprintf(value, ap));
    va_end(ap);
}

const gchar *
log4g_mdc_get(const gchar *key)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return NULL;
    }
    return g_hash_table_lookup(GET_PRIVATE(self)->table, key);
}

void
log4g_mdc_remove(const gchar *key)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return;
    }
    g_hash_table_remove(GET_PRIVATE(self)->table, key);
}

const GHashTable *
log4g_mdc_get_context(void)
{
    Log4gMDC *self = log4g_mdc_get_instance();
    if (!self) {
        return NULL;
    }
    return GET_PRIVATE(self)->table;
}
