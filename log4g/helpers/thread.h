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

#ifndef LOG4G_THREAD_H
#define LOG4G_THREAD_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_THREAD \
    (log4g_thread_get_type())

#define LOG4G_THREAD(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_THREAD, Log4gThread))

#define LOG4G_IS_THREAD(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_THREAD))

#define LOG4G_THREAD_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_THREAD, Log4gThreadClass))

#define LOG4G_IS_THREAD_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_THREAD))

#define LOG4G_THREAD_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_THREAD, \
            Log4gThreadClass))

typedef struct _Log4gThread Log4gThread;

typedef struct _Log4gThreadClass Log4gThreadClass;

/**
 * Log4gThread:
 *
 * The <structname>Log4gThread</structname> structure does not have any public
 * members.
 */
struct _Log4gThread {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gThreadClass:
 *
 * The <structname>Log4gThreadClass</structname> structure does not have any
 * public members.
 */
struct _Log4gThreadClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_thread_get_type(void);

const gchar *
log4g_thread_get_name(void);

void
log4g_thread_set_name(const gchar *name);

G_END_DECLS

#endif /* LOG4G_THREAD_H */
