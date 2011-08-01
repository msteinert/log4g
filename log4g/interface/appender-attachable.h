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

#ifndef LOG4G_APPENDER_ATTACHABLE_H
#define LOG4G_APPENDER_ATTACHABLE_H

#include <log4g/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER_ATTACHABLE \
	(log4g_appender_attachable_get_type())

#define LOG4G_APPENDER_ATTACHABLE(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), \
		LOG4G_TYPE_APPENDER_ATTACHABLE, Log4gAppenderAttachable))

#define LOG4G_IS_APPENDER_ATTACHABLE(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), \
		LOG4G_TYPE_APPENDER_ATTACHABLE))

#define LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(instance) \
	(G_TYPE_INSTANCE_GET_INTERFACE((instance), \
		LOG4G_TYPE_APPENDER_ATTACHABLE, \
		Log4gAppenderAttachableInterface));

typedef struct Log4gAppenderAttachable_ Log4gAppenderAttachable;

typedef struct Log4gAppenderAttachableInterface_
	Log4gAppenderAttachableInterface;

/**
 * Log4gAppenderAttachableAddAppender:
 * @self: An appender attachable object.
 * @appender: The appender to attach.
 *
 * Attach an appender to an object.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAttachableAddAppender)(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

/**
 * Log4gAppenderAttachableGetAllAppenders:
 * @self: An appender attachable object.
 *
 * Retrieve all previously attached appenders in an array.
 *
 * <note><para>
 * It is the responsibility of the caller to call g_array_free() on the
 * returned value.
 * </para></note>
 *
 * Returns: A new array of appenders attached to @self. If no appenders
 *          are attached, this function shall return %NULL.
 * Since: 0.1
 */
typedef const GArray *
(*Log4gAppenderAttachableGetAllAppenders)(Log4gAppenderAttachable *self);

/**
 * Log4gAppenderAttachableGetAppender:
 * @self: An appender attachable object.
 * @name: The name of the appneder to retrieve.
 *
 * Retrieve an attached appender by name.
 *
 * Returns: The appender identified by @name or %NULL if the named
 *          appender was not found.
 * Since: 0.1
 */
typedef Log4gAppender *
(*Log4gAppenderAttachableGetAppender)(Log4gAppenderAttachable *self,
		const gchar *name);

/**
 * Log4gAppenderAttachableIsAttached:
 * @self: An appender attachable object.
 * @appender: The appender to check.
 *
 * Determine if an appender is attached to an object.
 *
 * Returns: %TRUE if the named appender is attached to @self, %FALSE
 *          otherwise.
 * Since: 0.1
 */
typedef gboolean
(*Log4gAppenderAttachableIsAttached)(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

/**
 * Log4gAppenderAttachableRemoveAllAppenders:
 * @self: An appender attachable object.
 *
 * Remove all attached appenders from an object.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAttachableRemoveAllAppenders)(Log4gAppenderAttachable *self);

/**
 * Log4gAppenderAttachableRemoveAppender:
 * @self: An appender attachable object.
 * @appender: The appender to remove.
 *
 * Remove an attached appender from an object.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAttachableRemoveAppender)(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

/**
 * Log4gAppenderAttachableRemoveAppenderName:
 * @self: An appender attachable object.
 * @name: The name of the appedner to remove.
 *
 * Remove a named appender from an object.
 *
 * Since: 0.1
 */
typedef void
(*Log4gAppenderAttachableRemoveAppenderName)(Log4gAppenderAttachable *self,
		const gchar *name);

/**
 * Log4gAppenderAttachableInterface:
 * @add_appender: Attach an appender to an object.
 * @get_all_appenders: Get all appenders attached to an object.
 * @get_appender: Get an appender attached to an object by name.
 * @is_attached: Determine if an appender is attached to an object.
 * @remove_all_appenders: Remove all appenders attached to an object.
 * @remove_appender: Remove an appender attached to an object.
 * @remove_appender_name: Remove an appender attached to an object by name.
 */
struct Log4gAppenderAttachableInterface_ {
	/*< private >*/
	GTypeInterface parent_interface;
	/*< public >*/
	Log4gAppenderAttachableAddAppender add_appender;
	Log4gAppenderAttachableGetAllAppenders get_all_appenders;
	Log4gAppenderAttachableGetAppender get_appender;
	Log4gAppenderAttachableIsAttached is_attached;
	Log4gAppenderAttachableRemoveAllAppenders remove_all_appenders;
	Log4gAppenderAttachableRemoveAppender remove_appender;
	Log4gAppenderAttachableRemoveAppenderName remove_appender_name;
};

GType
log4g_appender_attachable_get_type(void) G_GNUC_CONST;

void
log4g_appender_attachable_add_appender(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

const GArray *
log4g_appender_attachable_get_all_appenders(Log4gAppenderAttachable *self);

Log4gAppender *
log4g_appender_attachable_get_appender(Log4gAppenderAttachable *self,
		const gchar *name);

gboolean
log4g_appender_attachable_is_attached(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

void
log4g_appender_attachable_remove_all_appenders(Log4gAppenderAttachable *self);

void
log4g_appender_attachable_remove_appender(Log4gAppenderAttachable *self,
		Log4gAppender *appender);

void
log4g_appender_attachable_remove_appender_name(Log4gAppenderAttachable *self,
		const gchar *name);

G_END_DECLS

#endif /* LOG4G_APPENDER_ATTACHABLE_H */
