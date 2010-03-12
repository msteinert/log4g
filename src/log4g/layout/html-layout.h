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
 * \date 2-16-2010
 */

#ifndef LOG4G_HTML_LAYOUT_H
#define LOG4G_HTML_LAYOUT_H

#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_HTML_LAYOUT \
    (log4g_html_layout_get_type())

#define LOG4G_HTML_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_HTML_LAYOUT, \
            Log4gHTMLLayout))

#define LOG4G_IS_HTML_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_HTML_LAYOUT))

#define LOG4G_HTML_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_HTML_LAYOUT, \
            Log4gHTMLLayoutClass))

#define LOG4G_IS_HTML_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_HTML_LAYOUT))

#define LOG4G_HTML_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_HTML_LAYOUT, \
            Log4gHTMLLayoutClass))

/** \brief Log4gHTMLLayout object type definition */
typedef struct _Log4gHTMLLayout Log4gHTMLLayout;

/** \brief Log4gHTMLLayout class type definition */
typedef struct _Log4gHTMLLayoutClass Log4gHTMLLayoutClass;

/** \brief Log4gHTMLLayoutClass definition */
struct _Log4gHTMLLayout {
    Log4gLayout parent_instance; /**< parent instance */
    /*< protected >*/
    gint BUF_SIZE;
    gint MAX_CAPACITY;
    /*< private >*/
    gchar *title;
    GString *string;
    gboolean info;
};

/** \brief Log4gHTMLLayoutClass definition */
struct _Log4gHTMLLayoutClass {
    Log4gLayoutClass parent_class; /**< parent class */
};

GType
log4g_html_layout_get_type(void);

/**
 */
Log4gLayout *
log4g_html_layout_new(void);

/**
 */
void
log4g_html_layout_set_title(Log4gLayout *base, const gchar *title);

/**
 */
const gchar *
log4g_html_layout_get_title(Log4gLayout *base);

/**
 */
void
log4g_html_layout_set_location_info(Log4gLayout *base, gboolean info);

/**
 */
gboolean
log4g_html_layout_get_location_info(Log4gLayout *base);

G_END_DECLS

#endif /* LOG4G_HTML_LAYOUT_H */