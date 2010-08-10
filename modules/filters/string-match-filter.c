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
 * SECTION: string-match-filter
 * @short_description: A filter based on string matching
 *
 * A simple filter based on string matching.
 *
 * This filter accepts two parameters:
 * <orderedlist>
 * <listitem><para>string-to-match</para></listitem>
 * <listitem><para>accept-on-match</para></listitem>
 * </orderedlist>
 *
 * If there is a match between the string-to-match value and the log event
 * message then the decide function returns:
 * <orderedlist>
 * <listitem><para>accept if accept-on-match is %TRUE</para></listitem>
 * <listitem><para>deny if accept-on-match is %FALSE</para></listitem>
 * </orderedlist>
 *
 * The default value for accept-on-match is %TRUE.
 *
 * If there is no match neutral is returned.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/string-match-filter.h"

enum _properties_t {
    PROP_O = 0,
    PROP_STRING_TO_MATCH,
    PROP_ACCEPT_ON_MATCH,
    PROP_MAX
};

G_DEFINE_DYNAMIC_TYPE(Log4gStringMatchFilter, log4g_string_match_filter,
        LOG4G_TYPE_FILTER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_STRING_MATCH_FILTER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean accept;
    gchar *string;
};

static void
log4g_string_match_filter_init(Log4gStringMatchFilter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->accept = TRUE;
    priv->string = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->string);
    priv->string = NULL;
    G_OBJECT_CLASS(log4g_string_match_filter_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_STRING_TO_MATCH:
        g_free(priv->string);
        const gchar *string = g_value_get_string(value);
        if (string) {
            priv->string = g_strdup(string);
        } else {
            priv->string = NULL;
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
    const gchar *message = log4g_logging_event_get_rendered_message(event);
    if (!message || !priv->string) { 
        return LOG4G_FILTER_NEUTRAL;
    }
    if (!g_strstr_len(message, -1, priv->string)) {
        return LOG4G_FILTER_NEUTRAL;
    } else {
        return (priv->accept ? LOG4G_FILTER_ACCEPT : LOG4G_FILTER_DENY);
    }
}

static void
log4g_string_match_filter_class_init(Log4gStringMatchFilterClass *klass)
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
    g_object_class_install_property(gobject_class, PROP_STRING_TO_MATCH,
            g_param_spec_string("string-to-match", Q_("String to Match"),
                    Q_("Log string to match"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_ACCEPT_ON_MATCH,
            g_param_spec_boolean("accept-on-match", Q_("Accept on Match"),
                    Q_("Accept or deny on log level match"),
                    TRUE, G_PARAM_WRITABLE));
}

static void
log4g_string_match_filter_class_finalize(Log4gStringMatchFilterClass *klass)
{
    /* do nothing */
}

void
log4g_string_match_filter_register(GTypeModule *module)
{
    log4g_string_match_filter_register_type(module);
}
