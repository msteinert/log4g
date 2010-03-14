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
 * \brief Defines the minimum set of logging levels recognized by the system.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * The following base log levels are defined:
 * -# OFF
 * -# FATAL
 * -# ERROR
 * -# WARN
 * -# INFO
 * -# DEBUG
 * -# ALL
 *
 * The \e Log4gLevel class may be sub-classed to define a larger level set.
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

/** \brief Integer values of logging levels */
typedef enum {
    LOG4G_LEVEL_ALL_INT = G_MININT, /**< ALL level integer value */
    LOG4G_LEVEL_TRACE_INT = 5000, /**< TRACE level integer value */
    LOG4G_LEVEL_DEBUG_INT = 10000, /**< DEBUG level integer value */
    LOG4G_LEVEL_INFO_INT = 20000, /**< INFO level integer value */
    LOG4G_LEVEL_WARN_INT = 30000, /**< WARN level integer value */
    LOG4G_LEVEL_ERROR_INT = 40000, /**< ERROR level integer value */
    LOG4G_LEVEL_FATAL_INT = 50000, /**< FATAL level integer value */
    LOG4G_LEVEL_OFF_INT = G_MAXINT /**< OFF *level integer value */
} Log4gLevelInt;

/** \brief Log4gLevelClass definition */
struct _Log4gLevel {
    GObject parent_instance;
};

/** \brief Log4gLevelClass definition */
struct _Log4gLevelClass {
    GObjectClass parent_class;
    /**
     * \brief Convert a string passed as an argument to a level.
     *
     * \param level [in] A string representation of a level.
     *
     * \return The level represented by \e string or \e DEBUG if the
     *         conversion failed.
     */
    Log4gLevel *(*string_to_level)(const gchar *level);
    /**
     * \brief Convert a string passed as an argument to a level.
     *
     * \param level [in] A string representation of a level.
     * \param def [in] The level to return if the conversion fails.
     *
     * \return The level represented by \e string or \e def if the
     *         conversion failed.
     */
    Log4gLevel *(*string_to_level_default)(const gchar *level,
            Log4gLevel *def);
    /**
     * \brief Convert an integer passed as an argument to a level.
     *
     * \param level [in] An integer representation of a level.
     *
     * \return The level represented by \e level or \e DEBUG if the
     *         conversion failed.
     */
    Log4gLevel *(*int_to_level)(gint level);
    /**
     * \brief Convert an integer passed as an argument to a level.
     *
     * \param level [in] An integer representation of a level.
     * \param def [in] The level to return if the conversion fails.
     *
     * \return The level represented by \e level or \e def if the
     *         conversion failed.
     */
    Log4gLevel *(*int_to_level_default)(gint level, Log4gLevel *def);
    /*< public >*/
    /**
     * The \e ALL level has the lowest possible rank and is intended to
     * turn on all logging.
     */
    Log4gLevel *ALL;
    /**
     * The \e TRACE level indicates finer-grained informational events
     * than \e DEBUG.
     */
    Log4gLevel *TRACE;
    /**
     * The \e DEBUG level indicates fine-grained informational events that
     * are useful for debugging.
     */
    Log4gLevel *DEBUG;
    /**
     * The \e INFO level indicates informational messages that highlight
     * the progress of the application at a coarse-grained level.
     */
    Log4gLevel *INFO;
    /**
     * The \e WARN indicates potentially harmful situations.
     */
    Log4gLevel *WARN;
    /**
     * The \e ERROR level indicates an error event that may allow the
     * application to continue.
     */
    Log4gLevel *ERROR;
    /**
     * The \e FATAL level indicates a very severe error events that will
     * presumably lead the application to abort.
     */
    Log4gLevel *FATAL;
    /**
     * The \e OFF level has the highest possible rank and is intended to
     * turn off logging.
     */
    Log4gLevel *OFF;
};

GType log4g_level_get_type(void);

/**
 * \brief Create a Log4gLevel object.
 *
 * \param level [in] The log priority.
 * \param string [in] The string representation of this level.
 * \param syslog [in] The syslog equivalent of this level.
 *
 * \return A new Log4gLevel object.
 */
