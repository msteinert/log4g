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
 * \brief Implements the API in console-appender.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/console-appender.h"
#include <unistd.h>

enum _properties_t {
    PROP_O = 0,
    PROP_TARGET,
    PROP_FOLLOW,
    PROP_MAX
};

G_DEFINE_DYNAMIC_TYPE(Log4gConsoleAppender, log4g_console_appender,
        LOG4G_TYPE_WRITER_APPENDER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_CONSOLE_APPENDER, \
            struct Log4gPrivate))

/** \brief System output identifier */
#define SYSTEM_OUT "stdout"

/** \brief System error identifier */
#define SYSTEM_ERR "stderr"

struct Log4gPrivate {
    gchar *target;
    gboolean follow;
};

static void
log4g_console_appender_init(Log4gConsoleAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->target = SYSTEM_OUT;
    priv->follow = FALSE;
}

static void
finalize(GObject *base)
{
    log4g_appender_close(LOG4G_APPENDER(base));
    G_OBJECT_CLASS(log4g_console_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gConsoleAppenderClass *klass;
    gchar *target;
    switch (id) {
    case PROP_TARGET:
        target = g_value_dup_string(value);
        if (!target) {
            break;
        }
        g_strstrip(target);
        klass = LOG4G_CONSOLE_APPENDER_GET_CLASS(base);
        if (g_ascii_strcasecmp(target, SYSTEM_OUT)) {
            priv->target = SYSTEM_OUT;
        } else if (g_ascii_strcasecmp(target, SYSTEM_ERR)) {
            priv->target = SYSTEM_ERR;
        } else {
            log4g_log_warn(Q_("[%s] should be %s or %s"),
                    target, SYSTEM_OUT, SYSTEM_ERR);
            log4g_log_warn(Q_("using previously set target, %s by default"),
                    SYSTEM_OUT);
            if (!priv->target) {
                priv->target = SYSTEM_OUT;
            }
        }
        g_free(target);
        break;
    case PROP_FOLLOW:
        priv->follow = g_value_get_boolean(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_get_closed(base)) {
        if (!priv->follow) {
            LOG4G_APPENDER_CLASS(log4g_console_appender_parent_class)->
                close(base);
        }
    }
}

static void
activate_options(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->follow) {
        if (g_ascii_strcasecmp(priv->target, SYSTEM_OUT)) {
            log4g_writer_appender_set_writer(LOG4G_APPENDER(base), stdout);
        } else {
            log4g_writer_appender_set_writer(LOG4G_APPENDER(base), stderr);
        }
    } else {
        int fd;
        if (g_ascii_strcasecmp(priv->target, SYSTEM_OUT)) {
            fd = dup(fileno(stdout));
        } else {
            fd = dup(fileno(stderr));
        }
        log4g_writer_appender_set_writer(LOG4G_APPENDER(base),
                fdopen(fd, "w"));
    }
}

static void
close_writer(Log4gAppender *base)
{
    if (!GET_PRIVATE(base)->follow) {
        LOG4G_WRITER_APPENDER_CLASS(log4g_console_appender_parent_class)->
            close_writer(base);
    }
}

static void
log4g_console_appender_class_init(Log4gConsoleAppenderClass *klass)
{
    /* initialize GObject */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->close = _close;
    appender_class->activate_options = activate_options;
    /* initialize Log4gWriterAppenderClass */
    Log4gWriterAppenderClass *writer_class =
        LOG4G_WRITER_APPENDER_CLASS(klass);
    writer_class->close_writer = close_writer;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_TARGET,
            g_param_spec_string("target", Q_("Target"),
                    Q_("Output target (\"stdout\" or \"stderr\")"),
                    "stdout", G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_FOLLOW,
            g_param_spec_boolean("follow", Q_("Follow"),
                    Q_("Output follows freopen()"), FALSE, G_PARAM_WRITABLE));
}

static void
log4g_console_appender_class_finalize(Log4gConsoleAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_console_appender_register(GTypeModule *module)
{
    log4g_console_appender_register_type(module);
}
