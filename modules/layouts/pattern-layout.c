/* Copyright 2010, 2011 Michael Steinert
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
 * SECTION: pattern-layout
 * @short_description: A layout configurable with a pattern string
 *
 * This class formats a log event and returns the results as a string. The
 * result of the formatting depends on the value of a conversion pattern.
 *
 * Pattern layouts accept a single property:
 * <orderedlist>
 * <listitem><para>conversion-pattern</para></listitem>
 * </orderedlist>
 *
 * The conversion pattern is similar in concept to the printf conversion
 * pattern. A conversion pattern is composed of literal text and format
 * control expressions called conversion specifiers.
 *
 * <note><para>
 * Literal text can be inserted within the conversion pattern.
 * </para></note>
 *
 * Conversion specifiers start with a percent sign (\%) and optional format
 * modifiers followed by a conversion character. The conversion character
 * specifies the type of data that should be formatted, e.g. category, level,
 * date, thread name, etc. Format modifiers control field width, padding and
 * left/right justification.
 *
 * The following is a simple example:
 *
 * Let the conversion pattern be "\%-5p [\%t]: \%m\%n".
 *
 * Assume the following messages have been logged to the root logger from
 * the main thread:
 *
 * |[
 * log4g_debug("Message 1");
 * log4g_warn("Message 2");
 * ]|
 *
 * The following output would appear in the log file:
 *
 * |[
 * DEBUG [main]: Message 1
 * WARN  [main]: Message 2
 * ]|
 *
 * There is no explicit separator between text and conversion specifiers.
 * The pattern parser knows it has reached the end of a conversion specifier
 * when it reads a conversion character. In the above example the conversion
 * specifier <emphasis>\%-5p</emphasis> means the priority of the log event
 * should be left justified to a width of five characters.
 *
 * <table frame="all">
 * <title>Recognized Conversion Characters</title>
 * <tgroup cols="2" align="center" colsep="1" rowsep="1">
 * <colspec colname="char" />
 * <colspec colname="effect" />
 * <thead>
 * <row>
 * <entry>Conversion Character</entry>
 * <entry>Effect</entry>
 * </row>
 * </thead>
 * <tfoot>
 * <row>
 * <entry><emphasis>c</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the logger category of a log event. The category conversion
 * specifier may be optionally followed by a precision specifier (a decimal
 * in constant brackets).
 * </para>
 * <para>
 * If the precision specifier is given then only the corresponding number of
 * right most components of the category name will be printed. By default the
 * category name is printed in full.
 * </para>
 * <para>
 * For example, the logger category name "a.b.c" the pattern
 * <emphasis>\%c{2}</emphasis> will output "b.c".
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>d</emphasis></entry>
 * <entry align="left">
 * <para>
 * Ouput the date of the logging event. The date conversion specifier may
 * be followed by a date format specifier enclosed in braces. For example:
 * <emphasis>\%d{\%H:\%M:\%S}</emphasis> or
 * <emphasis>\%d{\%l:\%M:\%S \%P - \%B \%e, \%G}</emphasis>.
 * </para>
 * <para>
 * The date format specifier accepts the same syntax as the strftime(3)
 * standard library function.
 * </para>
 * <para>
 * @See: strftime(3)
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>F</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the filename where the event was logged.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>l</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the full location information where the event was logged.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>L</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the line number where the event was logged.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>m</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the application supplied message associated with a log event.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>M</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the function (method) name where the event was logged.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>n</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the platform independent line separater character(s).
 * </para>
 * <para>
 * Use this conversion character in place of non-portable line separator
 * strings such as "\n" or "\r\n".
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>p</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the level (also known as the priority) of a log event.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>r</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the number of milliseconds elapsed from the initialization of the
 * Log4g environment until the create of the log event.
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>t</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the name of the thread that generated the log event.
 * </para>
 * <para>
 * @See: #Log4gThreadClass
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>x</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the NDC (nested diagnostic context) associated with the thread
 * that generated the log event.
 * </para>
 * <para>
 * @See: #Log4gNDCClass
 * </para>
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>X</emphasis></entry>
 * <entry align="left">
 * <para>
 * Output the MDC (mapped data context) associated with the thread that
 * generated the log event. The <emphasis>X</emphasis> conversion character
 * <emphasis>must</emphasis> be followed by the key for the map, placed between
 * braces. For example, <emphasis>\%X{count}</emphasis> where
 * <emphasis>count</emphasis> is the key. The value in the MDC of the
 * corresponding key will be output.
 * </para>
 * @See: log4g/mdc.h
 * </entry>
 * </row>
 * <row>
 * <entry><emphasis>\%</emphasis></entry>
 * <entry align="left">
 * <para>
 * The sequence <emphasis>\%</emphasis> outputs a single percent sign.
 * </para>
 * </entry>
 * </row>
 * </tfoot>
 * </tgroup>
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
 * from the <emphasis>beginning</emphasis> of the data item.
 *
 * <table frame="all"><title>Format Modifier Examples</title>
 * <tgroup cols="5" align="center" colsep="1" rowsep="1">
 * <colspec colname="modifier" />
 * <colspec colname="justify" />
 * <colspec colname="minimum" />
 * <colspec colname="maximum" />
 * <colspec colname="comment" />
 * <thead>
 * <row>
 * <entry>Format Modifier</entry>
 * <entry>Left Justify</entry>
 * <entry>Minimum Width</entry>
 * <entry>Maximum Width</entry>
 * <entry>Comment</entry>
 * </row>
 * </thead>
 * <tfoot>
 * <row>
 * <entry>\%20c</entry>
 * <entry>No</entry>
 * <entry>20</entry>
 * <entry>None</entry>
 * <entry align="left">
 * Left pad with spaces if the logger name is less than 20 characers long.
 * </entry>
 * </row>
 * <row>
 * <entry>\%-20c</entry>
 * <entry>Yes</entry>
 * <entry>20</entry>
 * <entry>None</entry>
 * <entry align="left">
 * Right pad with spaces if the logger name is less than 20 characters long.
 * </entry>
 * </row>
 * <row>
 * <entry>\%.30c</entry>
 * <entry>N/A</entry>
 * <entry>None</entry>
 * <entry>30</entry>
 * <entry align="left">
 * Truncate from the beginning if the logger name is longer than 30 characters.
 * </entry>
 * </row>
 * <row>
 * <entry>\%20.30c</entry>
 * <entry>No</entry>
 * <entry>20</entry>
 * <entry>30</entry>
 * <entry align="left">
 * Left pad with spaces if the category name is shorter than 20 characters.
 * If the category name is longer than 30 characters then truncate from the
 * beginning.
 * </entry>
 * </row>
 * <row>
 * <entry>\%-20.30c</entry>
 * <entry>Yes</entry>
 * <entry>20</entry>
 * <entry>30</entry>
 * <entry align="left">
 * Right pad with spaces if the category name is shorter than 20 characters.
 * If the category name is longer than 30 characters then truncate from the
 * beginning.
 * </entry>
 * </row>
 * </tfoot>
 * </tgroup>
 * </table>
 *
 * Below are some examples of conversion patterns.
 *
 * <emphasis>\%r [\%t] \%-5p \%c \%x - \%m\%n</emphasis>
 *
 * This is essentially the #Log4gTTCCLayout.
 *
 * <emphasis>\%-6r [\%15.15t] \%-5p \%30.30c \%x - \%m\%n</emphasis>
 *
 * Similar to the TTCC layout except:
 * <itemizedlist>
 * <listitem><para>The relative time is right-padded if less than six digits</para></listitem>
 * <listitem><para>The thread name is right padded if less than fifteen characters</para></listitem>
 * <listitem><para>The thread name is truncated if longer than fifteen characters</para></listitem>
 * <listitem><para>The log level is right-padded if less than 5 characters</para></listitem>
 * <listitem><para>The logger name is left padded if shorter than thirty characters</para></listitem>
 * <listitem><para>The logger name is truncated if longer than thirty characters</para></listitem>
 * </itemizedlist>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "layout/pattern-layout.h"

G_DEFINE_DYNAMIC_TYPE(Log4gPatternLayout, log4g_pattern_layout,
		LOG4G_TYPE_LAYOUT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_LAYOUT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gPatternLayout *)instance)->priv)

/** \brief The default size of a string buffer */
#define BUF_SIZE (256)

