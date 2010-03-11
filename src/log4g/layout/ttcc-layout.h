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
 * \date 2-9-2010
 */

#ifndef LOG4G_TTCC_LAYOUT_H
#define LOG4G_TTCC_LAYOUT_H

#include <log4g/helpers/date-layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_TTCC_LAYOUT \
    (log4g_ttcc_layout_get_type())

#define LOG4G_TTCC_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_TTCC_LAYOUT, \
            Log4gTTCCLayout))

#define LOG4G_IS_TTCC_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_TTCC_LAYOUT))

#define LOG4G_TTCC_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_TTCC_LAYOUT, \
            Log4gTTCCLayoutClass))

#define LOG4G_IS_TTCC_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_TTCC_LAYOUT))

#define LOG4G_TTCC_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_TTCC_LAYOUT, \
            Log4gTTCCLayoutClass))

/** \brief Log4gTTCCLayout object type definition */
typedef struct _Log4gTTCCLayout Log4gTTCCLayout;

/** \brief Log4gTTCCLayout class type definition */
typedef struct _Log4gTTCCLayoutClass Log4gTTCCLayoutClass;

/** \brief Log4gTTCCLayoutClass definition */
struct _Log4gTTCCLayout {
    Log4gDateLayout parent_instance; /**< parent instance */
};

/** \brief Log4gTTCCLayoutClass definition */
struct _Log4gTTCCLayoutClass {
    Log4gDateLayoutClass parent_class; /**< parent class */
};

GType
log4g_ttcc_layout_get_type(void);

/**
 */
Log4gLayout *
log4g_ttcc_layout_new(const char *format);

/**
 */
void
log4g_ttcc_set_thread_printing(Log4gLayout *base, gboolean thread);

/**
 */
gboolean
log4g_ttcc_get_thread_printing(Log4gLayout *base);

/**
 */
void
log4g_ttcc_set_category_prefixing(Log4gLayout *base, gboolean category);

/**
 */
gboolean
log4g_ttcc_get_category_prefixing(Log4gLayout *base);

/**
 */
void
log4g_ttcc_set_context_printing(Log4gLayout *base, gboolean context);

/**
 */
gboolean
log4g_ttcc_get_context_printing(Log4gLayout *base);

G_END_DECLS

#endif /* LOG4G_TTCC_LAYOUT_H */
