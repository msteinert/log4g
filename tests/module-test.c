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
 * Tests for Log4gModule
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/module.h"

#define CLASS "/log4g/Module"

void
test_001(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GTypeModule *module =
		log4g_module_new("modules/appenders/liblog4g-appenders.la");
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
test_002(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GTypeModule *module =
		log4g_module_new("modules/filters/liblog4g-filters.la");
	g_assert(module);
	g_assert(g_type_module_use(module));
	g_type_module_unuse(module);
	g_assert(g_type_from_name("Log4gDenyAllFilter"));
	g_assert(g_type_from_name("Log4gLevelMatchFilter"));
	g_assert(g_type_from_name("Log4gLevelRangeFilter"));
	g_assert(g_type_from_name("Log4gStringMatchFilter"));
}

void
test_003(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GTypeModule *module =
		log4g_module_new("modules/layouts/liblog4g-layouts.la");
	g_assert(module);
	g_assert(g_type_module_use(module));
	g_type_module_unuse(module);
	g_assert(g_type_from_name("Log4gTTCCLayout"));
	g_assert(g_type_from_name("Log4gHTMLLayout"));
	g_assert(g_type_from_name("Log4gPatternLayout"));
	g_assert(g_type_from_name("Log4gSimpleLayout"));
	g_assert(g_type_from_name("Log4gXMLLayout"));
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
#ifdef LOG4G_WITH_APPENDERS
	g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
#endif
#ifdef LOG4G_WITH_FILTERS
	g_test_add(CLASS"/002", gpointer, NULL, NULL, test_002, NULL);
#endif
#ifdef LOG4G_WITH_LAYOUTS
	g_test_add(CLASS"/003", gpointer, NULL, NULL, test_003, NULL);
#endif
	return g_test_run();
}
