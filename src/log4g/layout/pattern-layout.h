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
 * \date 2-12-2010
 */

#ifndef LOG4G_PATTERN_LAYOUT_H
#define LOG4G_PATTERN_LAYOUT_H

#include <log4g/helpers/pattern-parser.h>
#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_PATTERN_LAYOUT \
    (log4g_pattern_layout_get_type())

#define LOG4G_PATTERN_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayout))

#define LOG4G_IS_PATTERN_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_PATTERN_LAYOUT))

#define LOG4G_PATTERN_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayoutClass))

#define LOG4G_IS_PATTERN_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_PATTERN_LAYOUT))

#define LOG4G_PATTERN_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayoutClass))

/** \brief Log4gPatternLayout object type definition */
typedef struct _Log4gPatternLayout Log4gPatternLayout;

/** \brief Log4gPatternLayout class type definition */
typedef struct _Log4gPatternLayoutClass Log4gPatternLayoutClass;

/** \brief Log4gPatternLayoutClass definition */
struct _Log4gPatternLayout {
    Log4gLayout parent_instance; /**< parent instance */
};

/** \brief Log4gPatternLayoutClass definition */
struct _Log4gPatternLayoutClass {
    Log4gLayoutClass parent_class; /**< parent class */
    Log4gPatternParser *(*create_pattern_parser)(Log4gLayout *base,
            const gchar *pattern);
};

GType
log4g_pattern_layout_get_type(void);

/**
 */
Log4gLayout *
log4g_pattern_layout_new(const gchar *pattern);

/**
 */
void
log4g_pattern_layout_set_conversion_pattern(Log4gLayout *base,
        const gchar *pattern);

/**
 */
const gchar *
log4g_pattern_layout_get_conversion_pattern(Log4gLayout *base);

/**
 */
Log4gPatternParser *
log4g_pattern_layout_create_pattern_parser(Log4gLayout *base,
        const gchar *pattern);

G_END_DECLS

#endif /* LOG4G_PATTERN_LAYOUT_H */
