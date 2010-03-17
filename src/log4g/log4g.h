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
 *
 * The file defines the API that most users of Log4g will want to use.
 * Sticking to this API will allow you to easily compile Log4g out of your
 * application by defining LOG4G_DISABLE before including this file (i.e. in
 * config.h, etc).
 *
 * You may also wish to refer to the documentation for NDC (nested data
 * contexts) & MDC (mapped data contexts). The NDC & MDC APIs can also be
 * compiled out of your code.
 *
 * Defining LOG4G_LOG_DOMAIN before you include log4g/log4g.h will set the
 * log domain for the current file. Log statements that do not specify a
 * logger will use the LOG4G_LOG_DOMAIN logger by default.
 *
 * Example (log a message to the "org.gnome.test" logger):
 *
 * \code
 * #define LOG4G_LOG_DOMAIN "org.gnome.test"
 * #include <log4g/log4g.h>
 *
 * void
 * foo(void)
 * {
 *     log4g_warn("you had better watch out");
 * }
 * \endcode
 *
 * If LOG4G_LOG_DOMAIN is not defined then log statements will use the root
 * logger by default. You may also explicitly specify a logger when you log
 * a message.
 *
 * \see log4g/ndc.h, log4g/mdc.h
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

#ifdef LOG4G_LOG_DOMAIN
/**
 * \internal
 * \brief Retrieve the logger for the defined domain.
 *
 * This macro is meant to used internally.
 *
 * \param name [in] The name of the logger to retrieve.
 *
 * \return A logger instance.
 *
 * \see log4g_get_logger()
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_logger(name)
#else /* LOG4G_LOG_DOMAIN */
/**
 * \internal
 * \brief Get the root logger if not set
 */
#define LOG4G_LOG_DOMAIN ("")
/**
 * \internal
 * \brief Retrieve a named logger or the root logger.
 *
 * This macro is meant to used internally.
 *
 * \param name [in] Unused.
 *
 * \return The root logger.
 *
 * \see log4g_get_logger(), log4g_get_root_logger()
 */
#define _log4g_get_logger(name) \
    log4g_logger_get_root_logger()
#endif /* LOG4G_LOG_DOMAIN */

/**
 * \brief Initialize the Log4g package.
 *
 * Log4g accepts some standard command line arguments (all prepended with
 * \e log4g). Calling this function will parse out the arguments Log4g
 * understands and removes them from the argument list. If you call
 * log4g_init() before parsing application options your application will
 * not see the Log4g arguments.
 *
 * Example (initialize Log4g):
 *
 * \code
 * #include <log4g/log4g.h>
 * #include <stdlib.h>
 *
 * int
 * main(int argc, char **argv)
 * {
 *     g_thread_init(NULL);
 *     log4g_init(&argc, &argv);
 *     log4g_info("Log4g is initialized & ready to go!");
 *     return EXIT_SUCCESS;
 * }
 * \endcode
 *
 * Log4g accepts the following command line arguments:
 *
 * \arg --log4g-configuration=<FILE.xml>
 * \c Specify an XML configuration file to load. If the XML configuration file
 * could not be loaded (or if none was specified) then Log4g will be
 * configured using the basic configurator.
 *
 * \arg --log4g-flags=<FLAGS>
 * \c Specify flags that modify the behavior of Log4g. Currently Log4g
 * understands two flags.
 * -# debug: enable debug output on stdout
 * -# quiet: disable all error and debug output
 *
 * \arg --log4g-main-thread=<NAME>
 * \c Set the name of the main thread (the default is "main").
 *
 * \arg --log4g-version
 * \c Display the version of Log4g that your application is linked with.
 *
 * After calling this function the Log4g API is ready for use within your
 * application.
 *
 * \note You must call g_thread_init() before initializing Log4g if your
 *       application uses threads.
 *
 * \param argc [in/out] A pointer to main's \e argc variable.
 * \param argv [in/out] A pointer to main's \e argv variable.
 *
 * \see log4g_get_option_group(), log4g_finalize(), log4g/dom-configurator.h
 *      log4g/basic-configurator.h, log4g/helpers/log.h, log4g/helpers/thread.h
 */
void
log4g_init(int *argc, char ***argv);

