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
 * \date 2-5-2010
 */

#ifndef LOG4G_LAYOUT_H
#define LOG4G_LAYOUT_H

#include <log4g/interface/option-handler.h>
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
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LAYOUT, Log4gLayoutClass))

/** \brief Log4gLayout object type definition */
typedef struct _Log4gLayout Log4gLayout;

/** \brief Log4gLayout class type definition */
typedef struct _Log4gLayoutClass Log4gLayoutClass;

/** \brief Log4gLayoutClass definition */
struct _Log4gLayout {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gLayoutClass definition */
struct _Log4gLayoutClass {
    GObjectClass parent_class; /**< parent class */
    gchar *(*format)(Log4gLayout *self, Log4gLoggingEvent *event);
    const gchar *(*get_content_type)(Log4gLayout *self);
    const gchar *(*get_header)(Log4gLayout *self);
    const gchar *(*get_footer)(Log4gLayout *self);
    /*< public >*/
    gchar *LINE_SEP;
    gint LINE_SEP_LEN;
};

GType
log4g_layout_get_type(void);

/**
 */
gchar *
log4g_layout_format(Log4gLayout *self, Log4gLoggingEvent *event);

/**
 */
const gchar *
log4g_layout_get_content_type(Log4gLayout *self);

/**
 */
const gchar *
log4g_layout_get_header(Log4gLayout *self);

/**
 */
const gchar *
log4g_layout_get_footer(Log4gLayout *self);

/**
 */
void
log4g_layout_activate_options(Log4gLayout *self);

G_END_DECLS

#endif /* LOG4G_LAYOUT_H */
