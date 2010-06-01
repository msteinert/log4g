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
 * \brief Maintain the logger hierarchy.
 * \author Mike Steinert
 * \date 1-29-2010
 *
 * This class specializes in retrieving loggers by name and maintaining the
 * logger hierarchy.
 *
 * \note The casual user has no need to deal with this class directly.
 *
 * The structure of the logger hierarchy is maintained by the function
 * log4g_logger_repository_get_logger() function. Children in the hierarchy
 * link to their parent but parents do not have a pointer to their children.
 * Descendant logger may be instantiated before their ancestors.
 *
 * In the case where a descendant is created before a particular ancestor
 * a provision node (log4g/provision-node.h) is created for the ancestor and
 * the descendant is added to the provision node. Other descendants of the
 * same ancestor are added to the previously created provision node.
 *
 * \see log4g/interface/logger-repository.h
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

/** \brief Log4gHierarchy type definition */
typedef struct _Log4gHierarchy Log4gHierarchy;

/** \brief Log4gHierarchy class type definition */
typedef struct _Log4gHierarchyClass Log4gHierarchyClass;

/** \brief Log4gHierarchyClass definition */
struct _Log4gHierarchy {
    GObject parent_instance; /**< parent instance */
};

/** \brief Log4gHierarchyClass definition */
struct _Log4gHierarchyClass {
    GObjectClass parent_class; /**< parent class */
};

GType
log4g_hierarchy_get_type(void);

/**
 * \brief Create a new logger hierarchy.
 *
 * \param root [in] The root logger for the new hierarchy.
 *
 * \return A new logger hierarchy.
 */
Log4gLoggerRepository *
log4g_hierarchy_new(Log4gLogger *root);

/**
 * \brief Clear a logger hierarchy.
 *
 * \param base [in] The logger hierarchy to clear.
 */
void
log4g_hierarchy_clear(Log4gLoggerRepository *base);

G_END_DECLS

#endif /* LOG4G_HIERARCHY_H */
