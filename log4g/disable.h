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

/* Disable logging.
 *
 * Including this file instead of log4g/log4g.h will disable all logging in
 * your application. All functions and macros defined in log4g/log4g.h,
 * log4g/mdc.h, and log4g/ndc.h will be disabled. If you are using other
 * functions from the log4g package then you may disable them by enclosing
 * them in the LOG4G_DISABLE macro.
 */

#ifndef LOG4G_DISABLE_H
#define LOG4G_DISABLE_H

G_BEGIN_DECLS

/* log4g/log4g.h definitions */

#define log4g_init(argc, argv)

#define log4g_get_option_group() (NULL)

#define log4g_get_logger(name) (NULL)

#define log4g_get_root_logger() (NULL)

#define log4g_assert(assertion, format, args...)

#define log4g_logger_assert(logger, assertion, format, args...)

#define log4g_is_trace_enabled() (FALSE)

#define log4g_trace(format, args...)

#define log4g_logger_trace(logger, format, args...)

#define log4g_is_debug_enabled() (FALSE)

#define log4g_debug(format, args...)

#define log4g_logger_debug(logger, format, args...)

#define log4g_is_info_enabled() (FALSE)

#define log4g_info(format, args...)

#define log4g_logger_info(logger, format, args...)

#define log4g_is_warn_enabled() (FALSE)

#define log4g_warn(format, args...)

#define log4g_logger_warn(logger, format, args...)

#define log4g_is_error_enabled() (FALSE)

#define log4g_error(format, args...)

#define log4g_logger_error(logger, format, args...)

#define log4g_logger_warn(logger, format, args...)

#define log4g_is_fatal_enabled() (FALSE)

#define log4g_fatal(format, args...)

#define log4g_logger_fatal(logger, format, args...)

/* log4g/mdc.h definitions */

#define log4g_mdc_put(key, value, args...)

#define log4g_mdc_get(key) (NULL)

#define log4g_mdc_remove(key)

/* log4g/ndc.h definitions */

#define log4g_ndc_clear()

#define log4g_ndc_clone() (NULL)

#define log4g_ndc_inherit(stack)

#define log4g_ndc_get() (NULL)

#define log4g_ndc_push(message, args...)

#define log4g_ndc_peek() (NULL)

#define log4g_ndc_pop() (NULL)

#define log4g_ndc_remove()

#define log4g_ndc_set_max_depth(maxdepth)

G_END_DECLS

#endif /* LOG4G_DISABLE_H */
