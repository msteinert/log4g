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

#ifndef LOG4G_SIMPLE_LAYOUT_H
#define LOG4G_SIMPLE_LAYOUT_H

#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_SIMPLE_LAYOUT \
	(log4g_simple_layout_get_type())

#define LOG4G_SIMPLE_LAYOUT(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_SIMPLE_LAYOUT, \
		Log4gSimpleLayout))

#define LOG4G_IS_SIMPLE_LAYOUT(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_SIMPLE_LAYOUT))

#define LOG4G_SIMPLE_LAYOUT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_SIMPLE_LAYOUT, \
		Log4gSimpleLayoutClass))

#define LOG4G_IS_SIMPLE_LAYOUT_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_SIMPLE_LAYOUT))

#define LOG4G_SIMPLE_LAYOUT_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_SIMPLE_LAYOUT, \
		Log4gSimpleLayoutClass))

typedef struct Log4gSimpleLayout_ Log4gSimpleLayout;

typedef struct Log4gSimpleLayoutClass_ Log4gSimpleLayoutClass;

/**
 * Log4gSimpleLayout:
 *
 * The <structname>Log4gSimpleLayout</structname> structure does not have any
 * public members.
 */
struct Log4gSimpleLayout_ {
	/*< private >*/
	Log4gLayout parent_instance;
	gpointer priv;
};

/**
 * Log4gSimpleLayoutClass:
 *
 * The <structname>Log4gSimpleLayoutClass</structname> structure does not have
 * any public members.
 */
struct Log4gSimpleLayoutClass_ {
	/*< private >*/
	Log4gLayoutClass parent_class;
};

G_GNUC_INTERNAL GType
log4g_simple_layout_get_type(void);

G_GNUC_INTERNAL void
log4g_simple_layout_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_SIMPLE_LAYOUT_H */
