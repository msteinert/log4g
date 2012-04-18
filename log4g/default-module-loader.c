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
 * SECTION: default-module-loader
 * @short_description: An implementation of the module loader interface
 * @see_also: #Log4gModuleLoaderInterface
 *
 * This class provides the default module loader implementation used by Log4g.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gmodule.h>
#include "log4g/helpers/default-module-loader.h"

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, \
		LOG4G_TYPE_DEFAULT_MODULE_LOADER, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gDefaultModuleLoader *)instance)->priv)

struct Private {
	gboolean loaded;
	GSList *modules;
};

/* Default system plugin path environment variable */
#define MODULE_SYSTEM_PATH ("LOG4G_MODULE_SYSTEM_PATH")

/* Default plugin path environment variable */
#define MODULE_PATH ("LOG4G_MODULE_PATH")

static gboolean
is_valid_module_name(const gchar *basename)
{
#if !defined(G_OS_WIN32) && !defined(G_WITH_CYGWIN)
	return g_str_has_prefix(basename, "lib")
		&& g_str_has_suffix(basename, ".so");
#else
	return g_str_has_suffix(basename, ".dll");
#endif
}

static void
load_directory(Log4gModuleLoader *base, const gchar *dirname)
{
	if (!g_module_supported()) {
		return;
	}
	GError *error = NULL;
	GDir *dir = g_dir_open(dirname, 0, &error);
	if (!dir) {
		log4g_log_error("%s: %s", dirname, error->message);
		g_error_free(error);
		return;
	}
	struct Private *priv = GET_PRIVATE(base);
	const gchar *basename;
	while ((basename = g_dir_read_name(dir))) {
		if (!is_valid_module_name(basename)) {
			continue;
		}
		gchar *file = g_build_filename(dirname, basename, NULL);
		if (file) {
			GTypeModule *module = log4g_module_new(file);
			if (module) {
				if (g_type_module_use(module)) {
					log4g_log_debug("loaded module: %s",
							module->name);
					priv->modules =
						g_slist_append(priv->modules,
								module);
					g_type_module_unuse(module);
				} else {
					log4g_log_error("failed to load "
							"module: %s", file);
					g_object_unref(module);
				}
			}
			g_free(file);
		}
	}
	g_dir_close(dir);
}

static void
load_env_var(Log4gModuleLoader *base, const gchar *env)
{
	if ('\0' == *env) {
		return;
	}
	gchar **basename = g_strsplit(env, ":", -1);
	if (!basename) {
		log4g_log_error("g_strsplit() returned NULL");
		return;
	}
	for (gint i = 0; basename[i]; ++i) {
		load_directory(base, basename[i]);
	}
	g_strfreev(basename);
}

static void
load_modules(Log4gModuleLoader *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->loaded) {
		return;
	}
	const gchar *path = g_getenv(MODULE_SYSTEM_PATH);
	if (!path) {
		path = LOG4G_MODULEDIR;
		g_setenv(MODULE_SYSTEM_PATH, path, 1);
	}
	load_env_var(base, path);
	path = g_getenv(MODULE_PATH);
	if (path) {
		load_env_var(base, path);
	}
	priv->loaded = TRUE;
}

static void
module_loader_init(Log4gModuleLoaderInterface *interface,
		G_GNUC_UNUSED gpointer data)
{
	interface->load_modules = load_modules;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDefaultModuleLoader, log4g_default_module_loader,
		G_TYPE_OBJECT,
		G_IMPLEMENT_INTERFACE(LOG4G_TYPE_MODULE_LOADER,
			module_loader_init))

static void
log4g_default_module_loader_init(Log4gDefaultModuleLoader *self)
{
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->loaded = FALSE;
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->modules) {
		/* NOTE: GTypeModules must not be unref'd */
		g_slist_free(priv->modules);
	}
	G_OBJECT_CLASS(log4g_default_module_loader_parent_class)->
		finalize(base);
}

static void
log4g_default_module_loader_class_init(Log4gDefaultModuleLoaderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_default_module_loader_new:
 *
 * Create a new default module loader object.
 *
 * Returns: (transfer full): A new default module loader object.
 * Since: 0.1
 */
Log4gModuleLoader *
log4g_default_module_loader_new(void)
{
	return g_object_new(LOG4G_TYPE_DEFAULT_MODULE_LOADER, NULL);
}
