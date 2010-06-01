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
 * \brief Implements the API in log4g/interface/appender-attachable.h
 * \author Mike Steinert
 * \date 1-29-2010
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

void
log4g_appender_attachable_add_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->add_appender(self, appender);
}

const GArray *
log4g_appender_attachable_get_all_appenders(Log4gAppenderAttachable *self)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), NULL);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->get_all_appenders(self);
}

Log4gAppender *
log4g_appender_attachable_get_appender(Log4gAppenderAttachable *self,
        const gchar *name)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), NULL);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->get_appender(self, name);
}

gboolean
log4g_appender_attachable_is_attached(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self), FALSE);
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    return interface->is_attached(self, appender);
}

void
log4g_appender_attachable_remove_all_appenders(Log4gAppenderAttachable *self)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_all_appenders(self);
}

void
log4g_appender_attachable_remove_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_appender(self, appender);
}

void
log4g_appender_attachable_remove_appender_name(Log4gAppenderAttachable *self,
        const gchar *name)
{
    g_return_if_fail(LOG4G_IS_APPENDER_ATTACHABLE(self));
    Log4gAppenderAttachableInterface *interface =
        LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(self);
    interface->remove_appender_name(self, name);
}
