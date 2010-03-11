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
 * \date 2-5-2010
 */

#ifndef LOG4G_CONFIGURATOR_H
#define LOG4G_CONFIGURATOR_H

#include <log4g/interface/logger-repository.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_CONFIGURATOR \
    (log4g_configurator_get_type())

#define LOG4G_CONFIGURATOR(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_CONFIGURATOR, \
            Log4gConfigurator))

#define LOG4G_IS_CONFIGURATOR(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_CONFIGURATOR))

#define LOG4G_CONFIGURATOR_GET_INTERFACE(instance) \
    (G_TYPE_INSTANCE_GET_INTERFACE((instance), LOG4G_TYPE_CONFIGURATOR, \
            Log4gConfiguratorInterface));

/** \brief Log4gConfigurator object type definition */
typedef struct _Log4gConfigurator Log4gConfigurator;

/** \brief Log4gConfigurator class type definition */
typedef struct _Log4gConfiguratorInterface
                    Log4gConfiguratorInterface;

/** \brief Log4gConfiguratorClass definition */
struct _Log4gConfiguratorInterface {
    GTypeInterface parent_interface; /**< parent interface */
    gboolean (*do_configure)(Log4gConfigurator *self, const char *uri,
            Log4gLoggerRepository *repository, GError **error);
};

GType log4g_configurator_get_type(void);

/**
 */
gboolean
log4g_configurator_do_configure(Log4gConfigurator *self, const char *uri,
        Log4gLoggerRepository *repository, GError **error);

G_END_DECLS

#endif /* LOG4G_CONFIGURATOR_H */
