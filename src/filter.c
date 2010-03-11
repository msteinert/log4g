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
 * \brief Implements the API in log4g/filter.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include "log4g/filter.h"
#include "log4g/helpers/pattern-converter.h"
#include "log4g/interface/option-handler.h"

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = activate_options;
}

G_DEFINE_TYPE_WITH_CODE(Log4gFilter, log4g_filter, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_FILTER, \
                                 struct Log4gPrivate))

struct Log4gPrivate {
    Log4gFilter *next;
};

static void
log4g_filter_init(Log4gFilter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->next = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->next) {
        g_object_unref(priv->next);
        priv->next = NULL;
    }
    G_OBJECT_CLASS(log4g_filter_parent_class)->finalize(base);
}

static void
log4g_filter_class_init(Log4gFilterClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gFilter class */
    klass->decide = NULL;
    /* initialize class data */
    klass->ACCEPT = 1;
    klass->NEUTRAL = 0;
    klass->DENY = -1;
}

void
log4g_filter_activate_options(Log4gFilter *self)
{
    Log4gOptionHandlerInterface *interface;
    g_return_if_fail(LOG4G_IS_FILTER(self));
    interface = LOG4G_OPTION_HANDLER_GET_INTERFACE(self);
    interface->activate_options(LOG4G_OPTION_HANDLER(self));
}

gint
log4g_filter_decide(Log4gFilter *self, Log4gLoggingEvent *event)
{
    g_return_val_if_fail(LOG4G_IS_FILTER(self), 0);
    return LOG4G_FILTER_GET_CLASS(self)->decide(self, event);
}

Log4gFilter *
log4g_filter_get_next(Log4gFilter *self)
{
    return GET_PRIVATE(self)->next;
}

void
log4g_filter_set_next(Log4gFilter *self, Log4gFilter *next)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->next) {
        g_object_unref(priv->next);
    }
    g_object_ref(next);
    priv->next = next;
}
