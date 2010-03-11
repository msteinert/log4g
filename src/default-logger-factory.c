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
 * \brief Implements the API in log4g/default-logger-factory.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include "log4g/helpers/default-logger-factory.h"

static Log4gLogger *
make_new_logger_instance(Log4gLoggerFactory *base, const gchar *name)
{
    return log4g_logger_new(name);
}
    
static void
logger_factory_init(Log4gLoggerFactoryInterface *interface, gpointer data)
{
    interface->make_new_logger_instance = make_new_logger_instance;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDefaultLoggerFactory,
        log4g_default_logger_factory, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_LOGGER_FACTORY, logger_factory_init))

static void
log4g_default_logger_factory_init(Log4gDefaultLoggerFactory *self)
{
    /* do nothing */
}

static void
log4g_default_logger_factory_class_init(Log4gDefaultLoggerFactoryClass *klass)
{
    /* do nothing */
}

Log4gLoggerFactory *
log4g_default_logger_factory_new(void)
{
    return g_object_new(LOG4G_TYPE_DEFAULT_LOGGER_FACTORY, NULL);
}
