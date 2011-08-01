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
 * SECTION: appender-attachable-impl
 * @short_description: An implementation of the appender attachable interface
 * @see_also: #Log4gAppenderAttachableInterface
 *
 * This is a straightforward implementation of the appender attachable
 * interface.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/appender-attachable-impl.h"

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gAppenderAttachableImpl *)instance)->priv)

struct Private {
	GArray *list;
};

static void
add_appender(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
	struct Private *priv = GET_PRIVATE(base);
	g_return_if_fail(appender);
	if (!priv->list) {
		priv->list = g_array_sized_new(TRUE, FALSE,
				sizeof(Log4gAppender *), 1);
		if (!priv->list) {
			return;
		}
	}
	g_object_ref(appender);
	g_array_append_val(priv->list, appender);
}

static const GArray *
get_all_appenders(Log4gAppenderAttachable *base)
{
	return GET_PRIVATE(base)->list;
}

static Log4gAppender *
get_appender(Log4gAppenderAttachable *base, const gchar *name)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list || !name) {
		return NULL;
	}
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *appender =
			g_array_index(priv->list, Log4gAppender *, i);
		if (!appender) {
			continue;
		}
		if (!g_strcmp0(name, log4g_appender_get_name(appender))) {
			g_object_ref(appender);
			return appender;
		}
	}
	return NULL;
}

static gboolean
is_attached(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list || !appender) {
		return FALSE;
	}
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *stored = g_array_index(priv->list,
				Log4gAppender *, i);
		if (!stored) {
			continue;
		}
		if (stored == appender) {
			return TRUE;
		}
	}
	return FALSE;
}

static void
remove_all_appenders(Log4gAppenderAttachable *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list) {
		return;
	}
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *appender =
			g_array_index(priv->list, Log4gAppender *, i);
		if (!appender) {
			continue;
		}
		g_object_unref(appender);
	}
	g_array_free(priv->list, TRUE);
	priv->list = NULL;
}

static void
remove_appender(Log4gAppenderAttachable *base, Log4gAppender *appender)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list) {
		return;
	}
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *stored = g_array_index(priv->list,
				Log4gAppender *, i);
		if (!stored) {
			continue;
		}
		if (stored == appender) {
			g_object_unref(stored);
			g_array_remove_index(priv->list, i);
			return;
		}
	}
}

static void
remove_appender_name(Log4gAppenderAttachable *base, const gchar *name)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list || !name) {
		return;
	}
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *appender =
			g_array_index(priv->list, Log4gAppender *, i);
		if (!appender) {
			continue;
		}
		if (!g_strcmp0(name, log4g_appender_get_name(appender))) {
			g_object_unref(appender);
			g_array_remove_index(priv->list, i);
			return;
		}
	}
}

static void
log4g_appender_attachable_impl_interface_init(
		Log4gAppenderAttachableInterface *interface, gpointer data)
{
	interface->add_appender = add_appender;
	interface->get_all_appenders = get_all_appenders;
	interface->get_appender = get_appender;
	interface->is_attached = is_attached;
	interface->remove_all_appenders = remove_all_appenders;
	interface->remove_appender = remove_appender;
	interface->remove_appender_name = remove_appender_name;
}

G_DEFINE_TYPE_WITH_CODE(Log4gAppenderAttachableImpl,
		log4g_appender_attachable_impl, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER_ATTACHABLE,
			log4g_appender_attachable_impl_interface_init))

static void
log4g_appender_attachable_impl_init(Log4gAppenderAttachableImpl *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->list) {
		Log4gAppender *appender;
		for (guint i = 0; i < priv->list->len; ++i) {
			appender = g_array_index(priv->list,
					Log4gAppender *, i);
			g_object_unref(appender);
		}
		g_array_remove_range(priv->list, 0, priv->list->len);
	}
	G_OBJECT_CLASS(log4g_appender_attachable_impl_parent_class)->
		dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->list) {
		g_array_free(priv->list, TRUE);
	}
	G_OBJECT_CLASS(log4g_appender_attachable_impl_parent_class)->
		finalize(base);
}

static void
log4g_appender_attachable_impl_class_init(
		Log4gAppenderAttachableImplClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	/* initialize GObject */
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_appender_attachable_impl_new:
 *
 * Create a new appender attachable implementation object.
 *
 * Returns: A new appender attachable implementation object.
 * Since: 0.1
 */
Log4gAppenderAttachable *
log4g_appender_attachable_impl_new(void)
{
	return g_object_new(LOG4G_TYPE_APPENDER_ATTACHABLE_IMPL, NULL);
}

/**
 * log4g_appender_attachable_impl_append_loop_on_appenders:
 * @base: An appender attachable implementation object.
 * @event: A log event to append.
 *
 * Call the Log4gAppenderInterface::do_append() virtual function for all
 * attached appenders.
 *
 * Returns: The number of appenders @event was appended to.
 * Since: 0.1
 */
guint
log4g_appender_attachable_impl_append_loop_on_appenders(
		Log4gAppenderAttachable *base, Log4gLoggingEvent *event)
{
	g_return_val_if_fail(LOG4G_IS_APPENDER_ATTACHABLE_IMPL(base), 0);
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->list) {
		return 0;
	}
	guint size = 0;
	for (guint i = 0; i < priv->list->len; ++i) {
		Log4gAppender *appender =
			g_array_index(priv->list, Log4gAppender *, i);
		if (!appender) {
			continue;
		}
		log4g_appender_do_append(appender, event);
		++size;
	}
	return size;
}
