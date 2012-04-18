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

/**
 * SECTION: basic-configurator
 * @short_description: Quickly configure Log4g
 * @title: Log4gBasicConfigurator
 * @section_id: Log4gBasicConfigurator
 *
 * For file based configuration, see #Log4gDOMConfigurator
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/appender.h"
#include "log4g/basic-configurator.h"
#include "log4g/layout.h"
#include "log4g/log-manager.h"

/**
 * log4g_basic_configurator_configure:
 *
 * Quickly configure Log4g.
 *
 * This function adds a console appender to the root logger that uses the
 * TTCC layout and prints to stdout.
 *
 * See: #Log4gRootLogger, #Log4gTTCCLayout
 *
 * Since: 0.1
 */
void
log4g_basic_configurator_configure(void)
{
	Log4gLayout *layout = NULL;
	Log4gAppender *appender = NULL;
	Log4gLogger *root = log4g_logger_get_root_logger();
	if (!root) {
		log4g_log_error(Q_("log4g_logger_get_root_logger() "
					"returned NULL"));
		return;
	}
	GType type = g_type_from_name("Log4gTTCCLayout");
	if (!type) {
		log4g_log_warn(Q_("Log4gTTCCLayout: type not found"));
		return;
	}
	layout = g_object_new(type, NULL);
	if (!layout) {
		log4g_log_error(Q_("g_object_new() returned NULL"));
		return;
	}
	log4g_layout_activate_options(layout);
	type = g_type_from_name("Log4gConsoleAppender");
	if (!type) {
		log4g_log_warn(Q_("Log4gTTCCLayout: type not found"));
		goto exit;
	}
	appender = g_object_new(type, NULL);
	if (!layout) {
		log4g_log_error(Q_("g_object_new() returned NULL"));
		goto exit;
	}
	log4g_appender_set_layout(appender, layout);
	log4g_appender_activate_options(appender);
	log4g_logger_add_appender(root, appender);
exit:
	if (layout) {
		g_object_unref(layout);
	}
	if (appender) {
		g_object_unref(appender);
	}
}

/**
 * log4g_basic_configurator_configure_appender:
 * @appender: The appender to add to the root category.
 *
 * Add an appender to the root category.
 *
 * Since: 0.1
 */
void
log4g_basic_configurator_configure_appender(Log4gAppender *appender)
{
	Log4gLogger *root = log4g_logger_get_root_logger();
	if (!root) {
		return;
	}
	log4g_logger_add_appender(root, appender);
}

/**
 * log4g_basic_configurator_reset_configuration:
 *
 * Reset the default hierarchy to its default state.
 *
 * Since: 0.1
 */
void
log4g_basic_configurator_reset_configuration()
{
	log4g_log_manager_reset_configuration();
}
