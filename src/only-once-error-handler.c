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
 * \brief Implements the API in log4g/helpers/only-once-error-handler.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/only-once-error-handler.h"
#include "log4g/interface/option-handler.h"

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER, struct Log4gPrivate))

struct Log4gPrivate {
    gboolean first;
};

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = activate_options;
}

static void
set_logger(Log4gErrorHandler *base, Log4gLogger *logger)
{
    /* do nothing */
}

static void
error(Log4gErrorHandler *base, Log4gLoggingEvent *event,
        const char *message, va_list ap)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->first) {
        log4g_log_errorv(message, ap);
        priv->first = FALSE;
    }
}

static void
set_appender(Log4gErrorHandler *base, Log4gAppender *appender)
{
    /* do nothing */
}

static void
set_backup_appender(Log4gErrorHandler *base, Log4gAppender *appender)
{
    /* do nothing */
}
    
static void
error_handler_init(Log4gErrorHandlerInterface *interface, gpointer data)
{
    interface->set_logger = set_logger;
    interface->error = error;
    interface->set_appender = set_appender;
    interface->set_backup_appender = set_backup_appender;
}

G_DEFINE_TYPE_WITH_CODE(Log4gOnlyOnceErrorHandler,
        log4g_only_once_error_handler, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init)
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_ERROR_HANDLER, error_handler_init))

static void
log4g_only_once_error_handler_init(Log4gOnlyOnceErrorHandler *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->first = TRUE;
}

static void
log4g_only_once_error_handler_class_init(Log4gOnlyOnceErrorHandlerClass *klass)
{
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gErrorHandler *
log4g_only_once_error_handler_new(void)
{
    return g_object_new(LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER, NULL);
}
