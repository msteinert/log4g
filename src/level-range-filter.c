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
 * \brief Implements the API in log4g/filter/level-range-filter.h
 * \author Mike Steinert
 * \date 2-11-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/filter/level-range-filter.h"

enum _properties_t {
    PROP_O = 0,
    PROP_LEVEL_MIN,
    PROP_LEVEL_MAX,
    PROP_ACCEPT_ON_RANGE,
    PROP_MAX
};

G_DEFINE_TYPE(Log4gLevelRangeFilter, log4g_level_range_filter,
        LOG4G_TYPE_FILTER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL_RANGE_FILTER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean accept;
    Log4gLevel *min;
    Log4gLevel *max;
};

static void
log4g_level_range_filter_init(Log4gLevelRangeFilter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->accept = TRUE;
    priv->min = NULL;
    priv->max = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->min) {
        g_object_unref(priv->min);
        priv->min = NULL;
    }
    if (priv->max) {
        g_object_unref(priv->max);
        priv->max = NULL;
    }
    G_OBJECT_CLASS(log4g_level_range_filter_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *level;
    switch (id) {
    case PROP_LEVEL_MIN:
        if (priv->min) {
            g_object_unref(priv->min);
        }
        level = g_value_get_string(value);
        if (level) {
            priv->min = log4g_level_string_to_level(level);
            if (priv->min) {
                g_object_ref(priv->min);
            }
        } else {
            priv->min = NULL;
        }
        break;
    case PROP_LEVEL_MAX:
        if (priv->max) {
            g_object_unref(priv->max);
        }
        level = g_value_get_string(value);
        if (level) {
            priv->max = log4g_level_string_to_level(level);
            if (priv->max) {
                g_object_ref(priv->max);
            }
        } else {
            priv->max = NULL;
        }
        break;
    case PROP_ACCEPT_ON_RANGE:
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
    Log4gLevel *level = log4g_logging_event_get_level(event);
    if (priv->min) { 
        if (!log4g_level_is_greater_or_equal(level, priv->min)) {
            return LOG4G_FILTER_DENY;
        }
    }
    if (priv->max) {
        if (log4g_level_to_int(level) > log4g_level_to_int(priv->max)) {
            return LOG4G_FILTER_DENY;
        }
    }
    return (priv->accept ? LOG4G_FILTER_ACCEPT : LOG4G_FILTER_NEUTRAL);
}

static void
log4g_level_range_filter_class_init(Log4gLevelRangeFilterClass *klass)
{
    Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gFilter class */
    filter_class->decide = decide;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_LEVEL_MIN,
            g_param_spec_string("level-min", Q_("Minimum Level"),
                    Q_("Minimum log level to match"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_LEVEL_MAX,
            g_param_spec_string("level-max", Q_("Maximum Level"),
                    Q_("Maximum log level to match"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_ACCEPT_ON_RANGE,
            g_param_spec_boolean("accept-on-range", Q_("Accept on Range"),
                    Q_("Accept or deny on log level range match"),
                    TRUE, G_PARAM_WRITABLE));
}

Log4gFilter *
log4g_level_range_filter_new(void)
{
    return g_object_new(LOG4G_TYPE_LEVEL_RANGE_FILTER, NULL);
}

void
log4g_level_range_filter_set_level_min(Log4gFilter *base, const gchar *level)
{
    g_return_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base));
    g_object_set(base, "level-min", level, NULL);
}

Log4gLevel *
log4g_level_range_filter_get_level_min(Log4gFilter *base)
{
    g_return_val_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base), NULL);
    return GET_PRIVATE(base)->min;
}

void
log4g_level_range_filter_set_level_max(Log4gFilter *base, const gchar *level)
{
    g_return_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base));
    g_object_set(base, "level-max", level, NULL);
}

Log4gLevel *
log4g_level_range_filter_get_level_max(Log4gFilter *base)
{
    g_return_val_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base), NULL);
    return GET_PRIVATE(base)->max;
}

void
log4g_level_range_filter_set_accept_on_range(Log4gFilter *base,
        gboolean accept)
{
    g_return_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base));
    g_object_set(base, "accept-on-range", accept, NULL);
}

gboolean
log4g_level_range_filter_get_accept_on_range(Log4gFilter *base)
{
    g_return_val_if_fail(LOG4G_IS_LEVEL_RANGE_FILTER(base), FALSE);
    return GET_PRIVATE(base)->accept;
}
