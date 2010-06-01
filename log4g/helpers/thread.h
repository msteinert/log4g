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

/**
 * \file
 * \brief Set the thread name for log messages.
 * \author Mike Steinert
 * \date 2-14-2010
 *
 * In order for a thread to have a name in the log output it must first be
 * set. This class allows the user to set the thread name. If the thread
 * name is not explicitly set then the name "thread1", with the number
 * incrementing for each thread, is used.
 *
 * The initialization process will set the name of the main thread to "main".
 *
 * \note The thread numbers are created in the order that messages are logged
 *       not the order that the threads were created. If you are debugging
 *       a thread issue it is recommended to explicitly set the thread name.
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

/** \brief Log4gThread object type definition */
typedef struct _Log4gThread Log4gThread;

/** \brief Log4gThread class type definition */
typedef struct _Log4gThreadClass Log4gThreadClass;

/** \brief Log4gThreadClass definition */
struct _Log4gThread {
    GObject parent_instance;
};

/** \brief Log4gThreadClass definition */
struct _Log4gThreadClass {
    GObjectClass parent_class;
};

GType
log4g_thread_get_type(void);

/**
 * \internal
 * \brief Retrieve a Log4gThread object.
 *
 * If one does not exist a new one will be created.
 *
 * \return A Log4gThread object.
 */
Log4gThread *
log4g_thread_get_instance(void);

/**
 * \internal
 * \brief Remove the instance of Log4gThread from the current thread.
 */
void
log4g_thread_remove_instance(void);

/**
 * \brief Retrieve the name of the current thread.
 *
 * \return The name of the current thread.
 */
const gchar *
log4g_thread_get_name(void);

/**
 * \brief Set the name of the current thread.
 *
 * \param name [in] The new name of the current thread.
 */
void
log4g_thread_set_name(const gchar *name);

G_END_DECLS

#endif /* LOG4G_THREAD_H */
