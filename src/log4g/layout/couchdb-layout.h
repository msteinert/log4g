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
 * \brief Format events into a CouchDB document.
 * \author Mike Steinert
 * \date 5-20-2010
 *
 * This class formats a log event into a CouchDB document with the following
 * schema:
 *
 * \code
 * {
 *    "description": "Log4g-CouchDB JSON-Schema",
 *    "type": "object",
 *    "properties": {
 *        "_id": {
 *            "description": "Unique ID for this document, internal to CouchDB",
 *            "type": "string"
 *        },
 *        "_rev": {
 *            "description": "Revision for this document, internal to CouchDB",
 *            "type": "string"
 *        },
 *        "record_type": {
 *            "description": "The schema URI",
 *            "type": "string",
 *            "format": "uri"
 *        },
 *        "record_type_version": {
 *            "description": "The schema version number",
 *            "type": "string"
 *        },
 *        "message": {
 *            "description": "The log message",
 *            "type": "string"
 *        },
 *        "level": {
 *            "description": "The log level of the logging event",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "logger": {
 *            "description": "The name of the logger",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "mdc": {
 *            "description": "The mapped data context",
 *            "type": "object",
 *            "properties": {
 *                "type": { "type": "string" },
 *                "value": { "type": "string" },
 *            },
 *            "optional": true
 *        },
 *        "ndc": {
 *            "description": "The nested data context",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "timestamp": {
 *            "description": "The timestamp of the logging event",
 *            "type": "string",
 *            "format": "utc-millisec",
 *            "optional": true
 *        },
 *        "thread": {
 *            "description": "The thread where the event was logged",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "function": {
 *            "description": "The function name",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "file": {
 *            "description": "The file name",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "line": {
 *            "description": "The line number",
 *            "type": "string",
 *            "optional": true
 *        },
 *        "application_annotations": {
 *            "description": "Application-specific data",
 *            "type": "any",
 *            "optional": true
 *        }
 *    }
 * }
 * \endcode
 *
 * \see <a href="http://www.couchdb.apache.org/">Apache CouchDB</a>,
 *      <a href="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</a>
 */

#ifndef LOG4G_COUCHDB_LAYOUT_H
#define LOG4G_COUCHDB_LAYOUT_H

#include <couchdb-document.h>
#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_COUCHDB_LAYOUT \
    (log4g_couchdb_layout_get_type())

#define LOG4G_COUCHDB_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_COUCHDB_LAYOUT, \
            Log4gCouchdbLayout))

#define LOG4G_IS_COUCHDB_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_COUCHDB_LAYOUT))

#define LOG4G_COUCHDB_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_COUCHDB_LAYOUT, \
            Log4gCouchdbLayoutClass))

#define LOG4G_IS_COUCHDB_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_COUCHDB_LAYOUT))

#define LOG4G_COUCHDB_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_COUCHDB_LAYOUT, \
            Log4gCouchdbLayoutClass))

/** \brief Log4gCouchdbLayout object type definition */
typedef struct _Log4gCouchdbLayout Log4gCouchdbLayout;

/** \brief Log4gCouchdbLayout class type definition */
typedef struct _Log4gCouchdbLayoutClass Log4gCouchdbLayoutClass;

/** \brief Log4gCouchdbLayoutClass definition */
struct _Log4gCouchdbLayout {
    Log4gLayout parent_instance;
};

/** \brief Log4gCouchdbLayoutClass definition */
struct _Log4gCouchdbLayoutClass {
    Log4gLayoutClass parent_class;

    /**
     * \brief Create a new CouchdbDocument object.
     *
     * Sub-classes may override this function to return a custom CouchDB
     * document. The
     * \ref log4g/appender/couchdb-appender.h "CouchDB appender" will
     * call this function to format logging events before they are
     * inserted into the database.
     *
     * \param base [in] A couchdb layout object.
     * \param event [in] A logging event object to be laid out.
     * \param session [in] The CouchDB session with which the resulting
     *                     document will be associated.
     *
     * \return A new CouchdbDocument object. Call g_object_unref() on the
     *         returned object to free memory.
     *
     * \see log4g/appender/couchdb-appender.h
     */
    CouchdbDocument *
    (*format_document)(Log4gLayout *base, Log4gLoggingEvent *event,
            CouchdbSession *session);
};

GType
log4g_couchdb_layout_get_type(void);

void
log4g_couchdb_layout_register(GTypeModule *base);

/**
 * \brief Create a new couchdb layout object.
 *
 * \return A new CouchDB layout object.
 */
Log4gLayout *
log4g_couchdb_layout_new(void);

/**
 * \brief Invokes the virtual function
 *        _Log4gCouchdbLayoutClass::format_document().
 *
 * \param base [in] A CouchDB layout object.
 * \param event [in] The logging event object to be formatted into a
 *                   CouchDB document.
 * \param session [in] The CouchdbSession to associate the returned
 *                     document with.
 *
 * \return A logging event formatted into a CouchdbDocument object.
 */
CouchdbDocument *
log4g_couchdb_layout_format_document(Log4gLayout *base,
        Log4gLoggingEvent *event, CouchdbSession *session);

G_END_DECLS

#endif /* LOG4G_COUCHDB_LAYOUT_H */
