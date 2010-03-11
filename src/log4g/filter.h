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
 * \brief ...
 * \author Mike Steinert
 * \date 1-29-2010
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

/** \brief Log4gFilterClass definition */
struct _Log4gFilter {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gFilterClass definition */
struct _Log4gFilterClass {
    GObjectClass parent_class; /**< parent class */
    gint (*decide)(Log4gFilter *self, Log4gLoggingEvent *event);
    /*< public >*/
    gint ACCEPT; /**< ... */
    gint NEUTRAL; /**< ... */
    gint DENY; /**< ... */
};

GType
log4g_filter_get_type(void);

/**
 */
void
log4g_filter_activate_options(Log4gFilter *self);

/**
 */
gint
log4g_filter_decide(Log4gFilter *self, Log4gLoggingEvent *event);

/**
 */
Log4gFilter *
log4g_filter_get_next(Log4gFilter *self);

/**
 */
void
log4g_filter_set_next(Log4gFilter *self, Log4gFilter *next);

G_END_DECLS

#endif /* LOG4G_FILTER_H */
