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
 * \brief Implements the API in log4g/interface/error-handler.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/error-handler.h"

G_DEFINE_INTERFACE(Log4gErrorHandler, log4g_error_handler, 0)

static void
log4g_error_handler_default_init(Log4gErrorHandlerInterface *klass)
{
    /* do nothing */
}

void
log4g_error_handler_set_logger(Log4gErrorHandler *self,
        Log4gLogger *logger)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_logger(self, logger);
}

void
log4g_error_handler_error(Log4gErrorHandler *self, Log4gLoggingEvent *event,
        const char *message, ...)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    va_list ap;
    va_start(ap, message);
    interface->error(self, event, message, ap);
    va_end(ap);
}

void
log4g_error_handler_set_appender(Log4gErrorHandler *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_appender(self, appender);
}

void
log4g_error_handler_set_backup_appender(Log4gErrorHandler *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_appender(self, appender);
}