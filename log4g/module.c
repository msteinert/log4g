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
 * SECTION: module
 * @short_description: Loadable appender, filter & layout modules
 *
 * This class implements a dynamic module system in order to make appenders,
 * layouts & filters available to the Log4g system.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <gmodule.h>
#include "log4g/module.h"

G_DEFINE_TYPE(Log4gModule, log4g_module, G_TYPE_TYPE_MODULE)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_MODULE, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gModule *)instance)->priv)

struct Private {
	gchar *file;
	GModule *module;
};

static void
log4g_module_init(Log4gModule *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->module) {
		g_module_close(priv->module);
	}
	g_free(priv->file);
	G_OBJECT_CLASS(log4g_module_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_FILENAME,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_FILENAME:
		if (!priv->file) {
			const gchar *file = g_value_get_string(value);
			if (file) {
				priv->file = g_strdup(file);
			}
		}
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gboolean
load(GTypeModule *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->file) {
		log4g_log_warn("module filename not set");
		return FALSE;
	}
	priv->module = g_module_open(priv->file,
			G_MODULE_BIND_LAZY | G_MODULE_BIND_LOCAL);
	if (!priv->module) {
		log4g_log_error("%s: %s", base->name, g_module_error());
		return FALSE;
	}
	void (*load)(GTypeModule *base);
	if (!g_module_symbol(priv->module, "log4g_module_load",
				(gpointer)&load)) {
		log4g_log_error("%s: %s", base->name, g_module_error());
		g_module_close(priv->module);
		return FALSE;
	}
	load(base);
	return TRUE;
}

static void
unload(GTypeModule *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (!priv->module) {
		return;
	}
	g_module_close(priv->module);
	priv->module = NULL;
}

static void
log4g_module_class_init(Log4gModuleClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	GTypeModuleClass *module_class = G_TYPE_MODULE_CLASS(klass);
	module_class->load = load;
	module_class->unload = unload;
	g_type_class_add_private(klass, sizeof(struct Private));
	/**
	 * Log4gModule:filename:
	 *
	 * The filename of the external module to load.
	 *
	 * Since: 0.1
	 */
	g_object_class_install_property(object_class, PROP_FILENAME,
		g_param_spec_string("filename", Q_("Filename"),
			Q_("Filename of the module"), NULL, G_PARAM_WRITABLE));
}

/**
 * log4g_module_new:
 * @file: The path of the file to load as a module.
 *
 * Create a new module.
 *
 * Returns: A new module object.
 * Since: 0.1
 */
GTypeModule *
log4g_module_new(const gchar *file)
{
	g_return_val_if_fail(file, NULL);
	return g_object_new(LOG4G_TYPE_MODULE, "filename", file, NULL);
}
