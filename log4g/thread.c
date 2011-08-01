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
 * SECTION: thread
 * @short_description: Set the thread name for log messages
 *
 * In order for a thread to have a name in the log output it must first be
 * set. This class allows the user to set the thread name. If the thread
 * name is not explicitly set then the name "thread1", with the number
 * incrementing for each thread, is used.
 *
 * The initialization process will set the name of the main thread to "main".
 *
 * <note><para>
 * The thread numbers are created in the order that messages are logged not
 * the order that the threads were created. If you are debugging a thread
 * issue it is recommended to explicitly set the thread name.
 * </para></note>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/thread.h"

G_DEFINE_TYPE(Log4gThread, log4g_thread, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_THREAD, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gThread *)instance)->priv)

struct Private {
	gchar *name;
};

/* Thread specific data. */
static GPrivate *priv = NULL;

/* Count the number of thread name requests. */
static gint counter = 0;

static void
log4g_thread_init(Log4gThread *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
{
	if (g_thread_supported()) {
		static gsize once = 0;
		if (g_once_init_enter(&once)) {
			priv = g_private_new(g_object_unref);
			if (!priv) {
				return NULL;
			}
			g_once_init_leave(&once, 1);
		}
	}
	GObject *self = g_private_get(priv);
	if (!self) {
		self = G_OBJECT_CLASS(log4g_thread_parent_class)->
			constructor(type, n, params);
		if (!self) {
			return NULL;
		}
		g_private_set(priv, self);
	} else {
		g_object_ref(self);
	}
	return self;
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->name);
	priv->name = NULL;
	G_OBJECT_CLASS(log4g_thread_parent_class)->finalize(base);
}

static void
log4g_thread_class_init(Log4gThreadClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructor = constructor;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_thread_get_instance:
 *
 * Retrieve a Log4gThread object.
 *
 * If one does not exist a new one will be created.
 *
 * Returns: A Log4gThread object.
 */
static Log4gThread *
log4g_thread_get_instance(void)
{
	Log4gThread *self = (Log4gThread *)g_private_get(priv);
	if (!self) {
		self = g_object_new(LOG4G_TYPE_THREAD, NULL);
	}
	return self;
}

/**
 * log4g_thread_get_name:
 *
 * Retrieve the name of the current thread.
 *
 * Returns: The name of the current thread.
 * Since: 0.1
 */
const gchar *
log4g_thread_get_name(void)
{
	Log4gThread *self = log4g_thread_get_instance();
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->name) {
		g_atomic_int_inc(&counter);
		priv->name = g_strdup_printf("thread%d",
				g_atomic_int_get(&counter));
	}
	return priv->name;
}

/**
 * log4g_thread_set_name:
 * @name: The new name of the current thread.
 *
 * Set the name of the current thread.
 *
 * Since: 0.1
 */
void
log4g_thread_set_name(const gchar *name)
{
	Log4gThread *self = log4g_thread_get_instance();
	if (!self) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	g_free(priv->name);
	priv->name = g_strdup(name);
}
