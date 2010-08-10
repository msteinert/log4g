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

/*
 * Tests for Log4gMDC
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"

#define CLASS "/log4g/MDC"

void
test_001(gpointer *fixture, gconstpointer data)
{
    log4g_mdc_put("foo", "bar");
    const gchar *string = log4g_mdc_get("foo");
    g_assert(string);
    g_assert_cmpstr(string, ==, "bar");
    log4g_mdc_remove("foo");
    string = log4g_mdc_get("foo");
    g_assert(!string);
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
