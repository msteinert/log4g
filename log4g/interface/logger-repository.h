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

#ifndef LOG4G_LOGGER_REPOSITORY_H
#define LOG4G_LOGGER_REPOSITORY_H

#include <log4g/interface/logger-factory.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER_REPOSITORY \
    (log4g_logger_repository_get_type())

#define LOG4G_LOGGER_REPOSITORY(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER_REPOSITORY, \
            Log4gLoggerRepository))

#define LOG4G_IS_LOGGER_REPOSITORY(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER_REPOSITORY))

#define LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_LOGGER_REPOSITORY, \
            Log4gLoggerRepositoryInterface));

typedef struct _Log4gLoggerRepository Log4gLoggerRepository;

typedef struct _Log4gLoggerRepositoryInterface Log4gLoggerRepositoryInterface;

/**
 * Log4gLoggerRepositoryExists:
 * @self: A logger repository object.
 * @name: The name of the logger to check.
 *
 * Determine if a named logger exists.
 *
 * If the named logger does not exist it is not created.
 *
 * Returns: The logger named @name or %NULL if it does not exist.
 * Since: 0.1
 */
typedef Log4gLogger *
(*Log4gLoggerRepositoryExists)(Log4gLoggerRepository *self, const gchar *name);

/**
 * Log4gLoggerRepositoryGetCurrentLoggers:
 * @self: A logger repository object.
 *
 * Retrieve all loggers in the repository.
 *
 * <note><para>
 * It is the responsibility of the caller to call g_array_free() for the
 * returned value.
 * </para></note>
 *
 * Returns: An array containing all loggers in the repository or %NULL if
 *          there are none.
 * Since: 0.1
 */
typedef const GArray *
(*Log4gLoggerRepositoryGetCurrentLoggers)(Log4gLoggerRepository *self);

/**
 * Log4gLoggerRepositoryGetLogger:
 * @self: A logger repository object.
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger from the repository.
 *
 * If the logger named @name does not already exist it should be
 * created and added to the repository.
 *
 * Returns: The logger named @name.
 * Since: 0.1
 */
typedef Log4gLogger *
(*Log4gLoggerRepositoryGetLogger)(Log4gLoggerRepository *self,
        const gchar *name);

/**
 * Log4gLoggerRepositoryGetLoggerFactory:
 * @self: A logger repository object.
 * @name: The name of the logger to retrieve.
 * @factory: The factory to use.
 *
 * Retrieve a named logger from the repository.
 *
 * If the logger named @name does not already exist it should be
 * created using @factory and added to the repository.
 *
 * Returns: The logger named @name.
 * Since: 0.1
 */
typedef Log4gLogger *
(*Log4gLoggerRepositoryGetLoggerFactory)(Log4gLoggerRepository *self,
        const gchar *name, Log4gLoggerFactory *factory);

/**
 * Log4gLoggerRepositoryGetRootLogger:
 * @self: A logger repository object.
 *
 * Retrieve the root logger.
 *
 * Returns: The root logger.
 * Since: 0.1
 */
typedef Log4gLogger *
(*Log4gLoggerRepositoryGetRootLogger)(Log4gLoggerRepository *self);

/**
 * Log4gLoggerRepositoryGetThreshold:
 * @self: A logger repository object.
 *
 * Retrieve the repository threshold.
 *
 * @See: #Log4gLevelClass, #Log4gLoggerRepositoryInterface.set_threshold
 *
 * Returns: The threshold level for @self.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLoggerRepositoryGetThreshold)(Log4gLoggerRepository *self);

/**
 * Log4gLoggerRepositoryIsDisabled:
 * @self: A logger repository object.
 * @level: The integer representation of a log level.
 *
 * Determine if the repository is disabled for a given log level.
 *
 * \see #Log4gLevelClass, #Log4gLoggerRepositoryInterface.set_threshold
 *
 * Returns: %TRUE if @self is disabled for @level, %FALSE otherwise.
 * Since: 0.1
 */
typedef gboolean
(*Log4gLoggerRepositoryIsDisabled)(Log4gLoggerRepository *self, gint level);

