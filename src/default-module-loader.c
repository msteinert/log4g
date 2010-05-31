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
 * \brief Implements the API in log4g/default-module-loader.h
 * \author Mike Steinert
 * \date 5-30-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gmodule.h>
#include "log4g/helpers/default-module-loader.h"

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_DEFAULT_MODULE_LOADER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean loaded;
    GSList *modules;
};

/** \brief Default system plugin path environment variable */
#define MODULE_SYSTEM_PATH ("LOG4G_MODULE_SYSTEM_PATH")

/** \brief Default plugin path environment variable */
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

void
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
    struct Log4gPrivate *priv = GET_PRIVATE(base);
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
                    log4g_log_debug("loaded module: %s", module->name);
                    priv->modules = g_slist_append(priv->modules, module);
                    g_type_module_unuse(module);
                } else {
                    log4g_log_error("failed to load module: %s", file);
                    g_object_unref(module);
                }
            }
            g_free(file);
        }
    }
    g_dir_close(dir);
}

void
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
    struct Log4gPrivate *priv = GET_PRIVATE(base);
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
module_loader_init(Log4gModuleLoaderInterface *interface, gpointer data)
{
    interface->load_modules = load_modules;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDefaultModuleLoader, log4g_default_module_loader,
        G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_MODULE_LOADER, module_loader_init))

static void
log4g_default_module_loader_init(Log4gDefaultModuleLoader *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->loaded = FALSE;
    priv->modules = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->modules) {
        /* NOTE: GTypeModules must not be unref'd */
        g_slist_free(priv->modules);
        priv->modules = NULL;
    }
    G_OBJECT_CLASS(log4g_default_module_loader_parent_class)->dispose(base);
}

static void
log4g_default_module_loader_class_init(Log4gDefaultModuleLoaderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObjectClass */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gModuleLoader *
log4g_default_module_loader_new(void)
{
    return g_object_new(LOG4G_TYPE_DEFAULT_MODULE_LOADER, NULL);
}
