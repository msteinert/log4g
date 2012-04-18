/* Copyright 2010, 2011 Michael Steinert
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
 * SECTION: level-range-filter
 * @short_description: A filter based on level ranges
 * @see_also: #Log4gLevelClass
 *
 * This is a simple filter which can reject message with levels outside a
 * specified range.
 *
 * This filter accept three properties:
 * <orderedlist>
 * <listitem><para>level-min</para></listitem>
 * <listitem><para>level-max</para></listitem>
 * <listitem><para>accept-on-range</para></listitem>
 * </orderedlist>
 *
 * If the level of the logging event is not between level-min and level-max
 * (inclusive) then the decide function returns deny.
 *
 * If the logging logging event is within the specified range and
 * accept-on-range is %TRUE then the decide function returns accept. If
 * accept-on-range is set to %FALSE then decide will return neutral in this
 * case.
 *
 * The default value for accept-on-range is %TRUE.
 *
 * If level-min is not defined then there is no minimum level (a level is
 * never rejected for being too low). If level-max is not defined then
 * there is no maximum level (a level is never rejected for being too high).
 *
 * Refer to log4g_appender_set_threshold() for a more convenient way to filter
 * out log events by level.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/level-range-filter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gLevelRangeFilter, log4g_level_range_filter,
		LOG4G_TYPE_FILTER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL_RANGE_FILTER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gLevelRangeFilter *)instance)->priv)

struct Private {
	gboolean accept;
	Log4gLevel *min;
	Log4gLevel *max;
};

static void
log4g_level_range_filter_init(Log4gLevelRangeFilter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->accept = TRUE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->min) {
		g_object_unref(priv->min);
		priv->min = NULL;
	}
	if (priv->max) {
		g_object_unref(priv->max);
		priv->max = NULL;
	}
	G_OBJECT_CLASS(log4g_level_range_filter_parent_class)->dispose(base);
}

enum Properties {
	PROP_O = 0,
	PROP_LEVEL_MIN,
	PROP_LEVEL_MAX,
	PROP_ACCEPT_ON_RANGE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	const gchar *level;
	switch (id) {
	case PROP_LEVEL_MIN:
		if (priv->min) {
			g_object_unref(priv->min);
			priv->min = NULL;
		}
		level = g_value_get_string(value);
		if (level) {
			priv->min = log4g_level_string_to_level(level);
			if (priv->min) {
				g_object_ref(priv->min);
			}
		}
		break;
	case PROP_LEVEL_MAX:
		if (priv->max) {
			g_object_unref(priv->max);
			priv->max = NULL;
		}
		level = g_value_get_string(value);
		if (level) {
			priv->max = log4g_level_string_to_level(level);
			if (priv->max) {
				g_object_ref(priv->max);
			}
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
	struct Private *priv = GET_PRIVATE(base);
	Log4gLevel *level = log4g_logging_event_get_level(event);
	if (priv->min) { 
		if (log4g_level_to_int(level) < log4g_level_to_int(priv->min)) {
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
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	filter_class->decide = decide;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_LEVEL_MIN,
		g_param_spec_string("level-min", Q_("Minimum Level"),
			Q_("Minimum log level to match"), NULL,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LEVEL_MAX,
		g_param_spec_string("level-max", Q_("Maximum Level"),
			Q_("Maximum log level to match"), NULL,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_ACCEPT_ON_RANGE,
		g_param_spec_boolean("accept-on-range", Q_("Accept on Range"),
			Q_("Accept or deny on log level range match"),
			TRUE, G_PARAM_WRITABLE));
}

static void
log4g_level_range_filter_class_finalize(
		G_GNUC_UNUSED Log4gLevelRangeFilterClass *klass)
{
	/* do nothing */
}

void
log4g_level_range_filter_register(GTypeModule *module)
{
	log4g_level_range_filter_register_type(module);
}
