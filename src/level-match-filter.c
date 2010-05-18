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
 * \brief Implements the API in log4g/filter/level-match-filter.h
 * \author Mike Steinert
 * \date 2-11-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/filter/level-match-filter.h"

enum _properties_t {
    PROP_O = 0,
    PROP_LEVEL_TO_MATCH,
    PROP_ACCEPT_ON_MATCH,
    PROP_MAX
};

G_DEFINE_TYPE(Log4gLevelMatchFilter, log4g_level_match_filter,
        LOG4G_TYPE_FILTER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL_MATCH_FILTER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean accept;
    Log4gLevel *level;
};

static void
log4g_level_match_filter_init(Log4gLevelMatchFilter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->accept = TRUE;
    priv->level = NULL;
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->level) {
        g_object_unref(priv->level);
        priv->level = NULL;
    }
    G_OBJECT_CLASS(log4g_level_match_filter_parent_class)->dispose(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *level;
    switch (id) {
    case PROP_LEVEL_TO_MATCH:
        if (priv->level) {
            g_object_unref(priv->level);
        }
        level = g_value_get_string(value);
        if (level) {
            priv->level = log4g_level_string_to_level(level);
            if (priv->level) {
                g_object_ref(priv->level);
            }
        } else {
            priv->level = NULL;
        }
        break;
    case PROP_ACCEPT_ON_MATCH:
        priv->accept = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static Log4gFilterDecision
decide(Log4gFilter *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gLevel *level;
    if (!priv->level) { 
        return LOG4G_FILTER_NEUTRAL;
    }
    level = log4g_logging_event_get_level(event);
    if (log4g_level_equals(priv->level, level)) {
        return LOG4G_FILTER_NEUTRAL;
    } else {
        return (priv->accept ? LOG4G_FILTER_ACCEPT : LOG4G_FILTER_DENY);
    }
}

static void
log4g_level_match_filter_class_init(Log4gLevelMatchFilterClass *klass)
{
    Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gFilter class */
    filter_class->decide = decide;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_LEVEL_TO_MATCH,
            g_param_spec_string("level-to-match", Q_("Level to Match"),
                    Q_("Log level to match"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_ACCEPT_ON_MATCH,
            g_param_spec_boolean("accept-on-match", Q_("Accept on Match"),
                    Q_("Accept or deny on log level match"),
                    TRUE, G_PARAM_WRITABLE));
}

Log4gFilter *
log4g_level_match_filter_new(void)
{
    return g_object_new(LOG4G_TYPE_LEVEL_MATCH_FILTER, NULL);
}

void
log4g_level_match_filter_set_level_to_match(Log4gFilter *base,
        const gchar *level)
{
    g_return_if_fail(LOG4G_IS_LEVEL_MATCH_FILTER(base));
    g_object_set(base, "level-to-match", level, NULL);
}

Log4gLevel *
log4g_level_match_filter_get_level_to_match(Log4gFilter *base)
{
    g_return_val_if_fail(LOG4G_IS_LEVEL_MATCH_FILTER(base), FALSE);
    return GET_PRIVATE(base)->level;
}

void
log4g_level_match_filter_set_accept_on_match(Log4gFilter *base,
        gboolean accept)
{
    g_return_if_fail(LOG4G_IS_LEVEL_MATCH_FILTER(base));
    g_object_set(base, "accept-on-match", accept, NULL);
}

gboolean
log4g_level_match_filter_get_accept_on_match(Log4gFilter *base)
{
    g_return_val_if_fail(LOG4G_IS_LEVEL_MATCH_FILTER(base), FALSE);
    return GET_PRIVATE(base)->accept;
}
