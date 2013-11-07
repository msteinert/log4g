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
 * SECTION: json-layout
 * @short_description: Format events into JSON
 * @see_also: #Log4gMDCClass, #Log4gNDCClass
 *
 * The output of this layout consists of a series of Log4g log event objects.
 *
 * Json layouts accept three properties:
 * <orderedlist>
 * <listitem><para>properties</para></listitem>
 * <listitem><para>location-info</para></listitem>
 * <listitem><para>complete</para></listitem>
 * </orderedlist>
 *
 * Setting properties to %TRUE causes the JSON layout to output all MDC (mapped
 * data context) values. The default value is %TRUE.
 *
 * Setting the location-info property to %TRUE will cause the JSON layout to
 * include the log message location, i.e. function(file:line). The default
 * value is %TRUE.
 *
 * If the complete property is set to false then a well-formed JSON document is
 * not created. This output is designed to be included as an external entity
 * in a separate file to form a complete JSON document.
 *
 * For example, if "abc" is the name of the file where the JSON layout output
 * is directed, a well-formed JSON document would be:
 *
 * [
 *   {
 *     "logger": "org.gnome.gimp",
 *     "timestamp: "1383853166",
 *     "level": "DEBUG",
 *     "thread": "main",
 *     "message": "The swift brown fox jumps over the lazy dog"
 *   },
 *   {
 *     "logger": "org.gnome.gimp",
 *     "timestamp: "1383853166",
 *     "level": "DEBUG",
 *     "thread": "main",
 *     "message": "Lorem ipsum dolor sit amet, consectetur adipisicing elit"
 *   }
 * ]
 *
 * This approach enforces the independence of the JSON layout and the appender
 * where it is embedded.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/json-layout.h"
#include <errno.h>

G_DEFINE_DYNAMIC_TYPE(Log4gJsonLayout, log4g_json_layout, LOG4G_TYPE_LAYOUT)

struct Private {
	GString *string;
	volatile gboolean first_layout_done;
	gboolean properties;
	gboolean info;
	gboolean complete;
};

/* Default string buffer size */
#define BUF_SIZE (256)

/* Maximum string buffer size */
#define MAX_CAPACITY (2048)

static void
log4g_json_layout_init(Log4gJsonLayout *self)
{
	self->priv = G_TYPE_INSTANCE_GET_PRIVATE(self, LOG4G_TYPE_JSON_LAYOUT,
						 struct Private);
	self->priv->string = g_string_sized_new(BUF_SIZE);
	self->priv->first_layout_done = FALSE;
	self->priv->properties = TRUE;
	self->priv->info = TRUE;
	self->priv->complete = TRUE;
}

