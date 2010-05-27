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
 * \brief Drop all logging events.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * This filter drops all logging events.
 *
 * Add this filter to the end of a filter chain to switch from the default
 * behavior of "accept all unless instructed otherwise" to a "deny all unless
 * instructed otherwise" behavior.
 */

#ifndef LOG4G_DENY_ALL_FILTER_H
#define LOG4G_DENY_ALL_FILTER_H

#include <log4g/filter.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DENY_ALL_FILTER \
    (log4g_deny_all_filter_get_type())

#define LOG4G_DENY_ALL_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_DENY_ALL_FILTER, \
            Log4gDenyAllFilter))

#define LOG4G_IS_DENY_ALL_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_DENY_ALL_FILTER))

#define LOG4G_DENY_ALL_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DENY_ALL_FILTER, \
            Log4gDenyAllFilterClass))

#define LOG4G_IS_DENY_ALL_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DENY_ALL_FILTER))

#define LOG4G_DENY_ALL_FILTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_DENY_ALL_FILTER, \
            Log4gDenyAllFilterClass))

/** \brief Log4gDenyAllFilter object type definition */
typedef struct _Log4gDenyAllFilter Log4gDenyAllFilter;

/** \brief Log4gDenyAllFilter class type definition */
typedef struct _Log4gDenyAllFilterClass Log4gDenyAllFilterClass;

/** \brief Log4gDenyAllFilterClass definition */
struct _Log4gDenyAllFilter {
    Log4gFilter parent_instance;
};

/** \brief Log4gDenyAllFilterClass definition */
struct _Log4gDenyAllFilterClass {
    Log4gFilterClass parent_class;
};

GType
log4g_deny_all_filter_get_type(void);

void
log4g_deny_all_filter_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_DENY_ALL_FILTER_H */
