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
 * \brief Format events into XML.
 * \author Mike Steinert
 * \date 2-17-2010
 *
 * The output of this layout consists of a series of Log4g log event elements.
 * It does not output a complete well-formed XML document. The output is
 * designed to be included as an external entity in a separate file to form
 * a complete XML document.
 *
 * For example, if "abc" is the name of the file where the XML layout output
 * is directed, a well-formed XML document would be:
 *
 * \code
 * <?xml version="1.0" ?>
 * <!DOCTYPE log4g:events SYSTEM "log4g.dtd"
 *     [<!ENTITY data SYSTEM "abc">]>
 * <log4g:events version="1.0" xmlns:lo4g="http://gnome.org/log4g/1.0/">
 *     &data;
 * </log4g:events>
 * \endcode
 *
 * This approach enforces the independence of the XML layout and the appender
 * where it is embedded.
 *
 * XML layouts accept two properties:
 * -# properties
 * -# location-info
 *
 * Setting properties to \e TRUE causes the XML layout to output all
 * MDC (mapped data context) values.
 *
 * Setting the location-info property to \e TRUE will cause the HTML layout
 * to include the log message location, i.e. function(file:line). The default
 * value is \e FALSE.
 */

#ifndef LOG4G_XML_LAYOUT_H
#define LOG4G_XML_LAYOUT_H

#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_XML_LAYOUT \
    (log4g_xml_layout_get_type())

#define LOG4G_XML_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_XML_LAYOUT, \
            Log4gXMLLayout))

#define LOG4G_IS_XML_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_XML_LAYOUT))

#define LOG4G_XML_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_XML_LAYOUT, \
            Log4gXMLLayoutClass))

#define LOG4G_IS_XML_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_XML_LAYOUT))

#define LOG4G_XML_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_XML_LAYOUT, \
            Log4gXMLLayoutClass))

/** \brief Log4gXMLLayout object type definition */
typedef struct _Log4gXMLLayout Log4gXMLLayout;

/** \brief Log4gXMLLayout class type definition */
typedef struct _Log4gXMLLayoutClass Log4gXMLLayoutClass;

/** \brief Log4gXMLLayoutClass definition */
struct _Log4gXMLLayout {
    Log4gLayout parent_instance; /**< parent instance */
};

/** \brief Log4gXMLLayoutClass definition */
struct _Log4gXMLLayoutClass {
    Log4gLayoutClass parent_class; /**< parent class */
};

GType
log4g_xml_layout_get_type(void);

void
log4g_xml_layout_register(GTypeModule *module);

/**
 * \brief Create a new XML layout object.
 *
 * \return A new XML layout object.
 */
Log4gLayout *
log4g_xml_layout_new(void);

/**
 * \brief Set the properties property.
 *
 * \param base [in] An XML layout object.
 * \param properties [in] The new properties value for \e base.
 */
void
log4g_xml_layout_set_properties(Log4gLayout *base, gboolean properties);

/**
 * \brief Retrieve the properties property.
 *
 * \param base [in] An XML layout object.
 *
 * \return The properties value for \e base.
 */
gboolean
log4g_xml_layout_get_properties(Log4gLayout *base);

/**
 * \brief Set the location-info property.
 *
 * \param base [in] An XML layout object.
 * \param info [in] The new location info value for \e base.
 */
void
log4g_xml_layout_set_location_info(Log4gLayout *base, gboolean info);

/**
 * \brief Retrieve the location-info property.
 *
 * \param base [in] An XML layout object.
 *
 * \return The location info value for \e base.
 */
gboolean
log4g_xml_layout_get_location_info(Log4gLayout *base);

G_END_DECLS

#endif /* LOG4G_XML_LAYOUT_H */
