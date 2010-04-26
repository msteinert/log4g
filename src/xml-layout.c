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
 * \brief Implements the API in log4g/layout/xml-layout.h
 * \author Mike Steinert
 * \date 2-17-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/layout/xml-layout.h"
#include <time.h>

enum _properties_t {
    PROP_O = 0,
    PROP_PROPERTIES,
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

G_DEFINE_TYPE_WITH_CODE(Log4gXMLLayout, log4g_xml_layout, LOG4G_TYPE_LAYOUT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_XML_LAYOUT, \
            struct Log4gPrivate))

/** \brief Default string buffer size */
#define BUF_SIZE (256)

/** \brief Maximum string buffer size */
#define MAX_CAPACITY (2048)

struct Log4gPrivate {
    GString *string;
    gboolean properties;
    gboolean info;
};

static void
log4g_xml_layout_init(Log4gXMLLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->string = g_string_sized_new(BUF_SIZE);
    priv->properties = FALSE;
    priv->info = FALSE;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->string) {
        g_string_free(priv->string, TRUE);
        priv->string = NULL;
    }
    G_OBJECT_CLASS(log4g_xml_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_PROPERTIES:
        priv->properties = g_value_get_boolean(value);
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
    Log4gLevel *level = log4g_logging_event_get_level(event);
    struct timeval *tv = log4g_logging_event_get_time_stamp(event);
    time_t t = tv->tv_sec;
    gchar buffer[26];
    gchar *escaped;
    if (priv->string->len > MAX_CAPACITY) {
        g_string_free(priv->string, TRUE);
        priv->string = g_string_sized_new(BUF_SIZE);
        if (!priv->string) {
            return NULL;
        }
    } else {
        g_string_set_size(priv->string, 0);
    }
    g_string_append(priv->string, "<log4g:event logger=\"");
    escaped = g_strescape(log4g_logging_event_get_logger_name(event), NULL);
    if (escaped) {
        g_string_append(priv->string, escaped);
        g_free(escaped);
    }
    g_string_append(priv->string, "\" timestamp=\"");
    g_string_append(priv->string, ctime_r(&t, buffer));
    g_string_append(priv->string, "\" level=\"");
    escaped = g_strescape(log4g_level_to_string(level), NULL);
    if (escaped) {
        g_string_append(priv->string, escaped);
        g_free(escaped);
    }
    g_string_append(priv->string, "\" thread=\"");
    escaped = g_strescape(log4g_logging_event_get_thread_name(event), NULL);
    if (escaped) {
        g_string_append(priv->string, escaped);
        g_free(escaped);
    }
    g_string_append(priv->string, "\">\r\n");
    /* message */
    g_string_append(priv->string, "<log4g:message><![CDATA[");
    escaped =
        g_strescape(log4g_logging_event_get_rendered_message(event), NULL);
    if (escaped) {
        g_string_append(priv->string, escaped);
        g_free(escaped);
    }
    g_string_append(priv->string, "]]></log4g:message>\r\n");
    if (log4g_logging_event_get_ndc(event)) {
        /* NDC */
        g_string_append(priv->string, "<log4g:NDC><![CDATA[");
        escaped = g_strescape(log4g_logging_event_get_ndc(event), NULL);
        if (escaped) {
            g_string_append(priv->string, escaped);
            g_free(escaped);
        }
        g_string_append(priv->string, "]]></log4g:NDC>\r\n");
    }
    if (priv->info) {
        /* file:line */
        g_string_append(priv->string, "<log4g:locationInfo function=\"");
        escaped =
            g_strescape(log4g_logging_event_get_function_name(event), NULL);
        if (escaped) {
            g_string_append(priv->string, escaped);
            g_free(escaped);
        }
        g_string_append(priv->string, "\" file=\"");
        escaped = g_strescape(log4g_logging_event_get_file_name(event), NULL);
        if (escaped) {
            g_string_append(priv->string, escaped);
            g_free(escaped);
        }
        g_string_append(priv->string, "\" line=\"");
        escaped =
            g_strescape(log4g_logging_event_get_line_number(event), NULL);
        if (escaped) {
            g_string_append(priv->string, escaped);
            g_free(escaped);
        }
        g_string_append(priv->string, "\" />\r\n");
    }
    if (priv->properties) {
        const GArray *keyset = log4g_logging_event_get_property_key_set(event);
        if (keyset && keyset->len) {
            gchar *key;
            guint i;
            g_string_append(priv->string, "<log4g:properties>\r\n");
            for (i = 0; i < keyset->len; ++i) {
                key = g_array_index(keyset, gchar *, i);
                escaped =
                    g_strescape(log4g_logging_event_get_mdc(event, key), NULL);
                if (escaped) {
                    g_string_append(priv->string, "<log4g:data name=\"");
                    key = g_strescape(key, NULL);
                    if (key) {
                        g_string_append(priv->string, key);
                        g_free(key);
                    }
                    g_string_append(priv->string, "\" value=\"");
                    g_string_append(priv->string, escaped);
                    g_string_append(priv->string, "\" />\r\n");
                    g_free(escaped);
                }
            }
            g_string_append(priv->string, "</log4g:properties>\r\n");
        }
    }
    g_string_append(priv->string, "</log4g:event>\r\n\r\n");
    return priv->string->str;
}

static void log4g_xml_layout_class_init(Log4gXMLLayoutClass *klass)
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
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_PROPERTIES,
            g_param_spec_boolean("properties", Q_("Properties"),
                    Q_("Toggle MDC properties"), FALSE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_LOCATION_INFO,
            g_param_spec_boolean("location-info", Q_("Location Information"),
                    Q_("Toggle location information"),
                    FALSE, G_PARAM_WRITABLE));
}

Log4gLayout *
log4g_xml_layout_new(void)
{
    return g_object_new(LOG4G_TYPE_XML_LAYOUT, NULL);
}

void
log4g_xml_layout_set_properties(Log4gLayout *base, gboolean properties)
{
    g_return_if_fail(LOG4G_IS_XML_LAYOUT(base));
    g_object_set(base, "properties", properties, NULL);
}

gboolean
log4g_xml_layout_get_title(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_XML_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->properties;
}

void
log4g_xml_layout_set_location_info(Log4gLayout *base, gboolean info)
{
    g_return_if_fail(LOG4G_IS_XML_LAYOUT(base));
    g_object_set(base, "location-info", info, NULL);
}

gboolean
log4g_xml_layout_get_location_info(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_XML_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->info;
}
