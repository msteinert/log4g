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
 * Tests for Log4gNDC
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/log4g.h"

#define CLASS "/log4g/NDC"

void
teardown(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_ndc_remove();
}

void
test_001(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_ndc_push("foo");
	g_assert_cmpstr(log4g_ndc_peek(), == , "foo");
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo");
	log4g_ndc_push("bar");
	g_assert_cmpstr(log4g_ndc_peek(), == , "bar");
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar");
	log4g_ndc_push("baz");
	g_assert_cmpstr(log4g_ndc_peek(), == , "baz");
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar baz");
	const gchar *string = log4g_ndc_pop();
	g_assert(string);
	g_assert_cmpstr(string, ==, "baz");
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar");
	string = log4g_ndc_pop();
	g_assert(string);
	g_assert_cmpstr(string, ==, "bar");
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo");
	string = log4g_ndc_pop();
	g_assert(string);
	g_assert_cmpstr(string, ==, "foo");
	g_assert(log4g_ndc_get() == NULL);
}

void
test_002(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_ndc_push("foo");
	log4g_ndc_push("bar");
	log4g_ndc_push("baz");
	g_assert(log4g_ndc_size() == 3);
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar baz");
	log4g_ndc_clear();
	g_assert(log4g_ndc_size() == 0);
	g_assert(log4g_ndc_get() == NULL);
}

void
test_003(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_ndc_push("foo");
	log4g_ndc_push("bar");
	log4g_ndc_push("baz");
	g_assert(log4g_ndc_size() == 3);
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar baz");
	log4g_ndc_set_max_depth(1);
	g_assert(log4g_ndc_size() == 1);
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo");
}

void
test_004(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	log4g_ndc_push("foo");
	log4g_ndc_push("bar");
	log4g_ndc_push("baz");
	g_assert(log4g_ndc_size() == 3);
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar baz");
	GArray *ndc = log4g_ndc_clone();
	g_assert(ndc);
	log4g_ndc_remove();
	log4g_ndc_inherit(ndc);
	g_assert(log4g_ndc_size() == 3);
	g_assert_cmpstr(log4g_ndc_get(), ==, "foo bar baz");
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
#if !GLIB_CHECK_VERSION(2, 36, 0)
	g_type_init();
#endif
	g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, teardown);
	g_test_add(CLASS"/002", gpointer, NULL, NULL, test_002, teardown);
	g_test_add(CLASS"/003", gpointer, NULL, NULL, test_003, teardown);
	g_test_add(CLASS"/004", gpointer, NULL, NULL, test_004, teardown);
	return g_test_run();
}
