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

#ifndef LOG4G_MODULE_LOADER_H
#define LOG4G_MODULE_LOADER_H

#include <log4g/module.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_MODULE_LOADER \
    (log4g_module_loader_get_type())

#define LOG4G_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_MODULE_LOADER, \
            Log4gModuleLoader))

#define LOG4G_IS_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_MODULE_LOADER))

#define LOG4G_MODULE_LOADER_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), \
            LOG4G_TYPE_MODULE_LOADER, Log4gModuleLoaderInterface));

typedef struct _Log4gModuleLoader Log4gModuleLoader;

typedef struct _Log4gModuleLoaderInterface Log4gModuleLoaderInterface;

/**
 * Log4gModuleLoaderLoadModules:
 * @self: A module loader object.
 *
 * Load external modules for the current application context.
 * 
 * Since: 0.1
 */
typedef void
(*Log4gModuleLoaderLoadModules)(Log4gModuleLoader *self);

/**
 * Log4gModuleLoaderInterface:
 * @load_modules: Load external modules.
 */
struct _Log4gModuleLoaderInterface {
    /*< private >*/
    GTypeInterface parent_interface;
    /*< public >*/
    Log4gModuleLoaderLoadModules load_modules;
};

GType
log4g_module_loader_get_type(void) G_GNUC_CONST;

void
log4g_module_loader_load_modules(Log4gModuleLoader *self);

G_END_DECLS

#endif /* LOG4G_MODULE_LOADER_H */
