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
 * SECTION: logger-repository
 * @short_description: Create and retrieve loggers
 * @see_also: #Log4gLoggerClass
 *
 * A logger repository is used to create and retrieve loggers. The
 * relationship between loggers and the repository depends on the repository
 * implementation but usually loggers are arranged in a named hierarchy.
 *
 * In addition to the create functions, a logger repository can be queried
 * for existing loggers and act as a registry for events related to loggers.
 *
 * Logger repositories have the following signals:
 * <itemizedlist>
 * <listitem><para>add-appender</para></listitem>
 * <listitem><para>remove-appender</para></listitem>
 * </itemizedlist>
 *
 * The add-appender signal is invoked whenever an appender is added to a
 * logger in the repository.
 *
 * The type signature for the add-appender signal is:
 *
 * |[
 * typedef void
 * (*add_appender)(Log4gLogger *logger, Log4gAppender *appender);
 * ]|
 *
 * The remove-appender signal is invoked whenever an appender is removed from
 * a logger in the repository.
 *
 * The type signature for the remove-appender signal is:
 *
 * |[
 * typedef void
 * (*remove_appender)(Log4gLogger *logger, Log4gAppender *appender);
 * ]|
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/logger-repository.h"
#include "marshal.h"

G_DEFINE_INTERFACE(Log4gLoggerRepository, log4g_logger_repository,
	G_TYPE_INVALID)

enum Signals {
	SIGNAL_ADD_APPENDER,
	SIGNAL_REMOVE_APPENDER,
	SIGNAL_LAST
};

static guint signals[SIGNAL_LAST] = { 0 };

static void
log4g_logger_repository_default_init(Log4gLoggerRepositoryInterface *klass)
{
	/**
	 * Log4gLoggerRepository::add-appender
	 * @logger: The logger to which an appender was added.
	 * @appender: The appender that was added.
	 *
	 * The ::add-appender signal is emitted when an appender is added to
	 * a logger.
	 */
	signals[SIGNAL_ADD_APPENDER] =
		g_signal_new(Q_("add-appender"), G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_DETAILED,
			0, NULL, NULL, log4g_marshal_VOID__OBJECT_OBJECT,
			G_TYPE_NONE, 2, G_TYPE_OBJECT, G_TYPE_OBJECT);
	/**
	 * Log4gLoggerRepository::remove-appender
	 * @logger: The logger from which an appender was removed.
	 * @appender: The appender that was removed.
	 *
	 * The ::remove-appender signal is emitted when an appender is removed
	 * from a logger.
	 */
	signals[SIGNAL_REMOVE_APPENDER] =
		g_signal_new(Q_("remove-appender"), G_OBJECT_CLASS_TYPE(klass),
			G_SIGNAL_RUN_FIRST | G_SIGNAL_DETAILED,
			0, NULL, NULL, log4g_marshal_VOID__OBJECT_OBJECT,
			G_TYPE_NONE, 2, G_TYPE_OBJECT, G_TYPE_OBJECT);
}

/**
 * log4g_logger_repository_exists:
 * @self: A logger repository object.
 * @name: The name of the logger to check.
 *
 * Call the @exists function from the #Log4gLoggerRepositoryInterface of @self.
 *
 * Returns: (transfer none): The logger named @name or %NULL if it does not
 *          exist.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_repository_exists(Log4gLoggerRepository *self, const gchar *name)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->exists(self, name);
}

/**
 * log4g_logger_repository_emit_add_appender_signal:
 * @self: A logger repository object.
 * @logger: The logger from to @appender was added.
 * @appender: The appender that was added.
 *
 * Emit the the add-appender signal of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_emit_add_appender_signal(Log4gLoggerRepository *self,
		Log4gLogger *logger, Log4gAppender *appender)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	g_signal_emit(self, signals[SIGNAL_ADD_APPENDER],
			g_quark_from_string(log4g_logger_get_name(logger)),
			logger, appender);
}

/**
 * log4g_logger_repository_emit_remove_appender_signal:
 * @self: A logger repository object.
 * @logger: The logger from which @appender was removed.
 * @appender: The appender that was removed.
 *
 * Emit the remove-appender signal of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_emit_remove_appender_signal(
		Log4gLoggerRepository *self, Log4gLogger *logger,
		Log4gAppender *appender)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	g_signal_emit(self, signals[SIGNAL_REMOVE_APPENDER],
			g_quark_from_string(log4g_logger_get_name(logger)),
			logger, appender);
}

/**
 * log4g_logger_repository_get_current_loggers:
 * @self: A logger repository object.
 *
 * Call the @get_current_loggers function from the
 * #Log4gLoggerRepositoryInterface of @self.
 *
 * Returns: An array containing all loggers in the repository or %NULL
 *          if there are none.
 * Since: 0.1
 */
