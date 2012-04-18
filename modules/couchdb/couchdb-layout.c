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
 * SECTION: couchdb-layout
 * @short_description: Format events into a CouchDB document
 * @see_also: <ulink url="http://www.couchdb.apache.org/">Apache CouchDB</ulink>,
 *            <ulink url="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</ulink>
 *
 * This class formats a log event into a CouchDB document with the following
 * schema:
 *
 * |[
 * {
 *    "description": "Log4g-CouchDB JSON-Schema",
 *    "type": "object",
 *    "properties": {
 *        "_id": {
 *            "description": "Unique ID for this document, internal to CouchDB",
 *            "type": "string"
 *        },
 *        "_rev": {
 *            "description": "Revision for this document, internal to CouchDB",
 *            "type": "string"
 *        },
 *        "record_type": {
 *            "description": "The schema URI",
 *            "type": "string",
 *            "format": "uri"
 *        },
 *        "record_type_version": {
 *            "description": "The schema version number",
 *            "type": "string"
 *        },
 *        "message": {
 *            "description": "The log message",
 *            "type": "string"
 *        },
 *        "level": {
 *            "description": "The log level of the logging event",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "logger": {
 *            "description": "The name of the logger",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "mdc": {
 *            "description": "The mapped data context",
 *            "type": "object",
 *            "properties": {
 *                "type": { "type": "string" },
 *                "value": { "type": "string" },
 *            },
 *            "optional": true
 *        },
 *        "ndc": {
 *            "description": "The nested data context",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "timestamp": {
 *            "description": "The timestamp of the logging event",
 *            "type": "string",
 *            "format": "utc-millisec",
 *            "optional": true
 *        },
 *        "thread": {
 *            "description": "The thread where the event was logged",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "function": {
 *            "description": "The function name",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "file": {
 *            "description": "The file name",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "line": {
 *            "description": "The line number",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "application_annotations": {
 *            "description": "Application-specific data",
 *            "type": "any",
 *            "optional": true
 *        }
 *    }
 * }
 * ]|
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <couchdb-document.h>
#include "layout/couchdb-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gCouchdbLayout, log4g_couchdb_layout,
		LOG4G_TYPE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_COUCHDB_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gCouchdbLayout *)instance)->priv)

struct Private {
	gchar *string;
};

static void
log4g_couchdb_layout_init(Log4gCouchdbLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->string);
	G_OBJECT_CLASS(log4g_couchdb_layout_parent_class)->finalize(base);
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->string);
	CouchdbDocument *document =
		log4g_couchdb_layout_format_document(base, event);
	if (!document) {
		return NULL;
	}
	priv->string = couchdb_document_to_string(document);
	g_object_unref(document);
	return priv->string;
}

static CouchdbDocument *
format_document(Log4gLayout *base, Log4gLoggingEvent *event)
{
	CouchdbDocument *document = couchdb_document_new();
	if (!document) {
		return NULL;
	}
	/* destopcouch schema */
	couchdb_document_set_string_field(document, "record_type",
			"http://msteinert.github.com/log4g/couchdb");
	couchdb_document_set_string_field(document, "record_type_version",
			"1.0");
	/* message */
	const gchar *string = log4g_logging_event_get_message(event);
	couchdb_document_set_string_field(document, "message",
			string ? string : "");
	/* log level */
	Log4gLevel *level = log4g_logging_event_get_level(event);
	if (level) {
		couchdb_document_set_string_field(document, "level",
				log4g_level_to_string(level));
	}
	/* logger name */
	string = log4g_logging_event_get_logger_name(event);
	if (string) {
		couchdb_document_set_string_field(document, "logger", string);
	}
	/* MDC */
	const GArray *keys = log4g_logging_event_get_property_key_set(event);
	if (keys) {
		CouchdbStructField *mdc = couchdb_struct_field_new();
		if (mdc) {
			for (gint i = 0; i < keys->len; ++i) {
				string = g_array_index(keys, const gchar *, i);
				couchdb_struct_field_set_string_field(
						mdc, string,
						log4g_logging_event_get_mdc(
							event, string));
			}
			couchdb_document_set_struct_field(document, "mdc", mdc);
		}
	}
	/* NDC */
	string = log4g_logging_event_get_ndc(event);
	if (string) {
		couchdb_document_set_string_field(document, "ndc", string);
	}
	/* timestamp */
	GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
	if (tv) {
		gchar string[64];
		g_snprintf(string, sizeof string, "%lu",
				(gulong)((tv->tv_sec * 1000)
					+ (tv->tv_usec * 0.001)));
		couchdb_document_set_string_field(document, "timestamp",
				string);
	}
	/* thread */
	string = log4g_logging_event_get_thread_name(event);
	if (string) {
		couchdb_document_set_string_field(document, "thread", string);
	}
	/* function */
	string = log4g_logging_event_get_function_name(event);
	if (string) {
		couchdb_document_set_string_field(document, "function", string);
	}
	/* file */
	string = log4g_logging_event_get_file_name(event);
	if (string) {
		couchdb_document_set_string_field(document, "file", string);
	}
	/* line */
	string = log4g_logging_event_get_line_number(event);
	if (string) {
		couchdb_document_set_string_field(document, "line", string);
	}
	return document;
}

static void
activate_options(G_GNUC_UNUSED Log4gLayout *base)
{
	/* do nothing */
}

static void
log4g_couchdb_layout_class_init(Log4gCouchdbLayoutClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	object_class->finalize = finalize;
	layout_class->format = format;
	layout_class->activate_options = activate_options;
	klass->format_document = format_document;
	g_type_class_add_private(klass, sizeof(struct Private));
}

static void
log4g_couchdb_layout_class_finalize(
		G_GNUC_UNUSED Log4gCouchdbLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_couchdb_layout_register(GTypeModule *module)
{
	log4g_couchdb_layout_register_type(module);
}

/**
 * log4g_couchdb_layout_format_document:
 * @base: A CouchDB layout object.
 * @event: The logging event object to be formatted into a
 *         CouchDB document.
 *
 * Call the @format_document function from the #Log4gCouchdbLayoutClass
 * of @base.
 *
 * Returns: A logging event formatted into a CouchdbDocument object.
 * Since: 0.1
 */
CouchdbDocument *
log4g_couchdb_layout_format_document(Log4gLayout *base,
		Log4gLoggingEvent *event)
{
	g_return_val_if_fail(LOG4G_IS_COUCHDB_LAYOUT(base), NULL);
	return LOG4G_COUCHDB_LAYOUT_GET_CLASS(base)->
		format_document(base, event);
}
