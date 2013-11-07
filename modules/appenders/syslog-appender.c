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
 * SECTION: syslog-appender
 * @short_description: Append to the local syslog
 * @see_also: syslog(3), syslog.conf(5), syslogd(8)
 *
 * This is a simple syslog appender that appends to the local syslog (usually
 * /var/log/messages) using the syslog(3) system call.
 *
 * Syslog appenders accept the following properties:
 * <orderedlist>
 * <listitem><para>ident</para></listitem>
 * <listitem><para>option</para></listitem>
 * <listitem><para>facility</para></listitem>
 * </orderedlist>
 *
 * The value of ident will be the first parameter to openlog(3).
 *
 * The value of option will be the second parameter to openlog(3).
 *
 * The value of facility will be the third parameter to openlog(3).
 *
 * <note><para>
 * The behavior of this appender may be affected by the configuration of the
 * syslog daemon. Refer to the syslog documentation for more information.
 * </para></note>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/syslog-appender.h"
#include <syslog.h>

G_DEFINE_DYNAMIC_TYPE(Log4gSyslogAppender, log4g_syslog_appender,
        LOG4G_TYPE_APPENDER)

#define ASSIGN_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_SYSLOG_APPENDER, \
            struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gSyslogAppender *)instance)->priv)

struct Private {
	gchar *ident;
	gint option;
	gint facility;
};

static void
log4g_syslog_appender_init(Log4gSyslogAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	log4g_appender_close(LOG4G_APPENDER(base));
	G_OBJECT_CLASS(log4g_syslog_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->ident);
	G_OBJECT_CLASS(log4g_syslog_appender_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_IDENT,
	PROP_OPTION,
	PROP_FACILITY,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	const gchar *ident;
	switch (id) {
	case PROP_IDENT:
		g_free(priv->ident);
		ident = g_value_get_string(value);
		priv->ident = ident ? g_strdup(ident) : NULL;
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
close_(Log4gAppender *base)
{
	if (!log4g_appender_get_closed(base)) {
		log4g_appender_set_closed(base, TRUE);
		closelog();
	}
}

static gboolean
requires_layout(G_GNUC_UNUSED Log4gAppender *self)
{
	return TRUE;
}

static void
activate_options(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
	openlog(priv->ident, priv->option, priv->facility);
}

static void
log4g_syslog_appender_class_init(Log4gSyslogAppenderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->append = append;
	appender_class->close = close_;
	appender_class->requires_layout = requires_layout;
	appender_class->activate_options = activate_options;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_IDENT,
		g_param_spec_string("ident", Q_("Ident"),
			Q_("Syslog ident parameter"), NULL, G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_OPTION,
		g_param_spec_int("option", Q_("Option"),
			Q_("Syslog option parameter"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_FACILITY,
		g_param_spec_int("facility", Q_("Facility"),
			Q_("Syslog facility parameter"), 0, G_MAXINT, 0,
			G_PARAM_WRITABLE));
}

static void
log4g_syslog_appender_class_finalize(
		G_GNUC_UNUSED Log4gSyslogAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_syslog_appender_register(GTypeModule *module)
{
	log4g_syslog_appender_register_type(module);
}
