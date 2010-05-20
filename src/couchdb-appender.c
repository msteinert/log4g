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
 * \brief Implements the API in log4g/appender/couchdb-appender.h
 * \author Mike Steinert
 * \date 5-20-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/appender/couchdb-appender.h"

static void
_close(Log4gAppender *base)
{
    /* do nothing */
}

static void
do_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    /* do nothing */
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return FALSE;
}

static void
appender_init(Log4gAppenderInterface *interface)
{
    interface->close = _close;
    interface->do_append = do_append;
    interface->requires_layout = requires_layout;
}

G_DEFINE_TYPE_WITH_CODE(Log4gCouchDBAppender, log4g_couchdb_appender,
        LOG4G_TYPE_APPENDER_SKELETON,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_APPENDER, appender_init))

static void
log4g_couchdb_appender_init(Log4gCouchDBAppender *self)
{
}

static void
dispose(GObject *base)
{
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    G_OBJECT_CLASS(log4g_couchdb_appender_parent_class)->finalize(base);
}

static void
log4g_couchdb_appender_class_init(Log4gCouchDBAppenderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
}

Log4gAppender *
log4g_couchdb_appender_new(void)
{
    return g_object_new(LOG4G_TYPE_COUCHDB_APPENDER, NULL);
}
