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
 * SECTION: logger
 * @short_description: the central class in the Log4g package
 *
 * Most logging operations (except configuration) are performed through this
 * class.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/appender-attachable-impl.h"
#include "log4g/interface/logger-repository.h"
#include "log4g/log-manager.h"
#include "log4g/logger.h"
#include "log4g/ndc.h"

static void
appender_attachable_init(Log4gAppenderAttachableInterface *interface,
        gpointer data)
{
    interface->add_appender = (gconstpointer)log4g_logger_add_appender;
    interface->get_all_appenders =
        (gconstpointer)log4g_logger_get_all_appenders;
    interface->get_appender = (gconstpointer)log4g_logger_get_appender;
    interface->is_attached = (gconstpointer)log4g_logger_is_attached;
    interface->remove_all_appenders =
        (gconstpointer)log4g_logger_remove_all_appenders;
    interface->remove_appender = (gconstpointer)log4g_logger_remove_appender;
    interface->remove_appender_name =
        (gconstpointer)log4g_logger_remove_appender_name;
}

G_DEFINE_TYPE_WITH_CODE(Log4gLogger, log4g_logger, G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER_ATTACHABLE,
                appender_attachable_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LOGGER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean additive; /**< Indicates if children inherit appenders */
    Log4gLevel *level; /**< The assigned level of this logger */
    gchar *name; /**< The name of this logger */
    Log4gLogger *parent; /**< The parent of the logger */
    Log4gLoggerRepository *repository; /**< Owner of this logger */
    Log4gAppenderAttachable *aai; /**< Appenders attached to this logger */
    GMutex *lock; /**< Synchronizes access to \e aai */
};

static void
log4g_logger_init(Log4gLogger *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->additive = TRUE;
    priv->level = NULL;
    priv->name = NULL;
    priv->parent = NULL;
    priv->repository = NULL;
    priv->aai = NULL;
    if (g_thread_supported()) {
        priv->lock = g_mutex_new();
    } else {
        priv->lock = NULL;
    }
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->level) {
        g_object_unref(priv->level);
        priv->level = NULL;
    }
    if (priv->parent) {
        g_object_unref(priv->parent);
        priv->parent = NULL;
    }
    if (priv->repository) {
        priv->repository = NULL;
    }
    if (priv->aai) {
        g_object_unref(priv->aai);
        priv->aai = NULL;
    }
    G_OBJECT_CLASS(log4g_logger_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->name);
    priv->name = NULL;
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    G_OBJECT_CLASS(log4g_logger_parent_class)->finalize(base);
}

static Log4gLevel *
get_effective_level(Log4gLogger *self)
{
    Log4gLogger *logger = self;
    while (logger) {
        struct Log4gPrivate *priv = GET_PRIVATE(logger);
        if (priv->level) {
            return priv->level;
        }
        logger = priv->parent;
    }
    return NULL;
}

static void
set_level(Log4gLogger *self, Log4gLevel *level)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->level) {
        g_object_unref(priv->level);
    }
    g_object_ref(level);
    priv->level = level;
}

static void
log4g_logger_class_init(Log4gLoggerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLoggerClass */
    klass->get_effective_level = get_effective_level;
    klass->set_level = set_level;
}

/**
 * log4g_logger_new:
 * @name: The fully qualified name for the new logger.
 *
 * Create a new logger object and set its name.
 *
 * This fuction is intended for internal use. You should not create Loggers
 * directly. See log4g_logger_get_logger()
 *
 * Returns: A new #Log4gLogger object.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_new(const gchar *name)
{
    g_return_val_if_fail(name, NULL);
    Log4gLogger *self = g_object_new(LOG4G_TYPE_LOGGER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_logger_set_name(self, name);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->name) {
        g_object_unref(self);
        return NULL;
    }
    return self;
}

/**
 * log4g_logger_get_name:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the fully-qualified name of a logger.
 *
 * Returns: The name of @self.
 * Since: 0.1
 */
const gchar *
log4g_logger_get_name(Log4gLogger *self)
{
    return GET_PRIVATE(self)->name;
}

/**
 * log4g_logger_set_name:
 * @self: A #Log4gLogger object.
 * @name: The new name of @self.
 *
 * Set the name of a logger.
 *
 * This method should only be called by sub-classes. Calling this method on
 * a logger in the logger repository will have disastrous effects on the
 * logger hierarchy.
 *
 * Since: 0.1
 */
