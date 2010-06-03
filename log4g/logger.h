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
 * SECTION: logger
 * @short_description: the central class in the Log4g package
 *
 * Most logging operations (except configuration) are performed through this
 * class.
 */

#ifndef LOG4G_LOGGER_H
#define LOG4G_LOGGER_H

#include <log4g/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER \
    (log4g_logger_get_type())

#define LOG4G_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER, Log4gLogger))

#define LOG4G_IS_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOGGER, Log4gLoggerClass))

#define LOG4G_IS_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOGGER, \
            Log4gLoggerClass))

typedef struct _Log4gLogger Log4gLogger;

typedef struct _Log4gLoggerClass Log4gLoggerClass;

/**
 * Log4gLogger:
 *
 * The <structname>Log4gLogger</structname> structure does not have any public
 * members.
 */
struct _Log4gLogger {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gLoggerGetEffectiveLevel:
 * @self: A #Log4gLogger object.
 *
 * Starting from this category, search the category hierarchy for a
 * non-%NULL and return it. If a non-%NULL level is not found, this
 * function returns the level of the root logger.
 *
 * Returns The effect level threshold of @self.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLoggerGetEffectiveLevel)(Log4gLogger *self);

/**
 * Log4gLoggerSetLevel:
 * @self: A #Log4gLogger object.
 * @level: The new level threshold for @self. %NULL values are permitted.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerSetLevel)(Log4gLogger *self, Log4gLevel *level);

/**
 * Log4gLoggerClass:
 * @get_effective_level: Retrieve the effective level threshold of the logger.
 * @set_level: Set the level threshold of the logger.
 */
struct _Log4gLoggerClass {
    /*< private >*/
    GObjectClass parent_class;
    /*< public >*/
    Log4gLoggerGetEffectiveLevel get_effective_level;
    Log4gLoggerSetLevel set_level;
};

GType
log4g_logger_get_type(void);

/**
 * log4g_logger_new:
 * @name: The fully qualified name for the new logger.
 *
 * Create a new logger object and set its name.
 *
 * This fuction is intended for internal use. You should not create Loggers
 * directly. See log4g_logger_get_logger()
 *
 * Returns: A new #Log4gLogger object.
 */
Log4gLogger *
log4g_logger_new(const gchar *name);

/**
 * log4g_logger_get_name:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the fully-qualified name of a logger.
 *
 * Returns: The name of @self.
 */
const gchar *
log4g_logger_get_name(Log4gLogger *self);

/**
 * log4g_logger_set_name:
 * @self: A #Log4gLogger object.
 * @name: The new name of @self.
 *
 * Set the name of a logger.
 *
 * This method should only be called by sub-classes. Calling this method on
 * a logger in the logger repository will have disastrous effects on the
 * logger hierarchy.
 */
void
log4g_logger_set_name(Log4gLogger *self, const gchar *name);

/**
 * log4g_logger_get_parent:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the parent of this logger.
 *
 * <note><para>
 * The parent of a logger may change during its lifetime. The root logger
 * will return %NULL.
 * </para></note>
 *
 * Returns: The parent of @self.
 */
Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self);

/**
 * log4g_logger_set_parent:
 * @self: A #Log4gLogger object.
 * @parent: The new parent of @self.
 *
 * Set the parent of a logger.
 */
void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent);

/**
 * log4g_logger_get_level:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the level threshold of a logger.
 *
 * Returns: The log level threshold of @self.
 */
Log4gLevel *
log4g_logger_get_level(Log4gLogger *self);

/**
 * log4g_logger_set_level:
 * @self: A #Log4gLogger object.
 * @level: The new log level threshold for @self.
 *
 * Calls the @set_level function from the #Log4gLoggerClass of @self.
 */
void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level);

/**
 * log4g_logger_get_additivity:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the additivity flag for a logger. See
 * log4g_logger_set_additivity().
 *
 * Returns: %TRUE if @self is additive, %FALSE otherwise.
 */
gboolean
log4g_logger_get_additivity(Log4gLogger *self);

