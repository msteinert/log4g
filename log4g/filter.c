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
 * SECTION: filter
 * @short_description: Log event filter base class
 *
 * Users may extend this class to implement custom log event filtering. Note
 * that Log4gLogger & Log4gAppenderSkeleton (the parent of all standard
 * appenders) have builtin filter rules. You should understand and use the
 * builtin rules before writing custom filters.
 *
 * Filters are organized in a linear chain. Log4gAppenderSkeleton calls the
 * decide() function of each filter sequentially in order to determine the
 * outcome of the filtering process.
 *
 * Sub-classes must override the decide() virtual function. This function
 * must return one of the integer constants %LOG4G_FILTER_DENY,
 * %LOG4G_FILTER_NEUTRAL, or %LOG4G_FILTER_ACCEPT.
 *
 * If the value %LOG4G_FILTER_DENY is returned the log event is dropped
 * immediately without consulting the remaining filters.
 *
 * If the value %LOG4G_FILTER_NEUTRAL is returned the remaining filters in
 * the chain are consulted. If the final filter returns %LOG4G_FILTER_NEUTRAL
 * then the log event is logged. If no filters exist then all messages are
 * logged.
 *
 * If the value %LOG4G_FILTER_ACCEPT is returned the the log event is logged
 * immediately without consulting the remaining filters.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/filter.h"

G_DEFINE_ABSTRACT_TYPE(Log4gFilter, log4g_filter, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_FILTER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gFilter *)instance)->priv)

struct Private {
	Log4gFilter *next;
};

static void
log4g_filter_init(Log4gFilter *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->next) {
		g_object_unref(priv->next);
		priv->next = NULL;
	}
	G_OBJECT_CLASS(log4g_filter_parent_class)->dispose(base);
}

static void
activate_options(Log4gFilter *base)
{
	/* do nothing */
}

static void
log4g_filter_class_init(Log4gFilterClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	/* initialize GObject */
	object_class->dispose = dispose;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
	/* initialize Log4gFilter class */
	klass->decide = NULL;
	klass->activate_options = activate_options;
}

/**
 * log4g_filter_decide:
 * @self: A #Log4gFilter object.
 * @event: A logging event.
 *
 * Calls the @decide function from the #Log4gFilterClass of @self.
 *
 * Returns: A filter decision based upon the logging event.
 * Since: 0.1
 */
void
log4g_filter_activate_options(Log4gFilter *self)
{
	g_return_if_fail(LOG4G_IS_FILTER(self));
	LOG4G_FILTER_GET_CLASS(self)->activate_options(self);
}

/**
 * log4g_filter_activate_options:
 * @self: A #Log4gFilter object.
 *
 * Calls the @activate_options function from the #Log4gFilterClass of @self.
 *
 * Filters generally need to have their options activated before they can
 * be used. This class provides a do-nothing implementation for convenience.
 *
 * Since: 0.1
 */
Log4gFilterDecision
log4g_filter_decide(Log4gFilter *self, Log4gLoggingEvent *event)
{
	g_return_val_if_fail(LOG4G_IS_FILTER(self), 0);
	return LOG4G_FILTER_GET_CLASS(self)->decide(self, event);
}

/**
 * log4g_filter_get_next:
 * @self: A #Log4gFilter object.
 *
 * Retrieve the next filter in the chain.
 *
 * Filters are chained together. This function returns the next filter in
 * the chain, or %NULL if there are no more.
 *
 * Returns: The next filter in the chain, or \e NULL if there are no more.
 * Since: 0.1
 */
Log4gFilter *
log4g_filter_get_next(Log4gFilter *self)
{
	return GET_PRIVATE(self)->next;
}

/**
 * log4g_filter_set_next:
 * @self: A filter object.
 * @next: The filter to set as the next in the chain.
 *
 * Set the next filter in the chain.
 *
 * Filters are chained together. This function sets the filter that will
 * follow this one in the chain.
 *
 * Since: 0.1
 */
void
log4g_filter_set_next(Log4gFilter *self, Log4gFilter *next)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->next) {
		g_object_unref(priv->next);
	}
	g_object_ref(next);
	priv->next = next;
}
