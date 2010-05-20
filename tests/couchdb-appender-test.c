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
 * \brief Tests for Log4gFileAppender
 * \author Mike Steinert
 * \date 2-9-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include "log4g/appender/couchdb-appender.h"
#include <string.h>
#include <unistd.h>

#define CLASS "/log4g/appender/CouchDBAppender"

void
test_001(gpointer *fixture, gconstpointer data)
{
    Log4gAppender *appender = log4g_couchdb_appender_new(NULL, NULL);
    g_assert(appender);
    va_list ap = { 0 };
    for (gint i = 0; i < 5; ++i) {
        log4g_ndc_push("LOOP %d", i);
        Log4gLoggingEvent *event =
            log4g_logging_event_new("org.gnome.test", log4g_level_DEBUG(),
                    __func__, __FILE__, G_STRINGIFY(__LINE__),
                    "test message", ap);
        g_assert(event);
        log4g_appender_do_append(appender, event);
        g_object_unref(event);
        usleep(20);
    }
    g_object_unref(appender);
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
