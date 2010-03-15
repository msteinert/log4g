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
 * \brief Output the time, thread, category & context.
 * \author Mike Steinert
 * \date 2-9-2010
 *
 * The TTCC layout outputs the time, thread, category (logger name), and
 * context (nested diagnostic context) of a log event, hence the name.
 *
 * Some fields can be individually enabled or disabled via the corresponding
 * properties:
 * -# thread-printing
 * -# category-prefixing
 * -# context-printing
 *
 * Here is an example of the TTCC output:
 *
 * \code
 * 176 [main] INFO  org.gnome.log4g.foo - an example message
 * 225 [main] DEBUG org.gnome.log4g.bar - another example message
 * \endcode
 *
 * The first field is the number of milliseconds elapsed since the Log4g
 * system was initialized. The second field is the name of the thread that
 * logged the event. The third field is the log level. The fourth field is
 * the logger category to which the statement belongs.
 *
 * The fifth field (just before the '-') is the nested diagnostic context.
 *
 * \note The nested diagnostic context may be empty (as in the example).
 *
 * \warning Do not use the same TTCC layout instance from within different
 *          appenders. The TTCC layout is not thread-safe when used in this
 *          way.
 *
 * \note The pattern layout (log4g/layout/pattern-layout.h) provides a much
 *       more flexible alternative.
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
    Log4gDateLayout parent_instance;
};

/** \brief Log4gTTCCLayoutClass definition */
struct _Log4gTTCCLayoutClass {
    Log4gDateLayoutClass parent_class;
};

GType
log4g_ttcc_layout_get_type(void);

/**
 * \brief Create a new TTCC layout object.
 *
 * \param format [in] The strftime(3) date format to use (\e NULL is OK).
 *
 * \return A new TTCC layout object.
 *
 * \see strftime(3)
 */
Log4gLayout *
log4g_ttcc_layout_new(const char *format);

/**
 * \brief Set the thread-printing property.
 *
 * \param base [in] A TTCC layout object.
 * \param thread [in] The new thread printing value for \e base.
 */
void
log4g_ttcc_set_thread_printing(Log4gLayout *base, gboolean thread);

/**
 * \brief Get the thread-printing property.
 *
 * \param base [in] A TTCC layout object.
 *
 * \return The thread printing value for \e base.
 */
gboolean
log4g_ttcc_get_thread_printing(Log4gLayout *base);

/**
 * \brief Set the category-prefixing property.
 *
 * \param base [in] A TTCC layout object.
 * \param category [in] The new category prefixing value for \e base.
 */
void
log4g_ttcc_set_category_prefixing(Log4gLayout *base, gboolean category);

/**
 * \brief Get the category-prefixing property.
 *
 * \param base [in] A TTCC layout object.
 *
 * \return The category prefixing value for \e base.
 */
gboolean
log4g_ttcc_get_category_prefixing(Log4gLayout *base);

/**
 * \brief Set the context-printing property.
 *
 * \param base [in] A TTCC layout object.
 * \param context [in] The new context printing value for \e base.
 */
void
log4g_ttcc_set_context_printing(Log4gLayout *base, gboolean context);

/**
 * \brief Get the context-printing property.
 *
 * \param base [in] A TTCC layout object.
 *
 * \return The context printing value for \e base.
 */
gboolean
log4g_ttcc_get_context_printing(Log4gLayout *base);

G_END_DECLS

#endif /* LOG4G_TTCC_LAYOUT_H */
