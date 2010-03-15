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
 * \brief Implements the API in log4g/appender/console-appender.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#include "config.h"
#include "log4g/appender/console-appender.h"
#include <unistd.h>

enum _properties_t {
    PROP_O = 0,
    PROP_TARGET,
    PROP_FOLLOW,
    PROP_MAX
};

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_CONSOLE_APPENDER, \
                                 struct Log4gPrivate))

struct Log4gPrivate {
    gchar *target;
    gboolean follow;
};

static void
activate_options(Log4gOptionHandler *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gConsoleAppenderClass *klass = LOG4G_CONSOLE_APPENDER_GET_CLASS(base);
    if (priv->follow) {
        if (g_ascii_strcasecmp(priv->target, klass->SYSTEM_OUT)) {
            log4g_writer_appender_set_writer(LOG4G_APPENDER(base), stdout);
        } else {
            log4g_writer_appender_set_writer(LOG4G_APPENDER(base), stderr);
        }
    } else {
        int fd;
        if (g_ascii_strcasecmp(priv->target, klass->SYSTEM_OUT)) {
            fd = dup(fileno(stdout));
        } else {
            fd = dup(fileno(stderr));
        }
        log4g_writer_appender_set_writer(LOG4G_APPENDER(base), fdopen(fd, "w"));
    }
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface)
{
    interface->activate_options = activate_options;
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_skeleton_get_closed(base)) {
        if (!priv->follow) {
            Log4gAppenderInterface *interface =
                    LOG4G_APPENDER_GET_INTERFACE(base);
            Log4gAppenderInterface *parent_interface =
                    g_type_interface_peek_parent(interface);
            parent_interface->close(base);
        }
    }
}

static void
appender_init(Log4gAppenderInterface *interface)
{
    interface->close = _close;
}

G_DEFINE_TYPE_WITH_CODE(Log4gConsoleAppender, log4g_console_appender,
                        LOG4G_TYPE_WRITER_APPENDER,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init)
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

static void
log4g_console_appender_init(Log4gConsoleAppender *self)
{
    Log4gConsoleAppenderClass *klass = LOG4G_CONSOLE_APPENDER_GET_CLASS(self);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->target = klass->SYSTEM_OUT;
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
        if (g_ascii_strcasecmp(target, klass->SYSTEM_OUT)) {
            priv->target = klass->SYSTEM_OUT;
        } else if (g_ascii_strcasecmp(target, klass->SYSTEM_ERR)) {
            priv->target = klass->SYSTEM_ERR;
        } else {
            log4g_log_warn(Q_("[%s] should be stdout or stderr"), target);
            log4g_log_warn(Q_("using previously set target, "
                        "stdout by default"));
            if (!priv->target) {
                priv->target = klass->SYSTEM_OUT;
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
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gWriterAppenderClass *writer_class = LOG4G_WRITER_APPENDER_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize WriterAppenderClass */
    writer_class->close_writer = close_writer;
    /* initialize class data */
    klass->SYSTEM_OUT = g_strdup("stdout");
    klass->SYSTEM_ERR = g_strdup("stderr");
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_TARGET,
            g_param_spec_string("target", Q_("Target"),
                    Q_("Output target (\"stdout\" or \"stderr\")"),
                    "stdout", G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_FOLLOW,
            g_param_spec_boolean("follow", Q_("Follow"),
                    Q_("Output follows freopen()"), FALSE, G_PARAM_WRITABLE));
}

Log4gAppender *
log4g_console_appender_new(Log4gLayout *layout, const char *target)
{
    Log4gAppender *self;
    g_return_val_if_fail(layout, NULL);
    self = g_object_new(LOG4G_TYPE_CONSOLE_APPENDER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_appender_set_layout(self, layout);
    log4g_console_appender_set_target(self, (target ? target : "stdout"));
    log4g_appender_activate_options(self);
    return self;
}

void
log4g_console_appender_set_target(Log4gAppender *base, const gchar *target)
{
    g_return_if_fail(LOG4G_IS_CONSOLE_APPENDER(base));
    g_object_set(base, "target", target, NULL);
}

const gchar *
log4g_console_appender_get_target(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_CONSOLE_APPENDER(base), NULL);
    return GET_PRIVATE(base)->target;
}

void
log4g_console_appender_set_follow(Log4gAppender *base, gboolean follow)
{
    g_return_if_fail(LOG4G_IS_CONSOLE_APPENDER(base));
    g_object_set(base, "follow", follow, NULL);
}

gboolean
log4g_console_appender_get_follow(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_CONSOLE_APPENDER(base), FALSE);
    return GET_PRIVATE(base)->follow;
}
