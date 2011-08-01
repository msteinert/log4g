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

#ifndef LOG4G_FILTER_H
#define LOG4G_FILTER_H

#include <log4g/logging-event.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_FILTER \
	(log4g_filter_get_type())

#define LOG4G_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_FILTER, \
		Log4gFilter))

#define LOG4G_IS_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_FILTER))

#define LOG4G_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_FILTER, Log4gFilterClass))

#define LOG4G_IS_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_FILTER))

#define LOG4G_FILTER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_FILTER, \
		Log4gFilterClass))

typedef struct Log4gFilter_ Log4gFilter;

typedef struct Log4gFilterClass_ Log4gFilterClass;

/**
 * Log4gFilterDecision:
 * @LOG4G_FILTER_ACCEPT: The log event must be logged immediately. The
 *                       remaining filters (if any) should not be consulted.
 * @LOG4G_FILTER_NEUTRAL: This filter is neutral regarding the event. The
 *                        remaining filters (if any) should be consulted for a
 *                        final decision.
 * @LOG4G_FILTER_DENY: The log event must dropped immediately. The remaining
 *                     filters (if any) should not be consulted.
 */
typedef enum {
	LOG4G_FILTER_ACCEPT = 1,
	LOG4G_FILTER_NEUTRAL = 0,
	LOG4G_FILTER_DENY = -1
} Log4gFilterDecision;

/**
 * Log4gFilter:
 *
 * The <structname>Log4gFilter</structname> structure does not have any
 * public members.
 */
struct Log4gFilter_ {
	/*< private >*/
	GObject parent_instance;
	gpointer priv;
};

/**
 * Log4gFilterDecide:
 * @self: A #Log4gFilter object.
 * @event: A logging event.
 *
 * If the decision is %LOG4G_FILTER_DENY, the even will be dropped. If
 * the decision is %LOG4G_FILTER_ACCEPT, then any remaining filters
 * will be invoked. If the decision is %LOG4G_FILTER_ACCEPT the
 * event will be logged without consulting any other filters in the
 * chain.
 *
 * Returns: A filter decision based upon the logging event.
 * Since: 0.1
 */
typedef Log4gFilterDecision
(*Log4gFilterDecide)(Log4gFilter *self, Log4gLoggingEvent *event);

/**
 * Log4gFilterActivateOptions:
 * @self: A #Log4gFilter object.
 *
 * Activate all options set for this filter.
 *
 * Since: 0.1
 */
typedef void
(*Log4gFilterActivateOptions)(Log4gFilter *self);

/**
 * Log4gFilterClass:
 * @decide: Implements the filter decision.
 * @activate_options: Activate all options set for this filter.
 */
struct Log4gFilterClass_ {
	/*< private >*/
	GObjectClass parent_class;
	/*< public >*/
	Log4gFilterDecide decide;
	Log4gFilterActivateOptions activate_options;
};

GType
log4g_filter_get_type(void) G_GNUC_CONST;

Log4gFilterDecision
log4g_filter_decide(Log4gFilter *self, Log4gLoggingEvent *event);

void
log4g_filter_activate_options(Log4gFilter *self);

Log4gFilter *
log4g_filter_get_next(Log4gFilter *self);

void
log4g_filter_set_next(Log4gFilter *self, Log4gFilter *next);

G_END_DECLS

#endif /* LOG4G_FILTER_H */