void
log4g_logger_set_name(Log4gLogger *self, const gchar *name)
{
    g_return_if_fail(name);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_free(priv->name);
    priv->name = g_strdup(name);
}

/**
 * log4g_logger_get_parent:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the parent of this logger.
 *
 * <note><para>
 * The parent of a logger may change during its lifetime. The root logger
 * will return %NULL.
 * </para></note>
 *
 * Returns: The parent of @self.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self)
{
    return GET_PRIVATE(self)->parent;
}

/**
 * log4g_logger_set_parent:
 * @self: A #Log4gLogger object.
 * @parent: The new parent of @self.
 *
 * Set the parent of a logger.
 *
 * Since: 0.1
 */
void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent)
{
    g_object_ref(parent);
    GET_PRIVATE(self)->parent = parent;
}

/**
 * log4g_logger_get_level:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the level threshold of a logger.
 *
 * Returns: The log level threshold of @self.
 * Since: 0.1
 */
Log4gLevel *
log4g_logger_get_level(Log4gLogger *self)
{
    return GET_PRIVATE(self)->level;
}

/**
 * log4g_logger_set_level:
 * @self: A #Log4gLogger object.
 * @level: The new log level threshold for @self.
 *
 * Calls the @set_level function from the #Log4gLoggerClass of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level)
{
    g_return_if_fail(LOG4G_IS_LOGGER(self));
    LOG4G_LOGGER_GET_CLASS(self)->set_level(self, level);
}

/**
 * log4g_logger_get_additivity:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the additivity flag for a logger. See log4g_logger_set_additivity()
 *
 * Returns: %TRUE if @self is additive, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_get_additivity(Log4gLogger *self)
{
    return GET_PRIVATE(self)->additive;
}

/**
 * log4g_logger_set_additivity:
 * @self: A #Log4gLogger object.
 * @additive: The new additivity flag for @self.
 *
 * Set the additivity flag for a logger.
 *
 * Logger additivity determines if a logger inherits the appenders of its
 * ancestors. If this flag is set to %TRUE (the default value is %TRUE)
 * then events logged to this logger will be propagated to the appenders of
 * its ancestors. If this flags is set to %FALSE then the appenders of this
 * logger will not be inherited.
 *
 * Since: 0.1
 */
void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive)
{
    GET_PRIVATE(self)->additive = additive;
}

/**
 * log4g_logger_get_logger_repository:
 * @self: A #Log4gLogger object.
 *
 * Retrieve the repository where a logger is attached. See
 * #Log4gLoggerRepositoryClass.
 *
 * Returns: The logger repository @self is attached to.
 * Since: 0.1
 */
gpointer
log4g_logger_get_logger_repository(Log4gLogger *self)
{
    return GET_PRIVATE(self)->repository;
}

/**
 * log4g_logger_set_logger_repository:
 * @self: A #Log4gLogger object.
 * @repository: The new repository to attach to @self.
 *
 * Set the repository a logger is attached to.
 *
 * You probably do not want to call this function.
 *
 * See #Log4gLoggerRepositoryClass
 *
 * Since: 0.1
 */
void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(repository));
    GET_PRIVATE(self)->repository = repository;
}

/**
 * log4g_logger_get_effective_level:
 * @self: A #Log4gLogger object.
 *
 * Calls the @get_effective_level function from the #Log4gLoggerClass of @self.
 *
 * Returns: The effective level threshold of @self.
 * Since: 0.1
 */
Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER(self), NULL);
    return LOG4G_LOGGER_GET_CLASS(self)->get_effective_level(self);
}

/**
 * log4g_logger_add_appender:
 * @self: A #Log4gLogger object.
 * @appender: The appender to add to @self.
 *
 * Add an appender to the list of appenders for @self.
 *
 * If @appender is already in the list of appenders for @self then
 * it will not be added again.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_logger_add_appender(Log4gLogger *self, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_mutex_lock(priv->lock);
    if (!priv->aai) {
        priv->aai = log4g_appender_attachable_impl_new();
        if (!priv->aai) {
            goto exit;
        }
    }
    log4g_appender_attachable_add_appender(priv->aai, appender);
    log4g_logger_repository_emit_add_appender_signal(priv->repository, self,
            appender);
exit:
    g_mutex_unlock(priv->lock);
}

/**
 * log4g_logger_get_all_appenders:
 * @self: A #Log4gLogger object.
 *
 * Retrieve all appenders attached to a @self.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: A #GArray of appenders attached to @self or %NULL if there are
 *          none.
 * Since: 0.1
 */
