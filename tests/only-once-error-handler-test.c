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
 * Tests for Log4gOnlyOnceErrorHandler
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/only-once-error-handler.h"
#include "log4g/interface/error-handler.h"

#define CLASS "/log4g/helpers/OnlyOnceErrorHandler"

void
test_001(G_GNUC_UNUSED gpointer *fixture, G_GNUC_UNUSED gconstpointer data)
{
	Log4gErrorHandler *error = log4g_only_once_error_handler_new();
	g_assert(error);
	log4g_error_handler_set_logger(error, NULL);
	log4g_error_handler_error(error, NULL, "test %s message", "error");
	log4g_error_handler_error(error, NULL, "should not be printed");
	log4g_error_handler_set_appender(error, NULL);
	log4g_error_handler_set_backup_appender(error, NULL);
	g_object_unref(error);
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);
	g_test_add(CLASS"/001", gpointer, NULL, NULL, test_001, NULL);
	return g_test_run();
}
