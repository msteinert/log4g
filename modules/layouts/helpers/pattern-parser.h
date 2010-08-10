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

#ifndef LOG4G_PATTERN_PARSER_H
#define LOG4G_PATTERN_PARSER_H

#include "helpers/pattern-converter.h"

G_BEGIN_DECLS

#define LOG4G_TYPE_PATTERN_PARSER \
    (log4g_pattern_parser_get_type())

#define LOG4G_PATTERN_PARSER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_PATTERN_PARSER, \
            Log4gPatternParser))

#define LOG4G_IS_PATTERN_PARSER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_PATTERN_PARSER))

#define LOG4G_PATTERN_PARSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_PATTERN_PARSER, \
            Log4gPatternParserClass))

#define LOG4G_IS_PATTERN_PARSER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_PATTERN_PARSER))

#define LOG4G_PATTERN_PARSER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_PATTERN_PARSER, \
            Log4gPatternParserClass))

typedef struct _Log4gPatternParser Log4gPatternParser;

typedef struct _Log4gPatternParserClass Log4gPatternParserClass;

/**
 * Log4gPatternParser:
 *
 * The <structname>Log4gPatternParser</structname> structure does not have any
 * public members.
 */
struct _Log4gPatternParser {
    /*< private >*/
    GObject parent_instance;
};

/**
 * Log4gPatternParserClass:
 *
 * The <structname>Log4gPatternParserClass</structname> structure does not have
 * any public members.
 */
struct _Log4gPatternParserClass {
    /*< private >*/
    GObjectClass parent_class;
};

GType
log4g_pattern_parser_get_type(void);

void
log4g_pattern_parser_register(GTypeModule *module);

Log4gPatternParser *
log4g_pattern_parser_new(const gchar *pattern);

void
log4g_pattern_parser_add_to_list(Log4gPatternParser *self,
        Log4gPatternConverter *pc);

gchar *
log4g_pattern_parser_extract_option(Log4gPatternParser *self);

gint
log4g_pattern_parser_extract_precision_option(Log4gPatternParser *self);

Log4gPatternConverter *
log4g_pattern_parser_parse(Log4gPatternParser *self);

void
log4g_pattern_parser_finalize_converter(Log4gPatternParser *self, gchar c);

void
log4g_pattern_parser_add_converter(Log4gPatternParser *self,
        Log4gPatternConverter *pc);

G_END_DECLS

#endif /* LOG4G_PATTERN_PARSER_H */
