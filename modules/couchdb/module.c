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
 * \brief TODO
 * \author Mike Steinert
 * \date 5-25-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "couchdb-appender.h"
#include "couchdb-layout.h"
#include <log4g/module.h>

void
log4g_module_load(GTypeModule *base)
{
    g_type_module_set_name(base, "couchdb");
    log4g_couchdb_appender_register(base);
    log4g_couchdb_layout_register(base);
}
