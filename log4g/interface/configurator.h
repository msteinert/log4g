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

#ifndef LOG4G_CONFIGURATOR_H
#define LOG4G_CONFIGURATOR_H

#include <log4g/interface/logger-repository.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_CONFIGURATOR \
	(log4g_configurator_get_type())

#define LOG4G_CONFIGURATOR(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_CONFIGURATOR, \
		Log4gConfigurator))

#define LOG4G_IS_CONFIGURATOR(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_CONFIGURATOR))

#define LOG4G_CONFIGURATOR_GET_INTERFACE(instance) \
	(G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_CONFIGURATOR, \
		Log4gConfiguratorInterface));

typedef struct Log4gConfigurator_ Log4gConfigurator;

typedef struct Log4gConfiguratorInterface_ Log4gConfiguratorInterface;

/**
 * Log4gConfiguratorDoConfigure:
 * @self: A configurator object.
 * @uri: The URI from which load configuration.
 * @repository: The repository to configure.
 * @error: Errors are returned here.
 *
 * Interpret a resource pointed to a URI and configure Log4g.
 *
 * Returns: %TRUE if the configuration was successful, %FALSE otherwise.
 * Since: 0.1
 */
typedef gboolean
(*Log4gConfiguratorDoConfigure)(Log4gConfigurator *self, const char *uri,
		Log4gLoggerRepository *repository, GError **error);

/**
 * Log4gConfiguratorInterface:
 * @do_configure: Perform configuration actions.
 */
struct Log4gConfiguratorInterface_ {
	/*< private >*/
	GTypeInterface parent_interface;
	/*< public >*/
	Log4gConfiguratorDoConfigure do_configure;
};

GType
log4g_configurator_get_type(void) G_GNUC_CONST;

gboolean
log4g_configurator_do_configure(Log4gConfigurator *self, const char *uri,
		Log4gLoggerRepository *repository, GError **error);

G_END_DECLS

#endif /* LOG4G_CONFIGURATOR_H */
