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
 * \brief Implements the API in log4g/default-repository-selector.h
 * \author Mike Steinert
 * \date 2-11-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/default-repository-selector.h"

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, \
            LOG4G_TYPE_DEFAULT_REPOSITORY_SELECTOR, struct Log4gPrivate))

struct Log4gPrivate {
    Log4gLoggerRepository *repository;
};

static Log4gLoggerRepository *
get_logger_repository(Log4gRepositorySelector *base)
{
    return GET_PRIVATE(base)->repository;
}

static void repository_selector_init(
        Log4gRepositorySelectorInterface *interface, gpointer data)
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
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->repository = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->repository) {
        g_object_unref(priv->repository);
        priv->repository = NULL;
    }
}

static void log4g_default_repository_selector_class_init(
        Log4gDefaultRepositorySelectorClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObjectClass */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

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
