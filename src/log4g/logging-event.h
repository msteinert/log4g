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

#ifndef LOG4G_LOGGING_EVENT_H
#define LOG4G_LOGGING_EVENT_H

#include <log4g/level.h>
#include <sys/time.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGING_EVENT \
    (log4g_logging_event_get_type())

#define LOG4G_LOGGING_EVENT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGING_EVENT, \
            Log4gLoggingEvent))

#define LOG4G_IS_LOGGING_EVENT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGING_EVENT))

#define LOG4G_LOGGING_EVENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOGGING_EVENT, \
            Log4gLoggingEventClass))

#define LOG4G_IS_LOGGING_EVENT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOGGING_EVENT))

#define LOG4G_LOGGING_EVENT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOGGING_EVENT, \
            Log4gLoggingEventClass))

/** \brief Log4gLoggingEvent object type definition */
typedef struct _Log4gLoggingEvent Log4gLoggingEvent;

/** \brief Log4gLoggingEvent class type definition */
typedef struct _Log4gLoggingEventClass Log4gLoggingEventClass;

/** \brief Log4gLoggingEventClass definition */
struct _Log4gLoggingEvent {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gLoggingEventClass definition */
struct _Log4gLoggingEventClass {
    GObjectClass parent_class; /**< parent class */
    Log4gLevel *(*get_level)(Log4gLoggingEvent *self);
    /*< private >*/
    glong start;
};

GType
log4g_logging_event_get_type(void);

/**
 */
Log4gLoggingEvent *
log4g_logging_event_new(const gchar *logger, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *message, va_list ap);

/**
 */
Log4gLevel *
log4g_logging_event_get_level(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_logger_name(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_rendered_message(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_message(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_mdc(Log4gLoggingEvent *self, const gchar *key);

/**
 */
struct timeval *
log4g_logging_event_get_time_stamp(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_thread_name(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_ndc(Log4gLoggingEvent *self);

/**
 */
const GArray *
log4g_logging_event_get_property_key_set(Log4gLoggingEvent *self);

/**
 */
void
log4g_logging_event_get_thread_copy(Log4gLoggingEvent *self);

/**
 */
void
log4g_logging_event_get_mdc_copy(Log4gLoggingEvent *self);

/**
 */
void
log4g_logging_event_get_ndc_copy(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_function_name(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_file_name(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_line_number(Log4gLoggingEvent *self);

/**
 */
const gchar *
log4g_logging_event_get_full_info(Log4gLoggingEvent *self);

/**
 */
glong
log4g_logging_event_get_start_time(void);

G_END_DECLS

#endif /* LOG4G_LOGGING_EVENT_H */
