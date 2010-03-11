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
 * \date 2-11-2010
 */

#ifndef LOG4G_LEVEL_MATCH_FILTER_H
#define LOG4G_LEVEL_MATCH_FILTER_H

#include <log4g/filter.h>
#include <log4g/level.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LEVEL_MATCH_FILTER \
    (log4g_level_match_filter_get_type())

#define LOG4G_LEVEL_MATCH_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LEVEL_MATCH_FILTER, \
            Log4gLevelMatchFilter))

#define LOG4G_IS_LEVEL_MATCH_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LEVEL_MATCH_FILTER))

#define LOG4G_LEVEL_MATCH_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LEVEL_MATCH_FILTER, \
            Log4gLevelMatchFilterClass))

#define LOG4G_IS_LEVEL_MATCH_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LEVEL_MATCH_FILTER))

#define LOG4G_LEVEL_MATCH_FILTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LEVEL_MATCH_FILTER, \
            Log4gLevelMatchFilterClass))

/** \brief Log4gLevelMatchFilter object type definition */
typedef struct _Log4gLevelMatchFilter Log4gLevelMatchFilter;

/** \brief Log4gLevelMatchFilter class type definition */
typedef struct _Log4gLevelMatchFilterClass Log4gLevelMatchFilterClass;

/** \brief Log4gLevelMatchFilterClass definition */
struct _Log4gLevelMatchFilter {
    Log4gFilter parent_instance; /**< parent instance */
};

/** \brief Log4gLevelMatchFilterClass definition */
struct _Log4gLevelMatchFilterClass {
    Log4gFilterClass parent_class; /**< parent class */
};

GType
log4g_level_match_filter_get_type(void);

/**
 */
Log4gFilter *
log4g_level_match_filter_new(void);

/**
 */
void
log4g_level_match_filter_set_level_to_match(Log4gFilter *base,
        const gchar *level);

/**
 */
Log4gLevel *
log4g_level_match_filter_get_level_to_match(Log4gFilter *base);

/**
 */
void
log4g_level_match_filter_set_accept_on_match(Log4gFilter *base,
        gboolean accept);

/**
 */
gboolean
log4g_level_match_filter_get_accept_on_match(Log4gFilter *base);

G_END_DECLS

#endif /* LOG4G_LEVEL_MATCH_FILTER_H */
