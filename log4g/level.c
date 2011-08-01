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
 * SECTION: level
 * @short_description: Minimum set of logging levels recognized by the system
 *
 * The following base log levels are defined:
 * <itemizedlist>
 * <listitem><para>OFF</para></listitem>
 * <listitem><para>FATAL</para></listitem>
 * <listitem><para>ERROR</para></listitem>
 * <listitem><para>WARN</para></listitem>
 * <listitem><para>INFO</para></listitem>
 * <listitem><para>DEBUG</para></listitem>
 * <listitem><para>ALL</para></listitem>
 * </itemizedlist>
 *
 * The #Log4gLevel class may be sub-classed to define a larger level set.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/level.h"
#include <syslog.h>

G_DEFINE_TYPE(Log4gLevel, log4g_level, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gLevel *)instance)->priv)

struct Private {
	gint level;
	gchar *string;
	gint syslog;
};

static void
log4g_level_init(Log4gLevel *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->level = LOG4G_LEVEL_DEBUG_INT;
	priv->string = NULL; /* DEBUG ? */
	priv->syslog = 7;
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->string);
	priv->string = NULL;
	G_OBJECT_CLASS(log4g_level_parent_class)->finalize(base);
}

static void
log4g_level_class_init(Log4gLevelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	/* initialize GObject */
	object_class->finalize = finalize;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
	/* intialize Log4gLevel */
	klass->string_to_level = log4g_level_string_to_level;
	klass->string_to_level_default = log4g_level_string_to_level_default;
	klass->int_to_level = log4g_level_int_to_level;
	klass->int_to_level_default = log4g_level_int_to_level_default;
	/* initialize default log levels */
	klass->ALL = log4g_level_new(LOG4G_LEVEL_ALL_INT, "ALL", LOG_DEBUG);
	klass->TRACE =
		log4g_level_new(LOG4G_LEVEL_TRACE_INT, "TRACE", LOG_DEBUG);
	klass->DEBUG =
		log4g_level_new(LOG4G_LEVEL_DEBUG_INT, "DEBUG", LOG_DEBUG);
	klass->INFO = log4g_level_new(LOG4G_LEVEL_INFO_INT, "INFO", LOG_INFO);
	klass->WARN =
		log4g_level_new(LOG4G_LEVEL_WARN_INT, "WARN", LOG_WARNING);
	klass->ERROR =
		log4g_level_new(LOG4G_LEVEL_ERROR_INT, "ERROR", LOG_ERR);
	klass->FATAL =
		log4g_level_new(LOG4G_LEVEL_FATAL_INT, "FATAL", LOG_EMERG);
	klass->OFF = log4g_level_new(LOG4G_LEVEL_OFF_INT, "OFF", LOG_EMERG);
}

/**
 * log4g_level_new:
 * @level: The log priority.
 * @string: The string representation of this level.
 * @syslog: The syslog equivalent of this level.
 *
 * Create a Log4gLevel object.
 *
 * Returns: A new Log4gLevel object.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_new(gint level, const gchar *string, gint syslog)
{
	Log4gLevel *self = g_object_new(LOG4G_TYPE_LEVEL, NULL);
	struct Private *priv;
	if (!self) {
		return NULL;
	}
	priv = GET_PRIVATE(self);
	priv->level = level;
	priv->string = g_strdup(string);
	if (!priv->string) {
		g_object_unref(self);
		return NULL;
	}
	priv->syslog = syslog;
	return self;
}

/**
 * log4g_level_equals:
 * @self: A level object.
 * @level: A level to compare with @self.
 *
 * Determine if two log levels are equal.
 *
 * Returns: %TRUE if @self & @level are equal, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_level_equals(Log4gLevel *self, Log4gLevel *level)
{
	if (GET_PRIVATE(self)->level == GET_PRIVATE(level)->level) {
		return TRUE;
	}
	return FALSE;
}

/**
 * log4g_level_get_syslog_equivalent:
 * @self: A level object.
 *
 * Retrieve the syslog(3) equivalent integer of level object.
 *
 * Returns: The syslog equivalent integer of @self.
 * Since: 0.1
 */
gint
log4g_level_get_syslog_equivalent(Log4gLevel *self)
{
	return GET_PRIVATE(self)->syslog;
}

/**
 * log4g_level_is_greater_or_equal:
 * @self: A level object.
 * @level: A level object to compare with @self.
 *
 * Determine if a level is greater or equal than another level.
 *
 * Returns: %TRUE if @level is greater than or equal to @self, or %FALSE if
 *          @self is greater than @level.
 * Since: 0.1
 */
gboolean
log4g_level_is_greater_or_equal(Log4gLevel *self, Log4gLevel *level)
{
	if (GET_PRIVATE(self)->level >= GET_PRIVATE(level)->level) {
		return TRUE;
	} else {
		return FALSE;
	}
}

/**
 * log4g_level_to_string:
 * @self: A level object.
 *
 * Retrieve the string representation of a level.
 *
 * Returns: The string representation of @self.
 */
const gchar *
log4g_level_to_string(Log4gLevel *self)
{
	return GET_PRIVATE(self)->string;
}

/**
 * log4g_level_to_int:
 * @self: A level object.
 *
 * Retrieve the integer representation of a level.
 *
 * Returns: The integer representation of @self.
 * Since: 0.1
 */