static void
finalize(GObject *base)
{
	Log4gJsonLayout *self = LOG4G_JSON_LAYOUT(base);
	if (self->priv->string) {
		g_string_free(self->priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_json_layout_parent_class)->finalize(base);
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
	Log4gJsonLayout *self = LOG4G_JSON_LAYOUT(base);
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
strescape(const gchar *source, const gchar *exceptions)
{
	if (G_UNLIKELY(!source)) {
		return NULL;
	}
	return g_strescape(source, exceptions);
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	Log4gJsonLayout *self = LOG4G_JSON_LAYOUT(base);
	const GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
	Log4gLevel *level = log4g_logging_event_get_level(event);
	gboolean first_layout_done;
	gboolean delim = FALSE;
	time_t t = tv->tv_sec;
	const gchar *name;
	gchar *escaped;

	if (self->priv->string->len > MAX_CAPACITY) {
		g_string_free(self->priv->string, TRUE);
		self->priv->string = g_string_sized_new(BUF_SIZE);
	} else {
		g_string_set_size(self->priv->string, 0);
	}

	first_layout_done = g_atomic_int_get(&self->priv->first_layout_done);
	if (first_layout_done) {
		g_string_append(self->priv->string, ",\n");
	} else {
		g_string_append_c(self->priv->string, '\n');
		g_atomic_int_set(&self->priv->first_layout_done, TRUE);
	}
	g_string_append(self->priv->string, "  {\n");

	name = log4g_logging_event_get_logger_name(event);
	if (!name) {
		name = "root";
	}
	escaped = strescape(name, NULL);
	if (escaped) {
		g_string_append_printf(self->priv->string,
				"    \"logger\": \"%s\"",
				escaped);
		g_free(escaped);
		delim = TRUE;
	}

	g_string_append_printf(self->priv->string,
			"%s    \"timestamp\": %lu",
			delim ? ",\n" : "", (gulong)t);

	escaped = strescape(log4g_level_to_string(level), NULL);
	if (escaped) {
		g_string_append_printf(self->priv->string,
				",\n    \"level\": \"%s\"",
				escaped);
		g_free(escaped);
	}

	escaped = strescape(log4g_logging_event_get_thread_name(event), NULL);
	if (escaped) {
		g_string_append_printf(self->priv->string,
				",\n    \"thread\": \"%s\"",
				escaped);
		g_free(escaped);
	}

	escaped = strescape(log4g_logging_event_get_rendered_message(event), NULL);
	if (escaped) {
		g_string_append_printf(self->priv->string,
				",\n    \"message\": \"%s\"",
				escaped);
		g_free(escaped);
	}

	if (log4g_logging_event_get_ndc(event)) {
		escaped = strescape(log4g_logging_event_get_ndc(event), NULL);
		if (escaped) {
			g_string_append_printf(self->priv->string,
					",\n    \"ndc\": \"%s\"",
					escaped);
			g_free(escaped);
		}
	}

	if (self->priv->info) {
		guint64 line_number;
		g_string_append(self->priv->string, ",\n    \"locationInfo\": {\n");
		escaped = strescape(log4g_logging_event_get_file_name(event), NULL);
		if (escaped) {
			g_string_append_printf(self->priv->string,
					"      \"file\": \"%s\",\n",
					escaped);
			g_free(escaped);
		}
		errno = 0;
		line_number = g_ascii_strtoull(log4g_logging_event_get_line_number(event),
				NULL, 10);
		if (!errno) {
			g_string_append_printf(self->priv->string,
					"      \"line\": %"G_GUINT64_FORMAT",\n",
					line_number);
		}
		g_string_append_printf(self->priv->string,
				"      \"function\": \"%s\"\n",
				log4g_logging_event_get_function_name(event));
		g_string_append(self->priv->string, "    }");
	}

	if (self->priv->properties) {
		const GArray *keyset = log4g_logging_event_get_property_key_set(event);
		if (keyset && keyset->len) {
			const gchar *value;
			gchar *key;
			guint i;
			g_string_append(self->priv->string,
					",\n    \"properties\": [\n");
			for (i = 0; i < keyset->len; ++i) {
				key = g_array_index(keyset, gchar *, i);
				value = log4g_logging_event_get_mdc(event, key);
				if (!key || !escaped) {
					continue;
				}
				key = strescape(key, NULL);
				escaped = strescape(value, NULL);
				if (!key || !escaped) {
					g_free(key);
					g_free(escaped);
					continue;
				}
				g_string_append_printf(self->priv->string,
						"      {\n"
						"        \"name\": \"%s\",\n"
						"        \"value\": \"%s\"\n"
						"      }",
						key, escaped);
				g_free(key);
				g_free(escaped);
				if (i + 1 < keyset->len) {
					g_string_append_c(self->priv->string, ',');
				}
				g_string_append_c(self->priv->string, '\n');
			}
			g_string_append(self->priv->string, "    ]");
		}
	}

	g_string_append(self->priv->string, "\n  }");
	return self->priv->string->str;
}

static const gchar *
get_content_type(G_GNUC_UNUSED Log4gLayout *base)
{
	return "application/json";
}

static const gchar *
get_header(Log4gLayout *base)
{
	Log4gJsonLayout *self = LOG4G_JSON_LAYOUT(base);
	if (!self->priv->complete) {
		return "";
	}
	return "[";
}

static const gchar *
get_footer(G_GNUC_UNUSED Log4gLayout *base)
{
	Log4gJsonLayout *self = LOG4G_JSON_LAYOUT(base);
	if (!self->priv->complete) {
		return "";
	}
	return "\n]\n";
}

static void log4g_json_layout_class_init(Log4gJsonLayoutClass *klass)
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
		g_param_spec_boolean("properties",
			Q_("Properties"),
			Q_("Toggle MDC properties"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_LOCATION_INFO,
		g_param_spec_boolean("location-info",
			Q_("Location Information"),
			Q_("Toggle location information"),
			TRUE, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_COMPLETE,
		g_param_spec_boolean("complete",
			Q_("Complete Document"),
			Q_("Toggle writing a complete document"),
			TRUE, G_PARAM_WRITABLE));
}

static void
log4g_json_layout_class_finalize(G_GNUC_UNUSED Log4gJsonLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_json_layout_register(GTypeModule *module)
{
	log4g_json_layout_register_type(module);
}
