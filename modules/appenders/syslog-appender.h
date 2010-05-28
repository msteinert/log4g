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
 * \brief Append to the local syslog
 * \author Mike Steinert
 * \date 2-11-2010
 *
 * This is a simple syslog appender that appends to the local syslog (usually
 * /var/log/messages) using the syslog(3) system call.
 *
 * Syslog appenders accept the following properties:
 * -# ident
 * -# option
 * -# facility
 *
 * The value of ident will be the first parameter to openlog(3).
 *
 * The value of option will be the second parameter to openlog(3).
 *
 * The value of facility will be the third parameter to openlog(3).
 *
 * \note The behavior of this appender may be affected by the configuration
 *       of the syslog daemon. Refer to the syslog documentation for more
 *       information.
 *
 * \see syslog(3), syslog.conf(5), syslogd(8)
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

/** \brief Log4gSyslogAppender object type definition */
typedef struct _Log4gSyslogAppender Log4gSyslogAppender;

/** \brief Log4gSyslogAppender class type definition */
typedef struct _Log4gSyslogAppenderClass Log4gSyslogAppenderClass;

/** \brief Log4gSyslogAppenderClass definition */
struct _Log4gSyslogAppender {
    Log4gAppender parent_instance; /**< parent instance */
};

/** \brief Log4gSyslogAppenderClass definition */
struct _Log4gSyslogAppenderClass {
    Log4gAppenderClass parent_class; /**< parent class */
};

GType
log4g_syslog_appender_get_type(void);

void
log4g_syslog_appender_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_SYSLOG_APPENDER_H */
