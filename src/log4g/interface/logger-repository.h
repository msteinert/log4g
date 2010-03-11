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

#ifndef LOG4G_LOGGER_REPOSITORY_H
#define LOG4G_LOGGER_REPOSITORY_H

#include <log4g/interface/logger-factory.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER_REPOSITORY \
    (log4g_logger_repository_get_type())

#define LOG4G_LOGGER_REPOSITORY(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER_REPOSITORY, \
            Log4gLoggerRepository))

#define LOG4G_IS_LOGGER_REPOSITORY(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER_REPOSITORY))

#define LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_LOGGER_REPOSITORY, \
            Log4gLoggerRepositoryInterface));

/** \brief Log4gLoggerRepository object type definition */
typedef struct _Log4gLoggerRepository Log4gLoggerRepository;

/** \brief Log4gLoggerRepository class type definition */
typedef struct _Log4gLoggerRepositoryInterface Log4gLoggerRepositoryInterface;

/** \brief Log4gLoggerRepositoryClass definition */
struct _Log4gLoggerRepositoryInterface {
    GTypeInterface parent_interface; /**< parent interface */
    Log4gLogger *(*exists)(Log4gLoggerRepository *self, const gchar *name);
    const GArray *(*get_current_loggers)(Log4gLoggerRepository *self);
    Log4gLogger *(*get_logger)(Log4gLoggerRepository *self, const gchar *name);
    Log4gLogger *(*get_logger_factory)(Log4gLoggerRepository *self,
            const gchar *name, Log4gLoggerFactory *factory);
    Log4gLogger *(*get_root_logger)(Log4gLoggerRepository *self);
    Log4gLevel *(*get_threshold)(Log4gLoggerRepository *self);
    gboolean (*is_disabled)(Log4gLoggerRepository *self, gint level);
    void (*reset_configuration)(Log4gLoggerRepository *self);
    void (*set_threshold)(Log4gLoggerRepository *self, Log4gLevel *level);
    void (*set_threshold_string)(Log4gLoggerRepository *self,
            const gchar *level);
    void (*shutdown)(Log4gLoggerRepository *self);
    void (*emit_no_appender_warning)(Log4gLoggerRepository *self,
            Log4gLogger *logger);
};

GType log4g_logger_repository_get_type(void);

/**
 */
Log4gLogger *
log4g_logger_repository_exists(Log4gLoggerRepository *self, const gchar *name);

/**
 */
void
log4g_logger_repository_emit_add_appender_signal(Log4gLoggerRepository *self,
        Log4gLogger *logger, Log4gAppender *appender);

/**
 */
void
log4g_logger_repository_emit_remove_appender_signal(
        Log4gLoggerRepository *self, Log4gLogger *logger,
        Log4gAppender *appender);

/**
 */
const GArray *
log4g_logger_repository_get_current_loggers(Log4gLoggerRepository *self);

/**
 */
Log4gLogger *
log4g_logger_repository_get_logger(Log4gLoggerRepository *self,
        const gchar *name);

Log4gLogger *
log4g_logger_repository_get_logger_factory(Log4gLoggerRepository *self,
        const gchar *name, Log4gLoggerFactory *factory);

/**
 */
Log4gLogger *
log4g_logger_repository_get_root_logger(Log4gLoggerRepository *self);

/**
 */
Log4gLevel *
log4g_logger_repository_get_threshold(Log4gLoggerRepository *self);

/**
 */
gboolean
log4g_logger_repository_is_disabled(Log4gLoggerRepository *self, gint level);

/**
 */
void
log4g_logger_repository_reset_configuration(Log4gLoggerRepository *self);

/**
 */
void
log4g_logger_repository_set_threshold(Log4gLoggerRepository *self,
        Log4gLevel *level);

/**
 */
void
log4g_logger_repository_set_threshold_string(Log4gLoggerRepository *self,
        const gchar *string);

/**
 */
void
log4g_logger_repository_shutdown(Log4gLoggerRepository *self);

/**
 */
void
log4g_logger_repository_emit_no_appender_warning(Log4gLoggerRepository *self,
        Log4gLogger *logger);

G_END_DECLS

#endif /* LOG4G_LOGGER_REPOSITORY_H */
