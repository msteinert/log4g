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
 * SECTION: log
 * @short_description: Internal logging API
 * @title: Log4gInternalLogging
 * @section_id: Log4gInternalLogging
 *
 * This class is used to output log statements from within the Log4g package.
 *
 * Log4g components cannot use the Log4g API. This API allows Log4g to output
 * debugging statements to stdout. To enable debugging define the @debug
 * flag (Bash):
 * |[
 * export LOG4G_FLAGS=debug
 * ]|
 *
 * All internal @debug statements go to stdout, @error and @warn messages are
 * sent to stderr. All internal messages are prepended with the string
 * "log4g: ".
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>

/** Indicates if internal debugging is enabled */
static gboolean debug = FALSE;

/** Indicates if quiet mode is enabled */
static gboolean quiet = FALSE;

static void
log4g_log_log(FILE *stream, const gchar *prefix,
		const gchar *format, va_list ap)
{
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
	flockfile(stream);
	fputs_unlocked(prefix, stream);
#else
	fputs(prefix, stream);
#endif
	vfprintf(stream, format, ap);
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
	fputc_unlocked('\n', stream);
	funlockfile(stream);
#else
	fputc('\n', stream);
#endif
}

/**
 * log4g_log_debug:
 * @format: A printf message format.
 * @...: Format parameters.
 *
 * Log an debugging message.
 *
 * Since: 0.1
 */
void
log4g_log_debug(const gchar *format, ...)
{
	if (g_atomic_int_get(&debug) && !g_atomic_int_get(&quiet)) {
		va_list ap;
		va_start(ap, format);
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, ap);
#endif
		log4g_log_log(stdout, "log4g: ", format, ap);
		va_end(ap);
	}
}

/**
 * log4g_log_debugv:
 * @format: A printf message format.
 * @ap: Format parameters.
 *
 * Log an debugging message.
 *
 * Since: 0.1
 */
void
log4g_log_debugv(const gchar *format, va_list ap)
{
	if (g_atomic_int_get(&debug) && !g_atomic_int_get(&quiet)) {
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, ap);
#endif
		log4g_log_log(stdout, "log4g: ", format, ap);
	}
}

/**
 * log4g_log_warn:
 * @format: A printf message format.
 * @...: Format parameters.
 *
 * Log a warning message.
 *
 * Since: 0.1
 */
void
log4g_log_warn(const gchar *format, ...)
{
	if (!g_atomic_int_get(&quiet)) {
		va_list ap;
		va_start(ap, format);
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, format, ap);
#endif
		log4g_log_log(stderr, "log4g:WARN ", format, ap);
		va_end(ap);
	}
}

/**
 * log4g_log_warnv:
 * @format: A printf message format.
 * @ap: Format parameters.
 *
 * Log an warning message.
 *
 * Since: 0.1
 */
void
log4g_log_warnv(const gchar *format, va_list ap) 
{
	if (!g_atomic_int_get(&quiet)) {
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, format, ap);
#endif
		log4g_log_log(stderr, "log4g:WARN ", format, ap);
	}
}

/**
 * log4g_log_error:
 * @format: A printf message format.
 * @...: Format parameters.
 *
 * Log an error message.
 *
 * Since: 0.1
 */
void
log4g_log_error(const gchar *format, ...)
{
	if (!g_atomic_int_get(&quiet)) {
		va_list ap;
		va_start(ap, format);
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR, format, ap);
#endif
		log4g_log_log(stderr, "log4g:ERROR ", format, ap);
		va_end(ap);
	}
}

/**
 * log4g_log_errorv:
 * @format: A printf message format.
 * @ap: Format parameters.
 *
 * Log an error message.
 *
 * Since: 0.1
 */
void
log4g_log_errorv(const gchar *format, va_list ap)
{
	if (!g_atomic_int_get(&quiet)) {
#if 0
		g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR, format, ap);
#endif
		log4g_log_log(stderr, "log4g:ERROR ", format, ap);
	}
}

/**
 * log4g_set_internal_debugging:
 * @value: The new debugging flag.
 *
 * Set the internal debugging flag.
 *
 * If the debugging flag is %TRUE then Log4g will print debugging messages
 * to stdout.
 *
 * Since: 0.1
 */
void
log4g_set_internal_debugging(gboolean value)
{
	g_atomic_int_set(&debug, value);
}

/**
 * log4g_set_quiet_mode:
 * @value: The new quiet mode flag.
 *
 * Set the quiet mode flag.
 *
 * If the quiet mode flag is %TRUE then Log4g will not print any messages.
 *
 * Since: 0.1
 */
void
log4g_set_quiet_mode(gboolean value)
{
	g_atomic_int_set(&quiet, value);
}
