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
 * \brief Log events to stdout or stderr.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * The console appender logs events to \e stdout or \e stderr using a layout
 * specified by the user. The default target is \e stdout.
 *
 * Console appenders accept two properties:
 * -# target
 * -# follow
 *
 * The value of target determines where the output will be logged. The value
 * must be one of "stdout" or "stderr". The default is "stdout".
 *
 * The value of follow determines if the log output will follow reopens of
 * the target stream. The default value is \e TRUE.
 *
 * \see freopen(3)
 */

#ifndef LOG4G_CONSOLE_APPENDER_H
#define LOG4G_CONSOLE_APPENDER_H

#include "appender/writer-appender.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_CONSOLE_APPENDER \
    (log4g_console_appender_get_type())

#define LOG4G_CONSOLE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppender))

#define LOG4G_IS_CONSOLE_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_CONSOLE_APPENDER))

#define LOG4G_CONSOLE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppenderClass))

#define LOG4G_IS_CONSOLE_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_CONSOLE_APPENDER))

#define LOG4G_CONSOLE_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_CONSOLE_APPENDER, \
            Log4gConsoleAppenderClass))

/** \brief Log4gConsoleAppender object type definition */
typedef struct _Log4gConsoleAppender Log4gConsoleAppender;

/** \brief Log4gConsoleAppender class type definition */
typedef struct _Log4gConsoleAppenderClass Log4gConsoleAppenderClass;

/** \brief Log4gConsoleAppenderClass definition */
struct _Log4gConsoleAppender {
    Log4gWriterAppender parent_instance;
};

/** \brief Log4gConsoleAppenderClass definition */
struct _Log4gConsoleAppenderClass {
    Log4gWriterAppenderClass parent_class;
};

GType
log4g_console_appender_get_type(void);

void
log4g_console_appender_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_CONSOLE_APPENDER_H */