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
 * \brief The central class in the Log4g package.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * Most logging operations (except configuration) are performed through this
 * class.
 */

#ifndef LOG4G_LOGGER_H
#define LOG4G_LOGGER_H

#include <log4g/interface/appender.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_LOGGER \
    (log4g_logger_get_type())

#define LOG4G_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_LOGGER, Log4gLogger))

#define LOG4G_IS_LOGGER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_LOGGER, Log4gLoggerClass))

#define LOG4G_IS_LOGGER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_LOGGER))

#define LOG4G_LOGGER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_LOGGER, \
            Log4gLoggerClass))

/** \brief Log4gLogger object type definition */
typedef struct _Log4gLogger Log4gLogger;

/** \brief Log4gLogger class type definition */
typedef struct _Log4gLoggerClass Log4gLoggerClass;

/** \brief Log4gLoggerClass definition */
struct _Log4gLogger {
    GObject parent_instance;
};

/** \brief Log4gLoggerClass definition */
struct _Log4gLoggerClass {
    GObjectClass parent_class;

    /**
     * \brief Retrieve the effective level threshold of a logger.
     *
     * Starting from this category, search the category hierarchy for a
     * \e non-NULL and return it. If a \e non-NULL level is not found, this
     * function returns the level of the root logger.
     *
     * \param self [in] A logger object.
     *
     * \return The effect level threshold of \e self.
     */
    Log4gLevel *
    (*get_effective_level)(Log4gLogger *self);

    /**
     * \brief Set the level threshold of a logger.
     *
     * \param self [in] A logger object.
     * \param level [in] The new level threshold for \e self.
     *
     * \note \e NULL values are permitted.
     */
    void
    (*set_level)(Log4gLogger *self, Log4gLevel *level);
};

GType
log4g_logger_get_type(void);

/**
 * \brief Create a new logger object and set its name.
 *
 * This is intended to be used internally only. You should not create
 * Loggers directly.
 * \see log4g_logger_get_logger()
 *
 * \param name [in] The fully qualified name for the new logger.
 *
 * \return A new logger object.
 */
Log4gLogger *
log4g_logger_new(const gchar *name);

/**
 * \brief Retrieve the fully-qualified name of a logger.
 *
 * \param self [in] A logger object.
 *
 * \return The name of \e self.
 */
const gchar *
log4g_logger_get_name(Log4gLogger *self);

/**
 * \brief [protected] Set the name of a logger.
 *
 * \warning This method should only be called by sub-classes. Calling this
 *          method on a logger in the logger repository will have disastrous
 *          effects on the logger hierarchy.
 *
 * \param self [in] A logger object.
 * \param name [in] The new name for \e self.
 */
void
log4g_logger_set_name(Log4gLogger *self, const gchar *name);

/**
 * \brief Retrieve the parent of this logger.
 *
 * \note The parent of a logger may change during its lifetime.
 *       The root logger will return \e NULL.
 *
 * \param self [in] A logger object.
 *
 * \return The parent of \e self.
 */
Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self);

/**
 * \brief Set the parent of a logger.
 *
 * \param self [in] A logger object.
 * \param parent [in] The new parent of \e self.
 */
void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent);

/**
 * \brief Retrieve the level threshold of a logger.
 *
 * \param self [in] A logger object.
 *
 * \return The log level threshold for \e self.
 */
Log4gLevel *
log4g_logger_get_level(Log4gLogger *self);

/**
 * \brief Invokes the virtual function _Log4gLoggerClass::set_level().
 *
 * \param self [in] A logger object.
 * \param level [in] The new log level threshold for \e self.
 */
void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level);

/**
 * \brief Retrieve the additivity flag for a logger.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is additive, \e FALSE otherwise.
 *
 * \see log4g_logger_set_additivity()
 */
gboolean
log4g_logger_get_additivity(Log4gLogger *self);

