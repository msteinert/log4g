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

#ifndef LOG4G_APPENDER_H
#define LOG4G_APPENDER_H

#include <log4g/filter.h>
#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER \
    (log4g_appender_get_type())

#define LOG4G_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_APPENDER, \
            Log4gAppender))

#define LOG4G_IS_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_APPENDER))

#define LOG4G_APPENDER_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_APPENDER, \
            Log4gAppenderInterface));

/** \brief Log4gAppender object type definition */
typedef struct _Log4gAppender Log4gAppender;

/** \brief Log4gAppender class type definition */
typedef struct _Log4gAppenderInterface Log4gAppenderInterface;

/** \brief Log4gAppenderClass definition */
struct _Log4gAppenderInterface {
    GTypeInterface parent_interface; /**< parent interface */
    void (*add_filter)(Log4gAppender *self, Log4gFilter *filter);
    Log4gFilter *(*get_filter)(Log4gAppender *self);
    void (*close)(Log4gAppender *self);
    void (*do_append)(Log4gAppender *self, Log4gLoggingEvent *event);
    const gchar *(*get_name)(Log4gAppender *self);
    void (*set_error_handler)(Log4gAppender *self, gpointer handler);
    gpointer (*get_error_handler)(Log4gAppender *self);
    void (*set_layout)(Log4gAppender *self, Log4gLayout *layout);
    Log4gLayout *(*get_layout)(Log4gAppender *self);
    void (*set_name)(Log4gAppender *self, const gchar *name);
    gboolean (*requires_layout)(Log4gAppender *self);
};

GType
log4g_appender_get_type(void);

/**
 */
void
log4g_appender_add_filter(Log4gAppender *self, Log4gFilter *filter);

/**
 */
Log4gFilter *
log4g_appender_get_filter(Log4gAppender *self);

/**
 */
void
log4g_appender_close(Log4gAppender *self);

/**
 */
void
log4g_appender_do_append(Log4gAppender *self, Log4gLoggingEvent *event);

/**
 */
const gchar *
log4g_appender_get_name(Log4gAppender *self);

/**
 */
void
log4g_appender_set_error_handler(Log4gAppender *self, gpointer handler);

/**
 */
gpointer
log4g_appender_get_error_handler(Log4gAppender *self);

/**
 */
void
log4g_appender_set_layout(Log4gAppender *self, Log4gLayout *layout);

/**
 */
Log4gLayout *
log4g_appender_get_layout(Log4gAppender *self);

/**
 */
void
log4g_appender_set_name(Log4gAppender *self, const gchar *name);

/**
 */
gboolean
log4g_appender_requires_layout(Log4gAppender *self);

/**
 */
void
log4g_appender_activate_options(Log4gAppender *self);

G_END_DECLS

#endif /* LOG4G_APPENDER_H */
