/* Copyright 2013 Michael Steinert
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
 * SECTION: regex-filter
 * @short_description: Filter based on a regular expression
 *
 * This filter allows the formatted or unformatted message to be compared
 * against a regular expression.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/regex-filter.h"
#include "log4g/enum-types.h"

G_DEFINE_DYNAMIC_TYPE(Log4gRegexFilter, log4g_regex_filter, LOG4G_TYPE_FILTER)

struct Private {
	GRegex *regex;
	Log4gFilterDecision on_match;
	Log4gFilterDecision on_mismatch;
};

static void
log4g_regex_filter_init(Log4gRegexFilter *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE(self, LOG4G_TYPE_REGEX_FILTER,
						 struct Private);
	self->priv->regex = NULL;
	self->priv->on_match = LOG4G_FILTER_ACCEPT;
	self->priv->on_mismatch = LOG4G_FILTER_DENY;
}

static void
finalize(GObject *base)
{
	Log4gRegexFilter *self = LOG4G_REGEX_FILTER(base);
	if (G_LIKELY (self->priv->regex)) {
		g_regex_unref(self->priv->regex);
	}
	G_OBJECT_CLASS(log4g_regex_filter_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_REGEX,
	PROP_ON_MATCH,
	PROP_ON_MISMATCH,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	Log4gRegexFilter *self = LOG4G_REGEX_FILTER(base);
	GError *error = NULL;
	switch (id) {
	case PROP_REGEX:
		if (self->priv->regex) {
			g_regex_unref(self->priv->regex);
		}
		self->priv->regex = g_regex_new(g_value_get_string(value),
						0, 0, &error);
		if (!self->priv->regex) {
			log4g_log_error("g_regex_new(): %s: %s",
					g_value_get_string(value),
					error->message);
			g_error_free(error);
		}
		break;
	case PROP_ON_MATCH:
		self->priv->on_match = g_value_get_enum(value);
		break;
	case PROP_ON_MISMATCH:
		self->priv->on_mismatch = g_value_get_enum(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static Log4gFilterDecision
decide(Log4gFilter *base, Log4gLoggingEvent *event)
{
	const gchar *message = log4g_logging_event_get_message (event);
	Log4gRegexFilter *self = LOG4G_REGEX_FILTER(base);
	Log4gFilterDecision decision;
	if (G_UNLIKELY(self->priv->regex && message)) {
		gboolean match;
		match = g_regex_match(self->priv->regex, message, 0, NULL);
		if (match) {
			decision = self->priv->on_match;
		} else {
			decision = self->priv->on_mismatch;
		}
	} else {
		decision = LOG4G_FILTER_NEUTRAL;
	}
	return decision;
}

static void
log4g_regex_filter_class_init(Log4gRegexFilterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	filter_class->decide = decide;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_REGEX,
		g_param_spec_string("regex", Q_("Regular Expression"),
			Q_("The regular expression to match"),
			NULL, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_ON_MATCH,
		g_param_spec_enum("on-match", Q_("On Match"),
			Q_("The action to take upon match"),
			log4g_filter_decision_get_type(), LOG4G_FILTER_ACCEPT,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_ON_MISMATCH,
		g_param_spec_enum("on-mismatch", Q_("On Mismatch"),
			Q_("The action to take upon mismatch"),
			log4g_filter_decision_get_type(), LOG4G_FILTER_DENY,
			G_PARAM_WRITABLE));
}

static void
log4g_regex_filter_class_finalize(G_GNUC_UNUSED Log4gRegexFilterClass *klass)
{
	/* do nothing */
}

void
log4g_regex_filter_register(GTypeModule *module)
{
	log4g_regex_filter_register_type(module);
}