Log4gLevel *
log4g_level_new(gint level, const gchar *string, gint syslog);

/**
 * \brief Determine if two log levels are equal.
 *
 * \param self [in] A level object.
 * \param level [in] A level to compare with \e self.
 *
 * \return \e TRUE if \e self & \e level are equal, \e FALSE otherwise.
 */
gboolean
log4g_level_equals(Log4gLevel *self, Log4gLevel *level);

/**
 * \brief Get the syslog(3) equivalent integer of level object.
 *
 * \param self [in] A level object.
 *
 * \return The syslog equivalent integer of \e self.
 */
gint
log4g_level_get_syslog_equivalent(Log4gLevel *self);

/**
 * \brief Determine if a level is greater or equal than another level.
 *
 * \param self [in] A level object.
 * \param level [in] A level object to compare with \e self.
 *
 * \return \e TRUE if \e level is greater than or equal to \e self, or
 *         \e FALSE if \e self is greater than \e level.
 */
gboolean
log4g_level_is_greater_or_equal(Log4gLevel *self, Log4gLevel *level);

/**
 * \brief Get the string representation of a level.
 *
 * \param self [in] A level object.
 *
 * \return The string representation of \e self.
 */
const gchar *
log4g_level_to_string(Log4gLevel *self);

/**
 * \brief Get the integer representation of a level.
 *
 * \param self [in] A level object.
 *
 * \return The integer representation of \e self.
 */
gint
log4g_level_to_int(Log4gLevel *self);

/**
 * \brief Invokes the virtual static function \e string_to_level().
 *
 * \param string [in] A string representation of a level.
 *
 * \return The level represented by \e string.
 */
Log4gLevel *
log4g_level_string_to_level(const gchar *string);

/**
 * \brief Invokes the virtual static function \e string_to_level_default().
 *
 * \param string [in] A string representation of a level.
 * \param def [in] The level to return if the conversion failed.
 *
 * \return The level represented by \e string or \e def if the conversion
 *         failed.
 */
Log4gLevel *
log4g_level_string_to_level_default(const gchar *string, Log4gLevel *def);

/**
 * \brief Invokes the virtual static function \e int_to_level().
 *
 * \param level [in] An integer representation of a level.
 *
 * \return The level represented by \e level.
 */
Log4gLevel *
log4g_level_int_to_level(gint level);

/**
 * \brief Invokes the virtual static function \e int_to_level_default().
 *
 * \param level [in] An integer representation of a level.
 * \param def [in] The level to return if the conversion failed.
 *
 * \return The level represented by \e level or \e def if the conversion
 *         failed.
 */
Log4gLevel *
log4g_level_int_to_level_default(gint level, Log4gLevel *def);

/**
 * \brief Retrieve the log level \e ALL.
 *
 * \return The log level \e ALL.
 */
Log4gLevel *
log4g_level_ALL(void);

/**
 * \brief Retrieve the log level \e TRACE.
 *
 * \return The log level \e TRACE.
 */
Log4gLevel *
log4g_level_TRACE(void);

/**
 * \brief Retrieve the log level \e DEBUG.
 *
 * \return The log level \e DEBUG.
 */
Log4gLevel *
log4g_level_DEBUG(void);

/**
 * \brief Retrieve the log level \e INFO.
 *
 * \return The log level \e INFO.
 */
Log4gLevel *
log4g_level_INFO(void);

/**
 * \brief Retrieve the log level \e WARN.
 *
 * \return The log level \e WARN.
 */
Log4gLevel *
log4g_level_WARN(void);

/**
 * \brief Retrieve the log level \e ERROR.
 *
 * \return The log level \e ERROR.
 */
Log4gLevel *
log4g_level_ERROR(void);

/**
 * \brief Retrieve the log level \e FATAL.
 *
 * \return The log level \e FATAL.
 */
Log4gLevel *
log4g_level_FATAL(void);

/**
 * \brief Retrieve the log level \e OFF.
 *
 * \return The log level \e OFF.
 */
Log4gLevel *
log4g_level_OFF(void);

G_END_DECLS

#endif /* LOG4G_LEVEL_H */
