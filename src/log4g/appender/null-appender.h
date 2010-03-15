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
 * \brief A do-nothing appender.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * The null appender merely exists; It never outputs any log messages..
 */

#ifndef LOG4G_NULL_APPENDER_H
#define LOG4G_NULL_APPENDER_H

#include <log4g/appender/appender-skeleton.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_NULL_APPENDER \
    (log4g_null_appender_get_type())

#define LOG4G_NULL_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_NULL_APPENDER, \
            Log4gNullAppender))

#define LOG4G_IS_NULL_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_NULL_APPENDER))

#define LOG4G_NULL_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_NULL_APPENDER, \
            Log4gNullAppenderClass))

#define LOG4G_IS_NULL_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_NULL_APPENDER))

#define LOG4G_NULL_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_NULL_APPENDER, \
            Log4gNullAppenderClass))

/** \brief Log4gNullAppender object type definition */
typedef struct _Log4gNullAppender Log4gNullAppender;

/** \brief Log4gNullAppender class type definition */
typedef struct _Log4gNullAppenderClass Log4gNullAppenderClass;

/** \brief Log4gNullAppenderClass definition */
struct _Log4gNullAppender {
    Log4gAppenderSkeleton parent_instance;
};

/** \brief Log4gNullAppenderClass definition */
struct _Log4gNullAppenderClass {
    Log4gAppenderSkeletonClass parent_class;
};

GType
log4g_null_appender_get_type(void);

/**
 * \brief Create a new null appender object.
 *
 * This class is actually a singleton. This function simply adds a reference
 * the existing null appender object.
 *
 * \return A new null appender object.
 */
Log4gAppender *
log4g_null_appender_new(void);

G_END_DECLS

#endif /* LOG4G_NULL_APPENDER_H */
