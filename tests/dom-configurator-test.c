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
 * \brief Tests for Log4gDOMConfigurator
 * \author Mike Steinert
 * \date 2-23-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include "log4g/dom-configurator.h"
#include <stdlib.h>
#include <unistd.h>

#define CLASS "/log4g/DOMConfigurator"

typedef struct _Fixture {
    GString *file;
} Fixture;

void setup(Fixture *fixture, gconstpointer data)
{
    gchar *file;
    fixture->file = g_string_sized_new(128);
    g_assert(fixture->file);
    file = getenv("LOG4G_DOM_CONFIGURATOR_TEST");
    if (file) {
        g_string_append(fixture->file, file);
    } else {
        g_string_append(fixture->file, "../../tests");
    }
}

void teardown(Fixture *fixture, gconstpointer data)
{
    g_string_free(fixture->file, TRUE);
}

void test_001(Fixture *fixture, gconstpointer data)
{
    Log4gLogger *logger;
    GError *error = NULL;
    g_string_append(fixture->file, "/dom-configurator-001.xml");
    g_assert(log4g_dom_configurator_configure(fixture->file->str, &error));
    log4g_debug("debug message (match this string)");
    logger = log4g_get_logger("org.gnome.test");
    g_assert(logger);
    log4g_logger_warn(logger, "warning message (match this string)");
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
