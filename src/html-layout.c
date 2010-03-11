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
 * \brief Implements the API in log4g/layout/html-layout.h
 * \author Mike Steinert
 * \date 2-16-2010
 */

#include "config.h"
#include "log4g/layout/html-layout.h"
#include <time.h>

/** \brief The default HTML document title */
#define LOG4G_HTML_LAYOUT_TITLE ("Log4g Log Messages")

enum _properties_t {
    PROP_O = 0,
    PROP_TITLE,
    PROP_LOCATION_INFO,
    PROP_MAX
};

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface)
{
    interface->activate_options = activate_options;
}

G_DEFINE_TYPE_WITH_CODE(Log4gHTMLLayout, log4g_html_layout, LOG4G_TYPE_LAYOUT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_HTML_LAYOUT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gint BUF_SIZE;
    gint MAX_CAPACITY;
    gchar *title;
    GString *string;
    gboolean info;
};

static void
log4g_html_layout_init(Log4gHTMLLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->BUF_SIZE = 256;
    priv->MAX_CAPACITY = 1024;
    priv->title = g_strdup(Q_(LOG4G_HTML_LAYOUT_TITLE));
    priv->string = g_string_sized_new(priv->BUF_SIZE);
    priv->info = FALSE;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->title) {
        g_free(priv->title);
        priv->title = NULL;
    }
    if (priv->string) {
        g_string_free(priv->string, TRUE);
        priv->string = NULL;
    }
    G_OBJECT_CLASS(log4g_html_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    gchar *title;
    switch (id) {
    case PROP_TITLE:
        title = g_value_dup_string(value);
        if (title) {
            if (priv->title) {
                g_free(priv->title);
            }
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
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gLayoutClass *layout = g_type_class_peek(LOG4G_TYPE_LAYOUT);
    Log4gLevel *level = log4g_logging_event_get_level(event);
    struct timeval *tv = log4g_logging_event_get_time_stamp(event);
    gchar *escaped;
    glong start = log4g_logging_event_get_start_time();
    glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
    if (priv->string->len > priv->MAX_CAPACITY) {
        g_string_free(priv->string, TRUE);
        priv->string = g_string_sized_new(priv->BUF_SIZE);
        if (!priv->string) {
            return NULL;
        }
    } else {
        g_string_set_size(priv->string, 0);
    }
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<tr>");
    g_string_append(priv->string, layout->LINE_SEP);
    /* time */
    g_string_append(priv->string, "<td>");
    g_string_append_printf(priv->string, "%ld", time - start);
    g_string_append(priv->string, "</td>");
    g_string_append(priv->string, layout->LINE_SEP);
    /* thread */
    escaped = g_strescape(log4g_logging_event_get_thread_name(event), NULL);
    if (escaped) {
        g_string_append_printf(priv->string, "<td title=\"%s\">", escaped);
        g_string_append(priv->string, escaped);
        g_free(escaped);
    } else {
        g_string_append(priv->string, "<td>");
        g_string_append(priv->string, "&nbsp;");
    }
    g_string_append(priv->string, "</td>");
    g_string_append(priv->string, layout->LINE_SEP);
    /* level */
    g_string_append(priv->string, "<td title=\"Level\">");
    escaped = g_strescape(log4g_level_to_string(level), NULL);
    if (escaped) {
        if (log4g_level_equals(level, log4g_level_DEBUG())) {
            g_string_append(priv->string, "<font color=\"#339933\"><strong>");
            g_string_append(priv->string, escaped);
            g_string_append(priv->string, "</strong></font>");
        } else if (
                log4g_level_is_greater_or_equal(level, log4g_level_WARN())) {
            g_string_append(priv->string, "<font color=\"#993300\"><strong>");
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
    g_string_append(priv->string, layout->LINE_SEP);
    /* category */
    escaped = g_strescape(log4g_logging_event_get_logger_name(event), NULL);
    if (escaped) {
        g_string_append_printf(priv->string, "<td title=\"%s\">", escaped);
        g_string_append(priv->string, escaped);
        g_free(escaped);
    } else {
        g_string_append(priv->string, "<td>");
        g_string_append(priv->string, "&nbsp;");
    }
    g_string_append(priv->string, "</td>");
    g_string_append(priv->string, layout->LINE_SEP);
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
        g_string_append(priv->string, layout->LINE_SEP);
    }
    /* message */
    g_string_append_printf(priv->string, "<td title=\"%s\">", Q_("Message"));
    escaped =
        g_strescape(log4g_logging_event_get_rendered_message(event), NULL);
    if (escaped) {
        g_string_append(priv->string, escaped);
        g_free(escaped);
    } else {
        g_string_append(priv->string, "&nbsp;");
    }
    g_string_append(priv->string, "</td>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "</tr>");
    g_string_append(priv->string, layout->LINE_SEP);
    /* NDC */
    if (log4g_logging_event_get_ndc(event)) {
        escaped = g_strescape(log4g_logging_event_get_ndc(event), NULL);
        if (escaped) {
            g_string_append_printf(priv->string,
                    "<tr><td bgcolor=\"#eeeeee\" "
                    "style=\"font-size : xx-small;\" "
                    "colspan=\"%d\" title=\"%s\">",
                    (priv->info ? 6 : 5), Q_("Nested Diagnostic Context"));
            g_string_append(priv->string, "NDC: ");
            g_string_append(priv->string, escaped);
            g_string_append(priv->string, "</td></tr>");
            g_string_append(priv->string, layout->LINE_SEP);
            g_free(escaped);
        }
    }
    return priv->string->str;
}

static const gchar *
get_content_type(Log4gLayout *base)
{
    return "text/html";
}

static const gchar *
get_header(Log4gLayout *base)
{
    time_t t;
    gchar buffer[26];
    Log4gLayoutClass *layout = g_type_class_peek(LOG4G_TYPE_LAYOUT);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    time(&t);
    g_string_set_size(priv->string, 0);
    g_string_append(priv->string,
            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\""
            " \"http://www.w3.org/TR/html4/loose.dtd\">");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<html>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<head>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<title>");
    g_string_append(priv->string, priv->title);
    g_string_append(priv->string, "</title>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<style type=\"text/css\">");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<!--");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string,
            "body, table {font-family: arial,sans-serif; "
            "font-size: x-small;}");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string,
            "th {background: #336699; color: #ffffff; text-align: left;}");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "-->");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "</style>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "</head>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string,
            "<body bgcolor=\"#ffffff\" topmargin=\"6\" leftmargin=\"6\">");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<hr size=\"1\" noshade />");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, Q_("Log session start time "));
    g_string_append(priv->string, ctime_r(&t, buffer));
    g_string_append(priv->string, "<br />");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<br />");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string,
            "<table cellspacing=\"0\" cellpadding=\"4\" border=\"1\" "
                    "bordercolor=\"#224466\" width=\"100%\">");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<tr>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append_printf(priv->string, "<th>%s</th>%s",
                           Q_("Time"), layout->LINE_SEP);
    g_string_append_printf(priv->string, "<th>%s</th>%s",
                           Q_("Thread"), layout->LINE_SEP);
    g_string_append_printf(priv->string, "<th>%s</th>%s",
                           Q_("Level"), layout->LINE_SEP);
    g_string_append_printf(priv->string, "<th>%s</th>%s",
                           Q_("Category"), layout->LINE_SEP);
    if (priv->info) {
        g_string_append_printf(priv->string, "<th>%s</th>%s",
                               Q_("File:Line"), layout->LINE_SEP);
    }
    g_string_append_printf(priv->string, "<th>%s</th>%s",
                           Q_("Message"), layout->LINE_SEP);
    g_string_append(priv->string, "</tr>");
    return priv->string->str;
}

