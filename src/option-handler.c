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
 * \brief Implements the API in log4g/interface/option-handler.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/option-handler.h"

G_DEFINE_INTERFACE(Log4gOptionHandler, log4g_option_handler, G_TYPE_INVALID)

static void
log4g_option_handler_default_init(Log4gOptionHandlerInterface *klass)
{
    /* do nothing */
}

void
log4g_option_handler_activate_options(Log4gOptionHandler *self)
{
    g_return_if_fail(LOG4G_IS_OPTION_HANDLER(self));
    Log4gOptionHandlerInterface *interface =
        LOG4G_OPTION_HANDLER_GET_INTERFACE(self);
    interface->activate_options(self);
}
