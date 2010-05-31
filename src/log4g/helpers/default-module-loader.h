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
 * \brief An implementation of the module loader interface.
 * \author Mike Steinert
 * \date 5-30-2010
 *
 * This class provides the default module loader implementation used
 * by Log4g.
 *
 * \see log4g/interface/module-loader.h
 */

#ifndef LOG4G_DEFAULT_MODULE_LOADER_H
#define LOG4G_DEFAULT_MODULE_LOADER_H

#include <log4g/interface/module-loader.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DEFAULT_MODULE_LOADER \
    (log4g_default_module_loader_get_type())

#define LOG4G_DEFAULT_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoader))

#define LOG4G_IS_DEFAULT_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_DEFAULT_MODULE_LOADER))

#define LOG4G_DEFAULT_MODULE_LOADER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoaderClass))

#define LOG4G_IS_DEFAULT_MODULE_LOADER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DEFAULT_MODULE_LOADER))

#define LOG4G_DEFAULT_MODULE_LOADER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoaderClass))

/** \brief Log4gDefaultModuleLoader type definition */
typedef struct _Log4gDefaultModuleLoader Log4gDefaultModuleLoader;

/** \brief Log4gDefaultModuleLoader class type definition */
typedef struct _Log4gDefaultModuleLoaderClass
    Log4gDefaultModuleLoaderClass;

/** \brief Log4gDefaultModuleLoaderClass definition */
struct _Log4gDefaultModuleLoader {
    GObject parent_instance;
};

/** \brief Log4gDefaultModuleLoaderClass definition */
struct _Log4gDefaultModuleLoaderClass {
    GObjectClass parent_class;
};

GType
log4g_default_module_loader_get_type(void);

/**
 * \brief Create a new default repository selector object.
 *
 * \return A new default repository selector object.
 */
Log4gModuleLoader *
log4g_default_module_loader_new(void);

G_END_DECLS

#endif /* LOG4G_DEFAULT_MODULE_LOADER_H */
