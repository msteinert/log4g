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
 * \brief Implements the API in log4g/interface/configurator.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/configurator.h"

G_DEFINE_INTERFACE(Log4gConfigurator, log4g_configurator, G_TYPE_INVALID)

static void
log4g_configurator_default_init(Log4gConfiguratorInterface *klass)
{
    /* do nothing */
}

gboolean
log4g_configurator_do_configure(Log4gConfigurator *self, const char *url,
        Log4gLoggerRepository *repository, GError **error)
{
    Log4gConfiguratorInterface *interface;
    g_return_val_if_fail(LOG4G_IS_CONFIGURATOR(self), FALSE);
    interface = LOG4G_CONFIGURATOR_GET_INTERFACE(self);
    return interface->do_configure(self, url, repository, error);
}
