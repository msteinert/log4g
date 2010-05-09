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
 * \brief Abstract superclass of appenders in the Log4g package.
 * \author Mike Steinert
 * \date 2-8-2010
 *
 * This class provides code for common functionality, such as support for
 * threshold filtering and general filters.
 *
 * The appender skeleton accepts one property:
 * -# threshold
 *
 * Setting the threshold property enables log level theshold filtering. By
 * default there is no threshold filtering.
 */

#ifndef LOG4G_APPENDER_SKELETON_H
#define LOG4G_APPENDER_SKELETON_H

#include <log4g/interface/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER_SKELETON \
    (log4g_appender_skeleton_get_type())

#define LOG4G_APPENDER_SKELETON(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_APPENDER_SKELETON, \
            Log4gAppenderSkeleton))

#define LOG4G_IS_APPENDER_SKELETON(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_APPENDER_SKELETON))

#define LOG4G_APPENDER_SKELETON_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_APPENDER_SKELETON, \
            Log4gAppenderSkeletonClass))

#define LOG4G_IS_APPENDER_SKELETON_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_APPENDER_SKELETON))

#define LOG4G_APPENDER_SKELETON_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_APPENDER_SKELETON, \
            Log4gAppenderSkeletonClass))

/** \brief Log4gAppenderSkeleton object type definition */
typedef struct _Log4gAppenderSkeleton Log4gAppenderSkeleton;

/** \brief Log4gAppenderSkeleton class type definition */
typedef struct _Log4gAppenderSkeletonClass Log4gAppenderSkeletonClass;

/** \brief Log4gAppenderSkeletonClass definition */
struct _Log4gAppenderSkeleton {
    GObject parent_instance;
};

/** \brief Log4gAppenderSkeletonClass definition */
struct _Log4gAppenderSkeletonClass {
    GObjectClass parent_class;

    /**
     * \brief Perform actual logging.
     *
     * Sub-classes should implement this abstract virtual function to perform
     * actual logging.
     *
     * \param base [in] An appender skeleton object.
     * \param event [in] A log event.
     *
     * \see log4g/logging-event.h
     */
    void
    (*append)(Log4gAppender *base, Log4gLoggingEvent *event);
};

GType
log4g_appender_skeleton_get_type(void);

/**
 * \brief Invokes the abstract virtual function
 *        _Log4gAppenderSkeletonClass::append().
 *
 * \param base [in] An appender skeleton object.
 * \param event [in] A log event.
 */
void
log4g_appender_skeleton_append(Log4gAppender *base, Log4gLoggingEvent *event);

/**
 * \brief Retrieve the first filter in the filter chain.
 *
 * \param base [in] An appender skeleton object.
 *
 * \return The first filter in the filter chain, or \e NULL if there is none.
 */
Log4gFilter *
log4g_appender_skeleton_get_first_filter(Log4gAppender *base);

/**
 * \brief Determine if a log level is below the appender's threshold.
 *
 * If there is no threshold set then the return value is always \e TRUE.
 *
 * \param base [in] An appender skeleton object.
 * \param level [in] A log level.
 *
 * \return \e TRUE if \e level is above the level threshold of \e base,
 *         \e FALSE otherwise.
 *
 * \see log4g/level.h
 */
gboolean
log4g_appender_skeleton_is_as_severe_as(Log4gAppender *base,
        Log4gLevel *level);

/**
 * \brief Set the threshold property.
 *
 * \param base [in] A skeleton appender object.
 * \param threshold [in] A string representation of a log level.
 *
 * \see log4g/level.h
 */
void
log4g_appender_skeleton_set_threshold(Log4gAppender *base,
        const gchar *threshold);

/**
 * \brief Retrieve the threshold property.
 *
 * \param base [in] A skeleton appender object.
 *
 * \return The threshold value for \e base.
 */
Log4gLevel *
log4g_appender_skeleton_get_threshold(Log4gAppender *base);

/**
 * \brief Determine if an appender has been closed.
 *
 * \param base [in] An appender skeleton object.
 *
 * \return \e TRUE if \e base is closed, \e FALSE otherwise.
 */
gboolean
log4g_appender_skeleton_get_closed(Log4gAppender *base);

/**
 * \brief Set the closed parameter.
 *
 * Appenders should set this value appropriately. The default value is
 * \e FALSE.
 * 
 * \param base [in] An appender skeleton object.
 *
 * \return \e TRUE if \e base is closed, \e FALSE otherwise.
 */
void
log4g_appender_skeleton_set_closed(Log4gAppender *base, gboolean closed);

G_END_DECLS

#endif /* LOG4G_APPENDER_SKELETON_H */
