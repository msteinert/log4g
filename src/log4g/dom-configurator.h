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
 * \brief Initialize the Log4g environment using a DOM tree.
 * \author Mike Steinert
 * \date 2-23-2010
 *
 * The DTD is specified in \e log4g.dtd. The Log4g DTD differs in some small
 * ways from the official Log4j DTD:
 *
 * - Use "appender" everywhere instead of "appender-ref"
 *     - Appenders may be declared inline to a logger. Appender references
 *       simply use a named "appender" tag and do not define a new appender.
 * - Use "property" instead of "param"
 *     - The decision was made to use the keyword "property" instead of
 *       "param" to reflect the underlying GLib/GObject implementation.
 *
 * The following configuration provides a cut-and-paste example:
 *
 * \code
 * <?xml version="1.0" encoding="UTF-8" ?>
 * <!DOCTYPE log4g:configuration PUBLIC "-//GNOME//DTD LOG4G 1.0//EN"
 *     "http://www.gnome.org/log4g/1.0/log4g.dtd">
 * <log4g:configuration>
 *     <appender name="A1" type="Log4gConsoleAppender">
 *         <layout type="Log4gPatternLayout">
 *             <property name="conversion-pattern"
 *                       value="%-4r [%t] %-5p %c %x - %m%n" />
 *         </layout>
 *     </appender>
 *     <root>
 *         <level value="DEBUG" />
 *         <appender name="A1" />
 *     </root>
 * </log4g:configuration>
 * \endcode
 *
 * In this example all events will be logged to the console appender (stdout by
 * default) using the specified pattern layout.
 *
 * You may enable internal log messages by setting the \e LOG4G_FLAGS
 * environment variable to \e "debug". For example (Bash):
 * \code
 * $ export LOG4G_FLAGS=debug
 * \endcode
 *
 * Alternatively you may set the \e debug attribute in the
 * \e log4g:configuration element:
 * \code
 * <log4g:configuration debug="true">
 * </log4g:configuration>
 * \endcode
 *
 * \see log4g_init(), log4g_get_option_group()
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
    GObject parent_instance;
};

/** \brief Log4gDOMConfiguratorClass definition */
struct _Log4gDOMConfiguratorClass {
    GObjectClass parent_class;
};

GType
log4g_dom_configurator_get_type(void);

/**
 * \brief Create a new DOM configurator.
 *
 * \return A new DOM configurator object.
 */
Log4gConfigurator *
log4g_dom_configurator_new(void);

/**
 * \brief Configure Log4g by reading a log4g.dtd compliant XML configuration
 *        file.
 *
 * \param uri [in] A file or URI to load the configuration from.
 * \param error [out] Returns any error messages.
 *
 * \return \e TRUE if the configuration was successful, \e FALSE otherwise.
 */
gboolean
log4g_dom_configurator_configure(const gchar *uri, GError **error);

G_END_DECLS

#endif /* LOG4G_DOM_CONFIGURATOR_H */