/**
 * log4g_logger_set_additivity:
 * @self: A #Log4gLogger object.
 * @additive: The new additivity flag for @self.
 *
 * Set the additivity flag for a logger.
 *
 * Logger additivity determines if a logger inherits the appenders of its
 * ancestors. If this flag is set to %TRUE (the default value is %TRUE)
 * then events logged to this logger will be propagated to the appenders of
 * its ancestors. If this flags is set to %FALSE then the appenders of this
 * logger will not be inherited.
 */
void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive);

/**
 * log4g_logger_get_logger_repository:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the repository where a logger is attached. See
 * #Log4gLoggerRepositoryClass.
 *
 * Returns: The logger repository @self is attached to.
 */
gpointer
log4g_logger_get_logger_repository(Log4gLogger *self);

/**
 * log4g_logger_set_logger_repository:
 * @self: A #Log4gLogger object.
 * @repository: The new repository to attach to @self.
 *
 * Set the repository a logger is attached to.
 *
 * You probably do not want to call this function.
 *
 * See #Log4gLoggerRepositoryClass
 */
void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository);

/**
 * log4g_logger_get_effective_level:
 * @self: A #Log4gLogger object.
 *
 * Calls the @get_effective_level function from the #Log4gLoggerClass of @self.
 *
 * Returns: The effective level threshold of @self.
 */
Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self);

/**
 * log4g_logger_add_appender:
 * @base: A #Log4gLogger object.
 * @appender: The appender to add to @base.
 *
 * Add an appender to the list of appenders for @self.
 *
 * If @appender is already in the list of appenders for @self then
 * it will not be added again.
 *
 * See #Log4gAppenderAttachableInterface
 */
void
log4g_logger_add_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 * log4g_logger_get_all_appenders:
 * @base: A #Log4gLogger object.
 *
 * Retrieve all appenders attached to a @base.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: A #GArray of appenders attached to @self or %NULL if there are
 *          none.
 */
const GArray *
log4g_logger_get_all_appenders(Log4gLogger *base);

/**
 * log4g_logger_get_appender:
 * @base: A #Log4gLogger object.
 * @name: The name of the appender to retrieve.
 *
 * Retrieve an appender attached to a logger by name.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: The appender named @name or %NULL if no such appender is attached
 *          to @base.
 */
Log4gAppender *
log4g_logger_get_appender(Log4gLogger *base, const gchar *name);

/**
 * log4g_logger_is_attached:
 * @base: A #Log4gLogger object.
 * @appender: An appender object.
 *
 * Determine if an appender is attached to a logger.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: %TRUE if @appender is attached to @self, %FALSE otherwise.
 */
gboolean
log4g_logger_is_attached(Log4gLogger *base, Log4gAppender *appender);

/**
 * log4g_logger_remove_all_appenders:
 * @base: A #Log4gLogger object.
 *
 * Remove all appenders from a logger.
 *
 * See #Log4gAppenderAttachableInterface
 */
void
log4g_logger_remove_all_appenders(Log4gLogger *base);

/**
 * log4g_logger_remove_appender:
 * @base: A #Log4gLogger object.
 * @appender: An appender to remove.
 *
 * Remove an appender from a logger.
 *
 * If @appender is not attached to @base then this function does nothing.
 *
 * See #Log4gAppenderAttachableInterface
 */
void
log4g_logger_remove_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 * log4g_logger_remove_appender_name:
 * @base: A #Log4gLogger object.
 * @name: The name of the appender to remove.
 *
 * Remove an appender from a logger by name.
 *
 * If @name is not found then this function does nothing.
 *
 * See #Log4gAppenderAttachableInterface
 */
void
log4g_logger_remove_appender_name(Log4gLogger *base, const gchar *name);

/**
 * log4g_logger_close_nested_appenders:
 * @self: A #Log4gLogger object.
 *
 * Close all attached appenders implementing the #Log4gAppenderAttachable
 * interface.
 *
 * See #Log4gAppenderAttachableInterface
 */
void
log4g_logger_close_nested_appenders(Log4gLogger *self);

