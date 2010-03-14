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
 * \brief Mapped data context.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * The MDC class provides mapped data contexts. A mapped data context (MDC for
 * short) is an instrument for distinguishing interleaved log output from
 * different sources. An example of interleaved log output may occur when a
 * server handles multiple clients simultaneously.
 *
 * Mapped data context is managed on a per-thread basis. The main difference
 * between Log4g MDCs and Log4j MDCs is that Log4g contexts are \e not
 * inherited by child threads.
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
    GObject parent_instance;
};

/** \brief Log4gMDCClass definition */
struct _Log4gMDCClass {
    GObjectClass parent_class;
};

GType
log4g_mdc_get_type(void);

/**
 * \brief Retrieve the MDC instance object for currently executing thread.
 *
 * \return A mapped data context object.
 *
 * \note Log4g users do not need to call this function.
 */
Log4gMDC *
log4g_mdc_get_instance(void);

/**
 * \brief Put a context \e value as identified by a \e key into the current
 *        thread's context map.
 *
 * If a context map has not been created for the current thread it will be
 * created as a side-effect.
 *
 * \param key [in] The key to associate with \e value.
 * \param value [in] The value to associate with \e key (accepts
 *                   printf formats).
 * \param ... [in] Format parameters.
 */
void
log4g_mdc_put(const gchar *key, const gchar *value, ...) G_GNUC_PRINTF(2, 3);

/**
 * \brief Retrieve a the context value associated with a \e key from the
 *        current thread's context map.
 *
 * \param key [in] The key to retrieve.
 *
 * \return The context value associated with \e key.
 */
const gchar *
log4g_mdc_get(const gchar *key);

/**
 * \brief Remove a context value associated with a \e key from the current
 *        thread's context map.
 *
 * \param key [in] The key to remove.
 */
void
log4g_mdc_remove(const gchar *key);

/**
 * \brief Get the current thread's MDC as a hash table.
 *
 * This function is intended for internal use.
 *
 * \return The current MDC context as a hash table.
 */
const GHashTable *
log4g_mdc_get_context(void);

G_END_DECLS

#endif /* LOG4G_MDC_H */
