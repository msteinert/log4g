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
 * \brief ...
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifndef LOG4G_H
#define LOG4G_H
#ifndef LOG4G_DISABLE

#include <log4g/error.h>
#include <log4g/filter.h>
#include <log4g/interface/appender.h>
#include <log4g/level.h>
#include <log4g/logger.h>
#include <log4g/logging-event.h>
#include <log4g/mdc.h>
#include <log4g/ndc.h>

G_BEGIN_DECLS

/**
 */
void
log4g_init(int *argc, char ***argv);

/**
 */
void
log4g_finalize(void);

/**
 */
GOptionGroup *
log4g_get_option_group(void);

#ifdef LOG4G_LOG_DOMAIN
/**
 * \brief Retrieve the logger for the defined domain.
 * \param name [in] The name of the logger to retrieve.
 * \return A logger instance.
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_logger(name)
#else /* LOG4G_LOG_DOMAIN */
/** \brief Get the root logger if not set */
#define LOG4G_LOG_DOMAIN ("")
/**
 * \brief Retrieve the root logger.
 * \param name [in] Unused.
 * \return The root logger.
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_root_logger()
#endif /* LOG4G_LOG_DOMAIN */

/**
 */
#define log4g_get_logger(name) \
    log4g_logger_get_logger(name)

/**
 */
#define log4g_get_root_logger() \
    log4g_logger_get_root_logger()

/**
 */
#define log4g_assert(assertion, format, args...) \
    _log4g_logger_assert(_log4g_get_logger(LOG4G_LOG_DOMAIN), (assertion), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_assert(logger, assertion, format, args...) \
    _log4g_logger_trace(logger, (assertion), G_STRFUNC, __FILE__, \
            G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_is_trace_enabled() \
    log4g_logger_is_trace_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_trace(format, args...) \
    _log4g_logger_trace(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_trace(logger, format, args...) \
    _log4g_logger_trace(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 */
#define log4g_is_debug_enabled() \
    log4g_logger_is_debug_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_debug(format, args...) \
    _log4g_logger_debug(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_debug(logger, format, args...) \
    _log4g_logger_debug(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 */
#define log4g_is_info_enabled() \
    log4g_logger_is_info_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_info(format, args...) \
    _log4g_logger_info(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_info(logger, format, args...) \
    _log4g_logger_info(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 */
#define log4g_is_warn_enabled() \
    log4g_logger_is_warn_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_warn(format, args...) \
    _log4g_logger_warn(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_warn(logger, format, args...) \
    _log4g_logger_warn(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 */
#define log4g_is_error_enabled() \
    log4g_logger_is_error_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_error(format, args...) \
    _log4g_logger_error(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_error(logger, format, args...) \
    _log4g_logger_error(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 */
#define log4g_is_fatal_enabled() \
    log4g_logger_is_fatal_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 */
#define log4g_fatal(format, args...) \
    _log4g_logger_fatal(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 */
#define log4g_logger_fatal(logger, format, args...) \
    _log4g_logger_fatal(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

G_END_DECLS

#else /* LOG4G_DISABLE */
#include <log4g/disable.h>
#endif /* LOG4G_DISABLE */
#endif /* LOG4G_H */