const GArray *
log4g_logger_get_all_appenders(Log4gLogger *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!g_atomic_pointer_get(&priv->aai)) {
        return NULL;
    }
    g_mutex_lock(priv->lock);
    const GArray *appenders =
        log4g_appender_attachable_get_all_appenders(priv->aai);
    g_mutex_unlock(priv->lock);
    return appenders;
}

/**
 * log4g_logger_get_appender:
 * @self: A #Log4gLogger object.
 * @name: The name of the appender to retrieve.
 *
 * Retrieve an appender attached to a logger by name.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: The appender named @name or %NULL if no such appender is attached.
 * Since: 0.1
 */
Log4gAppender *
log4g_logger_get_appender(Log4gLogger *self, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->aai) {
        return NULL;
    }
    g_mutex_lock(priv->lock);
    Log4gAppender *appender =
        log4g_appender_attachable_get_appender(priv->aai, name);
    g_mutex_unlock(priv->lock);
    return appender;
}

/**
 * log4g_logger_is_attached:
 * @self: A #Log4gLogger object.
 * @appender: An appender object.
 *
 * Determine if an appender is attached to a logger.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Returns: %TRUE if @appender is attached to @self, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_attached(Log4gLogger *self, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->aai) {
        return FALSE;
    }
    return log4g_appender_attachable_is_attached(priv->aai, appender);
}

/**
 * log4g_logger_remove_all_appenders:
 * @self: A #Log4gLogger object.
 *
 * Remove all appenders from a logger.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_logger_remove_all_appenders(Log4gLogger *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!g_atomic_pointer_get(&priv->aai)) {
        return;
    }
    g_mutex_lock(priv->lock);
    const GArray *appenders =
        log4g_appender_attachable_get_all_appenders(priv->aai);
    if (!appenders) {
        goto exit;
    }
    GArray *mine = g_array_sized_new(FALSE, TRUE, sizeof(Log4gAppender *),
                appenders->len);
    if (!mine) {
        goto exit;
    }
    for (guint i = 0; i < appenders->len; ++i) {
        Log4gAppender *appender = g_array_index(appenders, Log4gAppender *, i);
        g_object_ref(appender);
        g_array_append_val(mine, appender);
    }
    log4g_appender_attachable_remove_all_appenders(priv->aai);
    for (guint i = 0; i < mine->len; ++i) {
        Log4gAppender *appender = g_array_index(mine, Log4gAppender *, i);
        log4g_logger_repository_emit_remove_appender_signal(priv->repository,
                self, appender);
        g_object_unref(appender);
    }
    g_array_free(mine, TRUE);
    g_object_unref(priv->aai);
    priv->aai = NULL;
exit:
    g_mutex_unlock(priv->lock);
}

/**
 * log4g_logger_remove_appender:
 * @self: A #Log4gLogger object.
 * @appender: An appender to remove.
 *
 * Remove an appender from a logger.
 *
 * If @appender is not attached to @self then this function does nothing.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_logger_remove_appender(Log4gLogger *self, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!g_atomic_pointer_get(&priv->aai)) {
        return;
    }
    g_mutex_lock(priv->lock);
    gboolean attached =
        log4g_appender_attachable_is_attached(priv->aai, appender);
    if (attached) {
        log4g_appender_attachable_remove_appender(priv->aai, appender);
        log4g_logger_repository_emit_remove_appender_signal(priv->repository,
                self, appender);
    }
    g_mutex_unlock(priv->lock);
}

/**
 * log4g_logger_remove_appender_name:
 * @self: A #Log4gLogger object.
 * @name: The name of the appender to remove.
 *
 * Remove an appender from a logger by name.
 *
 * If @name is not found then this function does nothing.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_logger_remove_appender_name(Log4gLogger *self, const gchar *name)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!g_atomic_pointer_get(&priv->aai)) {
        return;
    }
    g_mutex_lock(priv->lock);
    Log4gAppender *appender =
        log4g_appender_attachable_get_appender(priv->aai, name);
    if (appender) {
        log4g_appender_attachable_remove_appender_name(priv->aai, name);
        log4g_logger_repository_emit_remove_appender_signal(priv->repository,
                self, appender);
        g_object_unref(appender);
    }
    g_mutex_unlock(priv->lock);
}

/**
 * log4g_logger_close_nested_appenders:
 * @self: A #Log4gLogger object.
 *
 * Close all attached appenders implementing the #Log4gAppenderAttachable
 * interface.
 *
 * See #Log4gAppenderAttachableInterface
 *
 * Since: 0.1
 */
