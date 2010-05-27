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
 * \brief Tests for Log4gBasicConfigurator
 * \author Mike Steinert
 * \date 2-22-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/basic-configurator.h"

#define CLASS "/log4g/BasicConfigurator"

void
test_001(gpointer *fixture, gconstpointer data)
{
    log4g_basic_configurator_configure();
    log4g_debug("test message");
    gint i = 10;
    log4g_ndc_push("i=%d", i);
    log4g_info("use%s log message",
            (g_random_double() >= 0.5 ? "ful" : "less"));
    gint j = 20;
    log4g_ndc_push("j=%d", j);
    log4g_error("trace message");
    log4g_ndc_pop();
    log4g_warn("don't accept wooden nickels");
    log4g_ndc_pop();
    log4g_fatal("O, I am slain!");
    g_assert(TRUE);
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
