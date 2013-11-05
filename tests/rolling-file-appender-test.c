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
 * Tests for Log4gRollingFileAppender
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/module.h"
#include <unistd.h>

#define CLASS "/log4g/appender/RollingFileAppender"

void
test_001(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GType type = g_type_from_name("Log4gTTCCLayout");
	g_assert(type);
	Log4gLayout *layout = g_object_new(type, NULL);
	g_assert(layout);
	log4g_layout_activate_options(layout);
	type = g_type_from_name("Log4gRollingFileAppender");
	g_assert(type);
	Log4gAppender *appender = g_object_new(type,
			"file", "tests/rolling-file-appender-test.txt",
			"max-backup-index", 4,
			"maximum-file-size", 10,
			NULL);
	g_assert(appender);
	log4g_appender_set_layout(appender, layout);
	log4g_appender_activate_options(appender);
	g_object_unref(layout);
	for (gint i = 0; i < 10; ++i) {
		va_list ap;
		memset(&ap, 0, sizeof ap);
		Log4gLoggingEvent *event = log4g_logging_event_new(
				"org.gnome.test", log4g_level_DEBUG(),
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
#if !GLIB_CHECK_VERSION(2, 36, 0)
	g_type_init();
#endif
	GTypeModule *module =
		log4g_module_new("modules/layouts/liblog4g-layouts.la");
	g_assert(module);
	g_assert(g_type_module_use(module));
	g_type_module_unuse(module);
	module = log4g_module_new("modules/appenders/liblog4g-appenders.la");
	g_assert(module);
	g_assert(g_type_module_use(module));
	g_type_module_unuse(module);
	g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
	return g_test_run();
}
