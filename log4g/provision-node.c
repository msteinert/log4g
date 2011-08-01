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
 * SECTION: provision-node
 * @short_description: Log repository temporary nodes
 *
 * Provision nodes are place holders for non-instantiated loggers within the
 * logger hierarchy.
 *
 * @see_also: #Log4gHierarchy
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/provision-node.h"

G_DEFINE_TYPE(Log4gProvisionNode, log4g_provision_node, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PROVISION_NODE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gProvisionNode *)instance)->priv)

struct Private {
	GArray *array;
};

static void
log4g_provision_node_init(Log4gProvisionNode *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->array = g_array_new(FALSE, FALSE, sizeof(gpointer));
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
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
	struct Private *priv = GET_PRIVATE(base);
	if (priv->array) {
		g_array_free(priv->array, TRUE);
		priv->array = NULL;
	}
	G_OBJECT_CLASS(log4g_provision_node_parent_class)->finalize(base);
}

static void
log4g_provision_node_class_init(Log4gProvisionNodeClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	/* initialize GObject */
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_provision_node_new:
 * @object: The object to add to this provision node.
 *
 * Create a #Log4gProvisionNode object and add an element.
 *
 * Returns: A new Log4gProvisionNode object.
 * Since: 0.1
 */
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

/**
 * log4g_provision_node_add_element:
 * @self: A provision node object.
 * @object: The object to add to @self.
 *
 * Add an element to a provision node.
 *
 * Since: 0.1
 */
void
log4g_provision_node_add_element(Log4gProvisionNode *self, gpointer object)
{
	g_return_if_fail(G_IS_OBJECT(object));
	g_object_ref(object);
	g_array_append_val(GET_PRIVATE(self)->array, object);
}

/**
 * log4g_provision_node_size:
 * @self: A provision node object.
 *
 * Retrieve the number of objects in a provision node.
 *
 * Since: 0.1
 */
guint
log4g_provision_node_size(Log4gProvisionNode *self)
{
	return GET_PRIVATE(self)->array->len;
}

/**
 * log4g_provision_node_element_at:
 * @self: A provision node object.
 * @index: The object index to retrieve.
 *
 * Retrieve an indexed element from a provision node.
 *
 * Returns: The object at @index or %NULL if none exists.
 * Since: 0.1
 */
gpointer
log4g_provision_node_element_at(Log4gProvisionNode *self, guint index)
{
	return g_array_index(GET_PRIVATE(self)->array, gpointer, index);
}
