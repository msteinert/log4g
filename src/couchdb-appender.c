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
 * \brief Implements the API in log4g/appender/couchdb-appender.h
 * \author Mike Steinert
 * \date 5-20-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "couchdb-glib.h"
#include "log4g/appender/couchdb-appender.h"
#include "log4g/layout/couchdb-layout.h"

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_COUCHDB_APPENDER, \
            struct Log4gPrivate))

enum _properties_t {
    PROP_O = 0,
    PROP_URI,
    PROP_NAME,
    PROP_MAX
};

struct Log4gPrivate {
    gchar *uri; /**< CouchDB server */
    gchar *name; /**< database name */
    CouchdbSession *session;
};

static void
activate_options(Log4gOptionHandler *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->session) {
        g_object_unref(priv->session);
    }
    priv->session = couchdb_session_new(priv->uri);
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = activate_options;
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_skeleton_get_closed(base)) {
        log4g_appender_skeleton_set_closed(base, TRUE);
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
appender_init(Log4gAppenderInterface *interface)
{
    interface->close = _close;
    interface->requires_layout = requires_layout;
}

G_DEFINE_TYPE_WITH_CODE(Log4gCouchdbAppender, log4g_couchdb_appender,
        LOG4G_TYPE_APPENDER_SKELETON,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init)
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

static void
log4g_couchdb_appender_init(Log4gCouchdbAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->uri = NULL;
    priv->session = NULL;
    Log4gLayout *layout = log4g_couchdb_layout_new(NULL);
    if (layout) {
        log4g_appender_set_layout(LOG4G_APPENDER(self), layout);
    }
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->session) {
        g_object_unref(priv->session);
        priv->session = NULL;
    }
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->uri) {
        g_free(priv->uri);
        priv->uri = NULL;
    }
    if (priv->name) {
        g_free(priv->name);
        priv->name = NULL;
    }
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->finalize(base);
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
    if (!priv->name) {
        log4g_log_error(Q_("attempted to append to a CouchDB appender named "
                "[%s] without specifying a database"),
                log4g_appender_get_name(base));
        return;
    }
    if (LOG4G_IS_COUCHDB_LAYOUT(layout)) {
        CouchdbDocument *document =
            log4g_couchdb_layout_format_document(layout, event, priv->session);
        if (document) {
            GError *error = NULL;
            if (!couchdb_document_put(document, priv->name, &error)) {
                log4g_log_error(Q_("failed to store document in database "
                        "%s: %s"), priv->name, error->message);
                g_error_free(error);
            }
            g_object_unref(document);
        }
    }
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_URI:
        if (priv->uri) {
            g_free(priv->uri);
        }
        const gchar *uri = g_value_get_string(value);
        if (uri) {
            priv->uri = g_strdup(uri);
        } else {
            priv->uri = NULL;
        }
        break;
    case PROP_NAME:
        if (priv->name) {
            g_free(priv->name);
        }
        const gchar *name = g_value_get_string(value);
        if (name) {
            priv->name = g_strdup(name);
        } else {
            priv->name = NULL;
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
log4g_couchdb_appender_class_init(Log4gCouchdbAppenderClass *klass)
{
    /* initialize GObject */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderSkeleton */
    Log4gAppenderSkeletonClass *skeleton_class =
        LOG4G_APPENDER_SKELETON_CLASS(klass);
    skeleton_class->append = append;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_URI,
            g_param_spec_string("uri", Q_("URI"),
                    Q_("URI of the CouchDB server"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_NAME,
            g_param_spec_string("name", Q_("Name"),
                    Q_("Name of a CouchDB database"), NULL, G_PARAM_WRITABLE));
}

Log4gAppender *
log4g_couchdb_appender_new(const gchar *uri, const gchar *name)
{
    Log4gAppender *self = g_object_new(LOG4G_TYPE_COUCHDB_APPENDER, NULL);
    if (!self) {
        return NULL;
    }
    if (uri) {
        g_object_set(self, "uri", uri, NULL);
    }
    if (name) {
        g_object_set(self, "name", name, NULL);
    }
    log4g_option_handler_activate_options(LOG4G_OPTION_HANDLER(self));
    return self;
}
