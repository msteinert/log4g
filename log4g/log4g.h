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

#ifndef LOG4G_H
#define LOG4G_H
#ifndef LOG4G_DISABLE

#include <log4g/error.h>
#include <log4g/filter.h>
#include <log4g/appender.h>
#include <log4g/level.h>
#include <log4g/logger.h>
#include <log4g/logging-event.h>
#include <log4g/mdc.h>
#include <log4g/ndc.h>

G_BEGIN_DECLS

#ifdef LOG4G_LOG_DOMAIN
/**
 * _log4g_get_logger:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve the logger for the defined domain.
 *
 * This macro is meant to used internally.
 *
 * @See: log4g_get_logger()
 *
 * Returns: A logger instance.
 * Since: 0.1
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_logger(name)
#else /* LOG4G_LOG_DOMAIN */
#define LOG4G_LOG_DOMAIN ("")
/**
 * _log4g_get_logger:
 * @name: Unused.
 *
 * Retrieve a named logger or the root logger.
 *
 * This macro is meant to used internally.
 *
 * @See: log4g_get_logger(), log4g_get_root_logger()
 *
 * Returns: The root logger.
 * Since: 0.1
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_root_logger()
#endif /* LOG4G_LOG_DOMAIN */

void
log4g_init(int *argc, char ***argv);

void
log4g_finalize(void);

GOptionGroup *
log4g_get_option_group(void);

/**
 * log4g_get_logger:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger from the repository.
 *
 * If the named logger exists in the repository that instance is returned. If
 * the named logger does not exist it will be created.
 *
 * @See: #Log4gLoggerClass, log4g_logger_get_logger()
 *
 * Returns: The logger named @name.
 * Since: 0.1
 */
#define log4g_get_logger(name) \
    log4g_logger_get_logger(name)

/**
 * log4g_get_root_logger:
 *
 * Retrieve the root logger.
 *
 * @See: #Log4gRootLoggerClass
 *
 * Returns: The root logger.
 * Since: 0.1
 */
#define log4g_get_root_logger() \
    log4g_logger_get_root_logger()

/**
 * log4g_assert:
 * @assertion: The assertion to evaluate.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an error if an @assertion is %FALSE.
 *
 * Example:
 *
 * |[
 * log4g_assert((pointer != NULL), "%s: pointer is NULL!", "ERROR");
 * ]|
 *
 * @See: log4g_logger_assert(), _log4g_logger_assert()
 *
 * Since: 0.1
 */
