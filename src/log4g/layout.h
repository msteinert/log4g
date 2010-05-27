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
 * \brief Log event layout base class.
 * \author Mike Steinert
 * \date 2-5-2010
 *
 * Users may extend this class to implement custom log event layouts.
 *
 * Many appenders require a layout in order to log an event. Sub-classes
 * must override the _Log4gLayoutClass::format() virtual function to implement
 * custom formatting.
 */

#ifndef LOG4G_LAYOUT_H
#define LOG4G_LAYOUT_H

#include <log4g/logging-event.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LAYOUT \
    (log4g_layout_get_type())

#define LOG4G_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LAYOUT, Log4gLayout))

#define LOG4G_IS_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LAYOUT))

#define LOG4G_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LAYOUT, Log4gLayoutClass))

#define LOG4G_IS_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LAYOUT))

#define LOG4G_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LAYOUT, \
            Log4gLayoutClass))

/** \brief The line separator for this platform. */
#define LOG4G_LAYOUT_LINE_SEP "\n"

/** \brief Log4gLayout object type definition */
typedef struct _Log4gLayout Log4gLayout;

/** \brief Log4gLayout class type definition */
typedef struct _Log4gLayoutClass Log4gLayoutClass;

/** \brief Log4gLayoutClass definition */
struct _Log4gLayout {
    GObject parent_instance;
};

/** \brief Log4gLayoutClass definition */
struct _Log4gLayoutClass {
    GObjectClass parent_class;

    /**
     * \brief Implement this function to create your own layout format.
     *
     * \param self [in] A layout object.
     * \param event [in] A logging event object to be laid out.
     *
     * \return The formatted logging event.
     */
    gchar *
    (*format)(Log4gLayout *self, Log4gLoggingEvent *event);

    /**
     * \brief Retrieve the content type output by this layout.
     *
     * The base class returns "text/plain".
     *
     * \param self [in] A layout object.
     *
     * \return The layout content type.
     */
    const gchar *
    (*get_content_type)(Log4gLayout *self);

    /**
     * \brief Retrieve the header for the layout format.
     *
     * The base class returns \e NULL.
     *
     * \param self [in] A layout object.
     *
     * \return The layout header.
     */
    const gchar *
    (*get_header)(Log4gLayout *self);

    /**
     * \brief Retrieve the footer for the layout format.
     *
     * The base class returns \e NULL.
     *
     * \param self [in] A layout object.
     *
     * \return The layout footer.
     */
    const gchar *
    (*get_footer)(Log4gLayout *self);

    /**
     * \brief Activate all options set for this layout.
     *
     * \param self [in] An layout object.
     */
    void
    (*activate_options)(Log4gLayout *self);
};

GType
log4g_layout_get_type(void);

/**
 * \brief Invokes the virtual function _Log4gLayoutClass::format().
 *
 * \param self [in] A layout object.
 * \param event [in] A logging event object to be laid out.
 *
 * \return The formatted logging event.
 */
gchar *
log4g_layout_format(Log4gLayout *self, Log4gLoggingEvent *event);

/**
 * \brief Invokes the virtual function _Log4gLayoutClass::get_content_type().
 *
 * \param self [in] A layout object.
 *
 * \return The layout content type.
 */
const gchar *
log4g_layout_get_content_type(Log4gLayout *self);

/**
 * \brief Invokes the virtual function _Log4gLayoutClass::get_header().
 *
 * \param self [in] A layout object.
 *
 * \return The layout header.
 */
const gchar *
log4g_layout_get_header(Log4gLayout *self);

/**
 * \brief Invokes the virtual function _Log4gLayoutClass::get_footer().
 *
 * \param self [in] A layout object.
 *
 * \return The layout footer.
 */
const gchar *
log4g_layout_get_footer(Log4gLayout *self);

/**
 * \brief Invokes the virtual function _Log4gLayoutClass::activate_options().
 *
 * Layouts generally need to have their options activated before they can be
 * used. This class provides a do-nothing implementation for convenience.
 *
 * \see log4g/interface/option-handler.h
 *
 * \param self [in] A layout object.
 */
void
log4g_layout_activate_options(Log4gLayout *self);

G_END_DECLS

#endif /* LOG4G_LAYOUT_H */
