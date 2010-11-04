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

typedef struct _Log4gCouchdbLayout Log4gCouchdbLayout;

typedef struct _Log4gCouchdbLayoutClass Log4gCouchdbLayoutClass;

/**
 * Log4gCouchdbLayout:
 *
 * The <structname>Log4gCouchdbLayout</structname> structure does not have any
 * public members.
 */
struct _Log4gCouchdbLayout {
    /*< private >*/
    Log4gLayout parent_instance;
};

/**
 * Log4gCouchdbLayoutFormatDocument:
 * @base: A couchdb layout object.
 * @event: A logging event object to be laid out.
 *
 * Sub-classes may override this function to return a custom CouchDB document.
 * The #Log4gCouchdbAppender will call this function to format logging events
 * before they are inserted into the database.
 *
 * Returns: A new CouchdbDocument object. Call g_object_unref() on the
 *          returned object to free memory.
 * Since: 0.1
 */
typedef CouchdbDocument *
(*Log4gCouchdbLayoutFormatDocument)(Log4gLayout *base,
        Log4gLoggingEvent *event);

/**
 * Log4gCouchdbLayoutClass:
 * @format_document: Create a new CouchdbDocument object.
 */
struct _Log4gCouchdbLayoutClass {
    /*< private >*/
    Log4gLayoutClass parent_class;
    /*< public >*/
    Log4gCouchdbLayoutFormatDocument format_document;
};

GType
log4g_couchdb_layout_get_type(void);

void
log4g_couchdb_layout_register(GTypeModule *module);

CouchdbDocument *
log4g_couchdb_layout_format_document(Log4gLayout *base,
        Log4gLoggingEvent *event);

G_END_DECLS

#endif /* LOG4G_COUCHDB_LAYOUT_H */
