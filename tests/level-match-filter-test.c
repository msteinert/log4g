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
 * Tests for Log4gLevelMatchFilter
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"
#include "log4g/module.h"

#define CLASS "/log4g/filter/LevelMatchFilter"

typedef struct Fixture_ {
	Log4gLoggingEvent *event;
} Fixture;

void
setup(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_mdc_put("foo", "bar");
	log4g_ndc_push("baz");
	va_list ap;
	memset(&ap, 0, sizeof ap);
	fixture->event = log4g_logging_event_new("org.gnome.test",
			log4g_level_DEBUG(), __func__, __FILE__,
			G_STRINGIFY(__LINE__), "test message", ap);
	g_assert(fixture->event);
}

void
teardown(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	g_object_unref(fixture->event);
}

void
test_001(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	GType type = g_type_from_name("Log4gLevelMatchFilter");
	g_assert(type);
	Log4gFilter *filter = g_object_new(type, NULL);
	g_assert(filter);
	log4g_filter_activate_options(filter);
	g_assert_cmpint(LOG4G_FILTER_NEUTRAL, ==,
			log4g_filter_decide(filter, fixture->event));
	g_object_set(filter, "level-to-match", "DEBUG", NULL);
	log4g_filter_activate_options(filter);
	g_assert_cmpint(LOG4G_FILTER_ACCEPT, ==,
			log4g_filter_decide(filter, fixture->event));
	g_object_set(filter, "accept-on-match", FALSE, NULL);
	log4g_filter_activate_options(filter);
	g_assert_cmpint(LOG4G_FILTER_DENY, ==,
			log4g_filter_decide(filter, fixture->event));
	g_object_set(filter, "level-to-match", "FATAL", NULL);
	log4g_filter_activate_options(filter);
	g_assert_cmpint(LOG4G_FILTER_NEUTRAL, ==,
			log4g_filter_decide(filter, fixture->event));
	g_object_unref(filter);
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
	GTypeModule *module =
		log4g_module_new("modules/filters/liblog4g-filters.la");
	g_assert(module);
	g_assert(g_type_module_use(module));
	g_type_module_unuse(module);
	g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
	return g_test_run();
}
