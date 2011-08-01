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

#ifndef LOG4G_APPENDER_H
#define LOG4G_APPENDER_H

#include <log4g/filter.h>
#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER \
	(log4g_appender_get_type())

#define LOG4G_APPENDER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_APPENDER, \
		Log4gAppender))

#define LOG4G_IS_APPENDER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_APPENDER))

#define LOG4G_APPENDER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_APPENDER, \
		Log4gAppenderClass))

#define LOG4G_IS_APPENDER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_APPENDER))

#define LOG4G_APPENDER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_APPENDER, \
		Log4gAppenderClass))

typedef struct Log4gAppender_ Log4gAppender;

typedef struct Log4gAppenderClass_ Log4gAppenderClass;

/**
 * Log4gAppender:
 *
 * The <structname>Log4gAppender</structname> structure does not have any
 * public members.
 */
struct Log4gAppender_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

/**
 * Log4gAppenderAddFilter:
 * @self: A #Log4gAppender object.
 * @filter: A #Log4gFilter to add to this appender.
 *
 * Add a filter to the end of the filter chain.
 *
 * @See: #Log4gFilter
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAddFilter)(Log4gAppender *self, Log4gFilter *filter);

/**
 * Log4gAppenderGetFilter:
 * @self: A #Log4gAppender object.
 *
 * Filters are organized in a linked list so all filters are available
 * through the result of this function.
 *
 * @See: #Log4gFilter
 *
 * Returns: The first filter in the filter chain.
 * Since: 0.1
 */
typedef Log4gFilter *
(*Log4gAppenderGetFilter)(Log4gAppender *self);

/**
 * Log4gAppenderClose:
 * @self: A #Log4gAppender object.
 *
 * Release any resources allocated within the appender such as file
 * handles, network connections, etc. A closed appender is no longer
 * usable by the Log4g system.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderClose)(Log4gAppender *self);

/**
 * Log4gAppenderAppend:
 * @base: An #Log4gAppender object.
 * @event: A log event.
 *
 * Sub-classes should implement this abstract virtual function to perform
 * actual logging.
 *
 * @See: #Log4gLoggingEvent
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAppend)(Log4gAppender *base, Log4gLoggingEvent *event);

/**
 * Log4gAppenderDoAppend:
 * @self: A #Log4gAppender object.
 * @event: The logging event to append.
 *
 * Loggers will call this function in order to log events.
 *
 * @See: #Log4gLoggingEvent
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderDoAppend)(Log4gAppender *self, Log4gLoggingEvent *event);

/**
 * Log4gAppenderSetName:
 * @self: A #Log4gAppender object.
 * @name: The new name for this appender.
 *
 * The name may be used by other components to uniquely identify the
 * appender.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderSetName)(Log4gAppender *self, const gchar *name);

/**
 * Log4gAppenderGetName:
 * @self: A #Log4gAppender object.
 *
 * The name uniquely identifies this appender.
 *
 * Returns: The name of @self (may be %NULL).
 * Since: 0.1
 */
typedef const gchar *
(*Log4gAppenderGetName)(Log4gAppender *self);

/**
 * Log4gAppenderSetErrorHandler:
 * @self: An appender object.
 * @handler: An error handler.
 *
 * Set the error handler for this appender.
 *
 * @See: #Log4gErrorHandler
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderSetErrorHandler)(Log4gAppender *self, gpointer handler);

/**
 * Log4gAppenderGetErrorHandler:
 * @self: A #Log4gAppender object.
 *
 * Retrieve the error handler for an appender.
 *
 * @See: #Log4gErrorHandler
 *
 * Returns: The error handler set for this appender.
 * Since: 0.1
 */
typedef gpointer
(*Log4gAppenderGetErrorHandler)(Log4gAppender *self);

/**
 * Log4gAppenderSetLayout:
 * @self: A #Log4gAppender object.
 * @layout: The new layout for this appender.
 *
 * Set the layout for this appender.
 *
 * @See: #Log4gLayout
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderSetLayout)(Log4gAppender *self, Log4gLayout *layout);

/**
 * Log4gAppenderGetLayout:
 * @self: A #Log4gAppender object.
 *
 * Retrieve the layout for this appender.
 *
 * @See: #Log4gLayout
 *
 * Returns: The layout for this appender;
 * Since: 0.1
 */
