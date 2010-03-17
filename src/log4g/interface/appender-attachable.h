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
 * \brief Attach appenders to objects.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * This interface defines a standard API for attaching appenders to other
 * objects (e.g. loggers, other appenders).
 *
 * \see log4g/interface/appender.h
 */

#ifndef LOG4G_APPENDER_ATTACHABLE_H
#define LOG4G_APPENDER_ATTACHABLE_H

#include <log4g/interface/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_APPENDER_ATTACHABLE \
    (log4g_appender_attachable_get_type())

#define LOG4G_APPENDER_ATTACHABLE(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_APPENDER_ATTACHABLE, \
            Log4gAppenderAttachable))

#define LOG4G_IS_APPENDER_ATTACHABLE(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_APPENDER_ATTACHABLE))

#define LOG4G_APPENDER_ATTACHABLE_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), \
            LOG4G_TYPE_APPENDER_ATTACHABLE, Log4gAppenderAttachableInterface));

/** \brief Log4gAppenderAttachable object type definition */
typedef struct _Log4gAppenderAttachable Log4gAppenderAttachable;

/** \brief Log4gAppenderAttachable interface type definition */
typedef struct _Log4gAppenderAttachableInterface
                    Log4gAppenderAttachableInterface;

/** \brief Log4gAppenderAttachableInterface definition */
struct _Log4gAppenderAttachableInterface {
    GTypeInterface parent_interface;
    /**
     * \brief Attach an appender to an object.
     *
     * \param self [in] An appender attachable object.
     * \param appender [in] The appender to attach.
     */
    void (*add_appender)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    /**
     * \brief Retrieve all previously attached appenders in an array.
     *
     * \param self [in] An appender attachable object.
     *
     * \return A new array of appenders attached to \e self. If no appenders
     *         are attached, this function shall return \e NULL.
     *
     * \note It is the responsibility of the caller to call g_array_free() on
     *       the returned value.
     */
    const GArray *(*get_all_appenders)(Log4gAppenderAttachable *self);
    /**
     * \brief Retrieve an attached appender by name.
     *
     * \param self [in] An appender attachable object.
     * \param name [in] The name of the appneder to retrieve.
     *
     * \return The appender identified by \e name or \e NULL if the named
     *         appender was not found.
     */
    Log4gAppender *(*get_appender)(Log4gAppenderAttachable *self,
            const gchar *name);
    /**
     * \brief Determine if an appender is attached to an object.
     *
     * \param self [in] An appender attachable object.
     * \param appender [in] The appender to check.
     *
     * \return \e TRUE if the named appender is attached to \e self, \e FALSE
     *         otherwise.
     */
    gboolean (*is_attached)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    /**
     * \brief Remove all attached appenders from an object.
     *
     * \param self [in] An appender attachable object.
     */
    void (*remove_all_appenders)(Log4gAppenderAttachable *self);
    /**
     * \brief Remove an attached appender from an object.
     *
     * \param self [in] An appender attachable object.
     * \param appender [in] The appender to remove.
     */
    void (*remove_appender)(Log4gAppenderAttachable *self,
            Log4gAppender *appender);
    /**
     * \brief Remove a named appender from an object.
     *
     * \param self [in] An appender attachable object.
     * \param name [in] The name of the appedner to remove.
     */
    void (*remove_appender_name)(Log4gAppenderAttachable *self,
            const gchar *name);
};

GType
log4g_appender_attachable_get_type(void);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::add_appender().
 *
 * \param self [in] An appender attachable object.
 * \param appender [in] An appender.
 */
void
log4g_appender_attachable_add_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::get_all_appenders().
 *
 * \param self [in] An appender attachable object.
 *
 * \return An array of appenders attached to \e self.
 */
const GArray *
log4g_appender_attachable_get_all_appenders(Log4gAppenderAttachable *self);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::get_appender().
 *
 * \param self [in] An appender attachable object.
 * \param name [in] The name of the appender to retrieve.
 *
 * \return The appender named \e name, or \e NULL if not found.
 */
Log4gAppender *
log4g_appender_attachable_get_appender(Log4gAppenderAttachable *self,
        const gchar *name);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::is_attached().
 *
 * \param self [in] An appender attachable object.
 * \param apepnder [in] The appender to check.
 */
gboolean
log4g_appender_attachable_is_attached(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::remove_all_appenders().
 *
 * \param self [in] An appender attachable object.
 */
void
log4g_appender_attachable_remove_all_appenders(Log4gAppenderAttachable *self);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::remove_appender().
 *
 * \param self [in] An appender attachable object.
 * \param appender [in] The appender to remove.
 */
void
log4g_appender_attachable_remove_appender(Log4gAppenderAttachable *self,
        Log4gAppender *appender);

/**
 * \brief Invoke the virtual function
 *        _Log4gAppenderAttachableInterface::remove_appender_name().
 *
 * \param self [in] An appender attachable object.
 * \param name [in] The name of the appender to remove.
 */
void
log4g_appender_attachable_remove_appender_name(Log4gAppenderAttachable *self,
        const gchar *name);

G_END_DECLS

#endif /* LOG4G_APPENDER_ATTACHABLE_H */
