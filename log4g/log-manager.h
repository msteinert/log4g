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

#ifndef LOG4G_LOG_MANAGER_H
#define LOG4G_LOG_MANAGER_H

#include <log4g/interface/repository-selector.h>
#include <log4g/interface/logger-factory.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOG_MANAGER \
    (log4g_log_manager_get_type())

#define LOG4G_LOG_MANAGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManager))

#define LOG4G_IS_LOG_MANAGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOG_MANAGER))

#define LOG4G_LOG_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManagerClass))

#define LOG4G_IS_LOG_MANAGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOG_MANAGER))

#define LOG4G_LOG_MANAGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOG_MANAGER, \
            Log4gLogManagerClass))

typedef struct _Log4gLogManager Log4gLogManager;

typedef struct _Log4gLogManagerClass Log4gLogManagerClass;

/**
 * Log4gLogManager:
 *
 * The <structname>Log4gLogManager</structname> structure does not have any
 * public members.
 */
struct _Log4gLogManager {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gLogManagerClass:
 *
 * The <structname>Log4gLogManagerClass</structname> structure does not have
 * any public members.
 */
struct _Log4gLogManagerClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_log_manager_get_type(void) G_GNUC_CONST;

void
log4g_log_manager_remove_instance(void);

void
log4g_log_manager_set_repository_selector(Log4gRepositorySelector *selector,
        GObject *guard);

Log4gLoggerRepository *
log4g_log_manager_get_logger_repository(void);

Log4gLogger *
log4g_log_manager_get_root_logger(void);

Log4gLogger *
log4g_log_manager_get_logger(const gchar *name);

Log4gLogger *
log4g_log_manager_get_logger_factory(const gchar *name,
        Log4gLoggerFactory *factory);

Log4gLogger *
log4g_log_manager_exists(const gchar *name);

const GArray *
log4g_log_manager_get_current_loggers(void);

void
log4g_log_manager_shutdown(void);

void
log4g_log_manager_reset_configuration(void);

G_END_DECLS

#endif /* LOG4G_LOG_MANAGER_H */
