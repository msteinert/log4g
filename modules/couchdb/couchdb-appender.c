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
 * SECTION: couchdb-appender
 * @short_description: Log events to an Apache CouchDB
 * @see_also: <ulink url="http://www.couchdb.apache.org/">Apache CouchDB</ulink>,
 *            <ulink url="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</ulink>
 *
 * The CouchDB appender logs events to a specified database using a
 * #Log4gCouchdbLayoutClass.
 *
 * CouchDB appenders accept the following properties:
 * <orderedlist>
 * <listitem><para>uri</para></listitem>
 * <listitem><para>database-name</para></listitem>
 * <listitem><para>credentials</para></listitem>
 * </orderedlist>
 *
 * The value of uri determines the network location of the CouchDB where
 * output will be logged. If the value of uri is %NULL then this appender
 * will attempt to log messages to the CouchDB at &lt;http://127.0.0.1:5984&gt;.
 *
 * The value of database-name determines the name of the database that will
 * store the log output. The default value is "log4g_messages".
 *
 * The credentials property enables authentication with the CouchDB server.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/couchdb-appender.h"
#include "layout/couchdb-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gCouchdbAppender, log4g_couchdb_appender,
        LOG4G_TYPE_APPENDER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_COUCHDB_APPENDER, \
            struct Log4gPrivate))

enum _properties_t {
    PROP_O = 0,
    PROP_URI,
    PROP_DATABASE_NAME,
    PROP_CREDENTIALS,
    PROP_MAX
};

struct Log4gPrivate {
    gchar *uri; /* CouchDB server */
    gchar *name; /* database name */
    CouchdbSession *session;
    CouchdbDatabase *database;
    CouchdbCredentials *credentials;
};

static void
log4g_couchdb_appender_init(Log4gCouchdbAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->uri = NULL;
    priv->name = g_strdup("log4g_messages");
    priv->session = NULL;
    priv->database = NULL;
    priv->credentials = NULL;
    Log4gLayout *layout = g_object_new(LOG4G_TYPE_COUCHDB_LAYOUT, NULL);
    if (layout) {
        log4g_appender_set_layout(LOG4G_APPENDER(self), layout);
    }
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->database) {
        g_object_unref(priv->database);
        priv->database = NULL;
    }
    if (priv->session) {
        g_object_unref(priv->session);
        priv->session = NULL;
    }
    if (priv->credentials) {
        g_object_unref(priv->credentials);
        priv->credentials = NULL;
    }
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->uri);
    priv->uri = NULL;
    g_free(priv->name);
    priv->name = NULL;
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_URI:
        g_free(priv->uri);
        priv->uri = g_value_dup_string(value);
        break;
    case PROP_DATABASE_NAME:
        g_free(priv->name);
        priv->name = g_value_dup_string(value);
        break;
    case PROP_CREDENTIALS:
        if (priv->credentials) {
            g_object_unref(priv->credentials);
        }
        priv->credentials = g_value_dup_object(value);
        if (priv->session) {
            couchdb_session_enable_authentication(priv->session,
                    priv->credentials);
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    Log4gLayout *layout = log4g_appender_get_layout(base);
    if (!layout) {
        return;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!priv->session) {
        log4g_log_error(Q_("attempted to append to a disconnected CouchDB "
                    "appender named [%s]"), log4g_appender_get_name(base));
        return;
    }
    if (!priv->name || !priv->database) {
        log4g_log_error(Q_("attempted to append to a CouchDB appender named "
                "[%s] without specifying a database"),
                log4g_appender_get_name(base));
        return;
    }
    if (!LOG4G_IS_COUCHDB_LAYOUT(layout)) {
        return;
    }
    CouchdbDocument *document =
        log4g_couchdb_layout_format_document(layout, event);
    if (!document) {
        return;
    }
    GError *error = NULL;
    if (!couchdb_database_put_document(priv->database, document, &error)) {
        log4g_log_error(Q_("failed to store document in database %s: %s"),
                priv->name, error->message);
        g_error_free(error);
    }
    g_object_unref(document);
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_get_closed(base)) {
        log4g_appender_set_closed(base, TRUE);
        if (priv->session) {
            g_object_unref(priv->session);
            priv->session = NULL;
        }
    }
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return FALSE;
}

static void
activate_options(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->session) {
        g_object_unref(priv->session);
    }
    priv->session = couchdb_session_new(priv->uri);
    if (!priv->session) {
        log4g_log_error("couchdb_session_new() returned NULL");
        return;
    }
    log4g_log_debug("connected to CouchDB at %s",
            couchdb_session_get_uri(priv->session));
    if (priv->credentials) {
        couchdb_session_enable_authentication(priv->session,
                priv->credentials);
    }
    if (!priv->name) {
        log4g_log_warn("database not set for CouchDB connection at %s",
                couchdb_session_get_uri(priv->session));
        return;
    }
    GError *error = NULL;
    CouchdbDatabaseInfo *info =
        couchdb_session_get_database_info(priv->session, priv->name, &error);
    if (!info) {
        log4g_log_debug("%s: %s", priv->name, error->message);
        g_error_free(error);
        priv->database = couchdb_database_new(priv->session, priv->name);
        if (!priv->database) {
            log4g_log_error("failed to create database %s: %s", priv->name,
                    error->message);
        }
        log4g_log_debug("%s: created new database", priv->name);
        return;
    }
    log4g_log_debug("%s: %d documents", couchdb_database_info_get_dbname(info),
            couchdb_database_info_get_documents_count(info));
    couchdb_database_info_unref(info);
}

static void
log4g_couchdb_appender_class_init(Log4gCouchdbAppenderClass *klass)
{
    /* initialize GObjectClass */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->append = append;
    appender_class->close = _close;
    appender_class->requires_layout = requires_layout;
    appender_class->activate_options = activate_options;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_URI,
            g_param_spec_string("uri", Q_("CouchDB server URI"),
                    Q_("URI of a CouchDB server"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_DATABASE_NAME,
            g_param_spec_string("database-name", Q_("Database name"),
                    Q_("Name of a CouchDB database"), "log4g_messages",
                    G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_CREDENTIALS,
            g_param_spec_object("credentials", Q_("CouchDB credentials"),
                    Q_("Credentials for a CouchDB server"),
                    COUCHDB_TYPE_CREDENTIALS, G_PARAM_WRITABLE));
}

static void
log4g_couchdb_appender_class_finalize(Log4gCouchdbAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_couchdb_appender_register(GTypeModule *module)
{
    log4g_couchdb_layout_register(module);
    log4g_couchdb_appender_register_type(module);
}
