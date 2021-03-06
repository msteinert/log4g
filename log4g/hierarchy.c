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
 * SECTION: hierarchy
 * @short_description: Maintain the logger hierarchy
 * @see_also: #Log4gLoggerRepository
 *
 * This class specializes in retrieving loggers by name and maintaining the
 * logger hierarchy.
 *
 * <note><para>
 * The casual user has no need to deal with this class directly.
 * </para></note>
 *
 * The structure of the logger hierarchy is maintained by the function
 * log4g_logger_repository_get_logger() function. Children in the hierarchy
 * link to their parent but parents do not have a pointer to their children.
 * Descendant logger may be instantiated before their ancestors.
 *
 * In the case where a descendant is created before a particular ancestor
 * a provision node (#Log4gProvisionNode) is created for the ancestor and
 * the descendant is added to the provision node. Other descendants of the
 * same ancestor are added to the previously created provision node.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/default-logger-factory.h"
#include "log4g/hierarchy.h"
#include "log4g/provision-node.h"

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_HIERARCHY, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gHierarchy *)instance)->priv)

struct Private {
	Log4gLogger *root;
	GHashTable *table;
	Log4gLoggerFactory *factory;
	Log4gLevel *threshold;
	gint threshold_int;
	gboolean warning;
	GArray *loggers;
	GMutex lock; /* Synchronizes 'table' */
};

static Log4gLogger *
exists(Log4gLoggerRepository *base, const gchar *name)
{
	return g_hash_table_lookup(GET_PRIVATE(base)->table, name);
}

static void
foreach_current_logger(G_GNUC_UNUSED gpointer key, gpointer value,
		gpointer user_data)
{
	GArray *loggers = (GArray *)user_data;
	if (LOG4G_IS_LOGGER(value)) {
		g_array_append_val(loggers, value);
	}
}

static const GArray *
get_current_loggers(Log4gLoggerRepository *base)
{
	struct Private *priv = GET_PRIVATE(base);
	guint size = g_hash_table_size(priv->table);
	if (!size) {
		return NULL;
	}
	if (!priv->loggers) {
		priv->loggers = g_array_new(FALSE, TRUE,
				sizeof(Log4gLogger *));
		if (!priv->loggers) {
			return NULL;
		}
	} else {
		g_array_set_size(priv->loggers, 0);
	}
	g_hash_table_foreach(priv->table, foreach_current_logger,
			priv->loggers);
	return priv->loggers;
}

static gboolean
update_parents(Log4gHierarchy *self, Log4gLogger *logger)
{
	struct Private *priv = GET_PRIVATE(self);
	gboolean status = TRUE;
	gboolean found = FALSE;
	gchar *dot;
	gchar *name = g_strdup(log4g_logger_get_name(logger));
	if (!name) {
		return FALSE;
	}
	while ((dot = g_strrstr(name, "."))) {
		*dot = '\0';
		GObject *object = g_hash_table_lookup(priv->table, name);
		if (!object) {
			gchar *key;
			Log4gProvisionNode *node =
				log4g_provision_node_new(logger);
			if (!node) {
				status = FALSE;
				goto exit;
			}
			key = g_strdup(name);
			if (!key) {
				g_object_unref(node);
				status = FALSE;
				goto exit;
			}
			g_hash_table_insert(priv->table, key, node);
		} else if (LOG4G_IS_LOGGER(object)) {
			log4g_logger_set_parent(logger, LOG4G_LOGGER(object));
			found = TRUE;
		} else if (LOG4G_IS_PROVISION_NODE(object)) {
			Log4gProvisionNode *node =
				LOG4G_PROVISION_NODE(object);
			log4g_provision_node_add_element(node, logger);
		} else {
			log4g_log_warn(Q_("internal error: unexpected "
						"object type"));
		}
	}
	if (!found) {
		log4g_logger_set_parent(logger, priv->root);
	}
exit:
	g_free(name);
	return status;
}

static gboolean
update_children(G_GNUC_UNUSED Log4gHierarchy *self, Log4gProvisionNode *node,
		Log4gLogger *logger)
{
	guint last = log4g_provision_node_size(node);
	for (guint i = 0; i < last; ++i) {
		Log4gLogger *child =
			LOG4G_LOGGER(log4g_provision_node_element_at(node, i));
		if (!child) {
			continue;
		}
		const gchar *name = log4g_logger_get_name(logger);
		if (!g_strcmp0(log4g_logger_get_name(child), name)) {
			log4g_logger_set_parent(logger,
					log4g_logger_get_parent(child));
			log4g_logger_set_parent(child, logger);
		}
	}
	return TRUE;
}

