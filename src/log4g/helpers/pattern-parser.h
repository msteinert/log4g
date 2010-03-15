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
 * \brief Parse pattern layout conversion patterns.
 * \author Mike Steinert
 * \date 2-15-2010
 *
 * This class performs most of the work done by the pattern layout class.
 * The conversion pattern is parsed and a chained list of pattern converters
 * is created.
 *
 * \see log4g/layout/pattern-layout.h, log4g/helpers/pattern-converter.h
 */

#ifndef LOG4G_PATTERN_PARSER_H
#define LOG4G_PATTERN_PARSER_H

#include <log4g/helpers/pattern-converter.h>

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

/** \brief Log4gPatternParser object type definition */
typedef struct _Log4gPatternParser Log4gPatternParser;

/** \brief Log4gPatternParser class type definition */
typedef struct _Log4gPatternParserClass Log4gPatternParserClass;

/** \brief Log4gPatternParserClass definition */
struct _Log4gPatternParser {
    GObject parent_instance;
};

/** \brief Log4gPatternParserClass definition */
struct _Log4gPatternParserClass {
    GObjectClass parent_class;
};

GType
log4g_pattern_parser_get_type(void);

/**
 * \brief Create a new pattern parser object.
 *
 * \param pattern [in] The conversion pattern to parse.
 *
 * \return A new pattern parser object.
 */
Log4gPatternParser *
log4g_pattern_parser_new(const gchar *pattern);

/**
 * \brief Add a pattern converter to the current list.
 *
 * \param self [in] A pattern converter object.
 * \param pc [in] The pattern converter to add to \e self.
 *
 * \see log4g/helpers/pattern-converter.h
 */
void
log4g_pattern_parser_add_to_list(Log4gPatternParser *self,
        Log4gPatternConverter *pc);

/**
 * \brief Extract a braced conversion pattern option.
 *
 * \param self [in] A pattern converter object.
 *
 * \return A conversion pattern option or \e NULL if none was found. The
 *         caller must free the returned string with g_free().
 */
gchar *
log4g_pattern_parser_extract_option(Log4gPatternParser *self);

/**
 * \brief Extract a precision option from a conversion pattern.
 *
 * \param self [in] A pattern converter object.
 *
 * \return The precision value or zero if none was found.
 */
gint
log4g_pattern_parser_extract_precision_option(Log4gPatternParser *self);

/**
 * \brief Parse the conversion pattern.
 *
 * \param self [in] A pattern converter object.
 *
 * \return A chained list of pattern converters or \e NULL if none were
 *         created.
 *
 * \see log4g/helpers/pattern-converter.h
 */
Log4gPatternConverter *
log4g_pattern_parser_parse(Log4gPatternParser *self);

/**
 * \brief Finalize the conversion pattern being parsed.
 *
 * \param self [in] A pattern converter object.
 * \param c [in] The current conversion character.
 */
void
log4g_pattern_parser_finalize_converter(Log4gPatternParser *self, gchar c);

/**
 * \brief Add a pattern converter to a pattern parser object.
 *
 * \param self [in] A pattern converter object.
 * \param pc [in] The pattern converter to add to \e self.
 *
 * \see log4g/helpers/pattern-converter.h
 */
void
log4g_pattern_parser_add_converter(Log4gPatternParser *self,
        Log4gPatternConverter *pc);

G_END_DECLS

#endif /* LOG4G_PATTERN_PARSER_H */
