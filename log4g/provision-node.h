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

#ifndef LOG4G_PROVISION_NODE_H
#define LOG4G_PROVISION_NODE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_PROVISION_NODE \
    (log4g_provision_node_get_type())

#define LOG4G_PROVISION_NODE(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_PROVISION_NODE, \
            Log4gProvisionNode))

#define LOG4G_IS_PROVISION_NODE(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_PROVISION_NODE))

#define LOG4G_PROVISION_NODE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_PROVISION_NODE, \
            Log4gProvisionNodeClass))

#define LOG4G_IS_PROVISION_NODE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_PROVISION_NODE))

#define LOG4G_PROVISION_NODE_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_PROVISION_NODE, \
            Log4gProvisionNodeClass))

typedef struct _Log4gProvisionNode Log4gProvisionNode;

typedef struct _Log4gProvisionNodeClass Log4gProvisionNodeClass;

/**
 * Log4gProvisionNode:
 *
 * The <structname>Log4gProvisionNode</structname> structure does not have any
 * public members.
 */
struct _Log4gProvisionNode {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gProvisionNodeClass:
 *
 * The <structname>Log4gProvisionNodeClass</structname> structure does not have
 * any public members.
 */
struct _Log4gProvisionNodeClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_provision_node_get_type(void);

Log4gProvisionNode *
log4g_provision_node_new(gpointer object);

void
log4g_provision_node_add_element(Log4gProvisionNode *self, gpointer object);

guint
log4g_provision_node_size(Log4gProvisionNode *self);

gpointer
log4g_provision_node_element_at(Log4gProvisionNode *self, guint index);

G_END_DECLS

#endif /* LOG4G_PROVISION_NODE_H */