#define log4g_assert(assertion, format, args...) \
    _log4g_logger_assert(_log4g_get_logger(LOG4G_LOG_DOMAIN), (assertion), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_assert:
 * @logger: A logger object.
 * @assertion: The assertion to evaluate.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an error if an @assertion is %FALSE.
 *
 * Example:
 *
 * |[
 * log4g_logger_assert(log4g_get_logger("org.gnome.debug"),
 *     (NULL != pointer), "%s: pointer is NULL!", "ERROR");
 * ]|
 *
 * @See: log4g_assert(), _log4g_logger_assert()
 *
 * Since: 0.1
 */
#define log4g_logger_assert(logger, assertion, format, args...) \
    _log4g_logger_trace(logger, (assertion), G_STRFUNC, __FILE__, \
            G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_is_trace_enabled:
 *
 * Check if the trace level is enabled.
 *
 * Use this macro if you need to perform a resource-intensive operation
 * before logging a message.
 *
 * Example:
 *
 * |[
 * if (log4g_is_trace_enabled()) {
 *     char *string = perform_IO_intensive_operation();
 *     log4g_trace("here I am: %s", string);
 * }
 * ]|
 *
 * <note><para>
 * A similar macro exists for all of the default log levels.
 * </para></note>
 *
 * @See: log4g_logger_is_trace_enabled(), log4g/level.h
 *
 * Returns: %TRUE if trace is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_trace_enabled() \
    log4g_logger_is_trace_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_trace:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a trace message.
 *
 * Example:
 *
 * |[
 * log4g_trace("example %s message", "TRACE");
 * ]|
 *
 * @See: log4g_logger_trace(), _log4g_logger_trace()
 *
 * Since: 0.1
 */
#define log4g_trace(format, args...) \
    _log4g_logger_trace(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_trace:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a trace message.
 *
 * Example:
 *
 * |[
 * log4g_logger_trace(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "TRACE");
 * ]|
 *
 * @See: log4g_trace(), _log4g_logger_trace()
 *
 * Since: 0.1
 */
#define log4g_logger_trace(logger, format, args...) \
    _log4g_logger_trace(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * log4g_is_debug_enabled:
 *
 * Check if the debug level is enabled.
 *
 * @See: log4g_logger_is_debug_enabled(), #Log4gLevelClass,
 *       log4g_is_trace_enabled()
 *
 * Returns: %TRUE if debug is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_debug_enabled() \
    log4g_logger_is_debug_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_debug:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a debug message.
 *
 * Example:
 *
 * |[
 * log4g_debug("example %s message", "DEBUG");
 * ]|
 *
 * @See: log4g_logger_debug(), _log4g_logger_debug()
 *
 * Since: 0.1
 */
#define log4g_debug(format, args...) \
    _log4g_logger_debug(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_debug:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a debug message.
 *
 * Example:
 *
 * |[
 * log4g_logger_debug(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "DEBUG");
 * ]|
 *
 * @See: log4g_debug(), _log4g_logger_debug()
 *
 * Since: 0.1
 */
#define log4g_logger_debug(logger, format, args...) \
    _log4g_logger_debug(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * log4g_is_info_enabled:
 *
 * Check if the info level is enabled.
 *
 * @See: log4g_logger_is_info_enabled(), #Log4gLevelClass,
*        log4g_is_trace_enabled()
 *
 * Returns: %TRUE if info is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_info_enabled() \
    log4g_logger_is_info_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_info:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an info message.
 *
 * Example:
 *
 * |[
 * log4g_info("example %s message", "INFO");
 * ]|
 *
 * @See: log4g_logger_info(), _log4g_logger_info()
 *
 * Since: 0.1
 */
#define log4g_info(format, args...) \
    _log4g_logger_info(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_info:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an info message.
 *
 * Example:
 *
 * |[
 * log4g_logger_info(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "INFO");
 * ]|
 *
 * @See: log4g_info(), _log4g_logger_info()
 *
 * Since: 0.1
 */
#define log4g_logger_info(logger, format, args...) \
    _log4g_logger_info(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * log4g_is_warn_enabled:
 *
 * Check if the warn level is enabled.
 *
 * @See: log4g_logger_is_warn_enabled(), #Log4gLevelClass,
 *       log4g_is_trace_enabled()
 *
 * Returns: %TRUE if warn is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_warn_enabled() \
    log4g_logger_is_warn_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_warn:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a warn message.
 *
 * Example:
 *
 * |[
 * log4g_warn("example %s message", "WARN");
 * ]|
 *
 * @See: log4g_logger_warn(), _log4g_logger_warn()
 *
 * Since: 0.1
 */
#define log4g_warn(format, args...) \
    _log4g_logger_warn(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_warn:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a warn message.
 *
 * Example:
 *
 * |[
 * log4g_logger_warn(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "WARN");
 * ]|
 *
 * @See: log4g_warn(), _log4g_logger_warn()
 *
 * Since: 0.1
 */
#define log4g_logger_warn(logger, format, args...) \
    _log4g_logger_warn(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * log4g_is_error_enabled:
 *
 * Check if the error level is enabled.
 *
 * @See: log4g_logger_is_error_enabled(), #Log4gLevelClass,
 *       log4g_is_trace_enabled()
 *
 * Returns: %TRUE if error is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_error_enabled() \
    log4g_logger_is_error_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_error:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an error message.
 *
 * Example:
 *
 * |[
 * log4g_error("example %s message", "ERROR");
 * ]|
 *
 * @See: log4g_logger_error(), _log4g_logger_error()
 *
 * Since: 0.1
 */
#define log4g_error(format, args...) \
    _log4g_logger_error(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_error:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log an error message.
 *
 * Example:
 *
 * |[
 * log4g_logger_error(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "ERROR");
 * ]|
 *
 * @See: log4g_error(), _log4g_logger_error()
 *
 * Since: 0.1
 */
#define log4g_logger_error(logger, format, args...) \
    _log4g_logger_error(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * log4g_is_fatal_enabled:
 *
 * Check if the fatal level is enabled.
 *
 * @See: log4g_logger_is_fatal_enabled(), #Log4gLevelClass,
 *      log4g_is_trace_enabled()
 *
 * Returns: %TRUE if fatal is enabled, %FALSE otherwise.
 * Since: 0.1
 */
#define log4g_is_fatal_enabled() \
    log4g_logger_is_fatal_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * log4g_fatal:
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a fatal message.
 *
 * Example:
 *
 * |[
 * log4g_fatal("example %s message", "FATAL");
 * ]|
 *
 * @See: log4g_logger_fatal(), _log4g_logger_fatal()
 *
 * Since: 0.1
 */
#define log4g_fatal(format, args...) \
    _log4g_logger_fatal(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * log4g_logger_fatal:
 * @logger: A logger object.
 * @format: A printf formatted message.
 * @args...: Format parameters.
 *
 * Log a fatal message.
 *
 * Example:
 *
 * |[
 * log4g_logger_fatal(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "FATAL");
 * ]|
 *
 * @See: log4g_fatal(), _log4g_logger_fatal()
 *
 * Since: 0.1
 */
#define log4g_logger_fatal(logger, format, args...) \
    _log4g_logger_fatal(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

G_END_DECLS

#else /* LOG4G_DISABLE */
#include <log4g/disable.h>
#endif /* LOG4G_DISABLE */
#endif /* LOG4G_H */
