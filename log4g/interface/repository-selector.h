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

typedef struct _Log4gRepositorySelector Log4gRepositorySelector;

typedef struct _Log4gRepositorySelectorInterface
        Log4gRepositorySelectorInterface;

/**
 * Log4gRepositorySelectorGetLoggerRepository:
 * @self: A repository selector object.
 *
 * Select the logger repository for the current application context.
 * 
 * Implementors must assure that a valid (non-%NULL) logger repository
 * is returned.
 *
 * @See: #Log4gLoggerRepositoryInterface
 *
 * Returns: The logger repository for the current application context.
 * Since: 0.1
 */
typedef Log4gLoggerRepository *
(*Log4gRepositorySelectorGetLoggerRepository)(Log4gRepositorySelector *self);

/**
 * Log4gRepositorySelectorInterface:
 * @get_logger_repository: Select the current logger repository.
 */
struct _Log4gRepositorySelectorInterface {
    /*< private >*/
    GTypeInterface parent_interface;
    /*< public >*/
    Log4gRepositorySelectorGetLoggerRepository get_logger_repository;
};

GType
log4g_repository_selector_get_type(void);

Log4gLoggerRepository *
log4g_repository_selector_get_logger_repository(Log4gRepositorySelector *self);

G_END_DECLS

#endif /* LOG4G_REPOSITORY_SELECTOR_H */
