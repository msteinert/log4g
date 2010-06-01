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
 * \brief Load external modules.
 * \author Mike Steinert
 * \date 5-30-2010
 *
 * The log manager uses a module loader implementation to load external
 * modules for a particular application context.
 *
 * \see log4g/module.h
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

/** \brief Log4gModuleLoader object type definition */
typedef struct _Log4gModuleLoader Log4gModuleLoader;

/** \brief Log4gModuleLoader interface type definition */
typedef struct _Log4gModuleLoaderInterface
        Log4gModuleLoaderInterface;

/** \brief Log4gModuleLoaderInterface definition */
struct _Log4gModuleLoaderInterface {
    GTypeInterface parent_interface;

    /**
     * \brief Load external modules for the current application context.
     * 
     * \param self [in] A module loader object.
     */
    void
    (*load_modules)(Log4gModuleLoader *self);
};

GType
log4g_module_loader_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gModuleLoaderInterface::load_modules().
 *
 * \param self [in] A module loader object.
 */
void
log4g_module_loader_load_modules(Log4gModuleLoader *self);

G_END_DECLS

#endif /* LOG4G_MODULE_LOADER_H */
