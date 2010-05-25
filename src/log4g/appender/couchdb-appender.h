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
 * \brief Log events to an Apache CouchDB
 * \author Mike Steinert
 * \date 5-20-2010
 *
 * The CouchDB appender logs events to a specified database using a
 * \ref log4g/layout/couchdb-layout.h "CouchDB layout".
 *
 * CouchDB appenders accept the following properties:
 * -# uri
 * -# database-name
 * -# credentials
 *
 * The value of uri determines the network location of the CouchDB where
 * output will be logged. If the value of uri is \e NULL then this appender
 * will attempt to log messages to the CouchDB at <http://127.0.0.1:5984>.
 *
 * The value of database-name determines the name of the database that will
 * store the log output. The default value is "log4g_messages".
 *
 * The credentials property enables authentication with the CouchDB server.
 *
 * \see <a href="http://www.couchdb.apache.org/">Apache CouchDB</a>,
 *      <a href="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</a>
 */

#ifndef LOG4G_COUCHDB_APPENDER_H
#define LOG4G_COUCHDB_APPENDER_H

#include <couchdb-glib.h>
#include <log4g/appender/appender-skeleton.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_COUCHDB_APPENDER \
    (log4g_couchdb_appender_get_type())

#define LOG4G_COUCHDB_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_COUCHDB_APPENDER, \
            Log4gCouchdbAppender))

#define LOG4G_IS_COUCHDB_APPENDER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_COUCHDB_APPENDER))

#define LOG4G_COUCHDB_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_COUCHDB_APPENDER, \
            Log4gCouchdbAppenderClass))

#define LOG4G_IS_COUCHDB_APPENDER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_COUCHDB_APPENDER))

#define LOG4G_COUCHDB_APPENDER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_COUCHDB_APPENDER, \
            Log4gCouchdbAppenderClass))

/** \brief Log4gCouchdbAppender object type definition */
typedef struct _Log4gCouchdbAppender Log4gCouchdbAppender;

/** \brief Log4gCouchdbAppender class type definition */
typedef struct _Log4gCouchdbAppenderClass Log4gCouchdbAppenderClass;

/** \brief Log4gCouchdbAppenderClass definition */
struct _Log4gCouchdbAppender {
    Log4gAppenderSkeleton parent_instance;
};

/** \brief Log4gCouchdbAppenderClass definition */
struct _Log4gCouchdbAppenderClass {
    Log4gAppenderSkeletonClass parent_class;
};

GType
log4g_couchdb_appender_get_type(void);

void
log4g_couchdb_appender_register(GTypeModule *base);

/**
 * \brief Create a new couchdb appender object.
 *
 * \param uri [in] The URI of the CouchDB to connect to.
 * \param name [in] The name of the database to append to.
 *
 * \return A new couchdb appender object.
 */
Log4gAppender *
log4g_couchdb_appender_new(const gchar *uri, const gchar *name);

/**
 * \brief Set the uri property.
 *
 * \param base [in] A CouchDB appender object.
 * \param uri [in] The new uri value for \e base.
 */
void
log4g_couchdb_appender_set_uri(Log4gAppender *base, const gchar *uri);

/**
 * \brief Retrieve the uri property.
 *
 * \param base [in] A CouchDB appender object.
 *
 * \return The uri value for \e base.
 */
const gchar *
log4g_couchdb_appender_get_uri(Log4gAppender *base);

/**
 * \brief Set the database-name property.
 *
 * \param base [in] A CouchDB appender object.
 * \param name [in] The new database-name value for \e base.
 */
void
log4g_couchdb_appender_set_database_name(Log4gAppender *base,
        const gchar *name);

/**
 * \brief Retrieve the database-name property.
 *
 * \param base [in] A CouchDB appender object.
 *
 * \return The database-name value for \e base.
 */
const gchar *
log4g_couchdb_appender_get_database_name(Log4gAppender *base);

/**
 * \brief Set the credentials property.
 *
 * \param base [in] A CouchDB appender object.
 * \param credentials [in] A CouchDB credentials object.
 *
 * \note To disable authentication, set credentials to \e NULL.
 *
 * \see <a href="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</a>
 */
void
log4g_couchdb_appender_set_credentials(Log4gAppender *base,
        CouchdbCredentials *credentials);

/**
 * \brief Retrieve the credentials property.
 *
 * \param base [in] A CouchDB appender object.
 *
 * \return The credentials value for \e base.
 *
 * \see <a href="http://git.gnome.org/browse/couchdb-glib/">couchdb-glib</a>
 */
CouchdbCredentials *
log4g_couchdb_appender_get_credentials(Log4gAppender *base);

G_END_DECLS

#endif /* LOG4G_COUCHDB_APPENDER_H */
