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
 * SECTION: level-match-filter
 * @short_description: A filter based on level matching
 * @see_also: #Log4gLevelClass
 *
 * This is a simple filter based on level matching.
 *
 * This filter accepts two properties:
 * <orderedlist>
 * <listitem><para>level-to-match</para></listitem>
 * <listitem><para>accept-on-match</para></listitem>
 * </orderedlist>
 *
 * The level-to-match is a string that represents a log level. If there is
 * an exact match between the value of level-to-match and the level of the
 * logging event then the decide function returns accept.
 *
 * If the accept-on-match value is set to %FALSE then the decide function
 * will return deny when there is a level match. The default value is %TRUE.
 *
 * If there is no match then the decide function returns neutral.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/level-match-filter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gLevelMatchFilter, log4g_level_match_filter,
		LOG4G_TYPE_FILTER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL_MATCH_FILTER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gLevelMatchFilter *)instance)->priv)

struct Private {
	gboolean accept;
	Log4gLevel *level;
};

static void
log4g_level_match_filter_init(Log4gLevelMatchFilter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->accept = TRUE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->level) {
		g_object_unref(priv->level);
		priv->level = NULL;
	}
	G_OBJECT_CLASS(log4g_level_match_filter_parent_class)->dispose(base);
}

enum Properties {
	PROP_O = 0,
	PROP_LEVEL_TO_MATCH,
	PROP_ACCEPT_ON_MATCH,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
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
	struct Private *priv = GET_PRIVATE(base);
	Log4gLevel *level;
	if (!priv->level) {
		return LOG4G_FILTER_NEUTRAL;
	}
	level = log4g_logging_event_get_level(event);
	if (!log4g_level_equals(priv->level, level)) {
		return LOG4G_FILTER_NEUTRAL;
	} else {
		return (priv->accept ? LOG4G_FILTER_ACCEPT : LOG4G_FILTER_DENY);
	}
}

static void
log4g_level_match_filter_class_init(Log4gLevelMatchFilterClass *klass)
{
	Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->set_property = set_property;
	filter_class->decide = decide;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_LEVEL_TO_MATCH,
		g_param_spec_string("level-to-match", Q_("Level to Match"),
			Q_("Log level to match"), NULL, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_ACCEPT_ON_MATCH,
		g_param_spec_boolean("accept-on-match", Q_("Accept on Match"),
			Q_("Accept or deny on log level match"),
			TRUE, G_PARAM_WRITABLE));
}

static void
log4g_level_match_filter_class_finalize(
		G_GNUC_UNUSED Log4gLevelMatchFilterClass *klass)
{
	/* do nothing */
}

void
log4g_level_match_filter_register(GTypeModule *module)
{
	log4g_level_match_filter_register_type(module);
}
