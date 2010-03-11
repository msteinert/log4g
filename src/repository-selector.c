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
 * \brief Implements the API in log4g/interface/repository-selector.h
 * \author Mike Steinert
 * \date 2-11-2010
 */

#include "config.h"
#include "log4g/interface/repository-selector.h"

G_DEFINE_INTERFACE(Log4gRepositorySelector, log4g_repository_selector,
        G_TYPE_INVALID)

static void
log4g_repository_selector_default_init(Log4gRepositorySelectorInterface *klass)
{
    /* do nothing */
}

Log4gLoggerRepository *
log4g_repository_selector_get_logger_repository(Log4gRepositorySelector *self)
{
    Log4gRepositorySelectorInterface *interface;
    g_return_val_if_fail(LOG4G_IS_REPOSITORY_SELECTOR(self), NULL);
    interface = LOG4G_REPOSITORY_SELECTOR_GET_INTERFACE(self);
    return interface->get_logger_repository(self);
}
