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
 * \brief A filter based on level ranges.
 * \author Mike Steinert
 * \date 2-11-2010
 *
 * This is a simple filter which can reject message with levels outside a
 * specified range.
 *
 * This filter accept three properties:
 * -# level-min
 * -# level-max
 * -# accept-on-match
 *
 * If the level of the logging event is not between level-min and level-max
 * (inclusive) then the _Log4gLevelClass::decide() function returns \e DENY.
 *
 * If the logging logging event is within the specified range and
 * accept-on-match is \e TRUE then the _Log4gLevelClass::decide() function
 * returns \e ACCEPT. If accept-on-match is set to \e FALSE then
 * _Log4gLevelClass::decide() will return \e NEUTRAL in this case.
 *
 * The default value for accept-on-match is \e TRUE.
 *
 * If level-min is not defined then there is no minimum level (a level is
 * never rejected for being too low). If level-max is not defined then
 * there is no maximum level (a level is never rejected for being too high).
 *
 * Refer to log4g_appender_skeleton_set_threshold() for a more convenient way
 * to filter out log events by level.
 *
 * \see log4g/level.h
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

/** \brief Log4gLevelRangeFilter object type definition */
typedef struct _Log4gLevelRangeFilter Log4gLevelRangeFilter;

/** \brief Log4gLevelRangeFilter class type definition */
typedef struct _Log4gLevelRangeFilterClass Log4gLevelRangeFilterClass;

/** \brief Log4gLevelRangeFilterClass definition */
struct _Log4gLevelRangeFilter {
    Log4gFilter parent_instance;
};

/** \brief Log4gLevelRangeFilterClass definition */
struct _Log4gLevelRangeFilterClass {
    Log4gFilterClass parent_class;
};

GType
log4g_level_range_filter_get_type(void);

/**
 * \brief Create a new level range filter.
 *
 * \return A new level range filter.
 */
Log4gFilter *
log4g_level_range_filter_new(void);

/**
 * \brief Set the level-min property.
 *
 * \param base [in] A level range filter object.
 * \param level [in] A string representation of the new minimum level.
 */
void
log4g_level_range_filter_set_level_min(Log4gFilter *base, const gchar *level);

/**
 * \brief Retrieve the level-min property.
 *
 * \param base [in] A level range filter object.
 *
 * \return The current minimum level for \e base.
 */
Log4gLevel *
log4g_level_range_filter_get_level_min(Log4gFilter *base);

/**
 * \brief Set the level-max property.
 *
 * \param base [in] A level range filter object.
 * \param level [in] A string representation of the new maximum level.
 */
void
log4g_level_range_filter_set_level_max(Log4gFilter *base, const gchar *level);

/**
 * \brief Retrieve the level-max property.
 *
 * \param base [in] A level range filter object.
 *
 * \return The current maximum level for \e base.
 */
Log4gLevel *
log4g_level_range_filter_get_level_max(Log4gFilter *base);

/**
 * \brief Set the accept-on-match property.
 *
 * \param base [in] A level range filter object.
 * \param accept [in] The new accept on match value.
 */
void
log4g_level_range_filter_set_accept_on_range(Log4gFilter *base,
        gboolean accept);

/**
 * \brief Retrieve the accept-on-match property.
 *
 * \param base [in] A level match filter object.
 *
 * \return The current accept on match value for \e base.
 */
gboolean
log4g_level_range_filter_get_accept_on_range(Log4gFilter *base);

G_END_DECLS

#endif /* LOG4G_LEVEL_RANGE_FILTER_H */
