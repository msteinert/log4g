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

#ifndef LOG4G_SYSLOG_APPENDER_H
#define LOG4G_SYSLOG_APPENDER_H

#include <log4g/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_SYSLOG_APPENDER \
    (log4g_syslog_appender_get_type())

#define LOG4G_SYSLOG_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_SYSLOG_APPENDER, \
            Log4gSyslogAppender))

#define LOG4G_IS_SYSLOG_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_SYSLOG_APPENDER))

#define LOG4G_SYSLOG_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_SYSLOG_APPENDER, \
            Log4gSyslogAppenderClass))

#define LOG4G_IS_SYSLOG_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_SYSLOG_APPENDER))

#define LOG4G_SYSLOG_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_SYSLOG_APPENDER, \
            Log4gSyslogAppenderClass))

typedef struct _Log4gSyslogAppender Log4gSyslogAppender;

typedef struct _Log4gSyslogAppenderClass Log4gSyslogAppenderClass;

/**
 * Log4gSyslogAppender:
 *
 * The <structname>Log4gSyslogAppender</structname> structure does not have any
 * public members.
 */
struct _Log4gSyslogAppender {
    /*< private >*/
    Log4gAppender parent_instance;
};

/**
 * Log4gSyslogAppenderClass:
 *
 * The <structname>Log4gSyslogAppenderClass</structname> structure does not
 * have any public members.
 */
struct _Log4gSyslogAppenderClass {
    /*< private >*/
    Log4gAppenderClass parent_class;
};

GType
log4g_syslog_appender_get_type(void);

void
log4g_syslog_appender_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_SYSLOG_APPENDER_H */