/**
 * \brief Finalize the Log4g package.
 *
 * It is not strictly necessary to call this function to use Log4g.
 *
 * Calling this function will destroy the logger hierarchy and any cleanup
 * any instance data used by Log4g. One advantage of calling log4g_finalize()
 * is to allow asynchronous appenders a chance to flush any pending log
 * messages before your application exits.
 *
 * Example (initialize & cleanup Log4g):
 *
 * \code
 * #include <log4g/log4g.h>
 * #include <stdlib.h>
 *
 * int
 * main(int argc, char **argv)
 * {
 *     g_thread_init(NULL);
 *     log4g_init(&argc, &argv);
 *     log4g_info("Log4g is initialized & ready to go!");
 *     log4g_finalize();
 *     return EXIT_SUCCESS;
 * }
 * \endcode
 *
 * If you call this function you must call log4g_init() again before using
 * the Log4g API.
 *
 * \note You must call g_thread_init() before initializing Log4g if your
 *       application uses threads.
 *
 * \see log4g_init(), log4g_get_option_group(), log4g/appender/async-appender.h
 */
void
log4g_finalize(void);

/**
 * \brief Get the option group used by the Log4g package.
 *
 * If your application is using the GLib option parser then you may call
 * this function to retrieve the option group parsed by Log4g.
 *
 * Example (initialize Log4g via a GOptionGroup):
 *
 * \code
 * #include <log4g/log4g.h>
 * #include <stdlib.h>
 *
 * #define GETTEXT_PACKAGE "test-app"
 * #include <glib/gi18n-lib.h>
 *
 * int
 * main(int argc, char **argv)
 * {
 *     GError *error = NULL;
 *     GOptionContext *context;
 *     GOptionEntry entries[] = {
 *         { NULL }
 *     };
 *     g_thread_init(NULL);
 *     context = g_option_context_new("- demonstrate Log4g initialization");
 *     if (!context) {
 *         return EXIT_FAILURE;
 *     }
 *     g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
 *     g_option_context_add_group(context, log4g_get_option_group())
 *     if (!g_option_context_parse(context, &argc, &argv, &error)) {
 *         log4g_error("failed to parse options: %s", error->mesasge);
 *         g_error_free(error);
 *         return EXIT_FAILURE;
 *     }
 *     g_option_context_free();
 *     log4g_info("Log4g is initialized & ready to go!");
 *     log4g_finalize();
 *     return EXIT_SUCCESS;
 * }
 * \endcode
 *
 * If you use the returned option group in a GLib option parser you do not
 * need to call log4g_init().
 *
 * \note You must call g_thread_init() before initializing Log4g if your
 *       application uses threads.
 *
 * \return A GLib option group.
 *
 * \see log4g_init(), log4g_finalize()
 */
GOptionGroup *
log4g_get_option_group(void);

/**
 * \brief Retrieve a named logger from the repository.
 *
 * If the named logger exists in the repository that instance is returned. If
 * the named logger does not exist it will be created.
 *
 * \param name [in] The name of the logger to retrieve.
 *
 * \return The logger named \e name.
 *
 * \see log4g/logger.h, log4g_logger_get_logger()
 */
#define log4g_get_logger(name) \
    log4g_logger_get_logger(name)

/**
 * \brief Retrieve the root logger.
 *
 * \return The root logger.
 *
 * \see log4g/root-logger.h
 */
#define log4g_get_root_logger() \
    log4g_logger_get_root_logger()

/**
 * \brief Log an error if an \e assertion is \e FALSE.
 *
 * Example:
 *
 * \code
 * log4g_assert((pointer != NULL), "%s: pointer is NULL!", "ERROR");
 * \endcode
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
 * Example:
 *
 * \code
 * log4g_logger_assert(log4g_get_logger("org.gnome.debug"),
 *     (NULL != pointer), "%s: pointer is NULL!", "ERROR");
 * \endcode
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
 * Use this macro if you need to perform a resource-intensive operation
 * before logging a message.
 *
 * Example:
 *
 * \code
 * if (log4g_is_trace_enabled()) {
 *     char *string = perform_IO_intensive_operation();
 *     log4g_trace("here I am: %s", string);
 * }
 * \endcode
 *
 * \note A similar macro exists for all of the default log levels.
 *
 * \return \e TRUE if \e TRACE is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_trace_enabled(), log4g/level.h
 */
#define log4g_is_trace_enabled() \
    log4g_logger_is_trace_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e TRACE message.
 *
 * Example:
 *
 * \code
 * log4g_trace("example %s message", "TRACE");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_trace(), _log4g_logger_trace()
 */
