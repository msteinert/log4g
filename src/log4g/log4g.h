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
 * \brief The main Log4g interface.
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
 * \brief Initialize the Log4g package.
 *
 * Log4g accepts some standard command line arguments (all prepended with
 * \e log4g). Calling this function will parse out the arguments Log4g
 * understands and removes them from the argument list. If you call
 * log4g_init() before parsing application options your application will
 * not see the Log4g arguments.
 *
 * After calling this function the Log4g API is ready for use within your
 * application.
 *
 * \see log4g_get_option_group(), log4g_finalize()
 */
void
log4g_init(int *argc, char ***argv);

/**
 * \brief Finalize the Log4g package.
 *
 * Calling this function will destroy the logger hierarchy and any cleanup
 * any instance data used by Log4g.
 *
 * If you call this function you must call log4g_init() again before using
 * the Log4g API.
 */
void
log4g_finalize(void);

/**
 * \brief Get the option group used by the Log4g package.
 *
 * If your application is using the GLib option parser then you may call
 * this function to retrieve the option group parsed by Log4g.
 *
 * \note If you use the returned option group in a GLib option parser you
 *       do not need to call log4g_init().
 *
 * \return A GLib option group.
 */
GOptionGroup *
log4g_get_option_group(void);

#ifdef LOG4G_LOG_DOMAIN
/**
 * \brief Retrieve the logger for the defined domain.
 *
 * \param name [in] The name of the logger to retrieve.
 *
 * \return A logger instance.
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_logger(name)
#else /* LOG4G_LOG_DOMAIN */
/** \brief Get the root logger if not set */
#define LOG4G_LOG_DOMAIN ("")
/**
 * \brief Retrieve the root logger.
 *
 * \param name [in] Unused.
 *
 * \return The root logger.
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_root_logger()
#endif /* LOG4G_LOG_DOMAIN */

/**
 * \brief Retrieve a named logger from the repository.
 *
 * If the named logger exists in the repository that instance is returned. If
 * the named logger does not exist a new one is created.
 *
 * \see log4g_logger_get_logger()
 *
 * \param name [in] The name of the logger to retrieve.
 */
#define log4g_get_logger(name) \
    log4g_logger_get_logger(name)

/**
 * \brief Retrieve the root logger.
 *
 * \return The root logger.
 */
#define log4g_get_root_logger() \
    log4g_logger_get_root_logger()

/**
 * \brief Log an error if an \e assertion is \e FALSE.
 *
 * \param assertion [in] The assertion to evaluate.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_assert(), _log4g_logger_assert()
 */
#define log4g_assert(assertion, format, args...) \
    _log4g_logger_assert(_log4g_get_logger(LOG4G_LOG_DOMAIN), (assertion), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log an error if an \e assertion is \e FALSE.
 *
 * \param logger [in] A logger object.
 * \param assertion [in] The assertion to evaluate.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_assert(), _log4g_logger_assert()
 */
#define log4g_logger_assert(logger, assertion, format, args...) \
    _log4g_logger_trace(logger, (assertion), G_STRFUNC, __FILE__, \
            G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Check if the \e TRACE level is enabled.
 *
 * \see log4g_logger_is_trace_enabled(), log4g/level.h
 */
#define log4g_is_trace_enabled() \
    log4g_logger_is_trace_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e TRACE message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_trace(), _log4g_logger_trace()
 */
#define log4g_trace(format, args...) \
    _log4g_logger_trace(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_trace(logger, format, args...) \
    _log4g_logger_trace(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e DEBUG level is enabled.
 *
 * \see log4g_logger_is_debug_enabled(), log4g/level.h
 */
#define log4g_is_debug_enabled() \
    log4g_logger_is_debug_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e DEBUG message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_debug(), _log4g_logger_debug()
 */
#define log4g_debug(format, args...) \
    _log4g_logger_debug(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_debug(logger, format, args...) \
    _log4g_logger_debug(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e INFO level is enabled.
 *
 * \see log4g_logger_is_info_enabled(), log4g/level.h
 */
#define log4g_is_info_enabled() \
    log4g_logger_is_info_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e INFO message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_info(), _log4g_logger_info()
 */
#define log4g_info(format, args...) \
    _log4g_logger_info(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_info(logger, format, args...) \
    _log4g_logger_info(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e WARN level is enabled.
 *
 * \see log4g_logger_is_warn_enabled(), log4g/level.h
 */
#define log4g_is_warn_enabled() \
    log4g_logger_is_warn_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e WARN message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_warn(), _log4g_logger_warn()
 */
#define log4g_warn(format, args...) \
    _log4g_logger_warn(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_warn(logger, format, args...) \
    _log4g_logger_warn(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e ERROR level is enabled.
 *
 * \see log4g_logger_is_error_enabled(), log4g/level.h
 */
#define log4g_is_error_enabled() \
    log4g_logger_is_error_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e WARN message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_warn(), _log4g_logger_warn()
 */
#define log4g_error(format, args...) \
    _log4g_logger_error(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_error(logger, format, args...) \
    _log4g_logger_error(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e FATAL level is enabled.
 *
 * \see log4g_logger_is_fatal_enabled(), log4g/level.h
 */
#define log4g_is_fatal_enabled() \
    log4g_logger_is_fatal_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e FATAL message.
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_fatal(), _log4g_logger_fatal()
 */
#define log4g_fatal(format, args...) \
    _log4g_logger_fatal(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

#define log4g_logger_fatal(logger, format, args...) \
    _log4g_logger_fatal(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

G_END_DECLS

#else /* LOG4G_DISABLE */
#include <log4g/disable.h>
#endif /* LOG4G_DISABLE */
#endif /* LOG4G_H */
