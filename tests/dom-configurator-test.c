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
 * Tests for Log4gDOMConfigurator
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/dom-configurator.h"
#include "log4g/log4g.h"
#include <stdlib.h>

#define CLASS "/log4g/DOMConfigurator"

typedef struct Fixture_ {
	GString *file;
} Fixture;

void
setup(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	const gchar *file = getenv("srcdir");
	fixture->file = g_string_new(file ? file : "../../tests");
	g_assert(fixture->file);
}

void
teardown(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	g_string_free(fixture->file, TRUE);
}

void
test_001(Fixture *fixture, G_GNUC_UNUSED gconstpointer data)
{
	g_string_append(fixture->file, "tests/dom-configurator-001.xml");
	GError *error = NULL;
	g_assert(log4g_dom_configurator_configure(fixture->file->str, &error));
	log4g_debug("debug message (match this string)");
	Log4gLogger *logger = log4g_get_logger("org.gnome.test");
	g_assert(logger);
	log4g_logger_warn(logger, "warning message (match this string)");
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
	g_test_add(CLASS"/001", Fixture, NULL, setup, test_001, teardown);
	return g_test_run();
}
