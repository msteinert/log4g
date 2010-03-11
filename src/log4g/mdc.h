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
 * \date 1-29-2010
 */

#ifndef LOG4G_MDC_H
#define LOG4G_MDC_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_MDC \
    (log4g_mdc_get_type())

#define LOG4G_MDC(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_MDC, Log4gMDC))

#define LOG4G_IS_MDC(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_MDC))

#define LOG4G_MDC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_MDC, Log4gMDCClass))

#define LOG4G_IS_MDC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_MDC))

#define LOG4G_MDC_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_MDC, Log4gMDCClass))

/** \brief Log4gMDC object type definition */
typedef struct _Log4gMDC Log4gMDC;

/** \brief Log4gMDC class type definition */
typedef struct _Log4gMDCClass Log4gMDCClass;

/** \brief Log4gMDCClass definition */
struct _Log4gMDC {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gMDCClass definition */
struct _Log4gMDCClass {
    GObjectClass parent_class; /**< parent class */
};

GType
log4g_mdc_get_type(void);

/**
 */
Log4gMDC *
log4g_mdc_get_instance(void);

/**
 */
void
log4g_mdc_put(const gchar *key, const gchar *value, ...) G_GNUC_PRINTF(2, 3);

/**
 */
const gchar *
log4g_mdc_get(const gchar *key);

/**
 */
void
log4g_mdc_remove(const gchar *key);

/**
 */
const GHashTable *
log4g_mdc_get_context(void);

G_END_DECLS

#endif /* LOG4G_MDC_H */
