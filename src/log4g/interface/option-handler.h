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
 * \date 2-5-2010
 */

#ifndef LOG4G_OPTION_HANDLER_H
#define LOG4G_OPTION_HANDLER_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_OPTION_HANDLER \
    (log4g_option_handler_get_type())

#define LOG4G_OPTION_HANDLER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_OPTION_HANDLER, \
            Log4gOptionHandler))

#define LOG4G_IS_OPTION_HANDLER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_OPTION_HANDLER))

#define LOG4G_OPTION_HANDLER_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_OPTION_HANDLER, \
            Log4gOptionHandlerInterface));

/** \brief Log4gOptionHandler object type definition */
typedef struct _Log4gOptionHandler Log4gOptionHandler;

/** \brief Log4gOptionHandler class type definition */
typedef struct _Log4gOptionHandlerInterface Log4gOptionHandlerInterface;

/** \brief Log4gOptionHandlerClass definition */
struct _Log4gOptionHandlerInterface {
    GTypeInterface parent_interface; /**< parent interface */
    void (*activate_options)(Log4gOptionHandler *self);
};

GType
log4g_option_handler_get_type(void);

/**
 */
void
log4g_option_handler_activate_options(Log4gOptionHandler *self);

G_END_DECLS

#endif /* LOG4G_OPTION_HANDLER_H */
