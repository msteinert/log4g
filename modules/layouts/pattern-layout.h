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
 * \brief A layout configurable with a pattern string.
 * \author Mike Steinert
 * \date 2-12-2010
 *
 * This class formats a log event and returns the results as a string. The
 * result of the formatting depends on the value of a conversion pattern.
 *
 * Pattern layouts accept a single property:
 * -# conversion-pattern
 *
 * The conversion pattern is similar in concept to the printf conversion
 * pattern. A conversion pattern is composed of literal text and format
 * control expressions called conversion specifiers.
 *
 * \note Literal text can be inserted within the conversion pattern.
 *
 * Conversion specifiers start with a percent sign (%) and optional format
 * modifiers followed by a conversion character. The conversion character
 * specifies the type of data that should be formatted, e.g. category, level,
 * date, thread name, etc. Format modifiers control field width, padding and
 * left/right justification.
 *
 * The following is a simple example:
 *
 * Let the conversion pattern be "%-5p [%t]: %m%n".
 *
 * Assume the following messages have been logged to the root logger from
 * the main thread:
 *
 * \code
 * log4g_debug("Message 1");
 * log4g_warn("Message 2");
 * \endcode
 *
 * The following output would appear in the log file:
 *
 * \code
 * DEBUG [main]: Message 1
 * WARN  [main]: Message 2
 * \endcode
 *
 * \note There is no explicit separator between text and conversion specifiers.
 *       The pattern parser knows it has reached the end of a conversion
 *       specifier when it reads a conversion character. In the above example
 *       the conversion specifier \e %-5p means the priority of the log event
 *       should be left justified to a width of five characters.
 *
 * The recognized conversion characters are:
 *
 * <table>
 * <tr>
 * <th>Conversion Character</th>
 * <th>Effect</th>
 * </tr>
 * <tr>
 * <td align="center"><b>c</b></td>
 * <td>
 * Output the logger category of a log event. The category conversion
 * specifier may be optionally followed by a precision specifier (a decimal
 * in constant brackets).
 *
 * If the precision specifier is given then only the corresponding number of
 * right most components of the category name will be printed. By default the
 * category name is printed in full.
 *
 * For example, the logger category name "a.b.c" the pattern <b>%%c{2}</b>
 * will output "b.c".
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>d</b></td>
 * <td>
 * Ouput the date of the logging event. The date conversion specifier may
 * be followed by a date format specifier enclosed in braces. For example:
 * <b>%%d{%%H:%%M:%%S}</b> or <b>%%d{%%l:%%M:%%S %%P - %%B %%e, %%G}</b>.
 *
 * The date format specifier accepts the same syntax as the strftime(3)
 * standard library function.
 *
 * \see strftime(3)
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>F</b></td>
 * <td>
 * Output the filename where the event was logged.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>l</b></td>
 * <td>
 * Output the full location information where the event was logged.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>L</b></td>
 * <td>
 * Output the line number where the event was logged.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>m</b></td>
 * <td>
 * Output the application supplied message associated with a log event.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>M</b></td>
 * <td>
 * Output the function (method) name where the event was logged.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>n</b></td>
 * <td>
 * Output the platform independent line separater character(s).
 *
 * Use this conversion character in place of non-portable line separator
 * strings such as "\n" or "\r\n".
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>p</b></td>
 * <td>
 * Output the level (also known as the priority) of a log event.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>r</b></td>
 * <td>
 * Output the number of milliseconds elapsed from the initialization of the
 * Log4g environment until the create of the log event.
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>t</b></td>
 * <td>
 * Output the name of the thread that generated the log event.
 *
 * \see log4g/helpers/thread.h
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>x</b></td>
 * <td>
 * Output the NDC (nested diagnostic context) associated with the thread
 * that generated the log event.
 *
 * \see log4g/ndc.h
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>X</b></td>
 * <td>
 * Output the MDC (mapped data context) associated with the thread that
 * generated the log event. The <b>X</b> conversion character \e must
 * be followed by the key for the map, placed between braces. For example,
 * <b>%%X{count}</b> where <b>count</b> is the key. The value in the MDC of
 * the corresponding key will be output.
 *
 * \see log4g/mdc.h
 * </td>
 * </tr>
 * <tr>
 * <td align="center"><b>%</b></td>
 * <td>
 * The sequence <b>%%</b> outputs a single percent sign.
 * </td>
 * </tr>
 * </table>
 *
 * The default behavior is to output the information as-is. If the aid of
 * format modifiers it is possible to change the minimum field width, the
 * maximum field with and justification.
 *
 * The optional format modifier is placed between the percent sign and the
 * conversion character.
 *
 * The left justification flag is a minus (-) character. Justification may
 * be followed by a minimum field width modifier (an integer), indicating
 * the minimum number of characters to output. The default is to pad on the
 * left, however you may specify right-padding with the left justification
 * flag. The space character is used to perform padding. If the data item is
 * larger than the minimum field width the field is expanded to accomodate
 * the data (the value is never truncated).
 *
 * The truncation behavior can be changed using the maximum field width
 * modifier, designated by a period followed with an integer. If the data item
 * is larger than the maximum field width the extra characters are removed
 * from the \e beginning of the data item.
 *
 * Below are various format modifier examples:
 *
 * <table>
 * <tr>
 * <th>Format Modifier</th>
 * <th>Left Justify</th>
 * <th>Minimum Width</th>
 * <th>Maximum Width</th>
 * <th>Comment</th>
 * </tr>
 * <tr>
 * <td align="center">%%20c</td>
 * <td align="center">No</td>
 * <td align="center">20</td>
 * <td align="center">None</td>
 * <td>
 * Left pad with spaces if the logger name is less than 20 characers long.
 * </td>
 * </tr>
 * <tr>
 * <td align="center">%%-20c</td>
 * <td align="center">Yes</td>
 * <td align="center">20</td>
 * <td align="center">None</td>
 * <td>
 * Right pad with spaces if the logger name is less than 20 characters long.
 * </td>
 * </tr>
 * <tr>
 * <td align="center">%%.30c</td>
 * <td align="center">N/A</td>
 * <td align="center">None</td>
 * <td align="center">30</td>
 * <td>
 * Truncate from the beginning if the logger name is longer than 30 characters.
 * </td>
 * </tr>
 * <tr>
 * <td align="center">%%20.30c</td>
 * <td align="center">No</td>
 * <td align="center">20</td>
 * <td align="center">30</td>
 * <td>
 * Left pad with spaces if the category name is shorter than 20 characters.
 * If the category name is longer than 30 characters then truncate from the
 * beginning.
 * </td>
 * </tr>
 * <tr>
 * <td align="center">%%-20.30c</td>
 * <td align="center">Yes</td>
 * <td align="center">20</td>
 * <td align="center">30</td>
 * <td>
 * Right pad with spaces if the category name is shorter than 20 characters.
 * If the category name is longer than 30 characters then truncate from the
 * beginning.
 * </td>
 * </tr>
 * </table>
 *
 * Below are some examples of conversion patterns.
 *
 * <b>%%r [%%t] %%-5p %%c %%x - %%m%%n</b>
 *
 * This is essentially the \ref log4g/layout/ttcc-layout.h "TTCC layout".
 *
 * <b>%%-6r [%%15.15t] %%-5p %%30.30c %%x - %%m%%n</b>
 *
 * Similar to the TTCC layout except:
 * - The relative time is right-padded if less than six digits
 * - The thread name is right padded if less than fifteen characters
 * - The thread name is truncated if longer than fifteen characters
 * - The log level is right-padded if less than 5 characters
 * - The logger name is left padded if shorter than thirty characters
 * - The logger name is truncated if longer than thirty characters
 */

