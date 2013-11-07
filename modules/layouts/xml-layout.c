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
 * SECTION: xml-layout
 * @short_description: Format events into XML
 * @see_also: #Log4gMDCClass, #Log4gNDCClass
 *
 * The output of this layout consists of a series of Log4g log event elements.
 *
 * XML layouts accept three properties:
 * <orderedlist>
 * <listitem><para>properties</para></listitem>
 * <listitem><para>location-info</para></listitem>
 * <listitem><para>complete</para></listitem>
 * </orderedlist>
 *
 * Setting properties to %TRUE causes the XML layout to output all MDC (mapped
 * data context) values.
 *
 * Setting the location-info property to %TRUE will cause the XML layout to
 * include the log message location, i.e. function(file:line). The default
 * value is %FALSE.
 *
 * If the complete property is set to false then a well-formed XML document is
 * not created. This output is designed to be included as an external entity
 * in a separate file to form a complete XML document.
 *
 * For example, if "abc" is the name of the file where the XML layout output
 * is directed, a well-formed XML document would be:
 *
 * |[<![CDATA[
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE log4g:events SYSTEM "log4g.dtd"
 *     [<!ENTITY data SYSTEM "abc">]>
 * <log4g:events xmlns:lo4g="http://mike.steinert.ca/log4g/1.0/events">
 *     &data;
 * </log4g:events>
 * ]]>]|
 *
 * This approach enforces the independence of the XML layout and the appender
 * where it is embedded.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/xml-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gXMLLayout, log4g_xml_layout, LOG4G_TYPE_LAYOUT)

struct Private {
	GString *string;
	gboolean properties;
	gboolean info;
	gboolean complete;
};

/* Default string buffer size */
#define BUF_SIZE (256)

/* Maximum string buffer size */
#define MAX_CAPACITY (2048)

static void
log4g_xml_layout_init(Log4gXMLLayout *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE(self, LOG4G_TYPE_XML_LAYOUT,
						 struct Private);
	self->priv->string = g_string_sized_new(BUF_SIZE);
	self->priv->complete = TRUE;
}

