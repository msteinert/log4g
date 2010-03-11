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
 * \date 2-11-2010
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

/** \brief Log4gRepositorySelector class type definition */
typedef struct _Log4gRepositorySelectorInterface
        Log4gRepositorySelectorInterface;

/** \brief Log4gRepositorySelectorClass definition */
struct _Log4gRepositorySelectorInterface {
    GTypeInterface parent_interface; /**< parent interface */
    Log4gLoggerRepository *(*get_logger_repository)(
            Log4gRepositorySelector *self);
};

GType
log4g_repository_selector_get_type(void);

/**
 */
Log4gLoggerRepository *
log4g_repository_selector_get_logger_repository(Log4gRepositorySelector *self);

G_END_DECLS

#endif /* LOG4G_REPOSITORY_SELECTOR_H */