/**
 * _log4g_logger_assert:
 * @self: A #Log4gLogger object.
 * @assertion: The assertion to evaluate.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Logs an error if the @assertion parameter is %FALSE.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_assert(), log4g_logger_assert()
 */
void
_log4g_logger_assert(Log4gLogger *self, gboolean assertion,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, ...) G_GNUC_PRINTF(6, 7);

/**
 * log4g_logger_is_trace_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_TRACE level.
 *
 * This function is useful if you have a to perform a costly operation to
 * construct a log message.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_TRACE level, %FALSE
 *         otherwise.
 */
gboolean log4g_logger_is_trace_enabled(Log4gLogger *self);

/**
 * _log4g_logger_trace:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_TRACE level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_trace(), log4g_logger_trace()
 */
void
_log4g_logger_trace(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * log4g_logger_is_debug_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_DEBUG level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_DEBUG level, %FALSE
 *          otherwise.
 */
gboolean
log4g_logger_is_debug_enabled(Log4gLogger *self);

/**
 * _log4g_logger_debug:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_DEBUG level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_debug(), log4g_logger_debug()
 *
 */
void
_log4g_logger_debug(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * log4g_logger_is_info_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_INFO level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_INFO level, %FALSE
 *          otherwise.
 */
gboolean
log4g_logger_is_info_enabled(Log4gLogger *self);

/**
 * _log4g_logger_info:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_INFO level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_info(), log4g_logger_info()
 */
void
_log4g_logger_info(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * log4g_logger_is_warn_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_WARN level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_WARN level, %FALSE
 *          otherwise.
 */
gboolean
log4g_logger_is_warn_enabled(Log4gLogger *self);

/**
 * _log4g_logger_warn:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_WARN level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_warn(), log4g_logger_warn()
 */
void
_log4g_logger_warn(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * log4g_logger_is_error_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_ERROR level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_ERROR level, %FALSE
 *          otherwise.
 */
gboolean
log4g_logger_is_error_enabled(Log4gLogger *self);

/**
 * _log4g_logger_error:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_ERROR level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_error(), log4g_logger_error()
 */
void
_log4g_logger_error(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * log4g_logger_is_fatal_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_FATAL level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_FATAL level, %FALSE
 *          otherwise.
 */
gboolean
log4g_logger_is_fatal_enabled(Log4gLogger *self);

/**
 * _log4g_logger_fatal:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_FATAL level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_fatal(), log4g_logger_fatal()
 */
void
_log4g_logger_fatal(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * _log4g_logger_log:
 * @self: A Log4gLogger object.
 * @level: The level of the logging request.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * This is the most generic logging method.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_log(), log4g_logger_log()
 */
void
_log4g_logger_log(Log4gLogger *self, Log4gLevel *level, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(6, 7);

/**
 * log4g_logger_get_logger:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise a new instance is created.
 *
 * Loggers inherit their default level from their nearest ancestor with a
 * set level.
 *
 * Returns: The logger named @name.
 */
Log4gLogger *
log4g_logger_get_logger(const gchar *name);

/**
 * log4g_logger_get_root_logger:
 *
 * Retrieve the root logger for the current logger repository.
 *
 * Calling log4g_logger_get_name() on the root logger always returns the
 * string "root". Calling log4g_logger_get_logger("root") however does not
 * retrieve the root logger but a logger just under root named "root".
 *
 * Calling this function is the only way to retrieve the root logger.
 *
 * Returns: The root logger.
 */
Log4gLogger *
log4g_logger_get_root_logger(void);

/**
 * log4g_logger_get_logger_factory:
 * @name: The name of the logger to retrieve.
 * @factory: The logger factory to use if a logger named @name does not already
 *           exist.
 *
 * Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise @factory is used to create a new instance.
 *
 * Returns: The logger named @name.
 */
Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory);

/**
 * log4g_logger_forced_log:
 * @self: A #Log4gLogger object.
 * @level: The level of the log event.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf message format.
 * @ap: Format parameters.
 *
 * Create and log a new event without further checks.
 */
void
log4g_logger_forced_log(Log4gLogger *self, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, va_list ap);

G_END_DECLS

#endif /* LOG4G_LOGGER_H */
