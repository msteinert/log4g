/* Copyright 2010, 2011 Michael Steinert
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
 * Tests for Log4gHierarchy
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/hierarchy.h"
#include "log4g/module.h"
#include "log4g/root-logger.h"

#define CLASS "/log4g/Hierarchy"

typedef struct Fixture_ {
	Log4gLoggerRepository *repository;
} Fixture;

void
setup(Fixture *fixture, gconstpointer data)
{
	Log4gLogger *root = log4g_root_logger_new(log4g_level_DEBUG());
	g_assert(root);
	GType type = g_type_from_name("Log4gTTCCLayout");
	g_assert(type);
	Log4gLayout *layout = g_object_new(type,
			"date-format", "%c",
			NULL);
	g_assert(layout);
	log4g_layout_activate_options(layout);
	type = g_type_from_name("Log4gConsoleAppender");
	g_assert(type);
	Log4gAppender *appender = g_object_new(type,
			"target", "stdout",
			NULL);
	g_assert(appender);
	log4g_appender_set_layout(appender, layout);
	log4g_appender_activate_options(appender);
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
	Log4gLogger *logger = log4g_logger_repository_get_logger(
			fixture->repository, "org.gnome.test");
	g_assert(logger);
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
	g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
	return g_test_run();
}
