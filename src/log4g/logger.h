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

#ifndef LOG4G_LOGGER_H
#define LOG4G_LOGGER_H

#include <log4g/interface/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER \
    (log4g_logger_get_type())

#define LOG4G_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER, Log4gLogger))

#define LOG4G_IS_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOGGER, Log4gLoggerClass))

#define LOG4G_IS_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOGGER, \
            Log4gLoggerClass))

/** \brief Log4gLogger object type definition */
typedef struct _Log4gLogger Log4gLogger;

/** \brief Log4gLogger class type definition */
typedef struct _Log4gLoggerClass Log4gLoggerClass;

/** \brief Log4gLoggerClass definition */
struct _Log4gLogger {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gLoggerClass definition */
struct _Log4gLoggerClass {
    GObjectClass parent_class; /**< parent class */
    Log4gLevel *(*get_effective_level)(Log4gLogger *self);
    void (*set_level)(Log4gLogger *self, Log4gLevel *level);
};

GType
log4g_logger_get_type(void);

/**
 */
Log4gLogger *
log4g_logger_new(const gchar *name);

/**
 */
const gchar *
log4g_logger_get_name(Log4gLogger *self);

/**
 */
void
log4g_logger_set_name(Log4gLogger *self, const gchar *name);

/**
 */
Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self);

/**
 */
void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent);

/**
 */
Log4gLevel *
log4g_logger_get_level(Log4gLogger *self);

/**
 */
void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level);

/**
 */
gboolean
log4g_logger_get_additivity(Log4gLogger *self);

/**
 */
void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive);

/**
 */
gpointer
log4g_logger_get_logger_repository(Log4gLogger *self);

/**
 */
void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository);

/**
 */
Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self);

/**
 */
void
log4g_logger_add_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 */
const GArray *
log4g_logger_get_all_appenders(Log4gLogger *base);

/**
 */
Log4gAppender *
log4g_logger_get_appender(Log4gLogger *base, const gchar *name);

/**
 */
gboolean
log4g_logger_is_attached(Log4gLogger *base, Log4gAppender *appender);

/**
 */
void
log4g_logger_remove_all_appenders(Log4gLogger *base);

/**
 */
void
log4g_logger_remove_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 */
void
log4g_logger_remove_appender_name(Log4gLogger *base, const gchar *name);

/**
 */
void
log4g_logger_close_nested_appenders(Log4gLogger *self);

/**
 */
void
_log4g_logger_assert(Log4gLogger *self, gboolean assertion,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, ...) G_GNUC_PRINTF(6, 7);

/**
 */
gboolean log4g_logger_is_trace_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_trace(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 */
gboolean
log4g_logger_is_debug_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_debug(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 */
gboolean
log4g_logger_is_info_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_info(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 */
gboolean
log4g_logger_is_warn_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_warn(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 */
gboolean
log4g_logger_is_error_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_error(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 */
gboolean
log4g_logger_is_fatal_enabled(Log4gLogger *self);

/**
 */
void
_log4g_logger_fatal(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief This is the most generic logging method.
 * This method is intended for use by wrapper classes.
 * \param self [in] A Log4gLogger object.
 * \param level [in] The level of the logging request.
 * \param function [in] The function (__func__) where the event originated.
 * \param file [in] The file (__FILE__) where the event originated.
 * \param line [in] The line (__LINE__) where the event originated.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_log(Log4gLogger *self, Log4gLevel *level, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(6, 7);

/**
 */
Log4gLogger *
log4g_logger_get_logger(const gchar *name);

/**
 */
Log4gLogger *
log4g_logger_get_root_logger(void);

/**
 */
Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory);

/**
 */
void
log4g_logger_forced_log(Log4gLogger *self, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, va_list ap);

G_END_DECLS

#endif /* LOG4G_LOGGER_H */