void
log4g_logger_close_nested_appenders(Log4gLogger *self)
{
    const GArray *appenders = log4g_logger_get_all_appenders(self);
    if (!appenders) {
        return;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    g_mutex_lock(priv->lock);
    for (guint i = 0; i < appenders->len; ++i) {
        Log4gAppender *appender = g_array_index(appenders, Log4gAppender *, i);
        if (LOG4G_IS_APPENDER_ATTACHABLE(appender)) {
            log4g_appender_close(appender);
        }
    }
    g_mutex_unlock(priv->lock);
}

/**
 * log4g_logger_call_appenders:
 * @self: A #Log4gLogger object.
 * @event: An event to log.
 *
 * Append a logging event to all appenders attached to this logger.
 *
 * See #Log4gLoggingEvent
 *
 * Since: 0.1
 */
void
log4g_logger_call_appenders(Log4gLogger *self, Log4gLoggingEvent *event)
{
    guint writes = 0;
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    Log4gLogger *logger = self;
    while (logger) {
        priv = GET_PRIVATE(logger);
        g_mutex_lock(priv->lock);
        if (priv->aai) {
            writes += log4g_appender_attachable_impl_append_loop_on_appenders(
                            priv->aai, event);
        }
        if (!priv->additive) {
            g_mutex_unlock(priv->lock);
            break;
        }
        g_mutex_unlock(priv->lock);
        logger = priv->parent;
    }
    if (!writes) {
        log4g_logger_repository_emit_no_appender_warning(priv->repository,
                self);
    }
}

/**
 * _log4g_logger_assert:
 * @self: A #Log4gLogger object.
 * @assertion: The assertion to evaluate.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Logs an error if the @assertion parameter is %FALSE.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_assert(), log4g_logger_assert()
 *
 * Since: 0.1
 */
void
_log4g_logger_assert(Log4gLogger *self, gboolean assertion,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, ...)
{
    if (assertion) {
        return;
    }
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_ERROR_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->ERROR, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->ERROR, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_trace_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_TRACE level.
 *
 * This function is useful if you have a to perform a costly operation to
 * construct a log message.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_TRACE level, %FALSE
 *         otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_trace_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_TRACE_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->TRACE, effective);
}

/**
 * _log4g_logger_trace:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_TRACE level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_trace(), log4g_logger_trace()
 *
 * Since: 0.1
 */
void
_log4g_logger_trace(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_TRACE_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->TRACE, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->TRACE, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_debug_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_DEBUG level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_DEBUG level, %FALSE
 *          otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_debug_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_DEBUG_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->DEBUG, effective);
}

/**
 * _log4g_logger_debug:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_DEBUG level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_debug(), log4g_logger_debug()
 *
 * Since: 0.1
 */
void
_log4g_logger_debug(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_DEBUG_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->DEBUG, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->DEBUG, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_info_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_INFO level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_INFO level, %FALSE
 *          otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_info_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_INFO_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->INFO, effective);
}

/**
 * _log4g_logger_info:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_INFO level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_info(), log4g_logger_info()
 *
 * Since: 0.1
 */
void
_log4g_logger_info(Log4gLogger *self, const gchar *function, const gchar *file,
        const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_INFO_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->INFO, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->INFO, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_warn_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_WARN level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_WARN level, %FALSE
 *          otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_warn_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_WARN_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->WARN, effective);
}

/**
 * _log4g_logger_warn:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_WARN level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_warn(), log4g_logger_warn()
 *
 * Since: 0.1
 */
void
_log4g_logger_warn(Log4gLogger *self, const gchar *function, const gchar *file,
        const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_WARN_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->WARN, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->WARN, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_error_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_ERROR level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_ERROR level, %FALSE
 *          otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_error_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_ERROR_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->ERROR, effective);
}

/**
 * _log4g_logger_error:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_ERROR level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_error(), log4g_logger_error()
 *
 * Since: 0.1
 */