const GArray *
log4g_logger_repository_get_current_loggers(Log4gLoggerRepository *self)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->get_current_loggers(self);
}

/**
 * log4g_logger_repository_get_logger:
 * @self: A logger repository object.
 * @name: The name of the logger to retrieve.
 *
 * Call the @get_logger function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * Returns: (transfer none): The logger named @name.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_repository_get_logger(Log4gLoggerRepository *self,
		const gchar *name)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->get_logger(self, name);
}

/**
 * log4g_logger_repository_get_logger_factory:
 * @self: A logger repository object.
 * @name: The name of the logger to retrieve.
 * @factory: The factory to use.
 *
 * Call the @get_logger_factory function from the
 * #Log4gLoggerRepositoryInterface of @self.
 *
 * Returns: (transfer none): The logger named @name.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_repository_get_logger_factory(Log4gLoggerRepository *self,
		const gchar *name, Log4gLoggerFactory *factory)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->get_logger_factory(self, name, factory);
}

/**
 * log4g_logger_repository_get_root_logger:
 * @self: A logger repository object.
 *
 * Call the @get_root_logger function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * Returns: (transfer none): The root logger.
 * Since: 0.1
 */
Log4gLogger *
log4g_logger_repository_get_root_logger(Log4gLoggerRepository *self)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->get_root_logger(self);
}

/**
 * log4g_logger_repository_get_threshold:
 * @self: A logger repository object.
 *
 * Call the @get_threshold function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * See: #Log4gLevelClass
 *
 * Returns: (transfer none): The threshold level for @self.
 * Since: 0.1
 */
Log4gLevel *
log4g_logger_repository_get_threshold(Log4gLoggerRepository *self)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), NULL);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->get_threshold(self);
}

/**
 * log4g_logger_repository_is_disabled:
 * @self: A logger repository object.
 * @level: The integer representation of a log level.
 *
 * Call the @is_disabled function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * See: #Log4gLevelClass
 *
 * Returns: %TRUE if @self is disabled for @level, %FALSE otherwise.
 * Since: 0.1
 */
gboolean
log4g_logger_repository_is_disabled(Log4gLoggerRepository *self, gint level)
{
	g_return_val_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self), FALSE);
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	return interface->is_disabled(self, level);
}

/**
 * log4g_logger_repository_reset_configuration:
 * @self: A logger repository object.
 *
 * Call the @reset_configuration function from the
 * #Log4gLoggerRepositoryInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_reset_configuration(Log4gLoggerRepository *self)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	interface->reset_configuration(self);
}

/**
 * log4g_logger_repository_set_threshold:
 * @self: A logger repository object.
 * @level: The new threshold for @self.
 *
 * Call the @set_threshold function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * See: #Log4gLevelClass
 *
 * Since: 0.1
 */
void
log4g_logger_repository_set_threshold(Log4gLoggerRepository *self,
		Log4gLevel *level)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	interface->set_threshold(self, level);
}

/**
 * log4g_logger_repository_set_threshold_string:
 * @self: A logger repository object.
 * @string: The name of the new threshold to set.
 *
 * Call the @set_threshold_string function from the
 * #Log4gLoggerRepositoryInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_set_threshold_string(Log4gLoggerRepository *self,
		const gchar *string)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	interface->set_threshold_string(self, string);
}

/**
 * log4g_logger_repository_shutdown:
 * @self: A logger repository object.
 *
 * Call the @shutdown function from the #Log4gLoggerRepositoryInterface
 * of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_shutdown(Log4gLoggerRepository *self)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	interface->shutdown(self);
}

/**
 * log4g_logger_repository_emit_no_appender_warning:
 * @self: A logger repository object.
 * @logger: The logger that produced the warning.
 *
 * Call the @emit_no_appender_warning function from the
 * #Log4gLoggerRepositoryInterface of @self.
 *
 * Since: 0.1
 */
void
log4g_logger_repository_emit_no_appender_warning(Log4gLoggerRepository *self,
		Log4gLogger *logger)
{
	g_return_if_fail(LOG4G_IS_LOGGER_REPOSITORY(self));
	Log4gLoggerRepositoryInterface *interface =
		LOG4G_LOGGER_REPOSITORY_GET_INTERFACE(self);
	interface->emit_no_appender_warning(self, logger);
}
