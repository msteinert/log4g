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
 * SECTION: html-layout
 * @short_description: Format events into an HTML table
 *
 * This layout formats log events into an HTML table.
 *
 * HTML layouts accept two properties:
 * <orderedlist>
 * <listitem><para>title</para></listitem>
 * <listitem><para>location-info</para></listitem>
 * </orderedlist>
 *
 * Setting the title property will change the title of the generated HTML
 * document. If this property is not set then the title will be "Log4g Log
 * Messages".
 *
 * Setting the location-info property to %TRUE will cause the HTML layout
 * to include the log message location, i.e. function(file:line). The default
 * value is %FALSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/html-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gHTMLLayout, log4g_html_layout, LOG4G_TYPE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_HTML_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gHTMLLayout *)instance)->priv)

struct Private {
	gchar *title;
	GString *string;
	gboolean info;
};

/* The default HTML document title */
#define LOG4G_HTML_LAYOUT_TITLE ("Log4g Log Messages")

/* Default string buffer size */
#define BUF_SIZE (256)

/* Maximum string buffer size */
#define MAX_CAPACITY (1024)

static void
log4g_html_layout_init(Log4gHTMLLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->title = g_strdup(Q_(LOG4G_HTML_LAYOUT_TITLE));
	priv->string = g_string_sized_new(BUF_SIZE);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->title);
	if (priv->string) {
		g_string_free(priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_html_layout_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_TITLE,
	PROP_LOCATION_INFO,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	gchar *title;
	switch (id) {
	case PROP_TITLE:
		title = g_value_dup_string(value);
		if (title) {
			g_free(priv->title);
			priv->title = g_strdup(title);
		}
		break;
	case PROP_LOCATION_INFO:
		priv->info = g_value_get_boolean(value);
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
	Log4gLevel *level = log4g_logging_event_get_level(event);
	const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
	gchar *escaped;
	glong start = log4g_logging_event_get_start_time();
	glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
	if (priv->string->len > MAX_CAPACITY) {
		g_string_free(priv->string, TRUE);
		priv->string = g_string_sized_new(BUF_SIZE);
		if (!priv->string) {
			return NULL;
		}
	} else {
		g_string_set_size(priv->string, 0);
	}
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<tr>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	/* time */
	g_string_append(priv->string, "<td>");
	g_string_append_printf(priv->string, "%ld", time - start);
	g_string_append(priv->string, "</td>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	/* thread */
	escaped = g_strescape(log4g_logging_event_get_thread_name(event), NULL);
	if (escaped) {
		g_string_append_printf(priv->string, "<td title=\"%s\">",
				escaped);
		g_string_append(priv->string, escaped);
		g_free(escaped);
	} else {
		g_string_append(priv->string, "<td>");
		g_string_append(priv->string, "&nbsp;");
	}
	g_string_append(priv->string, "</td>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	/* level */
	g_string_append(priv->string, "<td title=\"Level\">");
	escaped = g_strescape(log4g_level_to_string(level), NULL);
	if (escaped) {
		if (log4g_level_equals(level, log4g_level_DEBUG())) {
			g_string_append(priv->string,
					"<font color=\"#339933\"><strong>");
			g_string_append(priv->string, escaped);
			g_string_append(priv->string, "</strong></font>");
		} else if (log4g_level_is_greater_or_equal(level,
					log4g_level_WARN())) {
			g_string_append(priv->string,
					"<font color=\"#993300\"><strong>");
			g_string_append(priv->string, escaped);
			g_string_append(priv->string, "</strong></font>");
		} else {
			g_string_append(priv->string, escaped);
		}
		g_free(escaped);
	} else {
		g_string_append(priv->string, "&nbsp;");
	}
	g_string_append(priv->string, "</td>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	/* category */
	escaped = g_strescape(log4g_logging_event_get_logger_name(event), NULL);
	if (escaped) {
		g_string_append_printf(priv->string, "<td title=\"%s\">",
				escaped);
		g_string_append(priv->string, escaped);
		g_free(escaped);
	} else {
		g_string_append(priv->string, "<td>");
		g_string_append(priv->string, "&nbsp;");
	}
	g_string_append(priv->string, "</td>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	if (priv->info) {
		/* file:line */
		g_string_append(priv->string, "<td>");
		escaped = g_strescape(log4g_logging_event_get_file_name(event), NULL);
		if (escaped) {
			g_string_append(priv->string, escaped);
			g_free(escaped);
		}
		g_string_append_c(priv->string, ':');
		escaped =
			g_strescape(log4g_logging_event_get_line_number(event), NULL);
		if (escaped) {
			g_string_append(priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(priv->string, "</td>");
		g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	}
	/* message */
	g_string_append_printf(priv->string, "<td title=\"%s\">",
			Q_("Message"));
	escaped = g_strescape(log4g_logging_event_get_rendered_message(event),
			NULL);
	if (escaped) {
		g_string_append(priv->string, escaped);
		g_free(escaped);
	} else {
		g_string_append(priv->string, "&nbsp;");
	}
	g_string_append(priv->string, "</td>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "</tr>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	/* NDC */
	if (log4g_logging_event_get_ndc(event)) {
		escaped = g_strescape(log4g_logging_event_get_ndc(event), NULL);
		if (escaped) {
			g_string_append_printf(priv->string,
					"<tr><td bgcolor=\"#eeeeee\" "
					"style=\"font-size : xx-small;\" "
					"colspan=\"%d\" title=\"%s\">",
					(priv->info ? 6 : 5),
					Q_("Nested Diagnostic Context"));
			g_string_append(priv->string, "NDC: ");
			g_string_append(priv->string, escaped);
			g_string_append(priv->string, "</td></tr>");
			g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
			g_free(escaped);
		}
	}
	return priv->string->str;
}

static const gchar *
get_content_type(G_GNUC_UNUSED Log4gLayout *base)
{
	return "text/html";
}

static const gchar *
get_header(Log4gLayout *base)
{
	time_t t;
	gchar buffer[26];
	struct Private *priv = GET_PRIVATE(base);
	time(&t);
	g_string_set_size(priv->string, 0);
	g_string_append(priv->string,
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 "
			"Transitional//EN "
			"\"http://www.w3.org/TR/html4/loose.dtd\">");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<html>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<head>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<title>");
	g_string_append(priv->string, priv->title);
	g_string_append(priv->string, "</title>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<style type=\"text/css\">");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<!--");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string,
			"body, table {font-family: arial,sans-serif; "
			"font-size: x-small;}");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string,
			"th {background: #336699; color: #ffffff; "
			"text-align: left;}");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "-->");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "</style>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "</head>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string,
			"<body bgcolor=\"#ffffff\" topmargin=\"6\" "
			"leftmargin=\"6\">");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<hr size=\"1\" noshade />");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, Q_("Log session start time "));
	g_string_append(priv->string, ctime_r(&t, buffer));
	g_string_erase(priv->string, priv->string->len - 1, 1);
	g_string_append(priv->string, "<br />");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<br />");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string,
			"<table cellspacing=\"0\" cellpadding=\"4\" "
			"border=\"1\" bordercolor=\"#224466\" "
			"width=\"100%\">");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<tr>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append_printf(priv->string, "<th>%s</th>%s",
			Q_("Time"), LOG4G_LAYOUT_LINE_SEP);
	g_string_append_printf(priv->string, "<th>%s</th>%s",
			Q_("Thread"), LOG4G_LAYOUT_LINE_SEP);
	g_string_append_printf(priv->string, "<th>%s</th>%s",
			Q_("Level"), LOG4G_LAYOUT_LINE_SEP);
	g_string_append_printf(priv->string, "<th>%s</th>%s",
			Q_("Category"), LOG4G_LAYOUT_LINE_SEP);
	if (priv->info) {
		g_string_append_printf(priv->string, "<th>%s</th>%s",
				Q_("File:Line"), LOG4G_LAYOUT_LINE_SEP);
	}
	g_string_append_printf(priv->string, "<th>%s</th>%s",
			Q_("Message"), LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "</tr>");
	return priv->string->str;
}

static const gchar *
get_footer(Log4gLayout *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_string_set_size(priv->string, 0);
	g_string_append(priv->string, "</table>");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "<br />");
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	g_string_append(priv->string, "</body></html>");
	return priv->string->str;
}

static void
log4g_html_layout_class_init(Log4gHTMLLayoutClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	layout_class->format = format;
	layout_class->get_content_type = get_content_type;
	layout_class->get_header = get_header;
	layout_class->get_footer = get_footer;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_TITLE,
		g_param_spec_string("title", Q_("Title"),
			Q_("HTML document title"), LOG4G_HTML_LAYOUT_TITLE,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LOCATION_INFO,
		g_param_spec_boolean("location-info",
			Q_("Location Information"),
			Q_("Toggle location information"),
			FALSE, G_PARAM_WRITABLE));
}

static void
log4g_html_layout_class_finalize(G_GNUC_UNUSED Log4gHTMLLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_html_layout_register(GTypeModule *module)
{
	log4g_html_layout_register_type(module);
}
