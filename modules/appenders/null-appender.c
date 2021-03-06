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
 * SECTION: null-appender
 * @short_description: A do-nothing appender
 *
 * The null appender merely exists; It never outputs any log messages.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "appender/null-appender.h"

G_DEFINE_DYNAMIC_TYPE(Log4gNullAppender, log4g_null_appender,
		LOG4G_TYPE_APPENDER)

/* The single instance of this class */
static GObject *singleton = NULL;

static void
log4g_null_appender_init(G_GNUC_UNUSED Log4gNullAppender *self)
{
	/* do nothing */
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
{
	GObject *self = g_atomic_pointer_get(&singleton);
	if (!self) {
		self = G_OBJECT_CLASS(log4g_null_appender_parent_class)->
			constructor(type, n, params);
		g_atomic_pointer_set(&singleton, self);
	} else {
		g_object_ref(self);
	}
	return self;
}

static void
append(G_GNUC_UNUSED Log4gAppender *base,
		G_GNUC_UNUSED Log4gLoggingEvent *event)
{
	/* do nothing */
}

static void
do_append(G_GNUC_UNUSED Log4gAppender *base,
		G_GNUC_UNUSED Log4gLoggingEvent *event)
{
	/* do nothing */
}

static void
close_(G_GNUC_UNUSED Log4gAppender *base)
{
	/* do nothing */
}

static gboolean
requires_layout(G_GNUC_UNUSED Log4gAppender *self)
{
	return FALSE;
}

static void
log4g_null_appender_class_init(Log4gNullAppenderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructor = constructor;
	Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
	appender_class->append = append;
	appender_class->do_append = do_append;
	appender_class->requires_layout = requires_layout;
	appender_class->close = close_;
}

static void
log4g_null_appender_class_finalize(G_GNUC_UNUSED Log4gNullAppenderClass *klass)
{
	/* do nothing */
}

void
log4g_null_appender_register(GTypeModule *module)
{
	log4g_null_appender_register_type(module);
}
