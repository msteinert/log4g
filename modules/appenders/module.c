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
 * \brief Register type for the Appenders module
 * \author Mike Steinert
 * \date 5-26-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/async-appender.h"
#include "appender/console-appender.h"
#include "appender/file-appender.h"
#include "appender/null-appender.h"
#include "appender/rolling-file-appender.h"
#include "appender/syslog-appender.h"
#include "appender/writer-appender.h"
#include "helpers/counting-quiet-writer.h"
#include "helpers/quiet-writer.h"
#include <log4g/module.h>

void
log4g_module_load(GTypeModule *module)
{
    g_type_module_set_name(module, "core");
    log4g_async_appender_register(module);
    log4g_writer_appender_register(module);
    log4g_console_appender_register(module);
    log4g_counting_quiet_writer_register(module);
    log4g_file_appender_register(module);
    log4g_null_appender_register(module);
    log4g_quiet_writer_register(module);
    log4g_rolling_file_appender_register(module);
    log4g_syslog_appender_register(module);
}
