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
 * \brief The root logger
 * \author Mike Steinert
 * \date 2-10-2010
 *
 * The root logger sits on top of the logger hierarchy. It is a normal logger
 * with the following exceptions:
 * -# It may not be assigned a NULL level threshold
 * -# The root logger cannot have a parent (the get_effective_threshold()
 *    method always returns the value of the level field without walking
 *    the hierarchy)
 */

#ifndef LOG4G_ROOT_LOGGER_H
#define LOG4G_ROOT_LOGGER_H

#include <log4g/logger.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_ROOT_LOGGER \
    (log4g_root_logger_get_type())

#define LOG4G_ROOT_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_ROOT_LOGGER, \
            Log4gRootLogger))

#define LOG4G_IS_ROOT_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_ROOT_LOGGER))

#define LOG4G_ROOT_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_ROOT_LOGGER, \
            Log4gRootLoggerClass))

#define LOG4G_IS_ROOT_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_ROOT_LOGGER))

#define LOG4G_ROOT_LOGGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_ROOT_LOGGER, \
            Log4gRootLoggerClass))

/** \brief Log4gRootLogger object type definition */
typedef struct _Log4gRootLogger Log4gRootLogger;

/** \brief Log4gRootLogger class type definition */
typedef struct _Log4gRootLoggerClass Log4gRootLoggerClass;

/** \brief Log4gRootLoggerClass definition */
struct _Log4gRootLogger {
    Log4gLogger parent_instance;
};

/** \brief Log4gRootLoggerClass definition */
struct _Log4gRootLoggerClass {
    Log4gLoggerClass parent_class;
};

GType
log4g_root_logger_get_type(void);

/**
 * \brief Create a new root logger.
 *
 * \param level [in] The level threshold of this root logger.
 *
 * \return A new root logger object.
 */
Log4gLogger *
log4g_root_logger_new(Log4gLevel *level);

G_END_DECLS

#endif /* LOG4G_ROOT_LOGGER_H */