/**
 * Log4gLoggerRepositoryResetConfiguration:
 * @self: A logger repository object.
 *
 * Reset a repository to its initial state.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerRepositoryResetConfiguration)(Log4gLoggerRepository *self);

/**
 * Log4gLoggerRepositorySetThreshold:
 * @self: A logger repository object.
 * @level: The new threshold for @self.
 *
 * Set the repository threshold.
 *
 * All logging requests below the threshold are immediately dropped.
 * By default the threshold is set to @ALL, which has the lowest
 * possible rank.
 *
 * @See: #Log4gLevelClass
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerRepositorySetThreshold)(Log4gLoggerRepository *self,
        Log4gLevel *level);

/**
 * Log4gLoggerRepositorySetThresholdString:
 * @self: A logger repository object.
 * @level: The new threshold level name to set.
 *
 * Set the repository threshold from a string.
 *
 * @See: #Log4gLevelClass
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerRepositorySetThresholdString)(Log4gLoggerRepository *self,
        const gchar *level);

/**
 * Log4gLoggerRepositoryShutdown:
 * @self: A logger repository object.
 *
 * Shutdown the repository.
 *
 * Once the repository is shut down it cannot be used by the Log4g system.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerRepositoryShutdown)(Log4gLoggerRepository *self);

/**
 * Log4gLoggerRepositoryGetCurrentEmitNoAppenderWarning:
 * @self: A logger repository object.
 * @logger: The logger that produced the warning.
 *
 * Emit a warning after attempting to use a logger that has no appenders
 * attached.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerRepositoryEmitNoAppenderWarning)(Log4gLoggerRepository *self,
        Log4gLogger *logger);

/**
 * Log4gLoggerRepositoryInterface:
 * @exists: Determine if a given logger exists in the repository.
 * @get_current_loggers: Get all loggers in the repository.
 * @get_logger: Get an existing logger or create a new one.
 * @get_logger_factory: Get an existing logger or create it with a factory.
 * @get_root_logger: Get the root logger.
 * @get_threshold: Get the repository threshold.
 * @is_disabled: Determine if the repository is disabled at a given level.
 * @reset_configuration: Reset the repository configuration.
 * @set_threshold: Set the repository threshold.
 * @set_threshold_string: Set the repository threshold by string value.
 * @shutdown: Shut down the logger repository.
 * @emit_no_appender_warning: Emit a warning if no appenders are attached.
 */
struct _Log4gLoggerRepositoryInterface {
    /*< private >*/
    GTypeInterface parent_interface;
    /*< public >*/
    Log4gLoggerRepositoryExists exists;
    Log4gLoggerRepositoryGetCurrentLoggers get_current_loggers;
    Log4gLoggerRepositoryGetLogger get_logger;
    Log4gLoggerRepositoryGetLoggerFactory get_logger_factory;
    Log4gLoggerRepositoryGetRootLogger get_root_logger;
    Log4gLoggerRepositoryGetThreshold get_threshold;
    Log4gLoggerRepositoryIsDisabled is_disabled;
    Log4gLoggerRepositoryResetConfiguration reset_configuration;
    Log4gLoggerRepositorySetThreshold set_threshold;
    Log4gLoggerRepositorySetThresholdString set_threshold_string;
    Log4gLoggerRepositoryShutdown shutdown;
    Log4gLoggerRepositoryEmitNoAppenderWarning emit_no_appender_warning;
};

GType log4g_logger_repository_get_type(void);

Log4gLogger *
log4g_logger_repository_exists(Log4gLoggerRepository *self, const gchar *name);

void
log4g_logger_repository_emit_add_appender_signal(Log4gLoggerRepository *self,
        Log4gLogger *logger, Log4gAppender *appender);

void
log4g_logger_repository_emit_remove_appender_signal(
        Log4gLoggerRepository *self, Log4gLogger *logger,
        Log4gAppender *appender);

const GArray *
log4g_logger_repository_get_current_loggers(Log4gLoggerRepository *self);

Log4gLogger *
log4g_logger_repository_get_logger(Log4gLoggerRepository *self,
        const gchar *name);

Log4gLogger *
log4g_logger_repository_get_logger_factory(Log4gLoggerRepository *self,
        const gchar *name, Log4gLoggerFactory *factory);

Log4gLogger *
log4g_logger_repository_get_root_logger(Log4gLoggerRepository *self);

Log4gLevel *
log4g_logger_repository_get_threshold(Log4gLoggerRepository *self);

gboolean
log4g_logger_repository_is_disabled(Log4gLoggerRepository *self, gint level);

void
log4g_logger_repository_reset_configuration(Log4gLoggerRepository *self);

void
log4g_logger_repository_set_threshold(Log4gLoggerRepository *self,
        Log4gLevel *level);

void
log4g_logger_repository_set_threshold_string(Log4gLoggerRepository *self,
        const gchar *string);

void
log4g_logger_repository_shutdown(Log4gLoggerRepository *self);

void
log4g_logger_repository_emit_no_appender_warning(Log4gLoggerRepository *self,
        Log4gLogger *logger);

G_END_DECLS

#endif /* LOG4G_LOGGER_REPOSITORY_H */
