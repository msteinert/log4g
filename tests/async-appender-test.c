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
 * \brief Tests for Log4gAsyncAppender
 * \author Mike Steinert
 * \date 2-17-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/module.h"

#define CLASS "/log4g/appender/AsyncAppender"

typedef struct _Fixture {
    Log4gLoggingEvent *event;
} Fixture;

void
setup(Fixture *fixture, gconstpointer data)
{
    va_list ap;
    memset(&ap, 0, sizeof ap);
    fixture->event =
        log4g_logging_event_new("org.gnome.test", log4g_level_DEBUG(),
                __func__, __FILE__, G_STRINGIFY(__LINE__), "test message", ap);
    g_assert(fixture->event);
}

void
teardown(Fixture *fixture, gconstpointer data)
{
    g_object_unref(fixture->event);
}

void
test_001(Fixture *fixture, gconstpointer data)
{
    Log4gLayout *layout = log4g_simple_layout_new();
    g_assert(layout);
    Log4gAppender *out = log4g_console_appender_new(layout, "stdout");
    g_assert(out);
    Log4gAppender *err = log4g_console_appender_new(layout, "stderr");
    g_assert(err);
    g_object_unref(layout);
    Log4gAppender *appender = log4g_async_appender_new();
    g_assert(appender);
    log4g_async_appender_add_appender(appender, out);
    log4g_async_appender_add_appender(appender, err);
    g_object_unref(out);
    g_object_unref(err);
    log4g_appender_do_append(appender, fixture->event);
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
        log4g_module_new("../modules/appenders/liblog4g-appenders.la");
    g_assert(module);
    g_type_module_use(module);
    g_type_module_unuse(module);
    g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
    return g_test_run();
}
