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
 * \brief Implements the API in log4g/appender/syslog-appender.h
 * \author Mike Steinert
 * \date 2-11-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/appender/syslog-appender.h"
#include <syslog.h>

enum _properties_t {
    PROP_O = 0,
    PROP_IDENT,
    PROP_OPTION,
    PROP_FACILITY,
    PROP_MAX
};

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_SYSLOG_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gchar *ident;
    gint option;
    gint facility;
};

static void
activate_options(Log4gOptionHandler *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    openlog(priv->ident, priv->option, priv->facility);
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface, gpointer data)
{
    interface->activate_options = activate_options;
}

static void
_close(Log4gAppender *base)
{
    if (!log4g_appender_skeleton_get_closed(base)) {
        log4g_appender_skeleton_set_closed(base, TRUE);
        closelog();
    }
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return TRUE;
}

static void
appender_init(Log4gAppenderInterface *interface)
{
    interface->close = _close;
    interface->requires_layout = requires_layout;
}

G_DEFINE_TYPE_WITH_CODE(Log4gSyslogAppender, log4g_syslog_appender,
        LOG4G_TYPE_APPENDER_SKELETON,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init)
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

static void
log4g_syslog_appender_init(Log4gSyslogAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->ident = NULL;
    priv->option = 0;
    priv->facility = 0;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_appender_close(LOG4G_APPENDER(base));
    if (priv->ident) {
        g_free(priv->ident);
        priv->ident = NULL;
    }
    G_OBJECT_CLASS(log4g_syslog_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *ident;
    switch (id) {
    case PROP_IDENT:
        if (priv->ident) {
            g_free(priv->ident);
        }
        ident = g_value_get_string(value);
        if (ident) {
            priv->ident = g_strdup(ident);
        } else {
            priv->ident = NULL;
        }
        break;
    case PROP_OPTION:
        priv->option = g_value_get_int(value);
        break;
    case PROP_FACILITY:
        priv->option = g_value_get_int(value);
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
    Log4gLevel *level = log4g_logging_event_get_level(event);
    if (layout && level) {
        syslog(log4g_level_get_syslog_equivalent(level), "%s",
                log4g_layout_format(layout, event));
    }
}

static void
log4g_syslog_appender_class_init(Log4gSyslogAppenderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gAppenderSkeletonClass *skeleton_class =
            LOG4G_APPENDER_SKELETON_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderSkeleton */
    skeleton_class->append = append;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_IDENT,
            g_param_spec_string("ident", Q_("Ident"),
                    Q_("Syslog ident parameter"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_OPTION,
            g_param_spec_int("option", Q_("Option"),
                    Q_("Syslog option parameter"), 0, G_MAXINT, 0,
                    G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_FACILITY,
            g_param_spec_int("facility", Q_("Facility"),
                    Q_("Syslog facility parameter"), 0, G_MAXINT, 0,
                    G_PARAM_WRITABLE));
}

Log4gAppender *
log4g_syslog_appender_new(Log4gLayout *layout, const char *ident, gint option,
        gint facility)
{
    Log4gAppender *self = g_object_new(LOG4G_TYPE_SYSLOG_APPENDER, NULL);
    if (!self) {
        return NULL;
    }
    if (layout) {
        log4g_syslog_appender_set_ident(self, ident);
    }
    if (option) {
        log4g_syslog_appender_set_option(self, option);
    }
    if (facility) {
        log4g_syslog_appender_set_facility(self, facility);
    }
    log4g_appender_set_layout(self, layout);
    log4g_option_handler_activate_options(LOG4G_OPTION_HANDLER(self));
    return self;
}

void
log4g_syslog_appender_set_ident(Log4gAppender *base, const gchar *ident)
{
    g_return_if_fail(LOG4G_IS_SYSLOG_APPENDER(base));
    g_object_set(base, "ident", ident, NULL);
}

const gchar *
log4g_syslog_appender_get_ident(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_SYSLOG_APPENDER(base), NULL);
    return GET_PRIVATE(base)->ident;
}

void
log4g_syslog_appender_set_option(Log4gAppender *base, gint option)
{
    g_return_if_fail(LOG4G_IS_SYSLOG_APPENDER(base));
    g_object_set(base, "option", option, NULL);
}

gint
log4g_syslog_appender_get_option(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_SYSLOG_APPENDER(base), 0);
    return GET_PRIVATE(base)->option;
}

void
log4g_syslog_appender_set_facility(Log4gAppender *base, gint facility)
{
    g_return_if_fail(LOG4G_IS_SYSLOG_APPENDER(base));
    g_object_set(base, "facility", facility, NULL);
}

gint
log4g_syslog_appender_get_facility(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_SYSLOG_APPENDER(base), 0);
    return GET_PRIVATE(base)->facility;
}
