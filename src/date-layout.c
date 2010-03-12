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
 * \brief Implements the API in log4g/helpers/date-layout.h
 * \author Mike Steinert
 * \date 2-5-2010
 */

#include "config.h"
#include <errno.h>
#include "log4g/helpers/date-layout.h"
#include <stdlib.h>

enum _properties_t {
    PROP_O = 0,
    PROP_DATE_FORMAT,
    PROP_TIME_ZONE,
    PROP_MAX
};

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_DATE_LAYOUT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    Log4gDateLayoutType type;
    gchar *format;
    gchar *tz;
};

static void
activate_options(Log4gOptionHandler *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->tz) {
        if (setenv("TZ", priv->tz, 1)) {
            log4g_log_error("setenv(): %s", g_strerror(errno));
        }
    }
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface)
{
    interface->activate_options = activate_options;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDateLayout, log4g_date_layout, LOG4G_TYPE_LAYOUT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

static void
log4g_date_layout_init(Log4gDateLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->type = RELATIVE_TIME_DATE_FORMAT;
    priv->format = NULL;
    priv->tz = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->format) {
        g_free(priv->format);
        priv->format = NULL;
    }
    if (priv->tz) {
        g_free(priv->tz);
        priv->tz = NULL;
    }
    G_OBJECT_CLASS(log4g_date_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *string;
    switch (id) {
    case PROP_DATE_FORMAT:
        if (priv->format) {
            g_free(priv->format);
        }
        string = g_value_get_string(value);
        if (string) {
            priv->format = g_strdup(string);
        } else {
            priv->type = RELATIVE_TIME_DATE_FORMAT;
            priv->format = NULL;
        }
        break;
    case PROP_TIME_ZONE:
        if (priv->tz) {
            g_free(priv->tz);
        }
        string = g_value_get_string(value);
        if (string) {
            priv->tz = g_strdup(string);
        } else {
            priv->tz = NULL;
        }
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
log4g_date_layout_class_init(Log4gDateLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_DATE_FORMAT,
            g_param_spec_string("date-format", Q_("Date Format"),
                    Q_("Date formatting string"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_TIME_ZONE,
            g_param_spec_string("time-zone", Q_("Time Zone"),
                    Q_("Time zone string"), NULL, G_PARAM_WRITABLE));
}

void
log4g_date_layout_set_date_format(Log4gLayout *base, const char *format)
{
    g_return_if_fail(LOG4G_IS_DATE_LAYOUT(base));
    g_object_set(base, "date-format", format, NULL);
}

const char *
log4g_date_layout_get_date_format(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_DATE_LAYOUT(base), NULL);
    return GET_PRIVATE(base)->format;
}

void
log4g_date_layout_set_time_zone(Log4gLayout *base, const char *tz)
{
    g_return_if_fail(LOG4G_IS_DATE_LAYOUT(base));
    g_object_set(base, "time-zone", tz, NULL);
}

const char *
log4g_date_layout_get_time_zone(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_DATE_LAYOUT(base), NULL);
    return GET_PRIVATE(base)->tz;
}

void
log4g_date_layout_date_format(Log4gLayout *base, GString *string,
                              Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv;
    g_return_if_fail(LOG4G_IS_DATE_LAYOUT(base));
    priv = GET_PRIVATE(base);
    struct timeval *tv;
    gchar buffer[128];
    struct tm tm;
    time_t time;
    tv = log4g_logging_event_get_time_stamp(event);
    if (!tv) {
        return;
    }
    if (priv->format) {
        time = tv->tv_sec;
        if (!localtime_r(&time, &tm)) {
            log4g_log_error("localtime_r(): %s", g_strerror(errno));
            return;
        }
        if (!strftime(buffer, sizeof(buffer), priv->format, &tm)) {
            return;
        }
    } else {
        switch (priv->type) {
        case RELATIVE_TIME_DATE_FORMAT: {
            glong start = log4g_logging_event_get_start_time();
            glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
            g_snprintf(buffer, sizeof(buffer), "%ld", time - start);
            break;
        }
        default:
            log4g_log_error(Q_("unrecognized date layout type: %d"),
                    priv->type);
            return;
        }
    }
    g_string_append(string, buffer);
}
