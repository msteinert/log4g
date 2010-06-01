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
 * \brief Select the correct logger repository.
 * \author Mike Steinert
 * \date 2-11-2010
 *
 * The log manager uses a repository selector implementation to select the
 * logger repository for a particular application context.
 *
 * It is the responsibility of the repository selector implementation to
 * track the application context. Log4g makes no assumptions about the
 * application context.
 *
 * \see log4g/interface/logger-repository.h
 */

#ifndef LOG4G_REPOSITORY_SELECTOR_H
#define LOG4G_REPOSITORY_SELECTOR_H

#include <log4g/interface/logger-repository.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_REPOSITORY_SELECTOR \
    (log4g_repository_selector_get_type())

#define LOG4G_REPOSITORY_SELECTOR(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_REPOSITORY_SELECTOR, \
            Log4gRepositorySelector))

#define LOG4G_IS_REPOSITORY_SELECTOR(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_REPOSITORY_SELECTOR))

#define LOG4G_REPOSITORY_SELECTOR_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), \
            LOG4G_TYPE_REPOSITORY_SELECTOR, Log4gRepositorySelectorInterface));

/** \brief Log4gRepositorySelector object type definition */
typedef struct _Log4gRepositorySelector Log4gRepositorySelector;

/** \brief Log4gRepositorySelector interface type definition */
typedef struct _Log4gRepositorySelectorInterface
        Log4gRepositorySelectorInterface;

/** \brief Log4gRepositorySelectorInterface definition */
struct _Log4gRepositorySelectorInterface {
    GTypeInterface parent_interface;

    /**
     * \brief Select the logger repository for the current application
     *        context.
     * 
     * Implementors must assure that a valid (\e non-NULL) logger repository
     * is returned.
     *
     * \param self [in] A repository selector object.
     *
     * \return The logger repository for the current application context.
     *
     * \see log4g/interface/logger-repository.h
     */
    Log4gLoggerRepository *
    (*get_logger_repository)(Log4gRepositorySelector *self);
};

GType
log4g_repository_selector_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gRepositorySelectorInterface::get_logger_repository().
 *
 * \param self [in] A repository selector object.
 *
 * \return The logger repository for the current application context.
 *
 * \see log4g/interface/logger-repository.h
 */
Log4gLoggerRepository *
log4g_repository_selector_get_logger_repository(Log4gRepositorySelector *self);

G_END_DECLS

#endif /* LOG4G_REPOSITORY_SELECTOR_H */