#ifndef LOG4G_PATTERN_LAYOUT_H
#define LOG4G_PATTERN_LAYOUT_H

#include "pattern-parser.h"
#include <log4g/layout.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_PATTERN_LAYOUT \
    (log4g_pattern_layout_get_type())

#define LOG4G_PATTERN_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayout))

#define LOG4G_IS_PATTERN_LAYOUT(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_PATTERN_LAYOUT))

#define LOG4G_PATTERN_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayoutClass))

#define LOG4G_IS_PATTERN_LAYOUT_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_PATTERN_LAYOUT))

#define LOG4G_PATTERN_LAYOUT_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_PATTERN_LAYOUT, \
            Log4gPatternLayoutClass))

/** \brief Log4gPatternLayout object type definition */
typedef struct _Log4gPatternLayout Log4gPatternLayout;

/** \brief Log4gPatternLayout class type definition */
typedef struct _Log4gPatternLayoutClass Log4gPatternLayoutClass;

/** \brief Log4gPatternLayoutClass definition */
struct _Log4gPatternLayout {
    Log4gLayout parent_instance;
};

/** \brief Log4gPatternLayoutClass definition */
struct _Log4gPatternLayoutClass {
    Log4gLayoutClass parent_class;

    /**
     * \brief Create a new pattern parser object.
     *
     * Sub-classes may override this function to return a subclass of
     * Log4gPatternParser which recognize custom conversion characters.
     *
     * \param base [in] A conversion pattern object.
     * \param pattern [in] The conversion pattern to use.
     *
     * \return A new pattern parser object.
     *
     * \see pattern-parser.h
     */
    Log4gPatternParser *
    (*create_pattern_parser)(Log4gLayout *base, const gchar *pattern);
};

GType
log4g_pattern_layout_get_type(void);

void
log4g_pattern_layout_register(GTypeModule *module);

/**
 * \brief Invokes the virtual function
 *        _Log4gPatternLayoutClass::create_pattern_parser().
 *
 * \param base [in] A conversion pattern object.
 * \param pattern [in] The conversion pattern to use.
 *
 * \return A new pattern parser object.
 */
Log4gPatternParser *
log4g_pattern_layout_create_pattern_parser(Log4gLayout *base,
        const gchar *pattern);

G_END_DECLS

#endif /* LOG4G_PATTERN_LAYOUT_H */
