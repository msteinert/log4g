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
 * \brief Implements the API in log4g/helpers/log.h
 * \author Mike Steinert
 * \date 3-10-2010
 */

#include "config.h"
#include <stdio.h>

/** \brief Indicates if internal debugging is enabled */
static gboolean debug = FALSE;

/** \brief Indicates if quiet mode is enabled */
static gboolean quiet = FALSE;

static void
log4g_log(FILE *stream, const gchar *prefix, const gchar *format, va_list ap)
{
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
        if (g_thread_supported()) {
            flockfile(stream);
        }
#endif
        fputs(prefix, stream);
        vfprintf(stream, format, ap);
        fputc('\n', stream);
#ifdef _POSIX_THREAD_SAFE_FUNCTIONS
        if (g_thread_supported()) {
            funlockfile(stream);
        }
#endif
}

void
log4g_debug(const gchar *format, ...)
{
    if (g_atomic_int_get(&debug) && !g_atomic_int_get(&quiet)) {
        va_list ap;
        va_start(ap, format);
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, ap); */
        log4g_log(stdout, "log4g: ", format, ap);
        va_end(ap);
    }
}

void
log4g_debugv(const gchar *format, va_list ap)
{
    if (g_atomic_int_get(&debug) && !g_atomic_int_get(&quiet)) {
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, format, ap); */
        log4g_log(stdout, "log4g: ", format, ap);
    }
}

void
log4g_warn(const gchar *format, ...)
{
    if (!g_atomic_int_get(&quiet)) {
        va_list ap;
        va_start(ap, format);
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, format, ap); */
        log4g_log(stderr, "log4g:WARN ", format, ap);
        va_end(ap);
    }
}

void
log4g_warnv(const gchar *format, va_list ap) 
{
    if (!g_atomic_int_get(&quiet)) {
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_WARNING, format, ap); */
        log4g_log(stderr, "log4g:WARN ", format, ap);
    }
}

void
log4g_error(const gchar *format, ...)
{
    if (!g_atomic_int_get(&quiet)) {
        va_list ap;
        va_start(ap, format);
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR, format, ap); */
        log4g_log(stderr, "log4g:ERROR ", format, ap);
        va_end(ap);
    }
}

void
log4g_errorv(const gchar *format, va_list ap)
{
    if (!g_atomic_int_get(&quiet)) {
        /* g_logv(G_LOG_DOMAIN, G_LOG_LEVEL_ERROR, format, ap); */
        log4g_log(stderr, "log4g:ERROR ", format, ap);
    }
}

void
log4g_set_internal_debugging(gboolean value)
{
    g_atomic_int_set(&debug, value);
}

void
log4g_set_quiet_mode(gboolean value)
{
    g_atomic_int_set(&quiet, value);
}
