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
 * SECTION: simple-layout
 * @short_description: A simple log layout
 * @see_also: #Log4gPatternLayoutClass
 *
 * The simple layout consists of the level of a log statement and the log
 * message itself separated by " - ". For example:
 *
 * |[
 * DEBUG - Hello, World!
 * ]|
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/simple-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gSimpleLayout, log4g_simple_layout,
		LOG4G_TYPE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_SIMPLE_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gSimpleLayout *)instance)->priv)

struct Private {
	GString *string;
};

static void
log4g_simple_layout_init(Log4gSimpleLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->string = g_string_sized_new(128);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->string) {
		g_string_free(priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_simple_layout_parent_class)->finalize(base);
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	g_string_set_size(priv->string, 0);
	Log4gLevel *level = log4g_logging_event_get_level(event);
	if (level) {
		g_string_append(priv->string, log4g_level_to_string(level));
	}
	g_string_append(priv->string, " - ");
	g_string_append(priv->string,
			log4g_logging_event_get_rendered_message(event));
	g_string_append(priv->string, LOG4G_LAYOUT_LINE_SEP);
	return priv->string->str;
}

static void
log4g_simple_layout_class_init(Log4gSimpleLayoutClass *klass)
{
	/* initialize GObject class */
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
	/* initialize Log4gLayout class */
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	layout_class->format = format;
}

static void
log4g_simple_layout_class_finalize(Log4gSimpleLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_simple_layout_register(GTypeModule *module)
{
	log4g_simple_layout_register_type(module);
}
