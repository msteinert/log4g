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
 * SECTION: console-appender
 * @short_description: Log events to stdout or stderr
 * @see_also: freopen(3)
 *
 * The console appender logs events to stdout or stderr using a layout
 * specified by the user. The default target is stdout.
 *
 * Console appenders accept two properties:
 * <orderedlist>
 * <listitem><para>target</para></listitem>
 * <listitem><para>follow</para></listitem>
 * </orderedlist>
 *
 * The value of target determines where the output will be logged. The value
 * must be one of "stdout" or "stderr". The default is "stdout".
 *
 * The value of follow determines if the log output will follow reopens of
 * the target stream. The default value is %TRUE.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/console-appender.h"
#include <unistd.h>

G_DEFINE_DYNAMIC_TYPE(Log4gConsoleAppender, log4g_console_appender,
		LOG4G_TYPE_WRITER_APPENDER)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_CONSOLE_APPENDER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gConsoleAppender *)instance)->priv)

/* System output identifier */
#define SYSTEM_OUT "stdout"

/* System error identifier */
#define SYSTEM_ERR "stderr"

struct Private {
	gchar *target;
	gboolean follow;
};

static void
log4g_console_appender_init(Log4gConsoleAppender *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->target = SYSTEM_OUT;
}

static void
finalize(GObject *base)
{
	log4g_appender_close(LOG4G_APPENDER(base));
	G_OBJECT_CLASS(log4g_console_appender_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_TARGET,
	PROP_FOLLOW,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
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
			log4g_log_warn(Q_("using previously set target, "
					"%s by default"),
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
close_(Log4gAppender *base)
{
	struct Private *priv = GET_PRIVATE(base);
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
	struct Private *priv = GET_PRIVATE(base);
	if (priv->follow) {
		if (g_ascii_strcasecmp(priv->target, SYSTEM_OUT)) {
			log4g_writer_appender_set_writer(LOG4G_APPENDER(base),
					stdout);
		} else {
			log4g_writer_appender_set_writer(LOG4G_APPENDER(base),
					stderr);
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
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->close = close_;
	appender_class->activate_options = activate_options;
	Log4gWriterAppenderClass *writer_class =
		LOG4G_WRITER_APPENDER_CLASS(klass);
	writer_class->close_writer = close_writer;
	g_type_class_add_private(klass, sizeof(struct Private));
	/* install properties */
	g_object_class_install_property(object_class, PROP_TARGET,
		g_param_spec_string("target", Q_("Target"),
			Q_("Output target (\"stdout\" or \"stderr\")"),
			"stdout", G_PARAM_WRITABLE));
	g_object_class_install_property(object_class, PROP_FOLLOW,
		g_param_spec_boolean("follow", Q_("Follow"),
			Q_("Output follows freopen()"), FALSE,
			G_PARAM_WRITABLE));
}

static void
log4g_console_appender_class_finalize(
		G_GNUC_UNUSED Log4gConsoleAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_console_appender_register(GTypeModule *module)
{
	log4g_console_appender_register_type(module);
}
