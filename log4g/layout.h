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

#ifndef LOG4G_LAYOUT_H
#define LOG4G_LAYOUT_H

#include <log4g/logging-event.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LAYOUT \
	(log4g_layout_get_type())

#define LOG4G_LAYOUT(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LAYOUT, \
		Log4gLayout))

#define LOG4G_IS_LAYOUT(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LAYOUT))

#define LOG4G_LAYOUT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LAYOUT, Log4gLayoutClass))

#define LOG4G_IS_LAYOUT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LAYOUT))

#define LOG4G_LAYOUT_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LAYOUT, \
		Log4gLayoutClass))

#define LOG4G_LAYOUT_LINE_SEP "\n"

typedef struct Log4gLayout_ Log4gLayout;

typedef struct Log4gLayoutClass_ Log4gLayoutClass;

/**
 * Log4gLayout:
 *
 * The <structname>Log4gLayout</structname> structure does not have any
 * public members.
 */
struct Log4gLayout_ {
	/*< private >*/
	GObject parent_instance;
};

/**
 * Log4gLayoutFormat:
 * @self: A layout object.
 * @event: A logging event object to be laid out.
 *
 * Implement this function to create your own layout format.
 *
 * Returns: The formatted logging event.
 * Since: 0.1
 */
typedef gchar *
(*Log4gLayoutFormat)(Log4gLayout *self, Log4gLoggingEvent *event);

/**
 * Log4gLayoutGetContentType:
 * @self: A layout object.
 *
 * Retrieve the content type output by this layout.
 *
 * The base class returns "text/plain".
 *
 * Returns: The layout content type.
 * Since: 0.1
 */
typedef const gchar *
(*Log4gLayoutGetContentType)(Log4gLayout *self);

/**
 * Log4gLayoutGetHeader:
 * @self: A layout object.
 *
 * Retrieve the header for the layout format.
 *
 * The base class returns \e NULL.
 *
 * Returns: The layout header.
 * Since: 0.1
 */
typedef const gchar *
(*Log4gLayoutGetHeader)(Log4gLayout *self);

/**
 * Log4gLayoutGetFooter:
 * @self: A layout object.
 *
 * Retrieve the footer for the layout format.
 *
 * The base class returns \e NULL.
 *
 * Returns: The layout footer.
 * Since: 0.1
 */
typedef const gchar *
(*Log4gLayoutGetFooter)(Log4gLayout *self);

/**
 * Log4gLayoutActivateOptions:
 * @self: An layout object.
 *
 * Activate all options set for this layout.
 *
 * Since: 0.1
 */
typedef void
(*Log4gLayoutActivateOptions)(Log4gLayout *self);

/**
 * Log4gLayoutClass:
 * @format: Implement this function to create your own layout format.
 * @get_content_type: Retrieve the content type output by this layout.
 * @get_header: Retrieve the header for this layout.
 * @get_footer: Retrieve the footer for this layout.
 * @activate_options: Activate all options set for this layout.
 */
struct Log4gLayoutClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	Log4gLayoutFormat format;
	Log4gLayoutGetContentType get_content_type;
	Log4gLayoutGetHeader get_header;
	Log4gLayoutGetFooter get_footer;
	Log4gLayoutActivateOptions activate_options;
};

GType
log4g_layout_get_type(void) G_GNUC_CONST;

gchar *
log4g_layout_format(Log4gLayout *self, Log4gLoggingEvent *event);

const gchar *
log4g_layout_get_content_type(Log4gLayout *self);

const gchar *
log4g_layout_get_header(Log4gLayout *self);

const gchar *
log4g_layout_get_footer(Log4gLayout *self);

void
log4g_layout_activate_options(Log4gLayout *self);

G_END_DECLS

#endif /* LOG4G_LAYOUT_H */
