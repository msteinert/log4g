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
 * \brief Tests for Log4gDenyAllFilter
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include "log4g/filter/deny-all-filter.h"

#define CLASS "/log4g/filter/DenyAllFilter"

void
test_001(gpointer *fixture, gconstpointer data)
{
    Log4gFilter *filter = log4g_deny_all_filter_new();
    Log4gFilter *deny = log4g_deny_all_filter_new();
    g_assert(filter);
    g_assert(deny);
    log4g_filter_activate_options(filter);
    log4g_filter_set_next(filter, deny);
    g_assert(deny == log4g_filter_get_next(filter));
    g_object_unref(filter);
    g_object_unref(deny);
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
