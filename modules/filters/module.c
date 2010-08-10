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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "filter/deny-all-filter.h"
#include "filter/level-range-filter.h"
#include "filter/level-match-filter.h"
#include "filter/string-match-filter.h"
#include "log4g/module.h"

void
log4g_module_load(GTypeModule *module)
{
    g_type_module_set_name(module, "core-filters");
    log4g_deny_all_filter_register(module);
    log4g_level_range_filter_register(module);
    log4g_level_match_filter_register(module);
    log4g_string_match_filter_register(module);
}