#define log4g_trace(format, args...) \
    _log4g_logger_trace(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log a \e TRACE message.
 *
 * Example:
 *
 * \code
 * log4g_logger_trace(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "TRACE");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_trace(), _log4g_logger_trace()
 */
#define log4g_logger_trace(logger, format, args...) \
    _log4g_logger_trace(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e DEBUG level is enabled.
 *
 * \return \e TRUE if \e DEBUG is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_debug_enabled(), log4g/level.h,
 *      log4g_is_trace_enabled()
 */
#define log4g_is_debug_enabled() \
    log4g_logger_is_debug_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e DEBUG message.
 *
 * Example:
 *
 * \code
 * log4g_debug("example %s message", "DEBUG");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_debug(), _log4g_logger_debug()
 */
#define log4g_debug(format, args...) \
    _log4g_logger_debug(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log a \e DEBUG message.
 *
 * Example:
 *
 * \code
 * log4g_logger_debug(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "DEBUG");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_debug(), _log4g_logger_debug()
 */
#define log4g_logger_debug(logger, format, args...) \
    _log4g_logger_debug(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e INFO level is enabled.
 *
 * \return \e TRUE if \e INFO is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_info_enabled(), log4g/level.h,
 *      log4g_is_trace_enabled()
 */
#define log4g_is_info_enabled() \
    log4g_logger_is_info_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log an \e INFO message.
 *
 * Example:
 *
 * \code
 * log4g_info("example %s message", "INFO");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_info(), _log4g_logger_info()
 */
#define log4g_info(format, args...) \
    _log4g_logger_info(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log an \e INFO message.
 *
 * Example:
 *
 * \code
 * log4g_logger_info(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "INFO");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_info(), _log4g_logger_info()
 */
#define log4g_logger_info(logger, format, args...) \
    _log4g_logger_info(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e WARN level is enabled.
 *
 * \return \e TRUE if \e WARN is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_warn_enabled(), log4g/level.h,
 *      log4g_is_trace_enabled()
 */
#define log4g_is_warn_enabled() \
    log4g_logger_is_warn_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e WARN message.
 *
 * Example:
 *
 * \code
 * log4g_warn("example %s message", "WARN");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_warn(), _log4g_logger_warn()
 */
#define log4g_warn(format, args...) \
    _log4g_logger_warn(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log a \e WARN message.
 *
 * Example:
 *
 * \code
 * log4g_logger_warn(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "WARN");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_warn(), _log4g_logger_warn()
 */
#define log4g_logger_warn(logger, format, args...) \
    _log4g_logger_warn(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e ERROR level is enabled.
 *
 * \return \e TRUE if \e ERROR is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_error_enabled(), log4g/level.h,
 *      log4g_is_trace_enabled()
 */
#define log4g_is_error_enabled() \
    log4g_logger_is_error_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log an \e ERROR message.
 *
 * Example:
 *
 * \code
 * log4g_error("example %s message", "ERROR");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_error(), _log4g_logger_error()
 */
#define log4g_error(format, args...) \
    _log4g_logger_error(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log an \e ERROR message.
 *
 * Example:
 *
 * \code
 * log4g_logger_error(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "ERROR");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_error(), _log4g_logger_error()
 */
#define log4g_logger_error(logger, format, args...) \
    _log4g_logger_error(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

/**
 * \brief Check if the \e FATAL level is enabled.
 *
 * \return \e TRUE if \e FATAL is enabled, \e FALSE otherwise.
 *
 * \see log4g_logger_is_fatal_enabled(), log4g/level.h,
 *      log4g_is_trace_enabled()
 */
#define log4g_is_fatal_enabled() \
    log4g_logger_is_fatal_enabled(_log4g_get_logger(LOG4G_LOG_DOMAIN))

/**
 * \brief Log a \e FATAL message.
 *
 * Example:
 *
 * \code
 * log4g_fatal("example %s message", "FATAL");
 * \endcode
 *
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_logger_fatal(), _log4g_logger_fatal()
 */
#define log4g_fatal(format, args...) \
    _log4g_logger_fatal(_log4g_get_logger(LOG4G_LOG_DOMAIN), \
            G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), format, ##args)

/**
 * \brief Log a \e FATAL message.
 *
 * Example:
 *
 * \code
 * log4g_logger_fatal(log4g_get_logger("org.gnome.debug"),
 *     "example %s message", "FATAL");
 * \endcode
 *
 * \param logger [in] A logger object.
 * \param format [in] A printf formatted message.
 * \param args... [in] Format parameters.
 *
 * \see log4g_fatal(), _log4g_logger_fatal()
 */
#define log4g_logger_fatal(logger, format, args...) \
    _log4g_logger_fatal(logger, G_STRFUNC, __FILE__, G_STRINGIFY(__LINE__), \
            format, ##args)

G_END_DECLS

#else /* LOG4G_DISABLE */
#include <log4g/disable.h>
#endif /* LOG4G_DISABLE */
#endif /* LOG4G_H */
