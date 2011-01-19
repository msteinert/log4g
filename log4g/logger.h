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

#ifndef LOG4G_LOGGER_H
#define LOG4G_LOGGER_H

#include <log4g/appender.h>

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

typedef struct _Log4gLogger Log4gLogger;

typedef struct _Log4gLoggerClass Log4gLoggerClass;

/**
 * Log4gLogger:
 *
 * The <structname>Log4gLogger</structname> structure does not have any public
 * members.
 */
struct _Log4gLogger {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gLoggerGetEffectiveLevel:
 * @self: A #Log4gLogger object.
 *
 * Starting from this category, search the category hierarchy for a
 * non-%NULL and return it. If a non-%NULL level is not found, this
 * function returns the level of the root logger.
 *
 * Returns: The effective level threshold of @self.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLoggerGetEffectiveLevel)(Log4gLogger *self);

/**
 * Log4gLoggerSetLevel:
 * @self: A #Log4gLogger object.
 * @level: The new level threshold for @self. %NULL values are permitted.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLoggerSetLevel)(Log4gLogger *self, Log4gLevel *level);

/**
 * Log4gLoggerClass:
 * @get_effective_level: Retrieve the effective level threshold of the logger.
 * @set_level: Set the level threshold of the logger.
 */
struct _Log4gLoggerClass {
    /*< private >*/
    GObjectClass parent_class;
    /*< public >*/
    Log4gLoggerGetEffectiveLevel get_effective_level;
    Log4gLoggerSetLevel set_level;
};

GType
log4g_logger_get_type(void) G_GNUC_CONST;

Log4gLogger *
log4g_logger_new(const gchar *name);

const gchar *
log4g_logger_get_name(Log4gLogger *self);

void
log4g_logger_set_name(Log4gLogger *self, const gchar *name);

Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self);

void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent);

Log4gLevel *
log4g_logger_get_level(Log4gLogger *self);

void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level);

gboolean
log4g_logger_get_additivity(Log4gLogger *self);

void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive);

gpointer
log4g_logger_get_logger_repository(Log4gLogger *self);

void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository);

Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self);

void
log4g_logger_add_appender(Log4gLogger *self, Log4gAppender *appender);

const GArray *
log4g_logger_get_all_appenders(Log4gLogger *self);

Log4gAppender *
log4g_logger_get_appender(Log4gLogger *self, const gchar *name);

gboolean
log4g_logger_is_attached(Log4gLogger *self, Log4gAppender *appender);

void
log4g_logger_remove_all_appenders(Log4gLogger *self);

void
log4g_logger_remove_appender(Log4gLogger *self, Log4gAppender *appender);

void
log4g_logger_remove_appender_name(Log4gLogger *self, const gchar *name);

void
log4g_logger_close_nested_appenders(Log4gLogger *self);

void
log4g_logger_call_appenders(Log4gLogger *self, Log4gLoggingEvent *event);

void
_log4g_logger_assert(Log4gLogger *self, gboolean assertion,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, ...) G_GNUC_PRINTF(6, 7);

gboolean
log4g_logger_is_trace_enabled(Log4gLogger *self);

void
_log4g_logger_trace(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

gboolean
log4g_logger_is_debug_enabled(Log4gLogger *self);

void
_log4g_logger_debug(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

gboolean
log4g_logger_is_info_enabled(Log4gLogger *self);

void
_log4g_logger_info(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

gboolean
log4g_logger_is_warn_enabled(Log4gLogger *self);

void
_log4g_logger_warn(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

gboolean
log4g_logger_is_error_enabled(Log4gLogger *self);

void
_log4g_logger_error(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

gboolean
log4g_logger_is_fatal_enabled(Log4gLogger *self);

void
_log4g_logger_fatal(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

void
_log4g_logger_log(Log4gLogger *self, Log4gLevel *level, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(6, 7);

Log4gLogger *
log4g_logger_get_logger(const gchar *name);

Log4gLogger *
log4g_logger_get_root_logger(void);

Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory);

void
log4g_logger_forced_log(Log4gLogger *self, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, va_list ap);

G_END_DECLS

#endif /* LOG4G_LOGGER_H */
