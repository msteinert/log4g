/* Copyright 2010, 2011 Michael Steinert
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

#ifndef LOG4G_ERROR_HANDLER_H
#define LOG4G_ERROR_HANDLER_H

#include <log4g/logger.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_ERROR_HANDLER \
	(log4g_error_handler_get_type())

#define LOG4G_ERROR_HANDLER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_ERROR_HANDLER, \
		Log4gErrorHandler))

#define LOG4G_IS_ERROR_HANDLER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_ERROR_HANDLER))

#define LOG4G_ERROR_HANDLER_GET_INTERFACE(instance) \
	(G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_ERROR_HANDLER, \
		Log4gErrorHandlerInterface));

typedef struct Log4gErrorHandler_ Log4gErrorHandler;

typedef struct Log4gErrorHandlerInterface_ Log4gErrorHandlerInterface;

/**
 * Log4gErrorHandlerSetLogger:
 * @self: An error handler object.
 * @logger: The logger to set for @self.
 *
 * Add a reference to a logger to which the failing appender might be attached.
 *
 * The failing appender will be searched an replaced only in the loggers
 * you add through this method.
 *
 * Since: 0.1
 */
typedef void
(*Log4gErrorHandlerSetLogger)(Log4gErrorHandler *self, Log4gLogger *logger);

/**
 * Log4gErrorHandlerError:
 * @self: An error handler object.
 * @event: The event for which the error occurred.
 * @message: A printf formatted error message.
 * @ap: Format parameters.
 *
 * Invoke this function to handler the error.
 *
 * Since: 0.1
 */
typedef void
(*Log4gErrorHandlerError)(Log4gErrorHandler *self, Log4gLoggingEvent *event,
		const gchar *message, va_list ap);

/**
 * Log4gErrorHandlerSetAppender:
 * @self: An error handler object.
 * @appender: The appender to set for @self.
 *
 * Set the appender for which errors are handled.
 *
 * This function is usually called when the error handler is configured.
 *
 * Since: 0.1
 */
typedef void
(*Log4gErrorHandlerSetAppender)(Log4gErrorHandler *self,
		Log4gAppender *appender);

/**
 * Log4gErrorHandlerSetBackupAppender:
 * @self: An error handler object.
 * @appender: The backup appender to set for @self.
 *
 * Set the appender to fall back on in case of failure.
 *
 * Since: 0.1
 */
typedef void
(*Log4gErrorHandlerSetBackupAppender)(Log4gErrorHandler *self,
		Log4gAppender *appender);

/**
 * Log4gErrorHandlerInterface:
 * @set_logger: Add a reference to a logger with an attached that failed.
 * @error: Handle the error (See: #Log4gErrorHandlerError).
 * @set_appender: Set an appender to handle error messages.
 * @set_backup_appender: Set a backup appender in case of failure.
 */
struct Log4gErrorHandlerInterface_ {
	/*< private >*/
	GTypeInterface parent_interface;
	/*< public >*/
	Log4gErrorHandlerSetLogger set_logger;
	/* note: this definition is here because g-ir-scanner doesn't seem
	   to like the Log4gErrorHandlerError definition */
	void (*error)(Log4gErrorHandler *self, Log4gLoggingEvent *event,
			const gchar *message, va_list ap);
	Log4gErrorHandlerSetAppender set_appender;
	Log4gErrorHandlerSetBackupAppender set_backup_appender;
};

GType
log4g_error_handler_get_type(void) G_GNUC_CONST;

void
log4g_error_handler_set_logger(Log4gErrorHandler *self, Log4gLogger *logger);

void
log4g_error_handler_error(Log4gErrorHandler *self, Log4gLoggingEvent *event,
		const gchar *message, ...) G_GNUC_PRINTF(3, 4);

void
log4g_error_handler_set_appender(Log4gErrorHandler *self,
		Log4gAppender *appender);

void
log4g_error_handler_set_backup_appender(Log4gErrorHandler *self,
		Log4gAppender *appender);

G_END_DECLS

#endif /* LOG4G_ERROR_HANDLER_H */
