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
 * \brief TODO
 * \author Mike Steinert
 * \date 5-20-2010
 *
 * TODO
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
     * \brief TODO
     *
     * \param base [in] TODO
     * \param event [in] TODO
     *
     * \return TODO
     */
    CouchdbDocument *
    (*format_document)(Log4gLayout *base, Log4gLoggingEvent *event,
            CouchdbSession *session);
};

GType
log4g_couchdb_layout_get_type(void);

/**
 * \brief Create a new couchdb layout object.
 *
 * \param session [in] TODO
 *
 * \return A new couchdb layout object.
 */
Log4gLayout *
log4g_couchdb_layout_new(CouchdbSession *session);

/**
 * \brief TODO
 *
 * \param base [in] TODO
 * \param event [in] TODO
 * \param session [in] TODO
 *
 * \return TODO
 */
CouchdbDocument *
log4g_couchdb_layout_format_document(Log4gLayout *base,
                Log4gLoggingEvent *event, CouchdbSession *session);

/**
 * \brief TODO
 *
 * \param base [in] TODO
 * \param event [in] TODO
 *
 * \return TODO
 */
void
log4g_couchdb_layout_set_session(Log4gLayout *base, CouchdbSession *session);

G_END_DECLS

#endif /* LOG4G_COUCHDB_LAYOUT_H */
