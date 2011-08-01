/* Copyright 2010, 2011 Michael Steinert
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
 * SECTION: configurator
 * @short_description: Log4g configuration interface
 *
 * This interface should be implemented by classes capable of configuring
 * Log4g via a URI.
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

/**
 * log4g_configurator_do_configure:
 * @self: A configurator object.
 * @uri: The URI from which load configuration.
 * @repository: The repository to configure.
 * @error: Errors are returned here.
 *
 * Calls the @do_configure function from the #Log4gConfiguratorInterface
 * of @self.
 *
 * Returns: %TRUE if the configuration was successful, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_configurator_do_configure(Log4gConfigurator *self, const char *url,
		Log4gLoggerRepository *repository, GError **error)
{
	g_return_val_if_fail(LOG4G_IS_CONFIGURATOR(self), FALSE);
	Log4gConfiguratorInterface *interface =
		LOG4G_CONFIGURATOR_GET_INTERFACE(self);
	return interface->do_configure(self, url, repository, error);
}
