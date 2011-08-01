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
 * SECTION: default-repository-selector
 * @short_description: An implementation of the repository selector interface
 * @see_also: #Log4gRepositorySelectorInterface
 *
 * This class provides the repository selector implementation used by Log4g.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/default-repository-selector.h"

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gDefaultRepositorySelector *)instance)->priv)

struct Private {
	Log4gLoggerRepository *repository;
};

static Log4gLoggerRepository *
get_logger_repository(Log4gRepositorySelector *base)
{
	return GET_PRIVATE(base)->repository;
}

static void
repository_selector_init(Log4gRepositorySelectorInterface *interface,
		gpointer data)
{
	interface->get_logger_repository = get_logger_repository;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDefaultRepositorySelector,
		log4g_default_repository_selector, G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(LOG4G_TYPE_REPOSITORY_SELECTOR,
			repository_selector_init))

static void
log4g_default_repository_selector_init(Log4gDefaultRepositorySelector *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->repository = NULL;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->repository) {
		g_object_unref(priv->repository);
		priv->repository = NULL;
	}
	G_OBJECT_CLASS(log4g_default_repository_selector_parent_class)->
		dispose(base);
}

static void
log4g_default_repository_selector_class_init(
		Log4gDefaultRepositorySelectorClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_default_repository_selector_new:
 * @repository: The logger repository to be selected.
 *
 * Create a new default repository selector object.
 *
 * Returns: A new default repository selector object.
 * Since: 0.1
 */
Log4gRepositorySelector *
log4g_default_repository_selector_new(Log4gLoggerRepository *repository)
{
	Log4gDefaultRepositorySelector *self =
		g_object_new(LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, NULL);
	if (!self) {
		return NULL;
	}
	g_object_ref(repository);
	GET_PRIVATE(self)->repository = repository;
	return LOG4G_REPOSITORY_SELECTOR(self);
}
