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

#ifndef LOG4G_DEFAULT_MODULE_LOADER_H
#define LOG4G_DEFAULT_MODULE_LOADER_H

#include <log4g/interface/module-loader.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DEFAULT_MODULE_LOADER \
    (log4g_default_module_loader_get_type())

#define LOG4G_DEFAULT_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoader))

#define LOG4G_IS_DEFAULT_MODULE_LOADER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_DEFAULT_MODULE_LOADER))

#define LOG4G_DEFAULT_MODULE_LOADER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoaderClass))

#define LOG4G_IS_DEFAULT_MODULE_LOADER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DEFAULT_MODULE_LOADER))

#define LOG4G_DEFAULT_MODULE_LOADER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            Log4gDefaultModuleLoaderClass))

typedef struct _Log4gDefaultModuleLoader Log4gDefaultModuleLoader;

typedef struct _Log4gDefaultModuleLoaderClass
    Log4gDefaultModuleLoaderClass;

/**
 * Log4gDefaultModuleLoader:
 *
 * The <structname>Log4gDefaultModuleLoader</structname> structure does not
 * have any public members.
 */
struct _Log4gDefaultModuleLoader {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gDefaultModuleLoaderClass:
 *
 * The <structname>Log4gDefaultModuleLoaderClass</structname> structure does
 * not have any public members.
 */
struct _Log4gDefaultModuleLoaderClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_default_module_loader_get_type(void) G_GNUC_CONST;

Log4gModuleLoader *
log4g_default_module_loader_new(void);

G_END_DECLS

#endif /* LOG4G_DEFAULT_MODULE_LOADER_H */