/** \brief The maximum size of a string buffer */
#define MAX_CAPACITY (1024)

struct Private {
	gchar *pattern;
	GString *string;
	Log4gPatternConverter *head;
};

static void
log4g_pattern_layout_init(Log4gPatternLayout *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->head) {
		g_object_unref(priv->head);
		priv->head = NULL;
	}
	G_OBJECT_CLASS(log4g_pattern_layout_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->pattern);
	if (priv->string) {
		g_string_free(priv->string, TRUE);
	}
	G_OBJECT_CLASS(log4g_pattern_layout_parent_class)->finalize(base);
}

enum Properties {
	PROP_O = 0,
	PROP_CONVERSION_PATTERN,
	PROP_MAX
};

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
	struct Private *priv = GET_PRIVATE(base);
	switch (id) {
	case PROP_CONVERSION_PATTERN:
		g_free(priv->pattern);
		if (priv->head) {
			g_object_unref(priv->head);
			priv->head = NULL;
		}
		const gchar *pattern = g_value_get_string(value);
		priv->pattern = g_strdup((pattern ? pattern : "%m%n"));
		if (!priv->pattern) {
			break;
		}
		Log4gPatternParser *parser =
			log4g_pattern_layout_create_pattern_parser(
					LOG4G_LAYOUT(base), priv->pattern);
		if (!parser) {
			g_free(priv->pattern);
			priv->pattern = NULL;
			break;
		}
		priv->head = log4g_pattern_parser_parse(parser);
		g_object_unref(parser);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
		break;
	}
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->string) {
		if (priv->string->allocated_len > MAX_CAPACITY) {
			g_string_free(priv->string, TRUE);
			priv->string = NULL;
		}
	}
	if (!priv->string) {
		priv->string = g_string_sized_new(BUF_SIZE);
		if (!priv->string) {
			return NULL;
		}
	} else {
		g_string_set_size(priv->string, 0);
	}
	for (Log4gPatternConverter *c = priv->head; c != NULL;
			c = log4g_pattern_converter_get_next(c)) {
		log4g_pattern_converter_format(c, priv->string, event);
	}
	return priv->string->str;
}