gint
log4g_level_to_int(Log4gLevel *self)
{
	return GET_PRIVATE(self)->level;
}

/**
 * log4g_level_string_to_level:
 * @string: A string representation of a level.
 *
 * Calls the @string_to_level function from the #Log4gLevelClass of @self.
 *
 * Returns: The level represented by @string.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_string_to_level(const gchar *level)
{
	Log4gLevel *to = NULL;
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	to = log4g_level_string_to_level_default(level, klass->DEBUG);
	g_type_class_unref(klass);
	return to;
}

/**
 * log4g_level_string_to_level_default:
 * @string: A string representation of a level.
 * @def: The level to return if the conversion failed.
 *
 * Calls the @string_to_level_default function of the #Log4gLevelClass of
 * @self.
 *
 * Returns: The level represented by @string or @def if the conversion failed.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_string_to_level_default(const gchar *level, Log4gLevel *def)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *to = NULL;
	if (!g_ascii_strcasecmp(level, "ALL")) {
		to = klass->ALL;
	} else if (!g_ascii_strcasecmp(level, "DEBUG")) {
		to = klass->DEBUG;
	} else if (!g_ascii_strcasecmp(level, "INFO")) {
		to = klass->INFO;
	} else if (!g_ascii_strcasecmp(level, "WARN")) {
		to = klass->WARN;
	} else if (!g_ascii_strcasecmp(level, "ERROR")) {
		to = klass->ERROR;
	} else if (!g_ascii_strcasecmp(level, "FATAL")) {
		to = klass->FATAL;
	} else if (!g_ascii_strcasecmp(level, "OFF")) {
		to = klass->OFF;
	} else if (!g_ascii_strcasecmp(level, "TRACE")) {
		to = klass->TRACE;
	} else {
		to = def;
	}
	g_type_class_unref(klass);
	return to;
}

/**
 * log4g_level_int_to_level:
 * @level: An integer representation of a level.
 *
 * Calls the @int_to_level function from the #Log4gLevelClass of @self.
 *
 * Returns: The level represented by @level.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_int_to_level(gint level)
{
	Log4gLevel *to = log4g_level_int_to_level_default(level,
			log4g_level_DEBUG());
	return to;
}

/**
 * log4g_level_int_to_level_default:
 * @level: An integer representation of a level.
 * @def: The level to return if the conversion failed.
 *
 * Calls the @int_to_level_default from the #Log4gLevelClass of @self.
 *
 * Returns: The level represented by @level or @def if the conversion failed.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_int_to_level_default(gint level, Log4gLevel *def)
{
	Log4gLevel *to = NULL;
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return def;
	}
	switch (level) {
	case LOG4G_LEVEL_ALL_INT:
		to = klass->ALL;
		break;
	case LOG4G_LEVEL_TRACE_INT:
		to = klass->TRACE;
		break;
	case LOG4G_LEVEL_DEBUG_INT:
		to = klass->DEBUG;
		break;
	case LOG4G_LEVEL_INFO_INT:
		to = klass->INFO;
		break;
	case LOG4G_LEVEL_WARN_INT:
		to = klass->WARN;
		break;
	case LOG4G_LEVEL_ERROR_INT:
		to = klass->ERROR;
		break;
	case LOG4G_LEVEL_FATAL_INT:
		to = klass->FATAL;
		break;
	case LOG4G_LEVEL_OFF_INT:
		to = klass->OFF;
		break;
	default:
		to = def;
		break;
	}
	g_type_class_unref(klass);
	return to;
}

/**
 * log4g_level_ALL:
 *
 * Retrieve the log level #Log4gLevelClass.ALL.
 *
 * Returns: The log level #Log4gLevelClass.ALL.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_ALL(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->ALL;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_TRACE:
 *
 * Retrieve the log level #Log4gLevelClass.TRACE.
 *
 * Returns: The log level #Log4gLevelClass.TRACE.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_TRACE(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->TRACE;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_DEBUG:
 *
 * Retrieve the log level #Log4gLevelClass.DEBUG.
 *
 * Returns: The log level #Log4gLevelClass.DEBUG.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_DEBUG(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->DEBUG;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_INFO:
 *
 * Retrieve the log level #Log4gLevelClass.INFO.
 *
 * Returns: The log level #Log4gLevelClass.INFO.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_INFO(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->INFO;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_WARN:
 *
 * Retrieve the log level #Log4gLevelClass.WARN.
 *
 * Returns: The log level #Log4gLevelClass.WARN.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_WARN(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->WARN;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_ERROR:
 *
 * Retrieve the log level #Log4gLevelClass.ERROR.
 *
 * Returns: The log level #Log4gLevelClass.ERROR.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_ERROR(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->ERROR;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_FATAL:
 *
 * Retrieve the log level #Log4gLevelClass.FATAL.
 *
 * Returns: The log level #Log4gLevelClass.FATAL.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_FATAL(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->FATAL;
	g_type_class_unref(klass);
	return level;
}

/**
 * log4g_level_OFF:
 *
 * Retrieve the log level #Log4gLevelClass.OFF.
 *
 * Returns: The log level #Log4gLevelClass.OFF.
 * Since: 0.1
 */
Log4gLevel *
log4g_level_OFF(void)
{
	Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
	if (!klass) {
		return NULL;
	}
	Log4gLevel *level = klass->OFF;
	g_type_class_unref(klass);
	return level;
}
