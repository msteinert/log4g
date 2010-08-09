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
 * SECTION: appender-attachable
 * @short_description: attach appenders to objects
 * @see_also: #Log4gAppenderClass
 *
 * This interface defines a standard API for attaching appenders to other
 * objects (e.g. loggers, other appenders).
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/appender-attachable.h"

G_DEFINE_INTERFACE(Log4gAppenderAttachable, log4g_appender_attachable,
        G_TYPE_INVALID)

static void
log4g_appender_attachable_default_init(Log4gAppenderAttachableInterface *klass)
{
    /* do nothing */
}

/**
 * log4g_appender_attachable_add_appender:
 * @self: An appender attachable object.
 * @appender: An appender.
 *
 * Call the @add_appender function from the #Log4gAppenderAttachableInterface
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_attachable_add_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->add_appender(self, appender);
}

/**
 * log4g_appender_attachable_get_all_appenders:
 * @self: An appender attachable object.
 *
 * Call the @get_all_appenders function from the
 * #Log4gAppenderAttachableInterface of @self.
 *
 * Returns: An array of appenders attached to @self.
 * Since: 0.1
 */
const GArray *
log4g_appender_attachable_get_all_appenders(Log4gAppenderAttachable *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), NULL);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->get_all_appenders(self);
}

/**
 * log4g_appender_attachable_get_appender:
 * @self: An appender attachable object.
 * @name: The name of the appender to retrieve.
 *
 * Call the @get_appender function from the #Log4gAppenderAttachableInterface
 * of @self.
 *
 * Returns: The appender named @name, or %NULL if not found.
 * Since: 0.1
 */
Log4gAppender *
log4g_appender_attachable_get_appender(Log4gAppenderAttachable *self,
        const gchar *name)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), NULL);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->get_appender(self, name);
}

/**
 * log4g_appender_attachable_is_attached:
 * @self: An appender attachable object.
 * @appender: The appender to check.
 *
 * Call the @is_attached function from the #Log4gAppenderAttachableInterface
 * of @self.
 *
 * Since: 0.1
 */
gboolean
log4g_appender_attachable_is_attached(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), FALSE);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->is_attached(self, appender);
}

/**
 * log4g_appender_attachable_remove_all_appenders:
 * @self: An appender attachable object.
 *
 * Call the @remove_all_appenders function from the
 * #Log4gAppenderAttachableInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_attachable_remove_all_appenders(Log4gAppenderAttachable *self)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_all_appenders(self);
}

/**
 * log4g_appender_attachable_remove_appender:
 * @self: An appender attachable object.
 * @appender: The appender to remove.
 *
 * Call the @remove_appender function from the
 * #Log4gAppenderAttachableInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_attachable_remove_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_appender(self, appender);
}

/**
 * log4g_appender_attachable_remove_appender_name:
 * @self: An appender attachable object.
 * @name: The name of the appender to remove.
 *
 * Call the @remove_appender_name function from the
 * #Log4gAppenderAttachableInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_appender_attachable_remove_appender_name(Log4gAppenderAttachable *self,
        const gchar *name)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_appender_name(self, name);
}
