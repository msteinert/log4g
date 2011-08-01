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
 * SECTION: deny-all-filter
 * @short_description: Drop all logging events
 *
 * This filter drops all logging events.
 *
 * Add this filter to the end of a filter chain to switch from the default
 * behavior of "accept all unless instructed otherwise" to a "deny all unless
 * instructed otherwise" behavior.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/deny-all-filter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gDenyAllFilter, log4g_deny_all_filter,
		LOG4G_TYPE_FILTER)

static void
log4g_deny_all_filter_init(Log4gDenyAllFilter *self)
{
	/* do nothing */
}

static Log4gFilterDecision
decide(Log4gFilter *self, Log4gLoggingEvent *event)
{
	return LOG4G_FILTER_DENY;
}

static void
log4g_deny_all_filter_class_init(Log4gDenyAllFilterClass *klass)
{
	/* initialize Log4gFilter class */
	Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
	filter_class->decide = decide;
}

static void
log4g_deny_all_filter_class_finalize(Log4gDenyAllFilterClass *klass)
{
	/* do nothing */
}

void
log4g_deny_all_filter_register(GTypeModule *module)
{
	log4g_deny_all_filter_register_type(module);
}
