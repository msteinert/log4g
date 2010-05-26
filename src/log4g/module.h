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
 * \brief Implements the API in log4g/module.h
 * \author Mike Steinert
 * \date 5-25-2010
 *
 * TODO
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

/** \brief Log4gModule object type definition */
typedef struct _Log4gModule Log4gModule;

/** \brief Log4gModule class type definition */
typedef struct _Log4gModuleClass Log4gModuleClass;

/** \brief Log4gModuleClass definition */
struct _Log4gModule {
    GTypeModule parent_instance;
};

/** \brief Log4gModuleClass definition */
struct _Log4gModuleClass {
    GTypeModuleClass parent_class;
};

GType
log4g_module_get_type(void);

/**
 * \brief Create a new module.
 *
 * \param file [in] The path of the file to load as a module.
 *
 * \return A new module object.
 */
GTypeModule *
log4g_module_new(const gchar *file);

/**
 * \brief Extension modules must implement this function.
 *
 * This function is run after the module has been loaded in order to
 * initialize the module.
 *
 * \param module [in] A Log4gModule object.
 */
void
log4g_module_load(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_MODULE_H */
