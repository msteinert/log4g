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
 * \brief Log events asynchronously.
 * \author Mike Steinert
 * \date 2-17-2010
 *
 * The async appender will collect events sent to it and then dispatch them
 * to all appenders that are attached to it. Multiple appenders may be
 * attached to an async appender.
 *
 * The async appender uses a separate thread to serve the events in its
 * buffer. You must call g_thread_init() before attempting to configure an
 * async appender.
 *
 * Async appenders accept the following properties:
 * -# blocking
 * -# buffer-size
 *
 * The blocking property determines the behavior of the async appender when
 * its log event buffer is full. If blocking is \e TRUE then the client will
 * block until there is room in the buffer. Otherwise the client will not
 * block and the log event will be dropped. In non-blocking mode the async
 * appender will keep a summary of all dropped logging events. The default
 * value is \e TRUE.
 *
 * The buffer-size property determines how many messages are allowed in the
 * buffer before the client will block. The default value is 128.
 *
 * \note If blocking is \e FALSE then the value of buffer-size has no effect.
 */

#ifndef LOG4G_ASYNC_APPENDER_H
#define LOG4G_ASYNC_APPENDER_H

#include <log4g/appender/appender-skeleton.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_ASYNC_APPENDER \
    (log4g_async_appender_get_type())

#define LOG4G_ASYNC_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppender))

#define LOG4G_IS_ASYNC_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_ASYNC_APPENDER))

#define LOG4G_ASYNC_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppenderClass))

#define LOG4G_IS_ASYNC_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_ASYNC_APPENDER))

#define LOG4G_ASYNC_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_ASYNC_APPENDER, \
            Log4gAsyncAppenderClass))

/** \brief Log4gAsyncAppender object type definition */
typedef struct _Log4gAsyncAppender Log4gAsyncAppender;

/** \brief Log4gAsyncAppender class type definition */
typedef struct _Log4gAsyncAppenderClass Log4gAsyncAppenderClass;

/** \brief Log4gAsyncAppenderClass definition */
struct _Log4gAsyncAppender {
    Log4gAppenderSkeleton parent_instance;
};

/** \brief Log4gAsyncAppenderClass definition */
struct _Log4gAsyncAppenderClass {
    Log4gAppenderSkeletonClass parent_class;
};

GType
log4g_async_appender_get_type(void);

void
log4g_async_appender_register(GTypeModule *module);

/**
 * \brief Create a new async appender object.
 *
 * \return A new async appender object.
 */
Log4gAppender *
log4g_async_appender_new(void);

/**
 * \brief Add an appender to an async appender.
 *
 * If \e appender is already attached to \e base then this function does not
 * do anything.
 *
 * \param base [in] An async appender object.
 * \param appender [in] The appender to add.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_async_appender_add_appender(Log4gAppender *base,
        Log4gAppender *appender);

/**
 * \brief Retrieve an array of appenders attached to an async appender.
 *
 * \param base [in] An async appender object.
 *
 * \return An array of appenders attached to \e base, or \e NULL if there are
 *         none. The caller is responsible for calling g_array_free() for the
 *         returned value.
 *
 * \see log4g/interface/appender-attachable.h
 */
const GArray *
log4g_async_appender_get_all_appenders(Log4gAppender *base);

/**
 * \brief Retrieve an attached named appender.
 *
 * \param base [in] An async appender object.
 * \param name [in] The name of the appender to look up.
 *
 * \return The appender named \e name or \e NULL if \e name is not found.
 *
 * \see log4g/interface/appender-attachable.h
 */
Log4gAppender *
log4g_async_appender_get_appender(Log4gAppender *base, const gchar *name);

/**
 * \brief Determine if an appender is attached.
 *
 * \param base [in] An async appender object.
 * \param appender [in] An appender.
 *
 * \return \e TRUE is \e appender is attached to \e base, \e FALSE otherwise.
 *
 * \see log4g/interface/appender-attachable.h
 */
gboolean
log4g_async_appender_is_attached(Log4gAppender *base, Log4gAppender *appender);

/**
 * \brief Remove all attached appenders.
 *
 * \param base [in] An async appender object.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_async_appender_remove_all_appenders(Log4gAppender *base);

/**
 * \brief Remove an attached appender.
 *
 * \param base [in] An async appender object.
 * \param appender [in] The appender to remove.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_async_appender_remove_appender(Log4gAppender *base,
        Log4gAppender *appender);

/**
 * \brief Remove a named appender.
 *
 * \param base [in] An async appender object.
 * \param name [in] The name of the appender to remove.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_async_appender_remove_appender_name(Log4gAppender *base,
        const gchar *name);

/**
 * \brief Set the blocking property.
 *
 * \param base [in] An async appender object.
 * \param blocking [in] The new blocking value for \e base.
 */
void
log4g_async_appender_set_blocking(Log4gAppender *base, gboolean blocking);

/**
 * \brief Retrieve the blocking property.
 *
 * \param base [in] An async appender object.
 *
 * \return The blocking value for \e base.
 */
gboolean
log4g_async_appender_get_blocking(Log4gAppender *base);

/**
 * \brief Set the buffer-size property.
 *
 * \param base [in] An async appender object.
 * \param size [in] The new buffer size value for \e base.
 */
void
log4g_async_appender_set_buffer_size(Log4gAppender *base, gint size);

/**
 * \brief Retrieve the buffer-size property.
 *
 * \param base [in] An async appender object.
 *
 * \return The buffer size value for \e base.
 */
gint
log4g_async_appender_get_buffer_size(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_ASYNC_APPENDER_H */