static void
finalize(GObject *base)
{
	Log4gXMLLayout *self = LOG4G_XML_LAYOUT(base);
	if (self->priv->string) {
		g_string_free(self->priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_xml_layout_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_PROPERTIES,
	PROP_LOCATION_INFO,
	PROP_COMPLETE,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	Log4gXMLLayout *self = LOG4G_XML_LAYOUT(base);
	switch (id) {
	case PROP_PROPERTIES:
		self->priv->properties = g_value_get_boolean(value);
		break;
	case PROP_LOCATION_INFO:
		self->priv->info = g_value_get_boolean(value);
		break;
	case PROP_COMPLETE:
		self->priv->complete = g_value_get_boolean(value);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	Log4gXMLLayout *self = LOG4G_XML_LAYOUT(base);
	Log4gLevel *level = log4g_logging_event_get_level(event);
	const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
	time_t t = tv->tv_sec;
	gchar buffer[26];
	gchar *escaped;
	if (self->priv->string->len > MAX_CAPACITY) {
		g_string_free(self->priv->string, TRUE);
		self->priv->string = g_string_sized_new(BUF_SIZE);
	} else {
		g_string_set_size(self->priv->string, 0);
	}
	g_string_append(self->priv->string, "<log4g:event logger=\"");
	escaped = g_strescape(log4g_logging_event_get_logger_name(event), NULL);
	if (escaped) {
		g_string_append(self->priv->string, escaped);
		g_free(escaped);
	}
	g_string_append(self->priv->string, "\" timestamp=\"");
	g_string_append(self->priv->string, ctime_r(&t, buffer));
	g_string_erase(self->priv->string, self->priv->string->len - 1, 1);
	g_string_append(self->priv->string, "\" level=\"");
	escaped = g_strescape(log4g_level_to_string(level), NULL);
	if (escaped) {
		g_string_append(self->priv->string, escaped);
		g_free(escaped);
	}
	g_string_append(self->priv->string, "\" thread=\"");
	escaped = g_strescape(log4g_logging_event_get_thread_name(event), NULL);
	if (escaped) {
		g_string_append(self->priv->string, escaped);
		g_free(escaped);
	}
	g_string_append(self->priv->string, "\">\r\n");
	/* message */
	g_string_append(self->priv->string, "<log4g:message><![CDATA[");
	escaped =
		g_strescape(log4g_logging_event_get_rendered_message(event),
				NULL);
	if (escaped) {
		g_string_append(self->priv->string, escaped);
		g_free(escaped);
	}
	g_string_append(self->priv->string, "]]></log4g:message>\r\n");
	if (log4g_logging_event_get_ndc(event)) {
		/* NDC */
		g_string_append(self->priv->string, "<log4g:NDC><![CDATA[");
		escaped = g_strescape(log4g_logging_event_get_ndc(event), NULL);
		if (escaped) {
			g_string_append(self->priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(self->priv->string, "]]></log4g:NDC>\r\n");
	}
	if (self->priv->info) {
		/* file:line */
		g_string_append(self->priv->string,
				"<log4g:locationInfo function=\"");
		escaped =
			g_strescape(log4g_logging_event_get_function_name(event),
					NULL);
		if (escaped) {
			g_string_append(self->priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(self->priv->string, "\" file=\"");
		escaped = g_strescape(log4g_logging_event_get_file_name(event),
				NULL);
		if (escaped) {
			g_string_append(self->priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(self->priv->string, "\" line=\"");
		escaped = g_strescape(log4g_logging_event_get_line_number(event),
				NULL);
		if (escaped) {
			g_string_append(self->priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(self->priv->string, "\" />\r\n");
	}
	if (self->priv->properties) {
		const GArray *keyset =
			log4g_logging_event_get_property_key_set(event);
		if (keyset && keyset->len) {
			gchar *key;
			guint i;
			g_string_append(self->priv->string, "<log4g:properties>\r\n");
			for (i = 0; i < keyset->len; ++i) {
				key = g_array_index(keyset, gchar *, i);
				if (!key) {
					continue;
				}
				const gchar *value =
					log4g_logging_event_get_mdc(event, key);
				if (!value) {
					continue;
				}
				escaped = g_strescape(value, NULL);
				if (escaped) {
					g_string_append(self->priv->string,
							"<log4g:data name=\"");
					key = g_strescape(key, NULL);
					if (key) {
						g_string_append(self->priv->string,
								key);
						g_free(key);
					}
					g_string_append(self->priv->string,
							"\" value=\"");
					g_string_append(self->priv->string, escaped);
					g_string_append(self->priv->string,
							"\" />\r\n");
					g_free(escaped);
				}
			}
			g_string_append(self->priv->string,
					"</log4g:properties>\r\n");
		}
	}
	g_string_append(self->priv->string, "</log4g:event>\r\n\r\n");
	return self->priv->string->str;
}

static const gchar *
get_content_type(G_GNUC_UNUSED Log4gLayout *base)
{
	return "application/xml";
}

static const gchar *
get_header(Log4gLayout *base)
{
	Log4gXMLLayout *self = LOG4G_XML_LAYOUT(base);
	if (!self->priv->complete) {
		return "";
	}
	return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
	       "<log4g:events xmlns:lo4g=\"http://mike.steinert.ca/log4g/1.0/events\">";
}

static const gchar *
get_footer(G_GNUC_UNUSED Log4gLayout *base)
{
	Log4gXMLLayout *self = LOG4G_XML_LAYOUT(base);
	if (!self->priv->complete) {
		return "";
	}
	return "</log4g:events>";
}

static void log4g_xml_layout_class_init(Log4gXMLLayoutClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	layout_class->format = format;
	layout_class->get_content_type = get_content_type;
	layout_class->get_header = get_header;
	layout_class->get_footer = get_footer;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_PROPERTIES,
		g_param_spec_boolean("properties", Q_("Properties"),
			Q_("Toggle MDC properties"), FALSE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LOCATION_INFO,
		g_param_spec_boolean("location-info",
			Q_("Location Information"),
			Q_("Toggle location information"),
			FALSE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_COMPLETE,
		g_param_spec_boolean("complete",
			Q_("Complete Document"),
			Q_("Toggle writing a complete document"),
			TRUE, G_PARAM_WRITABLE));
}

static void
log4g_xml_layout_class_finalize(G_GNUC_UNUSED Log4gXMLLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_xml_layout_register(GTypeModule *module)
{
	log4g_xml_layout_register_type(module);
}
