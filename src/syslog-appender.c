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

#include "config.h"
#include "log4g/appender/syslog-appender.h"
#include "log4g/interface/error-handler.h"
#include <string.h>
#include <syslog.h>

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
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_SYSLOG_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gchar *ident;
    gint option;
    gint facility;
};

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
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    va_list ap;
    memset(&ap, 0, sizeof(ap));
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gLayout *layout = log4g_appender_get_layout(base);
    const char *message = log4g_layout_format(layout, event);
    Log4gLevel *level = log4g_logging_event_get_level(event);
    memset(ap, 0, sizeof(ap));
    vsyslog(log4g_level_get_syslog_equivalent(level) | priv->facility,
            message, ap);
}
static void
log4g_syslog_appender_class_init(Log4gSyslogAppenderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gAppenderSkeletonClass *skeleton_class =
            LOG4G_APPENDER_SKELETON_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize Log4gAppenderSkeleton */
    skeleton_class->append = append;
}

Log4gAppender *
log4g_syslog_appender_new(Log4gLayout *layout, const char *ident, gint option,
        gint facility)
{
    struct Log4gPrivate *priv;
    Log4gAppender *self = g_object_new(LOG4G_TYPE_SYSLOG_APPENDER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_appender_set_layout(self, layout);
    priv = GET_PRIVATE(self);
    priv->ident = g_strdup(ident);
    if (!priv->ident) {
        g_object_unref(self);
        return NULL;
    }
    priv->option = option;
    priv->facility = facility;
    openlog(ident, option, facility);
    return self;
}
