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

#ifndef LOG4G_NDC_H
#define LOG4G_NDC_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_NDC \
    (log4g_ndc_get_type())

#define LOG4G_NDC(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_NDC, Log4gNDC))

#define LOG4G_IS_NDC(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_NDC))

#define LOG4G_NDC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_NDC, Log4gNDCClass))

#define LOG4G_IS_NDC_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_NDC))

#define LOG4G_NDC_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_NDC, Log4gNDCClass))

typedef struct _Log4gNDC Log4gNDC;

typedef struct _Log4gNDCClass Log4gNDCClass;

/**
 * Log4gNDC:
 *
 * The <structname>Log4gNDC</structname> structure does not have any public
 * members.
 */
struct _Log4gNDC {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gNDCClass:
 *
 * The <structname>Log4gNDCClass</structname> structure does not have any
 * public members.
 */
struct _Log4gNDCClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_ndc_get_type(void);

void
log4g_ndc_clear(void);

GArray *
log4g_ndc_clone(void);

void
log4g_ndc_inherit(GArray *stack);

const gchar *
log4g_ndc_get(void);

guint
log4g_ndc_size(void);

const gchar *
log4g_ndc_pop(void);

const gchar *
log4g_ndc_peek(void);

void
log4g_ndc_push(const char *message, ...) G_GNUC_PRINTF(1, 2);

void
log4g_ndc_remove(void);

void
log4g_ndc_set_max_depth(guint maxdepth);

G_END_DECLS

#endif /* LOG4G_NDC_H */
