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
 * SECTION: root-logger
 * @short_description: The root logger
 *
 * The root logger sits on top of the logger hierarchy. It is a normal logger
 * with the following exceptions:
 * <itemizedlist>
 * <listitem><para>
 * It may not be assigned a NULL level threshold
 * </para></listitem>
 * <listitem><para>
 * The root logger cannot have a parent (the
 * #Log4gLoggerClass.get_effective_level method always returns the value of
 * the level field without walking the hierarchy)
 * </para></listitem>
 * </itemizedlist>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/root-logger.h"

G_DEFINE_TYPE(Log4gRootLogger, log4g_root_logger, LOG4G_TYPE_LOGGER)

static void
log4g_root_logger_init(G_GNUC_UNUSED Log4gRootLogger *self)
{
	/* do nothing */
}

/**
 * get_effective_level:
 * @self: A root logger object.
 *
 * Get the effective log level of this root logger.
 *
 * Returns: The log level threshold for this root logger.
 */
static Log4gLevel *
get_effective_level(Log4gLogger *self)
{
	return log4g_logger_get_level(self);
}

/**
 * set_level:
 * @self: A root logger object.
 * @level: The new level threshold for this root logger.
 *
 * Set the log level threshold for this root logger.
 *
 * Setting a %NULL value to the level of the root logger may have
 * catastrophic results. This is prevented here.
 */
static void
set_level(Log4gLogger *self, Log4gLevel *level)
{
	if (!level) {
		log4g_log_warn(Q_("you have tried to set a NULL "
					"level to root"));
		return;
	}
	LOG4G_LOGGER_CLASS(log4g_root_logger_parent_class)->
		set_level(self, level);
}

static void
log4g_root_logger_class_init(Log4gRootLoggerClass *klass)
{
	Log4gLoggerClass *logger_class = LOG4G_LOGGER_CLASS(klass);
	logger_class->get_effective_level = get_effective_level;
	logger_class->set_level = set_level;
}

/**
 * log4g_root_logger_new:
 * @level: The level threshold of this root logger.
 *
 * Create a new root logger.
 *
 * Returns: A new root logger object.
 * Since: 0.1
 */
Log4gLogger *
log4g_root_logger_new(Log4gLevel *level)
{
	g_return_val_if_fail(level, NULL);
	Log4gLogger *self = g_object_new(LOG4G_TYPE_ROOT_LOGGER, NULL);
	if (!self) {
		return NULL;
	}
	log4g_logger_set_name(self, "root");
	if (!log4g_logger_get_name(self)) {
		g_object_unref(self);
		return NULL;
	}
	log4g_logger_set_level(self, level);
	return self;
}