/**
 * \brief Set the additivity flag for a logger.
 *
 * Logger additivity determines if a logger inherits the appenders of its
 * ancestors. If this flag is set to \e TRUE (the default value is \e TRUE)
 * then events logged to this logger will be propagated to the appenders of
 * its ancestors. If this flags is set to \e FALSE then the appenders of this
 * logger will not be inherited.
 *
 * \param self [in] A logger object.
 * \param additive [in] The new additivity flag for \e self.
 */
void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive);

/**
 * \brief Retrieve the repository where a logger is attached.
 *
 * \param self [in] A logger object.
 *
 * \return The logger repository \e self is attached to.
 *
 * \see log4g/interface/logger-repository.h
 */
gpointer
log4g_logger_get_logger_repository(Log4gLogger *self);

/**
 * \brief Set the repository a logger is attached to.
 *
 * You probably do not want to call this function.
 *
 * \param self [in] A logger object.
 * \param repository [in] The new repository to attach \e self to.
 *
 * \see log4g/interface/logger-repository.h
 */
void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository);

/**
 * \brief Invokes the virtual function
 *        _Log4gLoggerClass::get_effective_level().
 *
 * \param self [in] A logger object.
 *
 * \return The effective level threshold of \e self.
 */
Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self);

/**
 * \brief Add an appender to the list of appenders for a logger.
 *
 * If \e appender is already in the list of appenders for \e self then
 * it will not be added again.
 *
 * \param base [in] A logger object.
 * \param appender [in] The appender to add to \e base.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_logger_add_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 * \brief Retrieve all appenders attached to a logger.
 *
 * \param base [in] A logger object.
 *
 * \return A GArray of appenders attached to \e self or \e NULL if there are
 *         none.
 *
 * \see log4g/interface/appender-attachable.h
 */
const GArray *
log4g_logger_get_all_appenders(Log4gLogger *base);

/**
 * \brief Retrieve an appender attached to a logger by name.
 *
 * \param base [in] A logger object.
 * \param name [in] The name of the appender to retrieve.
 *
 * \return The appender named \e name or \e NULL if no such appender is
 *         attached to \e base.
 *
 * \see log4g/interface/appender-attachable.h
 */
Log4gAppender *
log4g_logger_get_appender(Log4gLogger *base, const gchar *name);

/**
 * \brief Determine if an appender is attached to a logger.
 *
 * \param base [in] A logger object.
 * \param appender [in] An appender object.
 *
 * \return \e TRUE if \e appender is attached to \e self, \e FALSE otherwise.
 *
 * \see log4g/interface/appender-attachable.h
 */
gboolean
log4g_logger_is_attached(Log4gLogger *base, Log4gAppender *appender);

/**
 * \brief Remove all appenders from a logger.
 *
 * \param base [in] A logger object.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_logger_remove_all_appenders(Log4gLogger *base);

/**
 * \brief Remove an appender from a logger.
 *
 * If \e appender is not attached to \e base then this function does nothing.
 *
 * \param base [in] A logger object.
 * \param appender [in] An appender to remove.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_logger_remove_appender(Log4gLogger *base, Log4gAppender *appender);

/**
 * \brief Remove an appender from a logger by name.
 *
 * If \e name is not found then this function does nothing.
 *
 * \param base [in] A logger object.
 * \param name [in] The name of the appender to remove.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_logger_remove_appender_name(Log4gLogger *base, const gchar *name);

/**
 * \brief Close all attached appenders implementing the
 *        Log4gAppenderAttachable interface.
 *
 * \param self [in] A logger object.
 *
 * \see log4g/interface/appender-attachable.h
 */
void
log4g_logger_close_nested_appenders(Log4gLogger *self);

