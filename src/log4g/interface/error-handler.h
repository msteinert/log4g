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
 * \brief Delegate error handling.
 * \author Mike Steinert
 * \date 2-5-2010
 *
 * Appenders may delegate their error handling via this interface.
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

/** \brief Log4gErrorHandler object type definition */
typedef struct _Log4gErrorHandler Log4gErrorHandler;

/** \brief Log4gErrorHandler interface type definition */
typedef struct _Log4gErrorHandlerInterface
                    Log4gErrorHandlerInterface;

/** \brief Log4gErrorHandlerInterface definition */
struct _Log4gErrorHandlerInterface {
    GTypeInterface parent_interface;

    /**
     * \brief Add a reference to a logger to which the failing appender might
     *        be attached to.
     *
     * The failing appender will be searched an replaced only in the loggers
     * you add through this method.
     *
     * \param self [in] An error handler object.
     * \param logger [in] The logger to set for \e self.
     */
    void
    (*set_logger)(Log4gErrorHandler *self, Log4gLogger *logger);

    /**
     * \brief Invoke this function to handler the error.
     *
     * \param self [in] An error handler object.
     * \param event [in] The event for which the error occurred.
     * \param message [in] A printf formatted error message.
     * \param ... [in] Format parameters.
     */
    void
    (*error)(Log4gErrorHandler *self, Log4gLoggingEvent *event,
            const char *message, va_list ap);

    /**
     * \brief Set the appender for which errors are handled.
     *
     * This function is usually called when the error handler is configured.
     *
     * \param self [in] An error handler object.
     * \param appender [in] The appender to set for \e self.
     */
    void
    (*set_appender)(Log4gErrorHandler *self, Log4gAppender *appender);

    /**
     * \brief Set the appender to fall back on in case of failure.
     *
     * \param self [in] An error handler object.
     * \param appender [in] The backup appender to set for \e self.
     */
    void
    (*set_backup_appender)(Log4gErrorHandler *self, Log4gAppender *appender);
};

GType
log4g_error_handler_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gErrorHandlerInterface::set_logger().
 *
 * \param self [in] An error handler object.
 * \param logger [in] The logger to set for \e self.
 */
void
log4g_error_handler_set_logger(Log4gErrorHandler *self, Log4gLogger *logger);

/**
 * \brief Invokes the virtual function
 *        _Log4gErrorHandlerInterface::error().
 *
 * \param self [in] An error handler object.
 * \param event [in] The event for which the error occurred.
 * \param message [in] A printf formatted error message.
 * \param ... [in] Format parameters.
 */
void
log4g_error_handler_error(Log4gErrorHandler *self, Log4gLoggingEvent *event,
        const char *message, ...);

/**
 * \brief Invokes the virtual function
 *        _Log4gErrorHandlerInterface::set_appender().
 *
 * \param self [in] An error handler object.
 * \param appender [in] The appender to set for \e self.
 */
void
log4g_error_handler_set_appender(Log4gErrorHandler *self,
        Log4gAppender *appender);

/**
 * \brief Invokes the virtual function
 *        _Log4gErrorHandlerInterface::set_backup_appender().
 *
 * \param self [in] An error handler object.
 * \param appender [in] The backup appender to set for \e self.
 */
void
log4g_error_handler_set_backup_appender(Log4gErrorHandler *self,
        Log4gAppender *appender);

G_END_DECLS

#endif /* LOG4G_ERROR_HANDLER_H */
