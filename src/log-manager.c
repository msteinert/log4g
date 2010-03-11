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
 * \brief Implements the API in log4g/log-manager.h
 * \author Mike Steinert
 * \date 2-10-2010
 */

#include "config.h"
#include "log4g/helpers/default-repository-selector.h"
#include "log4g/hierarchy.h"
#include "log4g/log-manager.h"
#include "log4g/root-logger.h"

G_DEFINE_TYPE(Log4gLogManager, log4g_log_manager, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LOG_MANAGER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    Log4gLoggerRepository *repository;
    Log4gRepositorySelector *selector;
    GObject *guard;
};

/** \brief The single instance of this class */
static Log4gLogManager *instance = NULL;

static void
log4g_log_manager_init(Log4gLogManager *self)
{
    Log4gLogger *root;
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->guard = NULL;
    priv->repository = NULL;
    priv->selector = NULL;
    /* set defaults */
    root = log4g_root_logger_new(log4g_level_DEBUG());
    if (root) {
        priv->repository = log4g_hierarchy_new(root);
        g_object_unref(root);
    }
    priv->selector = log4g_default_repository_selector_new(priv->repository);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    instance = NULL;
    if (priv->repository) {
        g_object_unref(priv->repository);
        priv->repository = NULL;
    }
    if (priv->selector) {
        g_object_unref(priv->selector);
        priv->selector = NULL;
    }
    if (priv->guard) {
        g_object_unref(priv->guard);
        priv->guard = NULL;
    }
    G_OBJECT_CLASS(log4g_log_manager_parent_class)->finalize(base);
}

static void
log4g_log_manager_class_init(Log4gLogManagerClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gLogManager *
log4g_log_manager_get_instance(void)
{
    Log4gLogManager *self = g_atomic_pointer_get(&instance);
    if (!self) {
        self = g_object_new(LOG4G_TYPE_LOG_MANAGER, NULL);
        if (self) {
            g_atomic_pointer_set(&instance, self);
        }
    }
    return self;
}

void
log4g_log_manager_remove_instance(void)
{
    Log4gLogManager *self = g_atomic_pointer_get(&instance);
    if (self) {
        g_atomic_pointer_set(&instance, NULL);
        g_object_unref(self);
    }
}

void
log4g_log_manager_set_repository_selector(Log4gRepositorySelector *selector,
        GObject *guard)
{
    Log4gLogManager *self;
    struct Log4gPrivate *priv;
    g_return_if_fail(selector);
    self = log4g_log_manager_get_instance();
    if (!self) {
        return;
    }
    priv = GET_PRIVATE(self);
    if (priv->guard && (guard != priv->guard)) {
        g_debug(Q_("attempted to reset the Log4gRepositorySelector without "
                   "possessing the guard"));
        return;
    }
    if (priv->guard) {
        g_object_unref(priv->guard);
    }
    if (guard) {
        g_object_ref(guard);
    }
    priv->guard = guard;
    if (priv->selector) {
        g_object_unref(priv->selector);
    }
    g_object_ref(selector);
    priv->selector = selector;
}

Log4gLoggerRepository *
log4g_log_manager_get_logger_repository(void)
{
    struct Log4gPrivate *priv;
    Log4gLogManager *self = log4g_log_manager_get_instance();
    if (!self) {
        return NULL;
    }
    priv = GET_PRIVATE(self);
#if 0
    if (!instance->selector) {
        Log4gLoggerRepository = log4g_nop_logger_repository_new();
        instance->selector = log4g_default_repository_selector_new(repository);
        if (instance->guard) {
            g_object_unref(instance->guard);
            instance->guard = NULL;
        }
        g_debug(Q_("RepositorySelector was NULL likely due to error in class "
                   "reloading, using Log4gNOPLoggerRepository"));
    }
#endif
    return log4g_repository_selector_get_logger_repository(priv->selector);
}

Log4gLogger *
log4g_log_manager_get_root_logger(void)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return NULL;
    }
    return log4g_logger_repository_get_root_logger(repository);
}

Log4gLogger *
log4g_log_manager_get_logger(const gchar *name)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return NULL;
    }
    return log4g_logger_repository_get_logger(repository, name);
}

Log4gLogger *
log4g_log_manager_get_logger_factory(const gchar *name,
                                     Log4gLoggerFactory *factory)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return NULL;
    }
    return
        log4g_logger_repository_get_logger_factory(repository, name, factory);
}

Log4gLogger *
log4g_log_manager_exists(const gchar *name)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return NULL;
    }
    return log4g_logger_repository_exists(repository, name);
}

const GArray *
log4g_log_manager_get_current_loggers(void)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return NULL;
    }
    return log4g_logger_repository_get_current_loggers(repository);
}

void
log4g_log_manager_shutdown(void)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return;
    }
    log4g_logger_repository_shutdown(repository);
}

void
log4g_log_manager_reset_configuration(void)
{
    Log4gLoggerRepository *repository =
        log4g_log_manager_get_logger_repository();
    if (!repository) {
        return;
    }
    log4g_logger_repository_reset_configuration(repository);
}
