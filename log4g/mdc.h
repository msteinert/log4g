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

typedef struct _Log4gMDC Log4gMDC;

typedef struct _Log4gMDCClass Log4gMDCClass;

/**
 * Log4gMDC:
 *
 * The <structname>Log4gMDC</structname> structure does not have any public
 * members.
 */
struct _Log4gMDC {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gMDCClass:
 *
 * The <structname>Log4gMDCClass</structname> structure does not have any
 * public members.
 */
struct _Log4gMDCClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_mdc_get_type(void);

void
log4g_mdc_put(const gchar *key, const gchar *value, ...) G_GNUC_PRINTF(2, 3);

const gchar *
log4g_mdc_get(const gchar *key);

void
log4g_mdc_remove(const gchar *key);

const GHashTable *
log4g_mdc_get_context(void);

G_END_DECLS

#endif /* LOG4G_MDC_H */
