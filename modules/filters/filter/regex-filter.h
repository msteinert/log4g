/* Copyright 2013 Michael Steinert
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

#ifndef LOG4G_REGEX_FILTER_H
#define LOG4G_REGEX_FILTER_H

#include <log4g/filter.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_REGEX_FILTER \
	(log4g_regex_filter_get_type())

#define LOG4G_REGEX_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_REGEX_FILTER, \
		Log4gRegexFilter))

#define LOG4G_IS_REGEX_FILTER(instance) \
	(G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_REGEX_FILTER))

#define LOG4G_REGEX_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_REGEX_FILTER, \
		Log4gRegexFilterClass))

#define LOG4G_IS_REGEX_FILTER_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_REGEX_FILTER))

#define LOG4G_REGEX_FILTER_GET_CLASS(instance) \
	(G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_REGEX_FILTER, \
		Log4gRegexFilterClass))

typedef struct Log4gRegexFilter_ Log4gRegexFilter;

typedef struct Log4gRegexFilterClass_ Log4gRegexFilterClass;

/**
 * Log4gRegexFilter:
 *
 * The <structname>Log4gRegexFilter</structname> structure does not have any
 * public members.
 */
struct Log4gRegexFilter_ {
	/*< private >*/
	Log4gFilter parent_instance;
	struct Private *priv;
};

/**
 * Log4gRegexFilterClass:
 *
 * The <structname>Log4gRegexFilterClass</structname> structure does not have
 * any public members.
 */
struct Log4gRegexFilterClass_ {
	/*< private >*/
	Log4gFilterClass parent_class;
};

G_GNUC_INTERNAL GType
log4g_regex_filter_get_type(void);

G_GNUC_INTERNAL void
log4g_regex_filter_register(GTypeModule *module);

G_END_DECLS

#endif /* LOG4G_REGEX_FILTER_H */
