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
 * SECTION: ttcc-layout
 * @short_description: Output the time, thread, category &amp; context
 *
 * The TTCC layout outputs the time, thread, category (logger name), and
 * context (nested diagnostic context) of a log event, hence the name.
 *
 * Some fields can be individually enabled or disabled via the corresponding
 * properties:
 * <orderedlist>
 * <listitem><para>thread-printing</para></listitem>
 * <listitem><para>category-prefixing</para></listitem>
 * <listitem><para>context-printing</para></listitem>
 * </orderedlist>
 *
 * Here is an example of the TTCC output:
 *
 * |[
 * 176 [main] INFO  org.gnome.log4g.foo - an example message
 * 225 [main] DEBUG org.gnome.log4g.bar - another example message
 * ]|
 *
 * The first field is the number of milliseconds elapsed since the Log4g
 * system was initialized. The second field is the name of the thread that
 * logged the event. The third field is the log level. The fourth field is
 * the logger category to which the statement belongs.
 *
 * The fifth field (just before the '-') is the nested diagnostic context.
 *
 * <note><para>
 * The nested diagnostic context may be empty (as in the example).
 * </para></note>
 *
 * <note><para>
 * Do not use the same TTCC layout instance from within different appenders.
 * The TTCC layout is not thread-safe when used in this way.
 * </para></note>
 *
 * <note><para>
 * The #Log4gPatternLayoutClass provides a more flexible alternative.
 * </para></note>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/ttcc-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gTTCCLayout, log4g_ttcc_layout,
		LOG4G_TYPE_DATE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_TTCC_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gTTCCLayout *)instance)->priv)

struct Private {
	gboolean thread;
	gboolean category;
	gboolean context;
	GString *string;
};

static void
log4g_ttcc_layout_init(Log4gTTCCLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->string = g_string_sized_new(256);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->string) {
		g_string_free(priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_ttcc_layout_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_THREAD_PRINTING,
	PROP_CATEGORY_PREFIXING,
	PROP_CONTEXT_PRINTING,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_THREAD_PRINTING:
		priv->thread = g_value_get_boolean(value);
		break;
	case PROP_CATEGORY_PREFIXING:
		priv->category = g_value_get_boolean(value);
		break;
	case PROP_CONTEXT_PRINTING:
		priv->context = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	g_string_set_size(priv->string, 0);
	log4g_date_layout_date_format(base, priv->string, event);
	g_string_append_c(priv->string, ' ');
	if (priv->thread) {
		g_string_append_printf(priv->string, "[%s] ",
				log4g_logging_event_get_thread_name(event));
	}
	g_string_append(priv->string,
			log4g_level_to_string(log4g_logging_event_get_level(
					event)));
	g_string_append_c(priv->string, ' ');
	if (priv->category) {
		g_string_append(priv->string,
				log4g_logging_event_get_logger_name(event));
		g_string_append_c(priv->string, ' ');
	}
	if (priv->context) {
		const gchar *ndc = log4g_logging_event_get_ndc(event);
		if (ndc) {
			g_string_append(priv->string, ndc);
			g_string_append_c(priv->string, ' ');
		}
	}
	g_string_append(priv->string, "- ");
	g_string_append(priv->string,
			log4g_logging_event_get_rendered_message(event));
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	return priv->string->str;
}

static void
log4g_ttcc_layout_class_init(Log4gTTCCLayoutClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	layout_class->format = format;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_THREAD_PRINTING,
		g_param_spec_boolean("thread-printing", Q_("Thread Printing"),
			Q_("Toggle thread printing"), TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_CATEGORY_PREFIXING,
		g_param_spec_boolean("category-prefixing",
			Q_("Category Prefixing"),
			Q_("Toggle category prefixing"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_CONTEXT_PRINTING,
		g_param_spec_boolean("context-printing",
			Q_("Context Printing"), Q_("Toggle context printing"),
			TRUE, G_PARAM_WRITABLE));
}

static void
log4g_ttcc_layout_class_finalize(Log4gTTCCLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_ttcc_layout_register(GTypeModule *module)
{
	log4g_date_layout_register(module);
	log4g_ttcc_layout_register_type(module);
}