static Log4gPatternParser *
create_pattern_parser(Log4gLayout *base, const gchar *pattern)
{
	return log4g_pattern_parser_new(pattern);
}

static void
log4g_pattern_layout_class_init(Log4gPatternLayoutClass *klass)
{
	/* initialize GObject class */
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	object_class->set_property = set_property;
	/* initialize private data */
	g_type_class_add_private(klass, sizeof(struct Private));
	/* initialize Log4gLayout class */
	Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
	layout_class->format = format;
	/* initialize Log4gPatternLayout class */
	klass->create_pattern_parser = create_pattern_parser;
	/* install properties */
	g_object_class_install_property(object_class, PROP_CONVERSION_PATTERN,
		g_param_spec_string("conversion-pattern",
			Q_("Conversion Pattern"),
			Q_("String that controls formatting"),
			NULL, G_PARAM_WRITABLE));
}

static void
log4g_pattern_layout_class_finalize(Log4gPatternLayoutClass *klass)
{
	/* do nothing */
}

void
log4g_pattern_layout_register(GTypeModule *module)
{
	log4g_pattern_layout_register_type(module);
}

/**
 * log4g_pattern_layout_create_pattern_parser:
 * @base: A conversion pattern object.
 * @pattern: The conversion pattern to use.
 *
 * Call the @create_pattern_parser function from the #Log4gPatternLayoutClass
 * of @base.
 *
 * Returns: A new pattern parser object.
 * Since: 0.1
 */
Log4gPatternParser *
log4g_pattern_layout_create_pattern_parser(Log4gLayout *base,
		const gchar *pattern)
{
	g_return_val_if_fail(LOG4G_IS_PATTERN_LAYOUT(base), NULL);
	Log4gPatternLayoutClass *klass =
		LOG4G_PATTERN_LAYOUT_GET_CLASS(base);
	return klass->create_pattern_parser(base, pattern);
}