static Log4gLogger *
get_logger_factory(Log4gLoggerRepository *base, const gchar *name,
		Log4gLoggerFactory *factory)
{
	struct Private *priv = GET_PRIVATE(base);
	Log4gLogger *logger = NULL;
	g_mutex_lock(&priv->lock);
	GObject *object = g_hash_table_lookup(priv->table, name);
	if (!object) {
		gchar *key;
		logger = log4g_logger_factory_make_new_logger_instance(factory,
				name);
		if (!logger) {
			goto exit;
		}
		log4g_logger_set_logger_repository(logger, base);
		if (!update_parents(LOG4G_HIERARCHY(base), logger)) {
			g_object_unref(logger);
			logger = NULL;
			goto exit;
		}
		key = g_strdup(name);
		if (!key) {
			g_object_unref(logger);
			logger = NULL;
			goto exit;
		}
		g_hash_table_insert(priv->table, key, logger);
	} else if (LOG4G_IS_LOGGER(object)) {
		logger = LOG4G_LOGGER(object);
	} else if (LOG4G_IS_PROVISION_NODE(object)) {
		gchar *key;
		Log4gProvisionNode *node = LOG4G_PROVISION_NODE(object);
		logger = log4g_logger_factory_make_new_logger_instance(factory,
				name);
		if (!logger) {
			goto exit;
		}
		log4g_logger_set_logger_repository(logger, base);
		update_children(LOG4G_HIERARCHY(base), node, logger);
		if (!update_parents(LOG4G_HIERARCHY(base), logger)) {
			g_object_unref(logger);
			logger = NULL;
			goto exit;
		}
		key = g_strdup(name);
		if (!key) {
			g_object_unref(logger);
			logger = NULL;
			goto exit;
		}
		g_hash_table_insert(priv->table, key, logger);
	} else {
		/* should be unreachable */
	}
exit:
	g_mutex_unlock(&priv->lock);
	return logger;
}

static Log4gLogger *
get_logger(Log4gLoggerRepository *base, const gchar *name)
{
	return log4g_logger_repository_get_logger_factory(base, name,
			GET_PRIVATE(base)->factory);
}

static Log4gLogger *
get_root_logger(Log4gLoggerRepository *base)
{
	return GET_PRIVATE(base)->root;
}

static Log4gLevel *
get_threshold(Log4gLoggerRepository *base)
{
	return GET_PRIVATE(base)->threshold;
}

static gboolean
is_disabled(Log4gLoggerRepository *base, gint level)
{
	return GET_PRIVATE(base)->threshold_int > level;
}

static void
set_threshold(Log4gLoggerRepository *base, Log4gLevel *level)
{
	if (level) {
		struct Private *priv = GET_PRIVATE(base);
		g_object_ref(level);
		priv->threshold = level;
		priv->threshold_int = log4g_level_to_int(level);
	}
}

static void
shutdown(Log4gLoggerRepository *base)
{
	struct Private *priv = GET_PRIVATE(base);
	Log4gLogger *root = log4g_logger_get_root_logger();
	/* close all appenders */
	log4g_logger_close_nested_appenders(root);
	g_mutex_lock(&priv->lock);
	const GArray *loggers =
		log4g_logger_repository_get_current_loggers(base);
	if (!loggers) {
		goto exit;
	}
	Log4gLogger *logger;
	for (guint i = 0; i < loggers->len; ++i) {
		logger = g_array_index(loggers, Log4gLogger *, i);
		log4g_logger_close_nested_appenders(logger);
	}
	/* remove all appenders */
	log4g_logger_remove_all_appenders(root);
	for (guint i = 0; i < loggers->len; ++i) {
		logger = g_array_index(loggers, Log4gLogger *, i);
		log4g_logger_remove_all_appenders(logger);
	}
exit:
	g_mutex_unlock(&priv->lock);
}

