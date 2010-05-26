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
 * \brief Implements the API in log4g/layout/simple-layout.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "simple-layout.h"

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface)
{
    interface->activate_options = activate_options;
}

G_DEFINE_DYNAMIC_TYPE_EXTENDED(Log4gSimpleLayout, log4g_simple_layout,
        LOG4G_TYPE_LAYOUT, 0,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_SIMPLE_LAYOUT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    GString *string;
};

static void
log4g_simple_layout_init(Log4gSimpleLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->string = g_string_sized_new(128);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->string) {
        g_string_free(priv->string, TRUE);
    }
    G_OBJECT_CLASS(log4g_simple_layout_parent_class)->finalize(base);
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_string_set_size(priv->string, 0);
    Log4gLevel *level = log4g_logging_event_get_level(event);
    if (level) {
        g_string_append(priv->string, log4g_level_to_string(level));
    }
    g_string_append(priv->string, " - ");
    g_string_append(priv->string,
            log4g_logging_event_get_rendered_message(event));
    g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
    return priv->string->str;
}

static void
log4g_simple_layout_class_init(Log4gSimpleLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayout class */
    layout_class->format = format;
}

static void
log4g_simple_layout_class_finalize(Log4gSimpleLayoutClass *klass)
{
    /* do nothing */
}

void
log4g_simple_layout_register(GTypeModule *module)
{
    log4g_simple_layout_register_type(module);
}

Log4gLayout *
log4g_simple_layout_new(void)
{
    return g_object_new(LOG4G_TYPE_SIMPLE_LAYOUT, NULL);
}
