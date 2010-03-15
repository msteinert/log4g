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
 * \brief Emit an error only once.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * This class implements Log4g's default error handling policy. Only the first
 * error will be logged, following errors will be ignored.
 *
 * The error message is printed on stderr.
 *
 * This policy is intended to prevent a working application from being
 * flooded with errors if there is a failure within Log4g.
 *
 * \see stdio(3), log4g/interface/error-handler.h
 */

#ifndef LOG4G_ONLY_ONCE_ERROR_HANDLER_H
#define LOG4G_ONLY_ONCE_ERROR_HANDLER_H

#include "log4g/interface/error-handler.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER \
    (log4g_only_once_error_handler_get_type())

#define LOG4G_ONLY_ONCE_ERROR_HANDLER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), \
            LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER, Log4gOnlyOnceErrorHandler))

#define LOG4G_IS_ONLY_ONCE_ERROR_HANDLER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), \
            LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER))

#define LOG4G_ONLY_ONCE_ERROR_HANDLER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER, \
            Log4gOnlyOnceErrorHandlerClass))

#define LOG4G_IS_ONLY_ONCE_ERROR_HANDLER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER))

#define LOG4G_ONLY_ONCE_ERROR_HANDLER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), \
            LOG4G_TYPE_ONLY_ONCE_ERROR_HANDLER, \
            Log4gOnlyOnceErrorHandlerClass))

/** \brief Log4gOnlyOnceErrorHandler object type definition */
typedef struct _Log4gOnlyOnceErrorHandler Log4gOnlyOnceErrorHandler;

/** \brief Log4gOnlyOnceErrorHandler class type definition */
typedef struct _Log4gOnlyOnceErrorHandlerClass Log4gOnlyOnceErrorHandlerClass;

/** \brief Log4gOnlyOnceErrorHandlerClass definition */
struct _Log4gOnlyOnceErrorHandler {
    GObject parent_instance;
};

/** \brief Log4gOnlyOnceErrorHandlerClass definition */
struct _Log4gOnlyOnceErrorHandlerClass {
    GObjectClass parent_class;
};

GType
log4g_only_once_error_handler_get_type(void);

/**
 * \brief Create a new only once error handler object.
 *
 * \return A new only once error handler object.
 */
Log4gErrorHandler *
log4g_only_once_error_handler_new(void);

G_END_DECLS

#endif /* LOG4G_ONLY_ONCE_ERROR_HANDLER_H */
