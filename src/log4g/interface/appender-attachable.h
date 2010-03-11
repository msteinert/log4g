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

#ifndef LOG4G_APPENDER_ATTACHABLE_H
#define LOG4G_APPENDER_ATTACHABLE_H

#include <log4g/interface/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER_ATTACHABLE \
    (log4g_appender_attachable_get_type())

#define LOG4G_APPENDER_ATTACHABLE(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_APPENDER_ATTACHABLE, \
            Log4gAppenderAttachable))

#define LOG4G_IS_APPENDER_ATTACHABLE(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_APPENDER_ATTACHABLE))

#define LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_APPENDER_ATTACHABLE, \
            Log4gAppenderAttachableInterface));

/** \brief Log4gAppenderAttachable object type definition */
typedef struct _Log4gAppenderAttachable Log4gAppenderAttachable;

/** \brief Log4gAppenderAttachable class type definition */
typedef struct _Log4gAppenderAttachableInterface
                    Log4gAppenderAttachableInterface;

/** \brief Log4gAppenderAttachableClass definition */
struct _Log4gAppenderAttachableInterface {
    GTypeInterface parent_interface; /**< parent interface */
    void (*add_appender)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    const GArray *(*get_all_appenders)(Log4gAppenderAttachable *self);
    Log4gAppender *(*get_appender)(Log4gAppenderAttachable *self,
            const gchar *name);
    gboolean (*is_attached)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    void (*remove_all_appenders)(Log4gAppenderAttachable *self);
    void (*remove_appender)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    void (*remove_appender_name)(Log4gAppenderAttachable *self,
            const gchar *name);
};

GType
log4g_appender_attachable_get_type(void);

/**
 */
void
log4g_appender_attachable_add_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 */
const GArray *
log4g_appender_attachable_get_all_appenders(Log4gAppenderAttachable *self);

/**
 */
Log4gAppender *
log4g_appender_attachable_get_appender(Log4gAppenderAttachable *self,
        const gchar *name);

/**
 */
gboolean
log4g_appender_attachable_is_attached(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 */
void
log4g_appender_attachable_remove_all_appenders(Log4gAppenderAttachable *self);

/**
 */
void
log4g_appender_attachable_remove_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 */
void
log4g_appender_attachable_remove_appender_name(Log4gAppenderAttachable *self,
        const gchar *name);

G_END_DECLS

#endif /* LOG4G_APPENDER_ATTACHABLE_H */
