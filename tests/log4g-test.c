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
 * \brief Tests for Log4g initialization functions
 * \author Mike Steinert
 * \date 2-4-2010
 */

#include "config.h"
#include <glib.h>
#include "log4g/log4g.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLASS "/log4g"

void
test_001(gpointer *fixture, gconstpointer data)
{
    log4g_error("log4g-test: logging message (match this string)");
    log4g_logger_error(log4g_get_logger("org.gnome.test"),
            "log4g-test: another logging message (match this string)");
}

void
perf_001(gpointer *fixture, gconstpointer data)
{
    int i;
    int log = 1000000;
    double e;
    g_test_timer_start();
    for (i = 0; i < log; ++i) {
        log4g_trace("skip this message");
    }
    e = g_test_timer_elapsed();
    g_test_minimized_result(e, "skipped messages, rate=%d/second",
            (gint)(log / e));
}

void
perf_002(gpointer *fixture, gconstpointer data)
{
    int i;
    int log = 1000000;
    double e;
    g_test_timer_start();
    for (i = 0; i < log; ++i) {
        log4g_error("%d log this message", i);
    }
    e = g_test_timer_elapsed();
    g_test_minimized_result(e, "logged messages, rate=%d/second",
            (gint)(log / e));
}

void
perf_003(gpointer *fixture, gconstpointer data)
{
    int i;
    int log = 1000000;
    double e;
    FILE *file = fopen("file.txt", "w");
    g_test_timer_start();
    for (i = 0; i < log; ++i) {
        fprintf(file, "%d log this message\n", i);
    }
    e = g_test_timer_elapsed();
    g_test_minimized_result(e, "logged messages, rate=%d/second",
            (gint)(log / e));
}

int
main(int argc, char *argv[])
{
    GError *error = NULL;
    GOptionContext *context;
    int status = EXIT_SUCCESS;
    GOptionEntry entries[] = {
        { NULL }
    };
    g_test_init(&argc, &argv, NULL);
#ifndef G_THREADS_IMPL_NONE
    if (!g_thread_supported()) {
        g_thread_init(NULL);
    }
#endif
    context = g_option_context_new("- test Log4g initialization");
    if (!context) {
        return EXIT_FAILURE;
    }
    g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
    g_option_context_add_group(context, log4g_get_option_group());
    if (!g_option_context_parse(context, &argc, &argv, &error)) {
        log4g_error("option parsing failed: %s", error->message);
        g_error_free(error);
        return EXIT_FAILURE;
    }
    g_option_context_free(context);
    g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
    if (g_test_perf()) {
        g_test_add(CLASS"/perf/001", gpointer, NULL, NULL, perf_001, NULL);
        g_test_add(CLASS"/perf/002", gpointer, NULL, NULL, perf_002, NULL);
        g_test_add(CLASS"/perf/003", gpointer, NULL, NULL, perf_003, NULL);
    }
    status = g_test_run();
    log4g_finalize();
    return status;
}
