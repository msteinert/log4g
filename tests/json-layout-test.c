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
 * Tests for Log4gJsonLayout
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/module.h"

#define CLASS "/log4g/layout/JsonLayout"

typedef struct Fixture_ {
	Log4gLoggingEvent *event0;
	Log4gLoggingEvent *event1;
} Fixture;

void
setup(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_mdc_put("foo", "FOO");
	log4g_mdc_put("bar", "BAR");
	log4g_mdc_put("baz", "BAZ");
	log4g_ndc_push("foo");
	log4g_ndc_push("bar");
	log4g_ndc_push("baz");
	va_list ap;
	memset(&ap, 0, sizeof ap);
	fixture->event0 = log4g_logging_event_new("org.gnome.test",
			log4g_level_DEBUG(), __func__, __FILE__,
			G_STRINGIFY(__LINE__),
			"The quick brown fox jumps over the lazy dog", ap);
	g_assert(fixture->event0);
	fixture->event1 = log4g_logging_event_new("org.gnome.test",
			log4g_level_WARN(), __func__, __FILE__,
			G_STRINGIFY(__LINE__),
			"Lorem ipsum dolor sit amet, consectetur adipisicing elit", ap);
	g_assert(fixture->event1);
}

void
teardown(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_mdc_remove("foo");
	log4g_mdc_remove("bar");
	log4g_mdc_remove("baz");
	log4g_ndc_remove();
	g_object_unref(fixture->event0);
	g_object_unref(fixture->event1);
}

void
test_001(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GType type = g_type_from_name("Log4gJsonLayout");
	gchar *s0, *s1;
	g_assert(type);
	Log4gLayout *layout = g_object_new(type,
			"properties", TRUE,
			"location-info", TRUE,
			NULL);
	g_assert(layout);
	log4g_layout_activate_options(layout);
	s0 = g_strdup(log4g_layout_format(layout, fixture->event0));
	s1 = g_strdup(log4g_layout_format(layout, fixture->event1));
	g_message("%s%s%s%s",
			log4g_layout_get_header(layout),
			s0, s1,
			log4g_layout_get_footer(layout));
	g_free(s0);
	g_free(s1);
	g_object_unref(layout);
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
	g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
	return g_test_run();
}
