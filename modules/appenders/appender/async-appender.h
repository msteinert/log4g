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

#ifndef LOG4G_ASYNC_APPENDER_H
#define LOG4G_ASYNC_APPENDER_H

#include <log4g/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_ASYNC_APPENDER \
    (log4g_async_appender_get_type())

#define LOG4G_ASYNC_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppender))

#define LOG4G_IS_ASYNC_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_ASYNC_APPENDER))

#define LOG4G_ASYNC_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppenderClass))

#define LOG4G_IS_ASYNC_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_ASYNC_APPENDER))

#define LOG4G_ASYNC_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppenderClass))

typedef struct _Log4gAsyncAppender Log4gAsyncAppender;

typedef struct _Log4gAsyncAppenderClass Log4gAsyncAppenderClass;

/**
 * Log4gAsyncAppender:
 *
 * The <structname>Log4gAsyncAppender</structname> structure does not have any
 * public members.
 */
struct _Log4gAsyncAppender {
    /*< private >*/
    Log4gAppender parent_instance;
};

/**
 * Log4gAsyncAppenderClass:
 *
 * The <structname>Log4gAsyncAppenderClass</structname> structure does not
 * have any public members.
 */
struct _Log4gAsyncAppenderClass {
    /*< private >*/
    Log4gAppenderClass parent_class;
};

G_GNUC_INTERNAL GType
log4g_async_appender_get_type(void);

G_GNUC_INTERNAL void
log4g_async_appender_register(GTypeModule *module);

G_GNUC_INTERNAL void
log4g_async_appender_add_appender(Log4gAppender *base,
        Log4gAppender *appender);

G_GNUC_INTERNAL const GArray *
log4g_async_appender_get_all_appenders(Log4gAppender *base);

G_GNUC_INTERNAL Log4gAppender *
log4g_async_appender_get_appender(Log4gAppender *base, const gchar *name);

G_GNUC_INTERNAL gboolean
log4g_async_appender_is_attached(Log4gAppender *base, Log4gAppender *appender);

G_GNUC_INTERNAL void
log4g_async_appender_remove_all_appenders(Log4gAppender *base);

G_GNUC_INTERNAL void
log4g_async_appender_remove_appender(Log4gAppender *base,
        Log4gAppender *appender);

G_GNUC_INTERNAL void
log4g_async_appender_remove_appender_name(Log4gAppender *base,
        const gchar *name);

G_END_DECLS

#endif /* LOG4G_ASYNC_APPENDER_H */
