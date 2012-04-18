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
 * It does not output a complete well-formed XML document. The output is
 * designed to be included as an external entity in a separate file to form
 * a complete XML document.
 *
 * For example, if "abc" is the name of the file where the XML layout output
 * is directed, a well-formed XML document would be:
 *
 * |[<![CDATA[
 * <?xml version="1.0" ?>
 * <!DOCTYPE log4g:events SYSTEM "log4g.dtd"
 *     [<!ENTITY data SYSTEM "abc">]>
 * <log4g:events version="1.0" xmlns:lo4g="http://gnome.org/log4g/1.0/">
 *     &data;
 * </log4g:events>
 * ]]>]|
 *
 * This approach enforces the independence of the XML layout and the appender
 * where it is embedded.
 *
 * XML layouts accept two properties:
 * <orderedlist>
 * <listitem><para>properties</para></listitem>
 * <listitem><para>location-info</para></listitem>
 * </orderedlist>
 *
 * Setting properties to %TRUE causes the XML layout to output all MDC (mapped
 * data context) values.
 *
 * Setting the location-info property to %TRUE will cause the XML layout to
 * include the log message location, i.e. function(file:line). The default
 * value is %FALSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/xml-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gXMLLayout, log4g_xml_layout, LOG4G_TYPE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_XML_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gXMLLayout *)instance)->priv)

struct Private {
	GString *string;
	gboolean properties;
	gboolean info;
};

/* Default string buffer size */
#define BUF_SIZE (256)

/* Maximum string buffer size */
#define MAX_CAPACITY (2048)

static void
log4g_xml_layout_init(Log4gXMLLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->string = g_string_sized_new(BUF_SIZE);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->string) {
		g_string_free(priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_xml_layout_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_PROPERTIES,
	PROP_LOCATION_INFO,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
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
	struct Private *priv = GET_PRIVATE(base);
	Log4gLevel *level = log4g_logging_event_get_level(event);
	const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
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
	g_string_erase(priv->string, priv->string->len - 1, 1);
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
		g_strescape(log4g_logging_event_get_rendered_message(event),
				NULL);
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
		g_string_append(priv->string,
				"<log4g:locationInfo function=\"");
		escaped =
			g_strescape(log4g_logging_event_get_function_name(event),
					NULL);
		if (escaped) {
			g_string_append(priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(priv->string, "\" file=\"");
		escaped = g_strescape(log4g_logging_event_get_file_name(event),
				NULL);
		if (escaped) {
			g_string_append(priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(priv->string, "\" line=\"");
		escaped = g_strescape(log4g_logging_event_get_line_number(event),
				NULL);
		if (escaped) {
			g_string_append(priv->string, escaped);
			g_free(escaped);
		}
		g_string_append(priv->string, "\" />\r\n");
	}
	if (priv->properties) {
		const GArray *keyset =
			log4g_logging_event_get_property_key_set(event);
		if (keyset && keyset->len) {
			gchar *key;
			guint i;
			g_string_append(priv->string, "<log4g:properties>\r\n");
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
				escaped = g_strescape(log4g_logging_event_get_mdc(event, key), NULL);
				if (escaped) {
					g_string_append(priv->string,
							"<log4g:data name=\"");
					key = g_strescape(key, NULL);
					if (key) {
						g_string_append(priv->string,
								key);
						g_free(key);
					}
					g_string_append(priv->string,
							"\" value=\"");
					g_string_append(priv->string, escaped);
					g_string_append(priv->string,
							"\" />\r\n");
					g_free(escaped);
				}
			}
			g_string_append(priv->string,
					"</log4g:properties>\r\n");
		}
	}
	g_string_append(priv->string, "</log4g:event>\r\n\r\n");
	return priv->string->str;
}

static void log4g_xml_layout_class_init(Log4gXMLLayoutClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	layout_class->format = format;
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
