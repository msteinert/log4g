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
 * SECTION: default-logger-factory
 * @short_description: an implementation of logger factory interface
 * @see_also: #Log4gLoggerFactoryInterface
 *
 * This class provides the default logger factory implementation used by Log4g.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
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

/**
 * log4g_default_logger_factory_new:
 *
 * Create a new default logger factory object.
 *
 * Returns: A new default logger factory object.
 * Since: 0.1
 */
Log4gLoggerFactory *
log4g_default_logger_factory_new(void)
{
    return g_object_new(LOG4G_TYPE_DEFAULT_LOGGER_FACTORY, NULL);
}
