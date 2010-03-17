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
 * \brief Implements the API in log4g/basic-configurator.h
 * \author Mike Steinert
 * \date 2-22-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/basic-configurator.h"
#include "log4g/appender/console-appender.h"
#include "log4g/layout/ttcc-layout.h"
#include "log4g/log-manager.h"

void
log4g_basic_configurator_configure(void)
{
    Log4gAppender *appender = NULL;
    Log4gLayout *layout = NULL;
    Log4gLogger *root;
    root = log4g_logger_get_root_logger();
    if (!root) {
        return;
    }
    layout = log4g_ttcc_layout_new(NULL);
    if (!layout) {
        return;
    }
    appender = log4g_console_appender_new(layout, NULL);
    if (!appender) {
        goto exit;
    }
    log4g_logger_add_appender(root, appender);
exit:
    if (layout) {
        g_object_unref(layout);
    }
    if (appender) {
        g_object_unref(appender);
    }
}

void
log4g_basic_configurator_configure_appender(Log4gAppender *appender)
{
    Log4gLogger *root = log4g_logger_get_root_logger();
    if (!root) {
        return;
    }
    log4g_logger_add_appender(root, appender);
}

void
log4g_basic_configurator_reset_configuration()
{
    log4g_log_manager_reset_configuration();
}
