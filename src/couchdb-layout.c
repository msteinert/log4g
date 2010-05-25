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
 * \brief Implements the API in log4g/layout/couchdb-layout.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "couchdb-document.h"
#include "log4g/layout/couchdb-layout.h"

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

G_DEFINE_DYNAMIC_TYPE_EXTENDED(Log4gCouchdbLayout, log4g_couchdb_layout,
        LOG4G_TYPE_LAYOUT, 0,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_COUCHDB_LAYOUT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gchar *string;
};

static void
log4g_couchdb_layout_init(Log4gCouchdbLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->string = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->string);
    priv->string = NULL;
    G_OBJECT_CLASS(log4g_couchdb_layout_parent_class)->finalize(base);
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->string);
    CouchdbDocument *document =
        log4g_couchdb_layout_format_document(base, event, NULL);
    if (!document) {
        return NULL;
    }
    priv->string = couchdb_document_to_string(document);
    g_object_unref(document);
    return priv->string;
}

static CouchdbDocument *
format_document(Log4gLayout *base, Log4gLoggingEvent *event,
        CouchdbSession *session)
{
    CouchdbDocument *document = couchdb_document_new(session);
    if (!document) {
        return NULL;
    }
    /* destopcouch schema */
    couchdb_document_set_string_field(document, "record_type",
            "http://msteinert.github.com/log4g/couchdb");
    couchdb_document_set_string_field(document, "record_type_version", "1.0");
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
                couchdb_struct_field_set_string_field(mdc, string,
                        log4g_logging_event_get_mdc(event, string));
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
    struct timeval *tv = log4g_logging_event_get_time_stamp(event);
    if (tv) {
        gchar string[64];
        g_snprintf(string, sizeof string, "%ld",
                (glong)((tv->tv_sec * 1000) + (tv->tv_usec * 0.001)));
        couchdb_document_set_string_field(document, "timestamp", string);
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
log4g_couchdb_layout_class_init(Log4gCouchdbLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayout class */
    layout_class->format = format;
    /* initialize Log4gCouchdbLayout class */
    klass->format_document = format_document;
}

static void
log4g_couchdb_layout_class_finalize(Log4gCouchdbLayoutClass *klass)
{
    /* do nothing */
}

void
log4g_couchdb_layout_register(GTypeModule *base)
{
    log4g_couchdb_layout_register_type(base);
}

Log4gLayout *
log4g_couchdb_layout_new(void)
{
    return g_object_new(LOG4G_TYPE_COUCHDB_LAYOUT, NULL);
}

CouchdbDocument *
log4g_couchdb_layout_format_document(Log4gLayout *base,
        Log4gLoggingEvent *event, CouchdbSession *session)
{
    g_return_val_if_fail(LOG4G_IS_COUCHDB_LAYOUT(base), NULL);
    return LOG4G_COUCHDB_LAYOUT_GET_CLASS(base)->
        format_document(base, event, session);
}
