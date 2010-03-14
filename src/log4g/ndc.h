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
 * \brief Nested data context.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * The NDC class implements nested data contexts. A nested data context (NDC
 * for short) is an instrument to distinguish interleaved log output from
 * different sources. An example of interleaved log output may occur when
 * a server handles multiple simultaneous connections.
 *
 * Nested diagnostic contexts are handled on a per-thread basis. Calling
 * log4g_ndc_push(), log4g_ndc_pop(), and log4g_ndc_clear() do not affect the
 * NDCs of other threads.
 *
 * Contexts are nested. When enting a context, call ndc_push(). If there is
 * currently no NDC for the current thread, one is created as a side-effect.
 *
 * When leaving a context call log4g_ndc_pop().
 *
 * To clear the current context call log4g_ndc_clear().
 *
 * \note It is not necessary to call log4g_ndc_clear() when exiting a thread.
 *       NDCs are automatically removed when a thread exits.
 *
 * Log4gPatternLayout (log4g/layout/pattern-layout.h) & Log4gTTCCLayout
 * (log4g/layout/ttcc-layout.h) may be configured to automatically retrieve
 * the the nested data context for the current thread without user
 * intervention.
 *
 * Child threads to not automatically inherit the NDC of their parent. To
 * force a thread to inherit a nested data context use log4g_ndc_clone() &
 * log4g_ndc_inherit().
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

/** \brief Log4gNDC object type definition */
typedef struct _Log4gNDC Log4gNDC;

/** \brief Log4gNDC class type definition */
typedef struct _Log4gNDCClass Log4gNDCClass;

/** \brief Log4gNDCClass definition */
struct _Log4gNDC {
    GObject parent_instance;
};

/** \brief Log4gNDCClass definition */
struct _Log4gNDCClass {
    GObjectClass parent_class;
};

GType
log4g_ndc_get_type(void);

/**
 * \brief Get the nested data context object for the current thread.
 *
 * \return A nested data context object.
 *
 * \note Log4g users do not need to call this function.
 */
Log4gNDC *
log4g_ndc_get_instance(void);

/**
 * \brief Clear current nested data context.
 *
 * This function is useful when a thread is used again in a different
 * unrelated context (e.g. thread pools).
 */
void
log4g_ndc_clear(void);

/**
 * \brief Clone the current nested data context.
 *
 * Another thread may inherit the value returned by this function by calling
 * log4g_ndc_inherit().
 *
 * \return A clone of the current nested data context.
 *
 * \note The caller is responsible for calling g_array_free() or
 *       log4g_ndc_inherit() for the returned value.
 */
GArray *
log4g_ndc_clone(void);

/**
 * \brief Inherit a cloned nested data context.
 *
 * \see log4g_ndc_clone()
 *
 * \param stack [in] A cloned nested data context.
 *
 * \note After calling this function the caller should no longer reference
 *       \e stack.
 */
void
log4g_ndc_inherit(GArray *stack);

/**
 * \brief Get the current diagnostic context formatted as a string.
 *
 * \note You should call log4g_logging_event_get_ndc() instead of this
 *       function.
 *
 * \return The current diagnostic context.
 */
const gchar *
log4g_ndc_get(void);

/**
 * \brief Get the size (depth) of the current nested data context.
 *
 * \return The number of elements on the nested data context stack.
 */
guint
log4g_ndc_size(void);

/**
 * \brief Call this function before leaving a diagnostic context.
 *
 * The returned value is the the value that most recently added with
 * log4g_ndc_push(). If no context is available, this function returns
 * \e NULL.
 *
 * \return The innermost diagnostic context.
 */
const gchar *
log4g_ndc_pop(void);

/**
 * \brief Look at the innermost diagnostic context without removing it.
 *
 * The returned value is the the value that most recently added with
 * log4g_ndc_push(). If no context is available, this function returns
 * \e NULL.
 *
 * \return The innermost diagnostic context.
 */
const gchar *
log4g_ndc_peek(void);

/**
 * \brief Push new diagnostic context information for the current thread.
 *
 * \param message [in] A NDC message (accepts printf formats).
 * \param ... [in] Format parameters.
 */
void
log4g_ndc_push(const char *message, ...) G_GNUC_PRINTF(1, 2);

/**
 * \brief Remove all diagnostic context for the current thread.
 */
void
log4g_ndc_remove(void);

/**
 * \brief Set the maximum depth of the current diagnostic context.
 *
 * If the current depth is smaller or equal to \e maxdepth then no action
 * is taken.
 *
 * This function is a convenient alternative to calling \e log4g_ndc_pop()
 * multiple times. The following code example will preserve the depth of the
 * diagnostic context stack after a complex sequence of calls:
 *
 * \code
 * void foo()
 * {
 *     guint depth = log4g_ndc_get_size();
 *     ... complex sequence of calls ...
 *     log4g_ndc_set_max_depth(depth);
 * }
 * \endcode
 *
 * \param maxdepth [in] The new size to truncate the context stack to.
 */
void
log4g_ndc_set_max_depth(guint maxdepth);

G_END_DECLS

#endif /* LOG4G_NDC_H */
