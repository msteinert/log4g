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
 * SECTION: repository-selector
 * @short_description: Select the correct logger repository
 * @see_also: #Log4gLoggerRepositoryInterface
 *
 * The log manager uses a repository selector implementation to select the
 * logger repository for a particular application context.
 *
 * It is the responsibility of the repository selector implementation to
 * track the application context. Log4g makes no assumptions about the
 * application context.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/repository-selector.h"

G_DEFINE_INTERFACE(Log4gRepositorySelector, log4g_repository_selector,
		G_TYPE_INVALID)

static void
log4g_repository_selector_default_init(
		G_GNUC_UNUSED Log4gRepositorySelectorInterface *klass)
{
	/* do nothing */
}

/**
 * log4g_repository_selector_get_logger_repository:
 * @self: A repository selector object.
 *
 * Call the @get_logger_repository function from the
 * #Log4gRepositorySelectorInterface of @self.
 *
 * See: #Log4gLoggerRepositoryInterface
 *
 * Returns: (transfer none): The logger repository for the current application
 *          context.
 * Since: 0.1
 */
Log4gLoggerRepository *
log4g_repository_selector_get_logger_repository(Log4gRepositorySelector *self)
{
	g_return_val_if_fail(LOG4G_IS_REPOSITORY_SELECTOR(self), NULL);
	Log4gRepositorySelectorInterface *interface =
		LOG4G_REPOSITORY_SELECTOR_GET_INTERFACE(self);
	return interface->get_logger_repository(self);
}
