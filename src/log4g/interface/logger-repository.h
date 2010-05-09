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
 * \brief Create and retrieve loggers.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * A logger repository is used to create and retrieve loggers. The
 * relationship between loggers and the repository depends on the repository
 * implementation but usually loggers are arranged in a named hierarchy.
 *
 * In addition to the create functions, a logger repository can be queried
 * for existing loggers and act as a registry for events related to loggers.
 *
 * Logger repositories have the following signals:
 * -# add-appender
 * -# remove-appender
 *
 * The add-appender signal is invoked whenever an appender is added to a
 * logger in the repository.
 *
 * The type signature for the add-appender signal is:
 *
 * \code
 * typedef void
 * (*add_appender)(Log4gLogger *logger, Log4gAppender *appender);
 * \endcode
 *
 * The remove-appender signal is invoked whenever an appender is removed from
 * a logger in the repository.
 *
 * The type signature for the remove-appender signal is:
 *
 * \code
 * typedef void
 * (*remove_appender)(Log4gLogger *logger, Log4gAppender *appender);
 * \endcode
 *
 * \see log4g/logger.h
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

/** \brief Log4gLoggerRepository object type definition */
typedef struct _Log4gLoggerRepository Log4gLoggerRepository;

/** \brief Log4gLoggerRepository interface type definition */
typedef struct _Log4gLoggerRepositoryInterface Log4gLoggerRepositoryInterface;

/** \brief Log4gLoggerRepositoryInterface definition */
struct _Log4gLoggerRepositoryInterface {
    GTypeInterface parent_interface;

    /**
     * \brief Determine if a named logger exists.
     *
     * If the named logger does not exist it is not created.
     *
     * \param self [in] A logger repository object.
     * \param name [in] The name of the logger to check.
     *
     * \return The logger named \e name or \e NULL if it does not exist.
     */
    Log4gLogger *
    (*exists)(Log4gLoggerRepository *self, const gchar *name);

    /**
     * \brief Retrieve all loggers in the repository.
     *
     * \param self [in] A logger repository object.
     *
     * \return An array containing all loggers in the repository or \e NULL
     *         if there are none.
     *
     * \note It is the responsibility of the caller to call g_array_free()
     *       for the returned value.
     */
    const GArray *
    (*get_current_loggers)(Log4gLoggerRepository *self);

    /**
     * \brief Retrieve a named logger from the repository.
     *
     * If the logger named \e name does not already exist it should be
     * created and added to the repository.
     *
     * \param self [in] A logger repository object.
     * \param name [in] The name of the logger to retrieve.
     *
     * \return The logger named \e name.
     */
    Log4gLogger *
    (*get_logger)(Log4gLoggerRepository *self, const gchar *name);

    /**
     * \brief Retrieve a named logger from the repository.
     *
     * If the logger named \e name does not already exist it should be
     * created using \e factory and added to the repository.
     *
     * \param self [in] A logger repository object.
     * \param name [in] The name of the logger to retrieve.
     * \param factory [in] The factory to use.
     *
     * \return The logger named \e name.
     */
    Log4gLogger *
    (*get_logger_factory)(Log4gLoggerRepository *self, const gchar *name,
            Log4gLoggerFactory *factory);

    /**
     * \brief Retrieve the root logger.
     *
     * \param self [in] A logger repository object.
     *
     * \return The root logger.
     */
    Log4gLogger *
    (*get_root_logger)(Log4gLoggerRepository *self);

    /**
     * \brief Retrieve the repository threshold.
     *
     * \see _Log4gLoggerRepositoryInterface::set_threshold()
     *
     * \param self [in] A logger repository object.
     *
     * \return The threshold level for \e self.
     *
     * \see log4g/level.h
     */
    Log4gLevel *
    (*get_threshold)(Log4gLoggerRepository *self);

    /**
     * \brief Determine if the repository is disabled for a given log level.
     *
     * \see _Log4gLoggerRepositoryInterface::set_threshold()
     *
     * \param self [in] A logger repository object.
     * \param level [in] The integer representation of a log level.
     *
     * \return \e TRUE if \e self is disabled for \e level, \e FALSE otherwise.
     *
     * \see log4g/level.h
     */
    gboolean
    (*is_disabled)(Log4gLoggerRepository *self, gint level);

    /**
     * \brief Reset a repository to its initial state.
     *
     * \param self [in] A logger repository object.
     */
    void
    (*reset_configuration)(Log4gLoggerRepository *self);

    /**
     * \brief Set the repository threshold.
     *
     * All logging requests below the threshold are immediately dropped.
     * By default the threshold is set to \e ALL, which has the lowest
     * possible rank.
     *
     * \param self [in] A logger repository object.
     * \param level [in] The new threshold for \e self.
     *
     * \see log4g/level.h
     */
    void
    (*set_threshold)(Log4gLoggerRepository *self, Log4gLevel *level);

