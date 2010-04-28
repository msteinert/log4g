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
 * \brief Tests for Log4gHierarchy
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include "log4g/log4g.h"
#include "log4g/appender/console-appender.h"
#include "log4g/hierarchy.h"
#include "log4g/layout/ttcc-layout.h"
#include "log4g/root-logger.h"

#define CLASS "/log4g/Hierarchy"

typedef struct _Fixture {
    Log4gLoggerRepository *repository;
} Fixture;

void
setup(Fixture *fixture, gconstpointer data)
{
    Log4gLogger *root;
    Log4gAppender *appender;
    Log4gLayout *layout;
    root = log4g_root_logger_new(log4g_level_DEBUG());
    g_assert(root);
    layout =  log4g_ttcc_layout_new("%c");
    g_assert(layout);
    appender = log4g_console_appender_new(layout, "stdout");
    g_assert(appender);
    g_object_unref(layout);
    fixture->repository = log4g_hierarchy_new(root);
    g_assert(fixture->repository);
    log4g_logger_add_appender(root, appender);
    g_object_unref(root);
    g_object_unref(appender);
}

void
teardown(Fixture *fixture, gconstpointer data)
{
    g_object_unref(fixture->repository);
}

void
test_001(Fixture *fixture, gconstpointer data)
{
    Log4gLogger *logger =
        log4g_logger_repository_get_logger(fixture->repository,
                "org.gnome.test");
    g_assert(logger);
    log4g_logger_debug(logger, "test message");
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
    g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
    return g_test_run();
}
