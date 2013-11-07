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
#include "helpers/pattern-converter.h"
#include "helpers/pattern-parser.h"
#include "layout/date-layout.h"
#include "layout/html-layout.h"
#include "layout/json-layout.h"
#include "layout/pattern-layout.h"
#include "layout/simple-layout.h"
#include "layout/ttcc-layout.h"
#include "layout/xml-layout.h"
#include "log4g/module.h"

void
log4g_module_load(GTypeModule *module)
{
	g_type_module_set_name(module, "core-layouts");
	log4g_date_layout_register(module);
	log4g_html_layout_register(module);
	log4g_json_layout_register(module);
	log4g_pattern_converter_register(module);
	log4g_pattern_parser_register(module);
	log4g_pattern_layout_register(module);
	log4g_simple_layout_register(module);
	log4g_ttcc_layout_register(module);
	log4g_xml_layout_register(module);
}
