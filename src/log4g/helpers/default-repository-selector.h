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

#ifndef LOG4G_DEFAULT_REPOSITORY_SELECTOR_H
#define LOG4G_DEFAULT_REPOSITORY_SELECTOR_H

#include <log4g/interface/repository-selector.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR \
    (log4g_default_repository_selector_get_type())

#define LOG4G_DEFAULT_REPOSITORY_SELECTOR(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, \
            Log4gDefaultRepositorySelector))

#define LOG4G_IS_DEFAULT_REPOSITORY_SELECTOR(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR))

#define LOG4G_DEFAULT_REPOSITORY_SELECTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, \
            Log4gDefaultRepositorySelectorClass))

#define LOG4G_IS_DEFAULT_REPOSITORY_SELECTOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR))

#define LOG4G_DEFAULT_REPOSITORY_SELECTOR_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, \
            Log4gDefaultRepositorySelectorClass))

/** \brief Log4gDefaultRepositorySelector type definition */
typedef struct _Log4gDefaultRepositorySelector Log4gDefaultRepositorySelector;

/** \brief Log4gDefaultRepositorySelector class type definition */
typedef struct _Log4gDefaultRepositorySelectorClass
    Log4gDefaultRepositorySelectorClass;

/** \brief Log4gDefaultRepositorySelectorClass definition */
struct _Log4gDefaultRepositorySelector {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gDefaultRepositorySelectorClass definition */
struct _Log4gDefaultRepositorySelectorClass {
    GObjectClass parent_class; /**< parent class */
};

GType
log4g_default_repository_selector_get_type(void);

/**
 */
Log4gRepositorySelector *
log4g_default_repository_selector_new(Log4gLoggerRepository *repository);

G_END_DECLS

#endif /* LOG4G_DEFAULT_REPOSITORY_SELECTOR_H */