static void
reset_configuration(Log4gLoggerRepository *base)
{
	struct Private *priv = GET_PRIVATE(base);
	log4g_logger_set_level(log4g_logger_get_root_logger(),
			log4g_level_DEBUG());
	log4g_logger_repository_set_threshold(base, log4g_level_ALL());
	log4g_logger_repository_shutdown(base);
	g_mutex_lock(&priv->lock);
	const GArray *loggers =
		log4g_logger_repository_get_current_loggers(base);
	if (!loggers) {
		goto exit;
	}
	Log4gLogger *logger;
	for (guint i = 0; i < loggers->len; ++i) {
		logger = g_array_index(loggers, Log4gLogger *, i);
		log4g_logger_set_level(logger, NULL);
		log4g_logger_set_additivity(logger, TRUE);
	}
exit:
	g_mutex_unlock(&priv->lock);
}

static void
set_threshold_string(Log4gLoggerRepository *base, const gchar *string)
{
	Log4gLevel *level = log4g_level_string_to_level(string);
	if (!level) {
		log4g_log_warn(Q_("could not convert [%s] to level."), string);
		return;
	}
	log4g_logger_repository_set_threshold(base, level);
}

static void
emit_no_appender_warning(Log4gLoggerRepository *base, Log4gLogger *logger)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->warning) {
		return;
	}
	log4g_log_warn(Q_("no appenders could be found for logger (%s)"),
			log4g_logger_get_name(logger));
	log4g_log_warn(Q_("please initialize the log4g system properly"));
	priv->warning = TRUE;
}

static void
logger_repository_init(Log4gLoggerRepositoryInterface *interface,
		G_GNUC_UNUSED gpointer data)
{
	interface->exists = exists;
	interface->get_current_loggers = get_current_loggers;
	interface->get_logger = get_logger;
	interface->get_logger_factory = get_logger_factory;
	interface->get_root_logger = get_root_logger;
	interface->get_threshold = get_threshold;
	interface->is_disabled = is_disabled;
	interface->reset_configuration = reset_configuration;
	interface->set_threshold = set_threshold;
	interface->set_threshold_string = set_threshold_string;
	interface->shutdown = shutdown;
	interface->emit_no_appender_warning = emit_no_appender_warning;
}

G_DEFINE_TYPE_WITH_CODE(Log4gHierarchy, log4g_hierarchy, G_TYPE_OBJECT,
	G_IMPLEMENT_INTERFACE(LOG4G_TYPE_LOGGER_REPOSITORY,
		logger_repository_init))

static void
log4g_hierarchy_init(Log4gHierarchy *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->root = NULL;
	priv->table = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, g_object_unref);
	priv->factory = log4g_default_logger_factory_new();
	priv->threshold = NULL;
	priv->threshold_int = 0;
	priv->warning = FALSE;
	priv->loggers = NULL;
	g_mutex_init(&priv->lock);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->root) {
		g_object_unref(priv->root);
		priv->root = NULL;
	}
	if (priv->table) {
		g_hash_table_destroy(priv->table);
		priv->table = NULL;
	}
	if (priv->factory) {
		g_object_unref(priv->factory);
		priv->factory = NULL;
	}
	if (priv->threshold) {
		g_object_unref(priv->threshold);
		priv->threshold = NULL;
	}
	G_OBJECT_CLASS(log4g_hierarchy_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->loggers) {
		g_array_free(priv->loggers, TRUE);
		priv->loggers = NULL;
	}
	g_mutex_clear(&priv->lock);
	G_OBJECT_CLASS(log4g_hierarchy_parent_class)->finalize(base);
}

static void
log4g_hierarchy_class_init(Log4gHierarchyClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_hierarchy_new:
 * @root: The root logger for the new hierarchy.
 *
 * Create a new logger hierarchy.
 *
 * Returns: (transfer none): A new logger hierarchy.
 * Since: 0.1
 */
Log4gLoggerRepository *
log4g_hierarchy_new(Log4gLogger *root)
{
	g_return_val_if_fail(root, NULL);
	Log4gLoggerRepository *self = g_object_new(LOG4G_TYPE_HIERARCHY, NULL);
	if (!self) {
		return NULL;
	}
	g_object_ref(root);
	GET_PRIVATE(self)->root = root;
	log4g_logger_repository_set_threshold(self, log4g_level_ALL());
	log4g_logger_set_logger_repository(root, G_OBJECT(self));
	return self;
}

/**
 * log4g_hierarchy_clear:
 * @base: The logger hierarchy to clear.
 *
 * Clear a logger hierarchy.
 *
 * Since: 0.1
 */
void
log4g_hierarchy_clear(Log4gLoggerRepository *self)
{
	g_return_if_fail(LOG4G_IS_HIERARCHY(self));
	g_hash_table_remove_all(GET_PRIVATE(self)->table);
}
