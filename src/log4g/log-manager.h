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
 * \brief Operate on the current logger repository.
 * \author Mike Steinert
 * \date 2-10-2010
 *
 * Use the log manager class to retrieve logger instances or operate on the
 * current logger repository.
 *
 * \see log4g/logger.h, log4g/interface/logger-repository.h
 */

#ifndef LOG4G_LOG_MANAGER_H
#define LOG4G_LOG_MANAGER_H

#include <log4g/interface/repository-selector.h>
#include <log4g/interface/logger-factory.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOG_MANAGER \
    (log4g_log_manager_get_type())

#define LOG4G_LOG_MANAGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManager))

#define LOG4G_IS_LOG_MANAGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOG_MANAGER))

#define LOG4G_LOG_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManagerClass))

#define LOG4G_IS_LOG_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOG_MANAGER))

#define LOG4G_LOG_MANAGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManagerClass))

/** \brief Log4gLogManager type definition */
typedef struct _Log4gLogManager Log4gLogManager;

/** \brief Log4gLogManager class type definition */
typedef struct _Log4gLogManagerClass Log4gLogManagerClass;

/** \brief Log4gLogManagerClass definition */
struct _Log4gLogManager {
    GObject parent_instance;
};

/** \brief Log4gLogManagerClass definition */
struct _Log4gLogManagerClass {
    GObjectClass parent_class;
};

GType
log4g_log_manager_get_type(void);

/**
 * \internal
 * \brief Retrieve the current log manager instance.
 *
 * If one does not exist it will be created.
 *
 * \return The current log manager instance.
 */
Log4gLogManager *
log4g_log_manager_get_instance(void);

/**
 * \internal
 * \brief Remove the current log manager instance.
 *
 * \warning Do not call this function unless you really know what you
 *          are doing.
 */
void
log4g_log_manager_remove_instance(void);

/**
 * \brief Retrieve the current logger repository.
 *
 * \return The current logger repository.
 *
 * \see log4g/interface/logger-repository.h
 */
Log4gLoggerRepository *
log4g_log_manager_get_logger_repository(void);

/**
 * \brief Retrieve the root logger.
 *
 * \return The root logger.
 */
Log4gLogger *
log4g_log_manager_get_root_logger(void);

/**
 * \brief Retrieve a named logger.
 *
 * If the named logger does not exist it will be created.
 *
 * \param name [in] The name of the logger to retrieve.
 *
 * \return A logger instance.
 */
Log4gLogger *
log4g_log_manager_get_logger(const gchar *name);

/**
 * \brief Retrieve a named logger.
 *
 * If the named logger does not exist it will be created using the provided
 * logger factory.
 *
 * \param name [in] The name of the logger to retrieve.
 * \param factory [in] The factory to use if \e name does not already exist.
 *
 * \see log4g/interface/logger-factory.h
 */
Log4gLogger *
log4g_log_manager_get_logger_factory(const gchar *name,
        Log4gLoggerFactory *factory);

/**
 * \brief Retrieve a named logger if it already exists.
 *
 * If \e name does not exist it will not be created.
 *
 * \param name [in] The naem of the logger to retrieve.
 *
 * \return A logger named \e name or \e NULL if \e name does not exist.
 */
Log4gLogger *
log4g_log_manager_exists(const gchar *name);

/**
 * \brief Retrieve an array containing all current existing loggers.
 *
 * \return An array containing all current loggers.
 *
 * \note It is the callers responsibility to call g_array_free() for the
 *       returned value.
 */
const GArray *
log4g_log_manager_get_current_loggers(void);

/**
 * \brief Shut down the current logger repository.
 */
void
log4g_log_manager_shutdown(void);

/**
 * \brief Reset the configuration for the current logger repository.
 */
void
log4g_log_manager_reset_configuration(void);

G_END_DECLS

#endif /* LOG4G_LOG_MANAGER_H */
