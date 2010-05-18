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
 * \brief Implements the API in log4g/logger-repository.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/logger-repository.h"
#include "support/marshal.h"

G_DEFINE_INTERFACE(Log4gLoggerRepository, log4g_logger_repository,
        G_TYPE_INVALID)

/** \brief Signals */
enum signals_t {
    SIGNAL_ADD_APPENDER, /**< Add an appender */
    SIGNAL_REMOVE_APPENDER, /**< Remove an appender */
    SIGNAL_LAST /**< Sentinel value */
};

/** \brief Signal definitions */
static guint signals[SIGNAL_LAST] = { 0 };

static void
log4g_logger_repository_default_init(Log4gLoggerRepositoryInterface *klass)
{
    /* install add-appender signal */
    signals[SIGNAL_ADD_APPENDER] =
        g_signal_new(Q_("add-appender"), G_OBJECT_CLASS_TYPE(klass),
                G_SIGNAL_RUN_FIRST | G_SIGNAL_DETAILED,
                0, NULL, NULL, g_cclosure_user_marshal_VOID__OBJECT_OBJECT,
                G_TYPE_NONE, 2, G_TYPE_OBJECT, G_TYPE_OBJECT);
    /* install remove-appender signal */
    signals[SIGNAL_REMOVE_APPENDER] =
        g_signal_new(Q_("remove-appender"), G_OBJECT_CLASS_TYPE(klass),
                G_SIGNAL_RUN_FIRST | G_SIGNAL_DETAILED,
                0, NULL, NULL, g_cclosure_user_marshal_VOID__OBJECT_OBJECT,
                G_TYPE_NONE, 2, G_TYPE_OBJECT, G_TYPE_OBJECT);
}

Log4gLogger *
log4g_logger_repository_exists(Log4gLoggerRepository *self, const gchar *name)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->exists(self, name);
}

const GArray *
log4g_logger_repository_get_current_loggers(Log4gLoggerRepository *self)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->get_current_loggers(self);
}

void
log4g_logger_repository_emit_add_appender_signal(Log4gLoggerRepository *self,
        Log4gLogger *logger, Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    g_signal_emit(self, signals[SIGNAL_ADD_APPENDER],
            g_quark_from_string(log4g_logger_get_name(logger)),
            logger, appender);
}

void
log4g_logger_repository_emit_remove_appender_signal(
        Log4gLoggerRepository *self, Log4gLogger *logger,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    g_signal_emit(self, signals[SIGNAL_REMOVE_APPENDER],
            g_quark_from_string(log4g_logger_get_name(logger)),
            logger, appender);
}

Log4gLogger *
log4g_logger_repository_get_logger(Log4gLoggerRepository *self,
        const gchar *name)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->get_logger(self, name);
}


Log4gLogger *
log4g_logger_repository_get_logger_factory(Log4gLoggerRepository *self,
        const gchar *name, Log4gLoggerFactory *factory)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->get_logger_factory(self, name, factory);
}

Log4gLogger *
log4g_logger_repository_get_root_logger(Log4gLoggerRepository *self)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->get_root_logger(self);
}

Log4gLevel *
log4g_logger_repository_get_threshold(Log4gLoggerRepository *self)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->get_threshold(self);
}

gboolean
log4g_logger_repository_is_disabled(Log4gLoggerRepository *self, gint level)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), FALSE);
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    return interface->is_disabled(self, level);
}

void
log4g_logger_repository_reset_configuration(Log4gLoggerRepository *self)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    interface->reset_configuration(self);
}

void
log4g_logger_repository_set_threshold(Log4gLoggerRepository *self,
        Log4gLevel *level)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    interface->set_threshold(self, level);
}

void
log4g_logger_repository_set_threshold_string(Log4gLoggerRepository *self,
        const gchar *string)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    interface->set_threshold_string(self, string);
}

void
log4g_logger_repository_shutdown(Log4gLoggerRepository *self)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    interface->shutdown(self);
}

void
log4g_logger_repository_emit_no_appender_warning(Log4gLoggerRepository *self,
        Log4gLogger *logger)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
    Log4gLoggerRepositoryInterface *interface =
        LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
    interface->emit_no_appender_warning(self, logger);
}
