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
 * \brief Tests for Log4gModule
 * \author Mike Steinert
 * \date 2-25-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/module.h"

#define CLASS "/log4g/Module"

void
test_001(gpointer *fixture, gconstpointer data)
{
    GTypeModule *module =
        log4g_module_new("../modules/appenders/liblog4g-appenders.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    g_assert(g_type_from_name("Log4gAsyncAppender"));
    g_assert(g_type_from_name("Log4gConsoleAppender"));
    g_assert(g_type_from_name("Log4gFileAppender"));
    g_assert(g_type_from_name("Log4gNullAppender"));
    g_assert(g_type_from_name("Log4gRollingFileAppender"));
    g_assert(g_type_from_name("Log4gSyslogAppender"));
    g_assert(g_type_from_name("Log4gWriterAppender"));
}

void
test_002(gpointer *fixture, gconstpointer data)
{
    GTypeModule *module =
        log4g_module_new("../modules/filters/liblog4g-filters.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    g_assert(g_type_from_name("Log4gDenyAllFilter"));
    g_assert(g_type_from_name("Log4gLevelMatchFilter"));
    g_assert(g_type_from_name("Log4gLevelRangeFilter"));
    g_assert(g_type_from_name("Log4gStringMatchFilter"));
}

void
test_003(gpointer *fixture, gconstpointer data)
{
    GTypeModule *module =
        log4g_module_new("../modules/layouts/liblog4g-layouts.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    g_assert(g_type_from_name("Log4gTTCCLayout"));
    g_assert(g_type_from_name("Log4gHTMLLayout"));
    g_assert(g_type_from_name("Log4gPatternLayout"));
    g_assert(g_type_from_name("Log4gSimpleLayout"));
    g_assert(g_type_from_name("Log4gXMLLayout"));
}

void
test_004(gpointer *fixture, gconstpointer data)
{
    GTypeModule *module =
        log4g_module_new("../modules/couchdb/liblog4g-couchdb.la");
    g_assert(module);
    g_assert(g_type_module_use(module));
    g_type_module_unuse(module);
    g_assert(g_type_from_name("Log4gCouchdbAppender"));
    g_assert(g_type_from_name("Log4gCouchdbLayout"));
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
    g_test_add(CLASS"/002", gpointer, NULL, NULL, test_002, NULL);
    g_test_add(CLASS"/003", gpointer, NULL, NULL, test_003, NULL);
    g_test_add(CLASS"/004", gpointer, NULL, NULL, test_004, NULL);
    return g_test_run();
}
