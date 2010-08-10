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
 * Tests for Log4gSyslogAppender
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/module.h"
#include <syslog.h>
#include <unistd.h>

#define CLASS "/log4g/appender/SyslogAppender"

void
test_001(gpointer *fixture, gconstpointer data)
{
    GType type = g_type_from_name("Log4gSimpleLayout");
    g_assert(type);
    Log4gLayout *layout = g_object_new(type, NULL);
    g_assert(layout);
    log4g_layout_activate_options(layout);
    type = g_type_from_name("Log4gSyslogAppender");
    g_assert(type);
    Log4gAppender *appender =
        g_object_new(type, "ident", "syslog-appender-test",
                "option", LOG_USER, "facility", LOG_CONS, NULL);
    g_assert(appender);
    log4g_appender_set_layout(appender, layout);
    log4g_appender_activate_options(appender);
    g_object_unref(layout);
    for (gint i = 0; i < 5; ++i) {
        log4g_ndc_push("LOOP %d", i);
        va_list ap;
        memset(&ap, 0, sizeof ap);
        Log4gLoggingEvent *event =
            log4g_logging_event_new("org.gnome.test", log4g_level_INFO(),
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
    GTypeModule *module =
        log4g_module_new("../modules/layouts/liblog4g-layouts.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    module = log4g_module_new("../modules/appenders/liblog4g-appenders.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
    return g_test_run();
}
