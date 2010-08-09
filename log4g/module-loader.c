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
 * SECTION: module-loader
 * @short_description: load external modules
 * @see_also: #Log4gModuleClass
 *
 * The log manager uses a module loader implementation to load external
 * modules for a particular application context.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/module-loader.h"

G_DEFINE_INTERFACE(Log4gModuleLoader, log4g_module_loader,
        G_TYPE_INVALID)

static void
log4g_module_loader_default_init(Log4gModuleLoaderInterface *klass)
{
    /* do nothing */
}

/**
 * log4g_module_loader_load_modules:
 * @self: A module loader object.
 *
 * Call the @load_modules function from the #Log4gModuleLoaderInterface
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_module_loader_load_modules(Log4gModuleLoader *self)
{
    g_return_if_fail(LOG4G_IS_MODULE_LOADER(self));
    Log4gModuleLoaderInterface *interface =
        LOG4G_MODULE_LOADER_GET_INTERFACE(self);
    return interface->load_modules(self);
}
