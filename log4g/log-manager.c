/* Copyright 2010, 2011 Michael Steinert
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
 * SECTION: log-manager
 * @short_description: Operate on the current logger repository
 * @see_also: #Log4gLogger, #log4gLoggerRepository
 *
 * Use the log manager class to retrieve logger instances or operate on the
 * current logger repository.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gmodule.h>
#include "log4g/helpers/default-module-loader.h"
#include "log4g/helpers/default-repository-selector.h"
#include "log4g/hierarchy.h"
#include "log4g/log-manager.h"
#include "log4g/root-logger.h"

G_DEFINE_TYPE(Log4gLogManager, log4g_log_manager, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LOG_MANAGER, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gLogManager *)instance)->priv)

struct Private {
	Log4gLoggerRepository *repository;
	Log4gRepositorySelector *selector;
	Log4gModuleLoader *modules;
	GObject *guard;
};

/** \brief The single instance of this class */
static GObject *singleton = NULL;

static void
log4g_log_manager_init(Log4gLogManager *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->modules = log4g_default_module_loader_new();
	if (priv->modules) {
		log4g_module_loader_load_modules(priv->modules);
	}
	/* set defaults */
	Log4gLogger *root = log4g_root_logger_new(log4g_level_DEBUG());
	if (root) {
		priv->repository = log4g_hierarchy_new(root);
		g_object_unref(root);
	}
	priv->selector =
		log4g_default_repository_selector_new(priv->repository);
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
{
	GObject *self = g_atomic_pointer_get(&singleton);
	if (!self) {
		self = G_OBJECT_CLASS(log4g_log_manager_parent_class)->
			constructor(type, n, params);
		g_atomic_pointer_set(&singleton, self);
	} else {
		g_object_ref(self);
	}
	return self;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->repository) {
		g_object_unref(priv->repository);
		priv->repository = NULL;
	}
	if (priv->selector) {
		g_object_unref(priv->selector);
		priv->selector = NULL;
	}
	if (priv->modules) {
		g_object_unref(priv->modules);
		priv->modules = NULL;
	}
	if (priv->guard) {
		g_object_unref(priv->guard);
		priv->guard = NULL;
	}
	G_OBJECT_CLASS(log4g_log_manager_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	g_atomic_pointer_set(&singleton, NULL);
	G_OBJECT_CLASS(log4g_log_manager_parent_class)->finalize(base);
}

static void
log4g_log_manager_class_init(Log4gLogManagerClass *klass)
{
	/* initialize GObjectClass */
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructor = constructor;
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_log_manager_get_instance:
 *
 * Retrieve the current log manager instance.
 *
 * If one does not exist it will be created.
 *
 * Returns: The current log manager instance.
 */
static Log4gLogManager *
log4g_log_manager_get_instance(void)
{
	Log4gLogManager *self = g_atomic_pointer_get(&singleton);
	if (!self) {
		return g_object_new(LOG4G_TYPE_LOG_MANAGER, NULL);
	}
	return self;
}

/**
 * log4g_log_manager_remove_instance:
 *
 * Remove the current log manager instance.
 *
 * <note><para>
 * Do not call this function unless you really know what you are doing.
 * </para></note>
 *
 * Since: 0.1
 */
void
log4g_log_manager_remove_instance(void)
{
	Log4gLogManager *self = g_atomic_pointer_get(&singleton);
	if (self) {
		g_object_unref(self);
	}
}

/**
 * log4g_log_manager_set_repository_selector:
 * @selector: The new repository selector.
 * @guard: The previous guard object (if one was set).
 *
 * Sets a new repository selector. If a guard object has been set then it
 * must be supplied.
 *
 * Since: 0.1
 */
void
log4g_log_manager_set_repository_selector(Log4gRepositorySelector *selector,
		GObject *guard)
{
	g_return_if_fail(selector);
	Log4gLogManager *self = log4g_log_manager_get_instance();
	if (!self) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (priv->guard && (guard != priv->guard)) {
		log4g_log_error(Q_("attempted to reset the "
					"Log4gRepositorySelector without "
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

/**
 * log4g_log_manager_get_logger_repository:
 *
 * Retrieve the current logger repository.
 *
 * @See: log4g/interface/logger-repository.h
 *
 * Returns: The current logger repository.
 * Since: 0.1
 */
Log4gLoggerRepository *
log4g_log_manager_get_logger_repository(void)
{
	Log4gLogManager *self = log4g_log_manager_get_instance();
	if (!self) {
		return NULL;
	}
	return log4g_repository_selector_get_logger_repository(
			GET_PRIVATE(self)->selector);
}

/**
 * log4g_log_manager_get_root_logger:
 *
 * Retrieve the root logger.
 *
 * Returns: The root logger.
 * Since: 0.1
 */
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

/**
 * log4g_log_manager_get_logger:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger.
 *
 * If the named logger does not exist it will be created.
 *
 * Returns: A logger instance.
 * Since: 0.1
 */
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

/**
 * log4g_log_manager_get_logger_factory:
 * @name: The name of the logger to retrieve.
 * @factory: The factory to use if @name does not already exist.
 *
 * Retrieve a named logger.
 *
 * If the named logger does not exist it will be created using the provided
 * logger factory.
 *
 * @See: log4g/interface/logger-factory.h
 *
 * Since: 0.1
 */
Log4gLogger *
log4g_log_manager_get_logger_factory(const gchar *name,
		Log4gLoggerFactory *factory)
{
	Log4gLoggerRepository *repository =
		log4g_log_manager_get_logger_repository();
	if (!repository) {
		return NULL;
	}
	return log4g_logger_repository_get_logger_factory(repository,
			name, factory);
}

/**
 * log4g_log_manager_exists:
 * @name: The name of the logger to retrieve.
 *
 * Retrieve a named logger if it already exists.
 *
 * If @name does not exist it will not be created.
 *
 * Returns: A logger named @name or %NULL if @name does not exist.
 * Since: 0.1
 */
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

/**
 * log4g_log_manager_get_current_loggers:
 *
 * Retrieve an array containing all current existing loggers.
 *
 * <note><para>
 * It is the callers responsibility to call g_array_free() for the returned
 * value.
 * </para></note>
 *
 * Returns: An array containing all current loggers.
 * Since: 0.1
 */
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

/**
 * log4g_log_manager_shutdown:
 *
 * Shut down the current logger repository.
 *
 * Since: 0.1
 */
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

/**
 * log4g_log_manager_reset_configuration:
 *
 * Reset the configuration for the current logger repository.
 *
 * Since: 0.1
 */
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
