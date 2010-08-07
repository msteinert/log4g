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

#ifndef LOG4G_HIERARCHY_H
#define LOG4G_HIERARCHY_H

#include <log4g/interface/logger-repository.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_HIERARCHY \
    (log4g_hierarchy_get_type())

#define LOG4G_HIERARCHY(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_HIERARCHY, \
            Log4gHierarchy))

#define LOG4G_IS_HIERARCHY(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_HIERARCHY))

#define LOG4G_HIERARCHY_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_HIERARCHY, \
            Log4gHierarchyClass))

#define LOG4G_IS_HIERARCHY_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_HIERARCHY))

#define LOG4G_HIERARCHY_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_HIERARCHY, \
            Log4gHierarchyClass))

typedef struct _Log4gHierarchy Log4gHierarchy;

typedef struct _Log4gHierarchyClass Log4gHierarchyClass;

/**
 * Log4gHierarchy:
 *
 * The <structname>Log4gHierarchy</structname> structure does not have any
 * public members.
 */
struct _Log4gHierarchy {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gHierarchyClass:
 *
 * The <structname>Log4gHierarchyClass</structname> structure does not have any
 * public members.
 */
struct _Log4gHierarchyClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_hierarchy_get_type(void);

Log4gLoggerRepository *
log4g_hierarchy_new(Log4gLogger *root);

void
log4g_hierarchy_clear(Log4gLoggerRepository *base);

G_END_DECLS

#endif /* LOG4G_HIERARCHY_H */