static const gchar *
get_footer(Log4gLayout *base)
{
    Log4gLayoutClass *layout = g_type_class_peek(LOG4G_TYPE_LAYOUT);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_string_set_size(priv->string, 0);
    g_string_append(priv->string, "</table>");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "<br />");
    g_string_append(priv->string, layout->LINE_SEP);
    g_string_append(priv->string, "</body></html>");
    return priv->string->str;
}

static void log4g_html_layout_class_init(Log4gHTMLLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayout class */
    layout_class->format = format;
    layout_class->get_content_type = get_content_type;
    layout_class->get_header = get_header;
    layout_class->get_footer = get_footer;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_TITLE,
            g_param_spec_string("title", Q_("Title"),
                    Q_("HTML document title"), LOG4G_HTML_LAYOUT_TITLE,
                    G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_LOCATION_INFO,
            g_param_spec_boolean("location-info", Q_("Location Information"),
                    Q_("Toggle location information"),
                    FALSE, G_PARAM_WRITABLE));
}

Log4gLayout *
log4g_html_layout_new(void)
{
    return g_object_new(LOG4G_TYPE_HTML_LAYOUT, NULL);
}

void
log4g_html_layout_set_title(Log4gLayout *base, const gchar *title)
{
    g_return_if_fail(LOG4G_IS_HTML_LAYOUT(base));
    g_object_set(base, "title", title, NULL);
}

const gchar *
log4g_html_layout_get_title(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_HTML_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->title;
}

void
log4g_html_layout_set_location_info(Log4gLayout *base, gboolean info)
{
    g_return_if_fail(LOG4G_IS_HTML_LAYOUT(base));
    g_object_set(base, "location-info", info, NULL);
}

gboolean
log4g_html_layout_get_location_info(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_HTML_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->info;
}