    /**
     * \brief
     *
     * \param self [in] A logger repository object.
     */
    void
    (*set_threshold_string)(Log4gLoggerRepository *self, const gchar *level);

    /**
     * \brief Shutdown the repository.
     *
     * Once the repository is shut down it cannot be used by the Log4g system.
     *
     * \param self [in] A logger repository object.
     */
    void
    (*shutdown)(Log4gLoggerRepository *self);

    /**
     * \brief Emit a warning after attempting to use a logger that has no
     *        appenders attached.
     *
     * \param self [in] A logger repository object.
     * \param logger [in] The logger that produced the warning.
     */
    void
    (*emit_no_appender_warning)(Log4gLoggerRepository *self,
            Log4gLogger *logger);
};

GType log4g_logger_repository_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::exists().
 *
 * \param self [in] A logger repository object.
 * \param name [in] The name of the logger to check.
 *
 * \return The logger named \e name or \e NULL if it does not exist.
 */
Log4gLogger *
log4g_logger_repository_exists(Log4gLoggerRepository *self, const gchar *name);

/**
 * \brief Invoke the add-appender signal.
 *
 * \param self [in] A logger repository object.
 * \param logger [in] The logger from to \e appender was added.
 * \param appender [in] The appender that was added.
 */
void
log4g_logger_repository_emit_add_appender_signal(Log4gLoggerRepository *self,
        Log4gLogger *logger, Log4gAppender *appender);

/**
 * \brief Invokes the remove-appender signal.
 *
 * \param self [in] A logger repository object.
 * \param logger [in] The logger from which \e appender was removed.
 * \param appender [in] The appender that was removed.
 */
void
log4g_logger_repository_emit_remove_appender_signal(
        Log4gLoggerRepository *self, Log4gLogger *logger,
        Log4gAppender *appender);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::get_current_loggers().
 *
 * \param self [in] A logger repository object.
 *
 * \return An array containing all loggers in the repository or \e NULL
 *         if there are none.
 */
const GArray *
log4g_logger_repository_get_current_loggers(Log4gLoggerRepository *self);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::get_logger().
 *
 * \param self [in] A logger repository object.
 * \param name [in] The name of the logger to retrieve.
 *
 * \return The logger named \e name.
 */
Log4gLogger *
log4g_logger_repository_get_logger(Log4gLoggerRepository *self,
        const gchar *name);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::get_logger_factory().
 *
 * \param self [in] A logger repository object.
 * \param name [in] The name of the logger to retrieve.
 * \param factory [in] The factory to use.
 *
 * \return The logger named \e name.
 */
Log4gLogger *
log4g_logger_repository_get_logger_factory(Log4gLoggerRepository *self,
        const gchar *name, Log4gLoggerFactory *factory);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::get_root_logger().
 *
 * \param self [in] A logger repository object.
 *
 * \return The root logger.
 */
Log4gLogger *
log4g_logger_repository_get_root_logger(Log4gLoggerRepository *self);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::get_threshold().
 *
 * \param self [in] A logger repository object.
 *
 * \return The threshold level for \e self.
 *
 * \see log4g/level.h
 */
Log4gLevel *
log4g_logger_repository_get_threshold(Log4gLoggerRepository *self);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::is_disabled().
 *
 * \param self [in] A logger repository object.
 * \param level [in] The integer representation of a log level.
 *
 * \return \e TRUE if \e self is disabled for \e level, \e FALSE otherwise.
 *
 * \see log4g/level.h
 */
gboolean
log4g_logger_repository_is_disabled(Log4gLoggerRepository *self, gint level);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::reset_configuration().
 *
 * \param self [in] A logger repository object.
 */
void
log4g_logger_repository_reset_configuration(Log4gLoggerRepository *self);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::set_threshold().
 *
 * \param self [in] A logger repository object.
 * \param level [in] The new threshold for \e self.
 *
 * \see log4g/level.h
 */
void
log4g_logger_repository_set_threshold(Log4gLoggerRepository *self,
        Log4gLevel *level);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::set_threshold_string().
 *
 * \param self [in] A logger repository object.
 */
void
log4g_logger_repository_set_threshold_string(Log4gLoggerRepository *self,
        const gchar *string);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::shutdown().
 *
 * \param self [in] A logger repository object.
 */
void
log4g_logger_repository_shutdown(Log4gLoggerRepository *self);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerRepositoryInterface::emit_no_appender_warning().
 *
 * \param self [in] A logger repository object.
     * \param logger [in] The logger that produced the warning.
 */
void
log4g_logger_repository_emit_no_appender_warning(Log4gLoggerRepository *self,
        Log4gLogger *logger);

G_END_DECLS

#endif /* LOG4G_LOGGER_REPOSITORY_H */
