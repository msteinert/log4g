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
 * \date 2-8-2010
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
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gAppenderSkeletonClass definition */
struct _Log4gAppenderSkeletonClass {
    GObjectClass parent_class; /**< parent class */
    void (*append)(Log4gAppender *base, Log4gLoggingEvent *event);
};

GType
log4g_appender_skeleton_get_type(void);

/**
 */
void
log4g_appender_skeleton_append(Log4gAppender *base, Log4gLoggingEvent *event);

/**
 */
Log4gFilter *
log4g_appender_skeleton_get_first_filter(Log4gAppender *base);

/**
 */
Log4gLevel *
log4g_appender_skeleton_get_threshold(Log4gAppender *base);

/**
 */
gboolean
log4g_appender_skeleton_is_as_severe_as(Log4gAppender *base,
        Log4gLevel *level);

/**
 */
void
log4g_appender_skeleton_set_threshold(Log4gAppender *base,
        const gchar *threshold);

/*< protected >*/

/**
 */
gboolean
log4g_appender_skeleton_get_closed(Log4gAppender *base);

/**
 */
void
log4g_appender_skeleton_set_closed(Log4gAppender *base, gboolean closed);

G_END_DECLS

#endif /* LOG4G_APPENDER_SKELETON_H */
