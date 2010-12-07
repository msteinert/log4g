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

#ifndef LOG4G_LEVEL_RANGE_FILTER_H
#define LOG4G_LEVEL_RANGE_FILTER_H

#include <log4g/filter.h>
#include <log4g/level.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LEVEL_RANGE_FILTER \
    (log4g_level_range_filter_get_type())

#define LOG4G_LEVEL_RANGE_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LEVEL_RANGE_FILTER, \
            Log4gLevelRangeFilter))

#define LOG4G_IS_LEVEL_RANGE_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LEVEL_RANGE_FILTER))

#define LOG4G_LEVEL_RANGE_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LEVEL_RANGE_FILTER, \
            Log4gLevelRangeFilterClass))

#define LOG4G_IS_LEVEL_RANGE_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LEVEL_RANGE_FILTER))

#define LOG4G_LEVEL_RANGE_FILTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LEVEL_RANGE_FILTER, \
            Log4gLevelRangeFilterClass))

typedef struct _Log4gLevelRangeFilter Log4gLevelRangeFilter;

typedef struct _Log4gLevelRangeFilterClass Log4gLevelRangeFilterClass;

/**
 * Log4gLevelRangeFilter:
 *
 * The <structname>Log4gLevelRangeFilter</structname> structure does not have
 * any public members.
 */
struct _Log4gLevelRangeFilter {
    /*< private >*/
    Log4gFilter parent_instance;
};

/**
 * Log4gLevelRangeFilterClass:
 *
 * The <structname>Log4gLevelRangeFilterClass</structname> structure does not
 * have any public members.
 */
struct _Log4gLevelRangeFilterClass {
    /*< private >*/
    Log4gFilterClass parent_class;
};

G_GNUC_INTERNAL GType
log4g_level_range_filter_get_type(void);

G_GNUC_INTERNAL void
log4g_level_range_filter_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_LEVEL_RANGE_FILTER_H */
