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
 * \brief TODO
 * \author Mike Steinert
 * \date 5-26-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/module.h"
#include "async-appender.h"
#include "console-appender.h"
#include "file-appender.h"
#include "null-appender.h"
#include "rolling-file-appender.h"
#include "syslog-appender.h"
#include "writer-appender.h"

void
log4g_module_load(GTypeModule *module)
{
    g_type_module_set_name(module, "core");
    log4g_async_appender_register(module);
    log4g_writer_appender_register(module);
    log4g_console_appender_register(module);
    log4g_file_appender_register(module);
    log4g_null_appender_register(module);
    log4g_rolling_file_appender_register(module);
    log4g_syslog_appender_register(module);
}