/**
 * \brief Logs an error if the \e assertion parameter is \e FALSE.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_assert(), log4g_logger_assert()
 *
 * \param self [in] A logger object.
 * \param assertion [in] The assertion to evaluate.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_assert(Log4gLogger *self, gboolean assertion,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, ...) G_GNUC_PRINTF(6, 7);

/**
 * \brief Check if a logger is enabled for the \e TRACE level.
 *
 * This function is useful if you have a to perform a costly operation to
 * construct a log message.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e TRACE level, \e FALSE
 *         otherwise.
 */
gboolean log4g_logger_is_trace_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e TRACE level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_trace(), log4g_logger_trace()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_trace(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief Check if a logger is enabled for the \e DEBUG level.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e DEBUG level, \e FALSE
 *         otherwise.
 */
gboolean
log4g_logger_is_debug_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e DEBUG level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_debug(), log4g_logger_debug()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_debug(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief Check if a logger is enabled for the \e INFO level.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e INFO level, \e FALSE
 *         otherwise.
 */
gboolean
log4g_logger_is_info_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e INFO level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_info(), log4g_logger_info()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_info(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief Check if a logger is enabled for the \e WARN level.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e WARN level, \e FALSE
 *         otherwise.
 */
gboolean
log4g_logger_is_warn_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e WARN level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_warn(), log4g_logger_warn()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_warn(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief Check if a logger is enabled for the \e ERROR level.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e ERROR level, \e FALSE
 *         otherwise.
 */
gboolean
log4g_logger_is_error_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e ERROR level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_error(), log4g_logger_error()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_error(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief Check if a logger is enabled for the \e FATAL level.
 *
 * \param self [in] A logger object.
 *
 * \return \e TRUE if \e self is enabled for the \e FATAL level, \e FALSE
 *         otherwise.
 */
gboolean
log4g_logger_is_fatal_enabled(Log4gLogger *self);

/**
 * \brief Log a message with the \e FATAL level.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_fatal(), log4g_logger_fatal()
 *
 * \param self [in] A logger object.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_fatal(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(5, 6);

/**
 * \brief This is the most generic logging method.
 *
 * This function is intended for use by wrapper classes.
 *
 * \see log4g_log(), log4g_logger_log()
 *
 * \param self [in] A Log4gLogger object.
 * \param level [in] The level of the logging request.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf formatted message.
 * \param ... [in] Format parameters.
 */
void
_log4g_logger_log(Log4gLogger *self, Log4gLevel *level, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
        G_GNUC_PRINTF(6, 7);

/**
 * \brief Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise a new instance is created.
 *
 * Loggers inherit their default level from their nearest ancestor with a
 * set level.
 *
 * \param name [in] The name of the logger to retrieve.
 *
 * \return The logger named \e name.
 */
Log4gLogger *
log4g_logger_get_logger(const gchar *name);

/**
 * \brief Retrieve the root logger for the current logger repository.
 *
 * Calling log4g_logger_get_name() on the root logger always returns the
 * string "root". Calling log4g_logger_get_logger("root") however does not
 * retrieve the root logger but a logger just under root named "root".
 *
 * Calling this function is the only way to retrieve the root logger.
 *
 * \return The root logger.
 */
Log4gLogger *
log4g_logger_get_root_logger(void);

/**
 * \brief Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise \e factory is used to create a new instance.
 *
 * \param name [in] The name of the logger to retrieve.
 * \param factory [in] The logger factory to use if a logger named \e name
 *                     does not already exist.
 *
 * \return The logger named \e name.
 */
Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory);

/**
 * \brief Create and log a new event without further checks.
 *
 * \param self [in] A logger object.
 * \param level [in] The level of the log event.
 * \param function [in] The function where the event was logged.
 * \param file [in] The file where the event was logged.
 * \param line [in] The line in \e file where the event was logged.
 * \param format [in] A printf message format.
 * \param ap [in] Format parameters.
 */
void
log4g_logger_forced_log(Log4gLogger *self, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, va_list ap);

G_END_DECLS

#endif /* LOG4G_LOGGER_H */