void
_log4g_logger_error(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_ERROR_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->ERROR, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->ERROR, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_is_fatal_enabled:
 * @self: A #Log4gLogger object.
 *
 * Check if a logger is enabled for the %LOG4G_LEVEL_FATAL level.
 *
 * Returns: %TRUE if @self is enabled for the %LOG4G_LEVEL_FATAL level, %FALSE
 *          otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_is_fatal_enabled(Log4gLogger *self)
{
    if (G_UNLIKELY(!self)) {
        return FALSE;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_FATAL_INT)) {
        return FALSE;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    return log4g_level_is_greater_or_equal(level->FATAL, effective);
}

/**
 * _log4g_logger_fatal:
 * @self: A #Log4gLogger object.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * Log a message with the %LOG4G_LEVEL_FATAL level.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_fatal(), log4g_logger_fatal()
 *
 * Since: 0.1
 */
void
_log4g_logger_fatal(Log4gLogger *self, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
            GET_PRIVATE(self)->repository, LOG4G_LEVEL_FATAL_INT)) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    Log4gLevelClass *level = g_type_class_peek(LOG4G_TYPE_LEVEL);
    if (log4g_level_is_greater_or_equal(level->FATAL, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level->FATAL, function, file, line,
                format, ap);
        va_end(ap);
    }
}

/**
 * _log4g_logger_log:
 * @self: A Log4gLogger object.
 * @level: The level of the logging request.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf formatted message.
 * @...: Format parameters.
 *
 * This is the most generic logging method.
 *
 * This function is intended for use by wrapper classes.
 *
 * See log4g_log(), log4g_logger_log()
 *
 * Since: 0.1
 */
void
_log4g_logger_log(Log4gLogger *self, Log4gLevel *level, const gchar *function,
        const gchar *file, const gchar *line, const gchar *format, ...)
{
    if (G_UNLIKELY(!self)) {
        return;
    }
    if (log4g_logger_repository_is_disabled(
                GET_PRIVATE(self)->repository, log4g_level_to_int(level))) {
        return;
    }
    Log4gLevel *effective = log4g_logger_get_effective_level(self);
    if (log4g_level_is_greater_or_equal(level, effective)) {
        va_list ap;
        va_start(ap, format);
        log4g_logger_forced_log(self, level, function, file, line, format, ap);
        va_end(ap);
    }
}

/**
 * log4g_logger_get_logger:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise a new instance is created.
 *
 * Loggers inherit their default level from their nearest ancestor with a
 * set level.
 *
 * Returns: The logger named @name.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_get_logger(const gchar *name)
{
    return log4g_log_manager_get_logger(name);
}

/**
 * log4g_logger_get_root_logger:
 *
 * Retrieve the root logger for the current logger repository.
 *
 * Calling log4g_logger_get_name() on the root logger always returns the
 * string "root". Calling log4g_logger_get_logger("root") however does not
 * retrieve the root logger but a logger just under root named "root".
 *
 * Calling this function is the only way to retrieve the root logger.
 *
 * Returns: The root logger.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_get_root_logger(void)
{
    return log4g_log_manager_get_root_logger();
}

/**
 * log4g_logger_get_logger_factory:
 * @name: The name of the logger to retrieve.
 * @factory: The logger factory to use if a logger named @name does not already
 *           exist.
 *
 * Retrieve a named logger.
 *
 * If the named logger already exists, then the existing instance will be
 * returned. Otherwise @factory is used to create a new instance.
 *
 * Returns: The logger named @name.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_FACTORY(factory), NULL);
    return log4g_log_manager_get_logger_factory(name, factory);
}

/**
 * log4g_logger_forced_log:
 * @self: A #Log4gLogger object.
 * @level: The level of the log event.
 * @function: The function where the event was logged.
 * @file: The file where the event was logged.
 * @line: The line in @file where the event was logged.
 * @format: A printf message format.
 * @ap: Format parameters.
 *
 * Create and log a new event without further checks.
 *
 * Since: 0.1
 */
void
log4g_logger_forced_log(Log4gLogger *self, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *format, va_list ap)
{
    Log4gLoggingEvent *event =
        log4g_logging_event_new(GET_PRIVATE(self)->name, level, function,
                file, line, format, ap);
    if (!event) {
        return;
    }
    log4g_logger_call_appenders(self, event);
    g_object_unref(event);
}
