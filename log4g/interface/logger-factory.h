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

#ifndef LOG4G_LOGGER_FACTORY_H
#define LOG4G_LOGGER_FACTORY_H

#include <log4g/logger.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER_FACTORY \
    (log4g_logger_factory_get_type())

#define LOG4G_LOGGER_FACTORY(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER_FACTORY, \
            Log4gLoggerFactory))

#define LOG4G_IS_LOGGER_FACTORY(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER_FACTORY))

#define LOG4G_LOGGER_FACTORY_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_LOGGER_FACTORY, \
            Log4gLoggerFactoryInterface));

typedef struct _Log4gLoggerFactory Log4gLoggerFactory;

typedef struct _Log4gLoggerFactoryInterface Log4gLoggerFactoryInterface;

/**
 * Log4gLoggerFactoryMakeNewLoggerInstance:
 * @self: A logger factory object.
 * @name: The name of the logger to create.
 *
 * Create a new logger object.
 *
 * Returns: A new logger named \e name.
 * Since: 0.1
 */
typedef Log4gLogger *
(*Log4gLoggerFactoryMakeNewLoggerInstance)(Log4gLoggerFactory *self,
        const gchar *name);

/**
 * Log4gLoggerFactoryInterface:
 * @make_new_logger_instance: Create a new logger object.
 */
struct _Log4gLoggerFactoryInterface {
    /*< private >*/
    GTypeInterface parent_interface;
    /*< public >*/
    Log4gLoggerFactoryMakeNewLoggerInstance make_new_logger_instance;
};

GType
log4g_logger_factory_get_type(void) G_GNUC_CONST;

Log4gLogger *
log4g_logger_factory_make_new_logger_instance(Log4gLoggerFactory *self,
        const gchar *name);

G_END_DECLS

#endif /* LOG4G_LOGGER_FACTORY_H */
