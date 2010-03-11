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
 * \brief ...
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifndef LOG4G_APPENDER_ATTACHABLE_IMPL_H
#define LOG4G_APPENDER_ATTACHABLE_IMPL_H

#include <log4g/interface/appender-attachable.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL \
    (log4g_appender_attachable_impl_get_type())

#define LOG4G_APPENDER_ATTACHABLE_IMPL(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, Log4gAppenderAttachableImpl))

#define LOG4G_IS_APPENDER_ATTACHABLE_IMPL(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL))

#define LOG4G_APPENDER_ATTACHABLE_IMPL_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, \
            Log4gAppenderAttachableImplClass))

#define LOG4G_IS_APPENDER_ATTACHABLE_IMPL_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL))

#define LOG4G_APPENDER_ATTACHABLE_IMPL_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, \
            Log4gAppenderAttachableImplClass))

/** \brief Log4gAppenderAttachableImpl object type definition */
typedef struct _Log4gAppenderAttachableImpl Log4gAppenderAttachableImpl;

/** \brief Log4gAppenderAttachableImpl class type definition */
typedef struct _Log4gAppenderAttachableImplClass
                        Log4gAppenderAttachableImplClass;

/** \brief Log4gAppenderAttachableImplClass definition */
struct _Log4gAppenderAttachableImpl {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gAppenderAttachableImplClass definition */
struct _Log4gAppenderAttachableImplClass {
    GObjectClass parent_class; /**< parent class */
};

GType
log4g_appender_attachable_impl_get_type(void);

/**
 */
Log4gAppenderAttachable *
log4g_appender_attachable_impl_new(void);

/**
 */
guint
log4g_appender_attachable_impl_append_loop_on_appenders(
        Log4gAppenderAttachable *base, Log4gLoggingEvent *event);

G_END_DECLS

#endif /* LOG4G_APPENDER_ATTACHABLE_IMPL_H */
