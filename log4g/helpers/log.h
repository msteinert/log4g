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

#ifndef LOG4G_LOG_H
#define LOG4G_LOG_H

#include <glib.h>

G_BEGIN_DECLS

void
log4g_log_debug(const gchar *format, ...) G_GNUC_PRINTF(1, 2);

void
log4g_log_debugv(const gchar *format, va_list ap);

void
log4g_log_warn(const gchar *format, ...) G_GNUC_PRINTF(1, 2);

void
log4g_log_warnv(const gchar *format, va_list ap); 

void
log4g_log_error(const gchar *format, ...) G_GNUC_PRINTF(1, 2);

void
log4g_log_errorv(const gchar *format, va_list ap); 

void
log4g_set_internal_debugging(gboolean value);

void
log4g_set_quiet_mode(gboolean value);

G_END_DECLS

#endif /* LOG4G_LOG_H */
