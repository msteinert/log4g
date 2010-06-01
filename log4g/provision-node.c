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
 * \brief Implements the API in log4g/provision-node.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/provision-node.h"

G_DEFINE_TYPE(Log4gProvisionNode, log4g_provision_node, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PROVISION_NODE, \
            struct Log4gPrivate))

struct Log4gPrivate {
    GArray *array;
};

static void
log4g_provision_node_init(Log4gProvisionNode *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->array = g_array_new(FALSE, FALSE, sizeof(gpointer));
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->array) {
        GObject *object;
        for (guint i = 0; i < priv->array->len; ++i) {
            object = g_array_index(priv->array, gpointer, i);
            if (object) {
                g_object_unref(object);
            }
        }
        g_array_remove_range(priv->array, 0, priv->array->len);
    }
    G_OBJECT_CLASS(log4g_provision_node_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->array) {
        g_array_free(priv->array, TRUE);
        priv->array = NULL;
    }
    G_OBJECT_CLASS(log4g_provision_node_parent_class)->finalize(base);
}

static void
log4g_provision_node_class_init(Log4gProvisionNodeClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gProvisionNode *
log4g_provision_node_new(gpointer object)
{
    Log4gProvisionNode *self;
    g_return_val_if_fail(G_IS_OBJECT(object), NULL);
    self = g_object_new(LOG4G_TYPE_PROVISION_NODE, NULL);
    if (!self) {
        return NULL;
    }
    log4g_provision_node_add_element(self, object);
    return self;
}

void
log4g_provision_node_add_element(Log4gProvisionNode *self, gpointer object)
{
    g_return_if_fail(G_IS_OBJECT(object));
    g_object_ref(object);
    g_array_append_val(GET_PRIVATE(self)->array, object);
}

guint
log4g_provision_node_size(Log4gProvisionNode *self)
{
    return GET_PRIVATE(self)->array->len;
}

gpointer
log4g_provision_node_element_at(Log4gProvisionNode *self, guint index)
{
    return g_array_index(GET_PRIVATE(self)->array, gpointer, index);
}