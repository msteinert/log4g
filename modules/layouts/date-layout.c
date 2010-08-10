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
 * SECTION: date-layout
 * @short_description: A layout that formats dates
 *
 * This is an abstract layout class that takes care of all date related
 * options and formatting. This class understands date formats understood
 * by strftime(3).
 *
 * Date layouts accept two properties:
 * <orderedlist>
 * <listitem><para>date-format</para></listitem>
 * <listitem><para>time-zone</para></listitem>
 * </orderedlist>
 *
 * The date-format property set the strftime(3) conversion pattern that will
 * be used to format the date.
 *
 * The time-zone property can be used to explicitly set the timezone. The
 * default is %NULL.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "layout/date-layout.h"

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

G_DEFINE_DYNAMIC_TYPE_EXTENDED(Log4gDateLayout, log4g_date_layout,
        LOG4G_TYPE_LAYOUT, G_TYPE_FLAG_ABSTRACT, {})

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
    g_free(priv->format);
    priv->format = NULL;
    g_free(priv->tz);
    priv->tz = NULL;
    G_OBJECT_CLASS(log4g_date_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_DATE_FORMAT:
        g_free(priv->format);
        const gchar *format = g_value_get_string(value);
        if (format) {
            priv->format = g_strdup(format);
        } else {
            priv->type = RELATIVE_TIME_DATE_FORMAT;
            priv->format = NULL;
        }
        break;
    case PROP_TIME_ZONE:
        g_free(priv->tz);
        const gchar *tz = g_value_get_string(value);
        priv->tz = tz ? g_strdup(tz) : NULL;
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
activate_options(Log4gLayout *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->tz) {
        if (g_setenv("TZ", priv->tz, 1)) {
            log4g_log_error("g_setenv(): %s", g_strerror(errno));
        }
    }
}

static void
log4g_date_layout_class_init(Log4gDateLayoutClass *klass)
{
    /* initialize GObject class */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayoutClass */
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    layout_class->activate_options = activate_options;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_DATE_FORMAT,
            g_param_spec_string("date-format", Q_("Date Format"),
                    Q_("Date formatting string"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_TIME_ZONE,
            g_param_spec_string("time-zone", Q_("Time Zone"),
                    Q_("Time zone string"), NULL, G_PARAM_WRITABLE));
}

static void
log4g_date_layout_class_finalize(Log4gDateLayoutClass *klass)
{
    /* do nothing */
}

void
log4g_date_layout_register(GTypeModule *module)
{
    log4g_date_layout_register_type(module);
}

/**
 * log4g_date_layout_date_format:
 * @base: A date format object.
 * @string: The buffer in which to layout the date.
 * @event: The log event to get the date from.
 *
 * Format a date layout for a log event info a buffer.
 *
 * Since: 0.1
 */
void
log4g_date_layout_date_format(Log4gLayout *base, GString *string,
        Log4gLoggingEvent *event)
{
    g_return_if_fail(LOG4G_IS_DATE_LAYOUT(base));
    GTimeVal *tv = log4g_logging_event_get_time_stamp(event);
    if (!tv) {
        return;
    }
    gchar buffer[128];
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->format) {
        struct tm tm;
        time_t time = tv->tv_sec;
        if (!localtime_r(&time, &tm)) {
            log4g_log_error("localtime_r(): %s", g_strerror(errno));
            return;
        }
        if (!strftime(buffer, sizeof buffer, priv->format, &tm)) {
            return;
        }
    } else {
        switch (priv->type) {
        case RELATIVE_TIME_DATE_FORMAT: {
            glong start = log4g_logging_event_get_start_time();
            glong time = (tv->tv_sec * 1000) + (tv->tv_usec * 0.001);
            g_snprintf(buffer, sizeof buffer, "%ld", time - start);
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
