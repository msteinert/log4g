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
 * SECTION: error-handler
 * @short_description: Delegate error handling
 * @see_also: #Log4gAppenderClass
 *
 * Appenders may delegate their error handling via this interface.
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

/**
 * log4g_error_handler_set_logger:
 * @self: An error handler object.
 * @logger: The logger to set for @self.
 *
 * Call the @set_logger function for the #Log4gErrorHandlerInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_error_handler_set_logger(Log4gErrorHandler *self,
        Log4gLogger *logger)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_logger(self, logger);
}

/**
 * log4g_error_handler_error:
 * @self: An error handler object.
 * @event: The event for which the error occurred.
 * @message: A printf formatted error message.
 * @...: Format parameters.
 *
 * Call the @error function from the #Log4gErrorHandlerInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_error_handler_error(Log4gErrorHandler *self, Log4gLoggingEvent *event,
        const gchar *message, ...)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    va_list ap;
    va_start(ap, message);
    interface->error(self, event, message, ap);
    va_end(ap);
}

/**
 * log4g_error_handler_set_appender:
 * @self: An error handler object.
 * @appender: The appender to set for @self.
 *
 * Call the @set_appender function from the #Log4gErrorHandlerInterface
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_error_handler_set_appender(Log4gErrorHandler *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_appender(self, appender);
}

/**
 * log4g_error_handler_set_backup_appender:
 * @self: An error handler object.
 * @appender: The backup appender to set for @self.
 *
 * Call the @set_backup_appender function from the #Log4gErrorHandlerInterface
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_error_handler_set_backup_appender(Log4gErrorHandler *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(self));
    Log4gErrorHandlerInterface *interface =
        LOG4G_ERROR_HANDLER_GET_INTERFACE(self);
    interface->set_appender(self, appender);
}
