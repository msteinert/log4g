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
 * \brief A simple log layout.
 * \author Mike Steinert
 * \date 2-5-2010
 *
 * The simple layout consists of the level of a log statement and the log
 * message itself separated by " - ". For example:
 *
 * \code
 * DEBUG - Hello, World!
 * \endcode
 *
 * \see log4g/layout/pattern-layout.h
 */

#ifndef LOG4G_SIMPLE_LAYOUT_H
#define LOG4G_SIMPLE_LAYOUT_H

#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_SIMPLE_LAYOUT \
    (log4g_simple_layout_get_type())

#define LOG4G_SIMPLE_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_SIMPLE_LAYOUT, \
            Log4gSimpleLayout))

#define LOG4G_IS_SIMPLE_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_SIMPLE_LAYOUT))

#define LOG4G_SIMPLE_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_SIMPLE_LAYOUT, \
            Log4gSimpleLayoutClass))

#define LOG4G_IS_SIMPLE_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_SIMPLE_LAYOUT))

#define LOG4G_SIMPLE_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_SIMPLE_LAYOUT, \
            Log4gSimpleLayoutClass))

/** \brief Log4gSimpleLayout object type definition */
typedef struct _Log4gSimpleLayout Log4gSimpleLayout;

/** \brief Log4gSimpleLayout class type definition */
typedef struct _Log4gSimpleLayoutClass Log4gSimpleLayoutClass;

/** \brief Log4gSimpleLayoutClass definition */
struct _Log4gSimpleLayout {
    Log4gLayout parent_instance;
};

/** \brief Log4gSimpleLayoutClass definition */
struct _Log4gSimpleLayoutClass {
    Log4gLayoutClass parent_class;
};

GType
log4g_simple_layout_get_type(void);

void
log4g_simple_layout_register(GTypeModule *module);

/**
 * \brief Create a new simple layout object.
 *
 * \return A new simple layout object.
 */
Log4gLayout *
log4g_simple_layout_new(void);

G_END_DECLS

#endif /* LOG4G_SIMPLE_LAYOUT_H */
