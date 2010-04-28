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
 * \file
 * \brief Tests for Log4gProvisionNode
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include "log4g/provision-node.h"

#define CLASS "/log4g/ProvisionNode"

void
test_001(gpointer *fixture, gconstpointer data)
{
    Log4gLogger *l0 = log4g_logger_new("org.gnome.test");
    Log4gLogger *l1 = log4g_logger_new("org.gnome.test.more");
    Log4gProvisionNode *node;
    g_assert(l0);
    g_assert(l1);
    node = log4g_provision_node_new(G_OBJECT(l0));
    g_assert(node);
    g_assert_cmpuint(log4g_provision_node_size(node), == , 1);
    log4g_provision_node_add_element(node, G_OBJECT(l1));
    g_assert_cmpuint(log4g_provision_node_size(node), == , 2);
    g_assert(LOG4G_LOGGER(log4g_provision_node_element_at(node, 0)) == l0);
    g_assert(LOG4G_LOGGER(log4g_provision_node_element_at(node, 1)) == l1);
    g_object_unref(node);
    g_object_unref(l0);
    g_object_unref(l1);
}

int
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_type_init();
#ifndef G_THREADS_IMPL_NONE
    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
#endif
    g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
    return g_test_run();
}
