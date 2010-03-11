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
 * \brief Implements the API in log4g/appender.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include "log4g/interface/appender.h"
#include "log4g/interface/error-handler.h"
#include "log4g/interface/option-handler.h"

G_DEFINE_INTERFACE(Log4gAppender, log4g_appender, LOG4G_TYPE_OPTION_HANDLER)

static void
log4g_appender_default_init(Log4gAppenderInterface *klass)
{
    /* do nothing */
}

void
log4g_appender_add_filter(Log4gAppender *self, Log4gFilter *filter)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->add_filter(self, filter);
}

Log4gFilter *
log4g_appender_get_filter(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    return interface->get_filter(self);
}

void
log4g_appender_close(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->close(self);
}

void
log4g_appender_do_append(Log4gAppender *self, Log4gLoggingEvent *event)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->do_append(self, event);
}

const gchar *
log4g_appender_get_name(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    return interface->get_name(self);
}

void
log4g_appender_set_error_handler(Log4gAppender *self, gpointer handler)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    g_return_if_fail(LOG4G_IS_ERROR_HANDLER(handler));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->set_error_handler(self, handler);
}

gpointer
log4g_appender_get_error_handler(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    return interface->get_error_handler(self);
}

void
log4g_appender_set_layout(Log4gAppender *self, Log4gLayout *layout)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->set_layout(self, layout);
}

Log4gLayout *
log4g_appender_get_layout(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), NULL);
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    return interface->get_layout(self);
}

void
log4g_appender_set_name(Log4gAppender *self, const gchar *name)
{
    Log4gAppenderInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    interface->set_name(self, name);
}

gboolean
log4g_appender_requires_layout(Log4gAppender *self)
{
    Log4gAppenderInterface *interface;
    g_return_val_if_fail(LOG4G_IS_APPENDER(self), FALSE);
    interface = LOG4G_APPENDER_GET_INTERFACE(self);
    return interface->requires_layout(self);
}

void
log4g_appender_activate_options(Log4gAppender *self)
{
    Log4gOptionHandlerInterface *interface;
    g_return_if_fail(LOG4G_IS_APPENDER(self));
    interface = LOG4G_OPTION_HANDLER_GET_INTERFACE(self);
    interface->activate_options(LOG4G_OPTION_HANDLER(self));
}
