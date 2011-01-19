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

#ifndef LOG4G_MODULE_H
#define LOG4G_MODULE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_MODULE \
    (log4g_module_get_type())

#define LOG4G_MODULE(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_MODULE, Log4gModule))

#define LOG4G_IS_MODULE(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_MODULE))

#define LOG4G_MODULE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_MODULE, Log4gModuleClass))

#define LOG4G_IS_MODULE_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_MODULE))

#define LOG4G_MODULE_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_MODULE, \
            Log4gModuleClass))

typedef struct _Log4gModule Log4gModule;

typedef struct _Log4gModuleClass Log4gModuleClass;

/**
 * Log4gModule:
 *
 * The <structname>Log4gModule</structname> structure does not have any public
 * members.
 */
struct _Log4gModule {
    /*< private >*/
    GTypeModule parent_instance;
};

/**
 * Log4gModuleClass:
 *
 * The <structname>Log4gModuleClass</structname> structure does not have any
 * public members.
 */
struct _Log4gModuleClass {
    /*< private >*/
    GTypeModuleClass parent_class;
};

GType
log4g_module_get_type(void) G_GNUC_CONST;

GTypeModule *
log4g_module_new(const gchar *file);

/**
 * log4g_module_load:
 * @module: A Log4gModule object.
 *
 * Extension modules must implement this function.
 *
 * This function is run after the module has been loaded in order to load
 * dynamic types for use by the Log4g system.
 *
 * Since: 0.1
 */
void
log4g_module_load(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_MODULE_H */
