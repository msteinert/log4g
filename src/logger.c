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
 * \brief Implements the API in log4g/logger.h
 * \author Mike Steinert
 * \date 1-29-2010
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
    if (priv->name) {
        g_free(priv->name);
        priv->name = NULL;
    }
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

const gchar *
log4g_logger_get_name(Log4gLogger *self)
{
    return GET_PRIVATE(self)->name;
}

void
log4g_logger_set_name(Log4gLogger *self, const gchar *name)
{
    g_return_if_fail(name);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->name) {
        g_free(priv->name);
    }
    priv->name = g_strdup(name);
}

Log4gLogger *
log4g_logger_get_parent(Log4gLogger *self)
{
    return GET_PRIVATE(self)->parent;
}

void
log4g_logger_set_parent(Log4gLogger *self, Log4gLogger *parent)
{
    g_object_ref(parent);
    GET_PRIVATE(self)->parent = parent;
}

Log4gLevel *
log4g_logger_get_level(Log4gLogger *self)
{
    return GET_PRIVATE(self)->level;
}

void
log4g_logger_set_level(Log4gLogger *self, Log4gLevel *level)
{
    g_return_if_fail(LOG4G_IS_LOGGER(self));
    LOG4G_LOGGER_GET_CLASS(self)->set_level(self, level);
}

gboolean
log4g_logger_get_additivity(Log4gLogger *self)
{
    return GET_PRIVATE(self)->additive;
}

void
log4g_logger_set_additivity(Log4gLogger *self, gboolean additive)
{
    GET_PRIVATE(self)->additive = additive;
}

gpointer
log4g_logger_get_logger_repository(Log4gLogger *self)
{
    return GET_PRIVATE(self)->repository;
}

void
log4g_logger_set_logger_repository(Log4gLogger *self, gpointer repository)
{
    g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(repository));
    GET_PRIVATE(self)->repository = repository;
}

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

gboolean
log4g_logger_is_attached(Log4gLogger *self, Log4gAppender *appender)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->aai) {
        return FALSE;
    }
    return log4g_appender_attachable_is_attached(priv->aai, appender);
}

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

Log4gLevel *
log4g_logger_get_effective_level(Log4gLogger *self)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER(self), NULL);
    return LOG4G_LOGGER_GET_CLASS(self)->get_effective_level(self);
}

Log4gLogger *
log4g_logger_get_logger(const gchar *name)
{
    return log4g_log_manager_get_logger(name);
}

Log4gLogger *
log4g_logger_get_root_logger(void)
{
    return log4g_log_manager_get_root_logger();
}

Log4gLogger *
log4g_logger_get_logger_factory(const gchar *name, gpointer factory)
{
    g_return_val_if_fail(LOG4G_IS_LOGGER_FACTORY(factory), NULL);
    return log4g_log_manager_get_logger_factory(name, factory);
}
