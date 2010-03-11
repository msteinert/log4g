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
 * \brief Implements the API in log4g/layout.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#include "config.h"
#include "log4g/layout.h"
#include <string.h>

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = NULL;
}

G_DEFINE_TYPE_WITH_CODE(Log4gLayout, log4g_layout, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

static void
log4g_layout_init(Log4gLayout *self)
{
    /* do nothing */
}

static const gchar *
get_content_type(Log4gLayout *self)
{
    return "text/plain";
}

static const gchar *
get(Log4gLayout *self)
{
    return NULL;
}

static void
log4g_layout_class_init(Log4gLayoutClass *klass)
{
    /* initialize Log4gLayout class */
    klass->format = NULL;
    klass->get_content_type = get_content_type;
    klass->get_header = get;
    klass->get_footer = get;
    /* initialize class data */
    klass->LINE_SEP = g_strdup("\n");
    klass->LINE_SEP_LEN = strlen(klass->LINE_SEP);
}

gchar *
log4g_layout_format(Log4gLayout *self, Log4gLoggingEvent *event)
{
    g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
    return LOG4G_LAYOUT_GET_CLASS(self)->format(self, event);
}

const gchar *
log4g_layout_get_content_type(Log4gLayout *self)
{
    g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
    return LOG4G_LAYOUT_GET_CLASS(self)->get_content_type(self);
}

const gchar *
log4g_layout_get_header(Log4gLayout *self)
{
    g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
    return LOG4G_LAYOUT_GET_CLASS(self)->get_header(self);
}

const gchar *
log4g_layout_get_footer(Log4gLayout *self)
{
    g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
    return LOG4G_LAYOUT_GET_CLASS(self)->get_footer(self);
}

void
log4g_layout_activate_options(Log4gLayout *self)
{
    Log4gOptionHandlerInterface *interface;
    g_return_if_fail(LOG4G_IS_LAYOUT(self));
    interface = LOG4G_OPTION_HANDLER_GET_INTERFACE(self);
    interface->activate_options(LOG4G_OPTION_HANDLER(self));
}