typedef Log4gLayout *
(*Log4gAppenderGetLayout)(Log4gAppender *self);

/**
 * Log4gAppenderRequiresLayout:
 * @self: A #Log4gAppender object.
 *
 * Configurators may call this method to determine if an appender requires
 * a layout.
 *
 * In the exceptional case where an appender accepts a layout but can also
 * work without it the appender should return %TRUE.
 *
 * @See: #Log4gAppenderSetLayout, #Log4gAppenderGetLayout, #Log4gLayout
 *
 * Returns: %TRUE if this appender requires a layout, %FALSE otherwise.
 * Since: 0.1
 */
typedef gboolean
(*Log4gAppenderRequiresLayout)(Log4gAppender *self);

/**
 * Log4gAppenderActivateOptions:
 * @self: A #Log4gAppender object.
 *
 * Activate all options set for this appender.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderActivateOptions)(Log4gAppender *self);

/**
 * Log4gAppenderClass:
 * @add_filter: Add a filter to the end of the filter chain.
 * @get_filter: Get the first filter in the filter chain.
 * @close: Release all resources.
 * @append: Perform actial logging.
 * @do_append: Log in an appender-specific way.
 * @set_name: Set the name of this appender.
 * @get_name: Retrieve the name of this appender.
 * @set_error_handler: Set the error handler for this appender.
 * @get_error_handler: Retrieve the error handler for this appender.
 * @set_layout: Set the layout for this appender.
 * @get_layout: Get the layout for this appender.
 * @requires_layout: Determine if this appender requires a layout.
 * @activate_options: Activate all options set for this appender.
 */
struct Log4gAppenderClass_ {
	/*< private >*/
	GObjectClass parent_interface;
	/*< public >*/
	Log4gAppenderAddFilter add_filter;
	Log4gAppenderGetFilter get_filter;
	Log4gAppenderClose close;
	Log4gAppenderAppend append;
	Log4gAppenderDoAppend do_append;
	Log4gAppenderSetName set_name;
	Log4gAppenderGetName get_name;
	Log4gAppenderSetErrorHandler set_error_handler;
	Log4gAppenderGetErrorHandler get_error_handler;
	Log4gAppenderSetLayout set_layout;
	Log4gAppenderGetLayout get_layout;
	Log4gAppenderRequiresLayout requires_layout;
	Log4gAppenderActivateOptions activate_options;
};

GType
log4g_appender_get_type(void) G_GNUC_CONST;

void
log4g_appender_clear_filters(Log4gAppender *self);

void
log4g_appender_add_filter(Log4gAppender *self, Log4gFilter *filter);

Log4gFilter *
log4g_appender_get_filter(Log4gAppender *self);

void
log4g_appender_close(Log4gAppender *self);

void
log4g_appender_do_append(Log4gAppender *self, Log4gLoggingEvent *event);

const gchar *
log4g_appender_get_name(Log4gAppender *self);

void
log4g_appender_set_error_handler(Log4gAppender *self, gpointer handler);

gpointer
log4g_appender_get_error_handler(Log4gAppender *self);

void
log4g_appender_set_layout(Log4gAppender *self, Log4gLayout *layout);

Log4gLayout *
log4g_appender_get_layout(Log4gAppender *self);

void
log4g_appender_set_name(Log4gAppender *self, const gchar *name);

gboolean
log4g_appender_requires_layout(Log4gAppender *self);

void
log4g_appender_activate_options(Log4gAppender *self);

void
log4g_appender_append(Log4gAppender *self, Log4gLoggingEvent *event);

Log4gFilter *
log4g_appender_get_first_filter(Log4gAppender *self);

gboolean
log4g_appender_is_as_severe_as(Log4gAppender *self, Log4gLevel *level);

void
log4g_appender_set_threshold(Log4gAppender *self, const gchar *threshold);

Log4gLevel *
log4g_appender_get_threshold(Log4gAppender *self);

gboolean
log4g_appender_get_closed(Log4gAppender *self);

void
log4g_appender_set_closed(Log4gAppender *self, gboolean closed);

G_END_DECLS

#endif /* LOG4G_APPENDER_H */
