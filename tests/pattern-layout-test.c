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
 * \brief Tests for Log4gPatternLayout
 * \author Mike Steinert
 * \date 2-12-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include "log4g/layout/pattern-layout.h"
#include <string.h>
#include <unistd.h>

#define CLASS "/log4g/layout/PatternLayout"

typedef struct _Fixture {
    Log4gLoggingEvent *event;
    gpointer level;
    gpointer le;
} Fixture;

void setup(Fixture *fixture, gconstpointer data)
{
    va_list ap;
    memset(&ap, 0, sizeof(ap));
    fixture->level = g_type_class_ref(LOG4G_TYPE_LEVEL);
    g_assert(fixture->level);
    fixture->le = g_type_class_ref(LOG4G_TYPE_LOGGING_EVENT);
    g_assert(fixture->le);
    log4g_mdc_put("foo", "bar");
    log4g_ndc_push("baz");
    fixture->event =
        log4g_logging_event_new("org.gnome.test", log4g_level_DEBUG(),
                __func__, __FILE__, G_STRINGIFY(__LINE__), "test message", ap);
    g_assert(fixture->event);
}

void teardown(Fixture *fixture, gconstpointer data)
{
    g_object_unref(fixture->event);
    g_type_class_unref(fixture->level);
    g_type_class_unref(fixture->le);
}

void test_001(Fixture *fixture, gconstpointer data)
{
    Log4gLayout *layout =
        log4g_pattern_layout_new("[%d{%c}] [%t] [%.7c{2}] %M(%F:%L) [%.5m] "
                "[%20p] [%r] [%X{foo}] [%x] %m%n");
    g_assert(layout);
    g_print("%s", log4g_layout_format(layout, fixture->event));
    g_object_unref(layout);
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);
    g_type_init();
#ifndef G_THREADS_IMPL_NONE
    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
#endif
    g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
    return g_test_run();
}
