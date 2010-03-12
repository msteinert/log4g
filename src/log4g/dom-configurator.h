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
 * \file
 * \brief ...
 * \author Mike Steinert
 * \date 2-23-2010
 */

#ifndef LOG4G_DOM_CONFIGURATOR_H
#define LOG4G_DOM_CONFIGURATOR_H

#include <log4g/interface/configurator.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_DOM_CONFIGURATOR \
    (log4g_dom_configurator_get_type())

#define LOG4G_DOM_CONFIGURATOR(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_DOM_CONFIGURATOR, \
            Log4gDOMConfigurator))

#define LOG4G_IS_DOM_CONFIGURATOR(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_DOM_CONFIGURATOR))

#define LOG4G_DOM_CONFIGURATOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_DOM_CONFIGURATOR, \
            Log4gDOMConfiguratorClass))

#define LOG4G_IS_DOM_CONFIGURATOR_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_DOM_CONFIGURATOR))

#define LOG4G_DOM_CONFIGURATOR_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_DOM_CONFIGURATOR, \
            Log4gDOMConfiguratorClass))

/** \brief Log4gDOMConfigurator object type definition */
typedef struct _Log4gDOMConfigurator Log4gDOMConfigurator;

/** \brief Log4gDOMConfigurator class type definition */
typedef struct _Log4gDOMConfiguratorClass Log4gDOMConfiguratorClass;

/** \brief Log4gDOMConfiguratorClass definition */
struct _Log4gDOMConfigurator {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gDOMConfiguratorClass definition */
struct _Log4gDOMConfiguratorClass {
    GObjectClass parent_class; /**< parent class */
};

GType
log4g_dom_configurator_get_type(void);

/**
 */
Log4gConfigurator *
log4g_dom_configurator_new(void);

/**
 */
gboolean
log4g_dom_configurator_configure(const gchar *uri, GError **error);

G_END_DECLS

#endif /* LOG4G_DOM_CONFIGURATOR_H */