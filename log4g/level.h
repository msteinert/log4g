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

typedef struct _Log4gLevel Log4gLevel;

typedef struct _Log4gLevelClass Log4gLevelClass;

/**
 * Log4gLevelInt:
 * @LOG4G_LEVEL_ALL_INT: ALL level integer value
 * @LOG4G_LEVEL_TRACE_INT: TRACE level integer value
 * @LOG4G_LEVEL_DEBUG_INT: DEBUG level integer value
 * @LOG4G_LEVEL_INFO_INT: INFO level integer value
 * @LOG4G_LEVEL_WARN_INT: WARN level integer value
 * @LOG4G_LEVEL_ERROR_INT: ERROR level integer value
 * @LOG4G_LEVEL_FATAL_INT: FATAL level integer value
 * @LOG4G_LEVEL_OFF_INT: OFF level integer value
 */
typedef enum {
    LOG4G_LEVEL_ALL_INT = G_MININT,
    LOG4G_LEVEL_TRACE_INT = 5000,
    LOG4G_LEVEL_DEBUG_INT = 10000,
    LOG4G_LEVEL_INFO_INT = 20000,
    LOG4G_LEVEL_WARN_INT = 30000,
    LOG4G_LEVEL_ERROR_INT = 40000,
    LOG4G_LEVEL_FATAL_INT = 50000,
    LOG4G_LEVEL_OFF_INT = G_MAXINT
} Log4gLevelInt;

/**
 * Log4gLevel:
 *
 * The <structname>Log4gLevel</structname> structure does not have any public
 * members.
 */
struct _Log4gLevel {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gLevelStringToLevel:
 * @level: A string representation of a level.
 *
 * Convert a string passed as an argument to a level.
 *
 * Returns: The level represented by @string or DEBUG if the conversion failed.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLevelStringToLevel)(const gchar *level);

/**
 * Log4gLevelStringToLevelDefault:
 * @level: A string representation of a level.
 * @def: The level to return if the conversion fails.
 *
 * Convert a string passed as an argument to a level.
 *
 * Returns: The level represented by @string or @def if the conversion failed.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLevelStringToLevelDefault)(const gchar *level, Log4gLevel *def);

/**
 * Log4gLevelIntToLevel:
 * @level: An integer representation of a level.
 *
 * Convert an integer passed as an argument to a level.
 *
 * Returns: The level represented by @level or DEBUG if the conversion failed.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLevelIntToLevel)(gint level);

/**
 * Log4gLevelIntToLevelDefault:
 * @level: An integer representation of a level.
 * @def: The level to return if the conversion fails.
 *
 * Convert an integer passed as an argument to a level.
 *
 * Returns: The level represented by @level or @def if the conversion failed.
 * Since: 0.1
 */
typedef Log4gLevel *
(*Log4gLevelIntToLevelDefault)(gint level, Log4gLevel *def);

/**
 * Log4gLevelClass:
 * @string_to_level: Convert a string passed as an argument to a level.
 * @string_to_level_default: Convert a string passed as an argument to a level.
 * @int_to_level: Convert an integer passed as an argument to a level.
 * @int_to_level_default: Convert an integer passed as an argument to a level.
 * @ALL: Has the lowest possible rank and is intended to turn on all logging.
 * @TRACE: Indicates finer-grained information than #Log4gLevelClass.DEBUG.
 * @DEBUG: Indicates fine-grained events that are useful for debugging.
 * @INFO: Indicates messages that highlight the progress of the application at
 *        a coarse-grained level.
 * @WARN: Indicates potentially harmful situations.
 * @ERROR: Indicates an error event that may allow the application to continue.
 * @FATAL: Indicates a very severe error that will presumably lead the
 *         application to abort.
 * @OFF: Has the highest possible rank and is intended to turn off logging.
 */
struct _Log4gLevelClass {
    /*< private >*/
    GObjectClass parent_class;
    /*< public >*/
    Log4gLevelStringToLevel string_to_level;
    Log4gLevelStringToLevelDefault string_to_level_default;
    Log4gLevelIntToLevel int_to_level;
    Log4gLevelIntToLevelDefault int_to_level_default;
    Log4gLevel *ALL;
    Log4gLevel *TRACE;
    Log4gLevel *DEBUG;
    Log4gLevel *INFO;
    Log4gLevel *WARN;
    Log4gLevel *ERROR;
    Log4gLevel *FATAL;
    Log4gLevel *OFF;
};

GType log4g_level_get_type(void);

Log4gLevel *
log4g_level_new(gint level, const gchar *string, gint syslog);

gboolean
log4g_level_equals(Log4gLevel *self, Log4gLevel *level);

gint
log4g_level_get_syslog_equivalent(Log4gLevel *self);

gboolean
log4g_level_is_greater_or_equal(Log4gLevel *self, Log4gLevel *level);

const gchar *
log4g_level_to_string(Log4gLevel *self);

gint
log4g_level_to_int(Log4gLevel *self);

Log4gLevel *
log4g_level_string_to_level(const gchar *string);

Log4gLevel *
log4g_level_string_to_level_default(const gchar *string, Log4gLevel *def);

Log4gLevel *
log4g_level_int_to_level(gint level);

Log4gLevel *
log4g_level_int_to_level_default(gint level, Log4gLevel *def);

Log4gLevel *
log4g_level_ALL(void);

Log4gLevel *
log4g_level_TRACE(void);

Log4gLevel *
log4g_level_DEBUG(void);

Log4gLevel *
log4g_level_INFO(void);

Log4gLevel *
log4g_level_WARN(void);

Log4gLevel *
log4g_level_ERROR(void);

Log4gLevel *
log4g_level_FATAL(void);

Log4gLevel *
log4g_level_OFF(void);

G_END_DECLS

#endif /* LOG4G_LEVEL_H */
