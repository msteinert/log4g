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
 * \brief Log event filter base class.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * Users may extend this class to implement custom log event filtering. Note
 * that Log4gLogger & Log4gAppenderSkeleton (the parent of all standard
 * appenders) have builtin filter rules. You should understand and use the
 * builtin rules before writing custom filters.
 *
 * Filters are organized in a linear chain. Log4gAppenderSkeleton calls the
 * \e decide() function of each filter sequentially in order to determine the
 * outcome of the filtering process.
 *
 * Sub-classes must override the \e decide() virtual function. This function
 * must return one of the integer constants \e LOG4G_FILTER_DENY,
 * \e LOG4G_FILTER_NEUTRAL, or \e LOG4G_FILTER_ACCEPT.
 *
 * If the value \e LOG4G_FILTER_DENY is returned the log event is dropped
 * immediately without consulting the remaining filters.
 *
 * If the value \e LOG4G_FILTER_NEUTRAL is returned the remaining filters in
 * the chain are consulted. If the final filter returns \e LOG4G_FILTER_NEUTRAL
 * then the log event is logged. If no filters exist then all messages are
 * logged.
 *
 * If the value \e LOG4G_FILTER_ACCEPT is returned the the log event is logged
 * immediately without consulting the remaining filters.
 */

#ifndef LOG4G_FILTER_H
#define LOG4G_FILTER_H

#include <log4g/logging-event.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_FILTER \
    (log4g_filter_get_type())

#define LOG4G_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_FILTER, Log4gFilter))

#define LOG4G_IS_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_FILTER))

#define LOG4G_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_FILTER, Log4gFilterClass))

#define LOG4G_IS_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_FILTER))

#define LOG4G_FILTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_FILTER, \
            Log4gFilterClass))

/** \brief Log4gFilter object type definition */
typedef struct _Log4gFilter Log4gFilter;

/** \brief Log4gFilter class type definition */
typedef struct _Log4gFilterClass Log4gFilterClass;

/** \brief Possible filter decisions */
typedef enum {
    /**
     *  The log event must be logged immediately. The remaining filters (if
     *  any) should not be consulted.
     */
    LOG4G_FILTER_ACCEPT = 1,
    /**
     * This filter is neutral regarding the event. The remaining filters
     * (if any) should be consulted for a final decision.
     */
    LOG4G_FILTER_NEUTRAL = 0,
    /**
     * The log event must dropped immediately. The remaining filters (if
     * any) should not be consulted.
     */
    LOG4G_FILTER_DENY = -1
} Log4gFilterDecision;

/** \brief Log4gFilterClass definition */
struct _Log4gFilter {
    GObject parent_instance;
};

/** \brief Log4gFilterClass definition */
struct _Log4gFilterClass {
    GObjectClass parent_class;
    /**
     * \brief Implements the filter decision.
     *
     * If the decision is \e LOG4G_FILTER_DENY, the even will be dropped. If
     * the decision is \e LOG4G_FILTER_ACCEPT, then any remaining filters
     * will * be invoked. If the decision is \e LOG4G_FILTER_ACCEPT the
     * event will be logged without consulting any other filters in the
     * chain.
     *
     * \param self [in] A filter object.
     * \param event [in] A logging event.
     *
     * \return A filter decision based upon the logging event.
     */
    Log4gFilterDecision
    (*decide)(Log4gFilter *self, Log4gLoggingEvent *event);
};

GType
log4g_filter_get_type(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gOptionHandlerInterface::activate_options().
 *
 * Filters generally need to have their options activated before they can
 * be used. This class provides a do-nothing implementation for convenience.
 *
 * \see log4g/interface/option-handler.h
 *
 * \param self [in] A filter object.
 */
void
log4g_filter_activate_options(Log4gFilter *self);

/**
 * \brief Invokes the virutal function _Log4gFilterClass::decide().
 *
 * \param self [in] A filter object.
 * \param event [in] A logging event.
 *
 * \return A filter decision based upon the logging event.
 */
Log4gFilterDecision
log4g_filter_decide(Log4gFilter *self, Log4gLoggingEvent *event);

/**
 * \brief Retrieve the next filter in the chain.
 *
 * Filters are chained together. This function returns the next filter in
 * the chain, or \e NULL if there are no more.
 *
 * \param self [in] A filter object.
 *
 * \return The next filter in the chain, or \e NULL if there are no more.
 */
Log4gFilter *
log4g_filter_get_next(Log4gFilter *self);

/**
 * \brief Set the next filter in the chain.
 *
 * Filters are chained together. This function sets the filter that will
 * follow this one in the chain.
 *
 * \brief self [in] A filter object.
 * \brief next [in] The filter to set as the next in the chain.
 */
void
log4g_filter_set_next(Log4gFilter *self, Log4gFilter *next);

G_END_DECLS

#endif /* LOG4G_FILTER_H */
