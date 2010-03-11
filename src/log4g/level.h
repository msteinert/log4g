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

#ifndef LOG4G_LEVEL_H
#define LOG4G_LEVEL_H

#include <glib-object.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LEVEL \
    (log4g_level_get_type())

#define LOG4G_LEVEL(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LEVEL, Log4gLevel))

#define LOG4G_IS_LEVEL(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LEVEL))

#define LOG4G_LEVEL_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LEVEL, Log4gLevelClass))

#define LOG4G_IS_LEVEL_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LEVEL))

#define LOG4G_LEVEL_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LEVEL, Log4gLevelClass))

/** \brief Log4gLevel object type definition */
typedef struct _Log4gLevel Log4gLevel;

/** \brief Log4gLevel class type definition */
typedef struct _Log4gLevelClass Log4gLevelClass;

/** \brief Integer representations of logging levels */
typedef enum {
    LOG4G_LEVEL_ALL_INT = G_MININT, /**< Integer representation of ALL */
    LOG4G_LEVEL_TRACE_INT = 5000, /**< Integer representation of TRACE */
    LOG4G_LEVEL_DEBUG_INT = 10000, /**< Integer representation of DEBUG */
    LOG4G_LEVEL_INFO_INT = 20000, /**< Integer representation of INFO */
    LOG4G_LEVEL_WARN_INT = 30000, /**< Integer representation of WARN */
    LOG4G_LEVEL_ERROR_INT = 40000, /**< Integer representation of ERROR */
    LOG4G_LEVEL_FATAL_INT = 50000, /**< Integer representation of FATAL */
    LOG4G_LEVEL_OFF_INT = G_MAXINT /**< Integer representation of OFF */
} Log4gLevelInt;

/** \brief Log4gLevelClass definition */
struct _Log4gLevel {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gLevelClass definition */
struct _Log4gLevelClass {
    GObjectClass parent_class; /**< parent class */
    Log4gLevel *(*string_to_level)(const gchar *level);
    Log4gLevel *(*string_to_level_default)(const gchar *level,
            Log4gLevel *def);
    Log4gLevel *(*int_to_level)(gint level);
    Log4gLevel *(*int_to_level_default)(gint level, Log4gLevel *def);
    /*< public >*/
    Log4gLevel *ALL; /**< ... */
    Log4gLevel *TRACE; /**< ... */
    Log4gLevel *DEBUG; /**< ... */
    Log4gLevel *INFO; /**< ... */
    Log4gLevel *WARN; /**< ... */
    Log4gLevel *ERROR; /**< ... */
    Log4gLevel *FATAL; /**< ... */
    Log4gLevel *OFF; /**< ... */
};

GType log4g_level_get_type(void);

/**
 * \brief Create a Log4gLevel object.
 * \param level [in] The log priority.
 * \param string [in] The string representation of this level.
 * \param syslog [in] The syslog equivalent of this level.
 * \return A new Log4gLevel object.
 */
Log4gLevel *
log4g_level_new(gint level, const gchar *string, gint syslog);

/**
 */
gboolean
log4g_level_equals(Log4gLevel *self, Log4gLevel *level);

/**
 */
gint
log4g_level_get_syslog_equivalent(Log4gLevel *self);

/**
 */
gboolean
log4g_level_is_greater_or_equal(Log4gLevel *self, Log4gLevel *priority);

/**
 */
const gchar *
log4g_level_to_string(Log4gLevel *self);

/**
 */
gint
log4g_level_to_int(Log4gLevel *self);

/**
 * \brief ...
 * \note This is a static method.
 * \param string [in] ...
 * \return ...
 */
Log4gLevel *
log4g_level_string_to_level(const gchar *string);

/**
 * \brief ...
 * \note This is a static method.
 * \param string [in] ...
 * \param def [in] This value is returned if no match is found.
 * \return ...
 */
Log4gLevel *
log4g_level_string_to_level_default(const gchar *string, Log4gLevel *def);

/**
 * \brief ...
 * \note This is a static method.
 * \param level [in] ...
 * \return ...
 */
Log4gLevel *
log4g_level_int_to_level(gint level);

/**
 * \brief ...
 * \note This is a static method.
 * \param level [in] ...
 * \param def [in] This value is returned if no match is found.
 * \return ...
 */
Log4gLevel *
log4g_level_int_to_level_default(gint level, Log4gLevel *def);

/**
 * \brief Retrieve the log level ALL.
 * \note This is a static method.
 * \return The log level ALL.
 */
Log4gLevel *
log4g_level_ALL(void);

/**
 * \brief Retrieve the log level TRACE.
 * \note This is a static method.
 * \return The log level TRACE.
 */
Log4gLevel *
log4g_level_TRACE(void);

/**
 * \brief Retrieve the log level DEBUG.
 * \note This is a static method.
 * \return The log level DEBUG.
 */
Log4gLevel *
log4g_level_DEBUG(void);

/**
 * \brief Retrieve the log level INFO.
 * \note This is a static method.
 * \return The log level INFO.
 */
Log4gLevel *
log4g_level_INFO(void);

/**
 * \brief Retrieve the log level WARN.
 * \note This is a static method.
 * \return The log level WARN.
 */
Log4gLevel *
log4g_level_WARN(void);

/**
 * \brief Retrieve the log level ERROR.
 * \note This is a static method.
 * \return The log level ERROR.
 */
Log4gLevel *
log4g_level_ERROR(void);

/**
 * \brief Retrieve the log level FATAL.
 * \note This is a static method.
 * \return The log level FATAL.
 */
Log4gLevel *
log4g_level_FATAL(void);

/**
 * \brief Retrieve the log level OFF.
 * \note This is a static method.
 * \return The log level OFF.
 */
Log4gLevel *
log4g_level_OFF(void);

G_END_DECLS

#endif /* LOG4G_LEVEL_H */
