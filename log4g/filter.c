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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/filter.h"

G_DEFINE_TYPE_EXTENDED(Log4gFilter, log4g_filter, G_TYPE_OBJECT,
        G_TYPE_FLAG_ABSTRACT, {})

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
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->next) {
        g_object_unref(priv->next);
        priv->next = NULL;
    }
    G_OBJECT_CLASS(log4g_filter_parent_class)->dispose(base);
}

static void
activate_options(Log4gFilter *base)
{
    /* do nothing */
}

static void
log4g_filter_class_init(Log4gFilterClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gFilter class */
    klass->decide = NULL;
    klass->activate_options = activate_options;
}

void
log4g_filter_activate_options(Log4gFilter *self)
{
    g_return_if_fail(LOG4G_IS_FILTER(self));
    LOG4G_FILTER_GET_CLASS(self)->activate_options(self);
}

Log4gFilterDecision
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
