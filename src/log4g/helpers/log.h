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
 * \file
 * \brief Internal logging API
 * \author Mike Steinert
 * \date 3-10-2010
 *
 * This class is used to output log statements from within the Log4g package.
 *
 * Log4g components cannot use the Log4g API. This API allows Log4g to output
 * debugging statements to stdout. To enable debugging define the \e debug
 * flag (Bash):
 * \code
 * export LOG4G_FLAGS=debug
 * \endcode
 *
 * All internal \e debug statements go to stdout, \e error and \e warn
 * messages are sent to stderr. All internal messages are prepended with
 * the string "log4g: ".
 */

#ifndef LOG4G_LOG_H
#define LOG4G_LOG_H

#include <glib.h>

G_BEGIN_DECLS

/**
 * \brief Log an debugging message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_debug(const gchar *format, ...);

/**
 * \brief Log an debugging message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_debugv(const gchar *format, va_list ap);

/**
 * \brief Log a warning message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_warn(const gchar *format, ...);

/**
 * \brief Log an warning message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_warnv(const gchar *format, va_list ap); 

/**
 * \brief Log an error message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_error(const gchar *format, ...);

/**
 * \brief Log an error message.
 *
 * \param format [in] A printf message format.
 * \param ... [in] Format parameters.
 */
void
log4g_log_errorv(const gchar *format, va_list ap); 

/**
 * \brief Set the internal debugging flag.
 *
 * If the debugging flag is \e TRUE then Log4g will print debugging
 * messages to stdout.
 *
 * \param The new debugging flag.
 */
void
log4g_set_internal_debugging(gboolean value);

/**
 * \brief Set the quiet mode flag.
 *
 * If the quiet mode flag is \e TRUE then Log4g will not print any messages.
 *
 * \param The new quiet mode flag.
 */
void
log4g_set_quiet_mode(gboolean value);

G_END_DECLS

#endif /* LOG4G_LOG_H */
