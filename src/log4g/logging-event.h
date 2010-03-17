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
 * \brief The internal representation of logging events.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * When an affirmative decision is made to log an event then a logging event
 * instance is created. This instance is passed to appenders and filters to
 * perform actual logging.
 *
 * \note This class is only useful to those wishing to extend Log4g.
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
 * \brief Create a new logging event.
 *
 * \param logger [in] The name of the logger that is creating this event.
 * \param level [in] The log level of this event (log4g/level.h).
 * \param function [in] The function where this event was logged.
 * \param file [in] The file where this event was logged.
 * \param line [in] The line in \e file where this event was logged.
 * \param message [in] A printf formatted log message.
 * \param ap [in] Format parameters.
 *
 * \return A new logging event object.
 */
Log4gLoggingEvent *
log4g_logging_event_new(const gchar *logger, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *message, va_list ap);

/**
 * \brief Retrieve the log level of a logging event.
 *
 * \param self [in] A logging event object.
 *
 * \return The log level of \e self.
 *
 * \see log4g/level.h
 */
Log4gLevel *
log4g_logging_event_get_level(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the name of the logger that created a logging event.
 *
 * \param self [in] A logging event object.
 *
 * \return The name of the logger that created \e self.
 */
const gchar *
log4g_logging_event_get_logger_name(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the rendered logging message.
 *
 * \param self [in] A logging event object.
 *
 * \return The rendered logging message.
 *
 * \see log4g_logging_event_get_message()
 */
const gchar *
log4g_logging_event_get_rendered_message(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the log message.
 *
 * This function is equivalent to
 * \e log4g_logging_event_get_rendered_message().
 *
 * \param self [in] A logging event object.
 *
 * \return The log message.
 */
const gchar *
log4g_logging_event_get_message(Log4gLoggingEvent *self);

/**
 * \brief Retrieve a mapped data context value for a logging event.
 *
 * \param self [in] A logging event object.
 * \param key [in] A mapped data context key.
 *
 * \return The MDC value for \e key.
 *
 * \see log4g/mdc.h
 */
const gchar *
log4g_logging_event_get_mdc(Log4gLoggingEvent *self, const gchar *key);

/**
 * \brief Retrieve the timestamp of a logging event.
 *
 * \param self [in] A logging event object.
 *
 * \return The timestamp of \e self.
 */
struct timeval *
log4g_logging_event_get_time_stamp(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the name of the thread where a logging event was logged.
 *
 * \param self [in] A logging event object.
 *
 * \return The name of the thread where \e self was logged.
 */
const gchar *
log4g_logging_event_get_thread_name(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the nested data context for a logging event.
 *
 * \param self [in] A logging event object.
 *
 * \return The rendered NDC string for \e self.
 *
 * \see log4g/ndc.h
 */
const gchar *
log4g_logging_event_get_ndc(Log4gLoggingEvent *self);

/**
 * \param self [in] A logging event object.
 */
const GArray *
log4g_logging_event_get_property_key_set(Log4gLoggingEvent *self);

/**
 * \brief Copy the current thread name into a logging object.
 *
 * Asynchronous appenders should call this function.
 *
 * \param self [in] A logging event object.
 *
 * \see log4g/helpers/thread.h
 */
void
log4g_logging_event_get_thread_copy(Log4gLoggingEvent *self);

/**
 * \brief Copy the current mapped data context into a logging event.
 *
 * Asynchronous appenders should call this function.
 *
 * \param self [in] A logging event object.
 *
 * \see log4g/mdc.h
 */
void
log4g_logging_event_get_mdc_copy(Log4gLoggingEvent *self);

/**
 * \brief Copy the current nested data context into a logging event.
 *
 * Asynchronous appenders should call this function.
 *
 * \param self [in] A logging event object.
 *
 * \see log4g/ndc.h
 */
void
log4g_logging_event_get_ndc_copy(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the function where a logging event was logged.
 *
 * \param self [in] A logging event object.
 *
 * \return The function where \e self was logged.
 */
const gchar *
log4g_logging_event_get_function_name(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the file where a logging event was logged.
 *
 * \param self [in] A logging event object.
 *
 * \return The file where \e self was logged.
 */
const gchar *
log4g_logging_event_get_file_name(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the line number where a logging event was logged.
 *
 * \param self [in] A logging event object.
 *
 * \return The line number where \e self was logged.
 */
const gchar *
log4g_logging_event_get_line_number(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the full location information where a logging event was
 *        logged.
 *
 * The full location information is in the format:
 *
 * \code
 * function(file:line)
 * \endcode
 *
 * \param self [in] A logging event object.
 *
 * \return The full log location information for \e self.
 */
const gchar *
log4g_logging_event_get_full_info(Log4gLoggingEvent *self);

/**
 * \brief Retrieve the time when the log system was initialized.
 *
 * \param self [in] A logging event object.
 *
 * \return The number of seconds elapsed since the Unix epoch when the log
 *         system was initialized
 */
glong
log4g_logging_event_get_start_time(void);

G_END_DECLS

#endif /* LOG4G_LOGGING_EVENT_H */
