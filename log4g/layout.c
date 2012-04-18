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
 * SECTION: layout
 * @short_description: Log event layout base class
 *
 * Users may extend this class to implement custom log event layouts.
 *
 * Many appenders require a layout in order to log an event. Sub-classes
 * must override the Log4gLayoutClass_::format() virtual function to implement
 * custom formatting.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/layout.h"

G_DEFINE_ABSTRACT_TYPE(Log4gLayout, log4g_layout, G_TYPE_OBJECT)

static void
log4g_layout_init(G_GNUC_UNUSED Log4gLayout *self)
{
	/* do nothing */
}

static const gchar *
get_content_type(G_GNUC_UNUSED Log4gLayout *self)
{
	return "text/plain";
}

static const gchar *
get(G_GNUC_UNUSED Log4gLayout *self)
{
	return NULL;
}

static void
activate_options(G_GNUC_UNUSED Log4gLayout *self)
{
	/* do nothing */
}

static void
log4g_layout_class_init(Log4gLayoutClass *klass)
{
	klass->format = NULL;
	klass->get_content_type = get_content_type;
	klass->get_header = get;
	klass->get_footer = get;
	klass->activate_options = activate_options;
}

/**
 * log4g_layout_format:
 * @self: A layout object.
 * @event: A logging event object to be laid out.
 *
 * Calls the @format function from the #Log4gLayoutClass of @self.
 *
 * Returns: The formatted logging event.
 * Since: 0.1
 */
gchar *
log4g_layout_format(Log4gLayout *self, Log4gLoggingEvent *event)
{
	g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
	return LOG4G_LAYOUT_GET_CLASS(self)->format(self, event);
}

/**
 * log4g_layout_get_content_type:
 * @self: A layout object.
 *
 * Calls the @get_content_type function from the #Log4gLayoutClass of @self.
 *
 * Returns: The layout content type.
 * Since: 0.1
 */
const gchar *
log4g_layout_get_content_type(Log4gLayout *self)
{
	g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
	return LOG4G_LAYOUT_GET_CLASS(self)->get_content_type(self);
}

/**
 * log4g_layout_get_header:
 * @self: A layout object.
 *
 * Calls the @get_header function from the #Log4gLayoutClass of @self.
 *
 * Returns: The layout header.
 * Since: 0.1
 */
const gchar *
log4g_layout_get_header(Log4gLayout *self)
{
	g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
	return LOG4G_LAYOUT_GET_CLASS(self)->get_header(self);
}

/**
 * log4g_layout_get_footer:
 * @self: A layout object.
 *
 * Calls the @get_footer function from the #Log4gLayoutClass of @self.
 *
 * Returns: The layout footer.
 * Since: 0.1
 */
const gchar *
log4g_layout_get_footer(Log4gLayout *self)
{
	g_return_val_if_fail(LOG4G_IS_LAYOUT(self), NULL);
	return LOG4G_LAYOUT_GET_CLASS(self)->get_footer(self);
}

/**
 * log4g_layout_activate_options:
 * @self: A layout object.
 *
 * Calls the @activate_options function from the #Log4gLayoutClass of @self.
 *
 * Layouts generally need to have their options activated before they can be
 * used. This class provides a do-nothing implementation for convenience.
 *
 * Since: 0.1
 */
void
log4g_layout_activate_options(Log4gLayout *self)
{
	g_return_if_fail(LOG4G_IS_LAYOUT(self));
	LOG4G_LAYOUT_GET_CLASS(self)->activate_options(self);
}
