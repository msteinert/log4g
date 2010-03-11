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
 * \brief Disables log4g.
 * \author Mike Steinert
 * \date 2-22-2010
 * Including this file instead of log4g/log4g.h will disable all logging in
 * your application. All functions defined in log4g/log4g.h, log4g/mdc.h, and
 * log4g/ndc.h will be disabled. If you are using other functions from the
 * log4g package then you may disable them by enclosing them in the
 * LOG4G_DISABLE macro.
 */

#ifndef LOG4G_DISABLE_H
#define LOG4G_DISABLE_H

G_BEGIN_DECLS

/** \brief ... */
#define log4g_init(argc, argv)

/** \brief ... */
#define log4g_get_option_group() (NULL)

/** \brief ... */
#define log4g_get_logger(name) (NULL)

/** \brief ... */
#define log4g_get_root_logger() (NULL)

/** \brief ... */
#define log4g_assert(assertion, format, args...)

/** \brief ... */
#define log4g_logger_assert(logger, assertion, format, args...)

/** \brief ... */
#define log4g_is_trace_enabled()

/** \brief ... */
#define log4g_trace(format, args...)

/** \brief ... */
#define log4g_logger_trace(logger, format, args...)

/** \brief ... */
#define log4g_is_debug_enabled()

/** \brief ... */
#define log4g_debug(format, args...)

/** \brief ... */
#define log4g_logger_debug(logger, format, args...)

/** \brief ... */
#define log4g_is_info_enabled()

/** \brief ... */
#define log4g_info(format, args...)

/** \brief ... */
#define log4g_logger_info(logger, format, args...)

/** \brief ... */
#define log4g_is_warn_enabled()

/** \brief ... */
#define log4g_warn(format, args...)

/** \brief ... */
#define log4g_logger_warn(logger, format, args...)

/** \brief ... */
#define log4g_is_error_enabled()

/** \brief ... */
#define log4g_error(format, args...)

/** \brief ... */
#define log4g_logger_warn(logger, format, args...)

/** \brief ... */
#define log4g_is_fatal_enabled()

/** \brief ... */
#define log4g_fatal(format, args...)

/** \brief ... */
#define log4g_logger_fatal(logger, format, args...)

/* MDC definitions */

/** \brief ... */
#define log4g_mdc_put(key, value, args...)

/** \brief ... */
#define log4g_mdc_get(key)

/** \brief ... */
#define log4g_mdc_remove(key)

/* NDC definitions */

/** \brief ... */
#define log4g_ndc_clear()

/** \brief ... */
#define log4g_ndc_clone() NULL

/** \brief ... */
#define log4g_ndc_inherit(stack)

/** \brief ... */
#define log4g_ndc_push(message, args...)

/** \brief ... */
#define log4g_ndc_pop() NULL

/** \brief ... */
#define log4g_ndc_pop() NULL 

/** \brief ... */
#define log4g_ndc_remove()

/** \brief ... */
#define log4g_ndc_set_max_depth(maxdepth)

G_END_DECLS

#endif /* LOG4G_DISABLE_H */
