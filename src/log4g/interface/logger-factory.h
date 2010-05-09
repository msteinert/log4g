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
 * \file
 * \brief Create a logger via a factory class.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * Implement this interface to create new instances of Log4gLogger or a
 * sub-class of Log4gLogger.
 *
 * \see log4g/logger.h
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

/** \brief Log4gLoggerFactory object type definition */
typedef struct _Log4gLoggerFactory Log4gLoggerFactory;

/** \brief Log4gLoggerFactory interface type definition */
typedef struct _Log4gLoggerFactoryInterface Log4gLoggerFactoryInterface;

/** \brief Log4gLoggerFactoryInterface definition */
struct _Log4gLoggerFactoryInterface {
    GTypeInterface parent_interface;

    /**
     * \brief Create a new logger object.
     *
     * \param self [in] A logger factory object.
     * \param name [in] The name of the logger to create.
     *
     * \return A new logger named \e name.
     */
    Log4gLogger *
    (*make_new_logger_instance)(Log4gLoggerFactory *self, const gchar *name);
};

GType
log4g_logger_factory_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerFactoryInterface::make_new_logger_instance().
 *
 * \param self [in] A logger factory object.
 * \param name [in] The name of the logger to create.
 *
 * \return A new logger named \e name.
 */
Log4gLogger *
log4g_logger_factory_make_new_logger_instance(Log4gLoggerFactory *self,
        const gchar *name);

G_END_DECLS

#endif /* LOG4G_LOGGER_FACTORY_H */
