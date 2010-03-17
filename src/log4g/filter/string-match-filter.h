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
 * \brief A filter based on string matching.
 * \author Mike Steinert
 * \date 2-11-2010
 *
 * A simple filter based on string matching.
 *
 * This filter accepts two parameters:
 * -# string-to-match
 * -# accept-on-match
 *
 * If there is a match between the string-to-match value and the log event
 * message then the decide() function returns:
 * - \e ACCEPT if accept-on-match is \e TRUE
 * - \e DENY if accept-on-match is \e FALSE
 *
 * The default value for accept-on-match is \e TRUE.
 *
 * If there is no match \e NEUTRAL is returned.
 */

#ifndef LOG4G_STRING_MATCH_FILTER_H
#define LOG4G_STRING_MATCH_FILTER_H

#include <log4g/filter.h>

G_BEGIN_DECLS

#define LOG4G_TYPE_STRING_MATCH_FILTER \
    (log4g_string_match_filter_get_type())

#define LOG4G_STRING_MATCH_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_CAST((instance), LOG4G_TYPE_STRING_MATCH_FILTER, \
            Log4gStringMatchFilter))

#define LOG4G_IS_STRING_MATCH_FILTER(instance) \
    (G_TYPE_CHECK_INSTANCE_TYPE((instance), LOG4G_TYPE_STRING_MATCH_FILTER))

#define LOG4G_STRING_MATCH_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_CAST((klass), LOG4G_TYPE_STRING_MATCH_FILTER, \
            Log4gStringMatchFilterClass))

#define LOG4G_IS_STRING_MATCH_FILTER_CLASS(klass) \
    (G_TYPE_CHECK_CLASS_TYPE((klass), LOG4G_TYPE_STRING_MATCH_FILTER))

#define LOG4G_STRING_MATCH_FILTER_GET_CLASS(instance) \
    (G_TYPE_INSTANCE_GET_CLASS((instance), LOG4G_TYPE_STRING_MATCH_FILTER, \
            Log4gStringMatchFilterClass))

/** \brief Log4gStringMatchFilter object type definition */
typedef struct _Log4gStringMatchFilter Log4gStringMatchFilter;

/** \brief Log4gStringMatchFilter class type definition */
typedef struct _Log4gStringMatchFilterClass Log4gStringMatchFilterClass;

/** \brief Log4gStringMatchFilterClass definition */
struct _Log4gStringMatchFilter {
    Log4gFilter parent_instance;
};

/** \brief Log4gStringMatchFilterClass definition */
struct _Log4gStringMatchFilterClass {
    Log4gFilterClass parent_class;
};

GType
log4g_string_match_filter_get_type(void);

/**
 * \brief Create a new string match filter.
 *
 * \return A new string match filter object.
 */
Log4gFilter *
log4g_string_match_filter_new(void);

/**
 * \brief Set the string-to-match property.
 *
 * \param base [in] A string match filter object.
 * \param string [in] The new string to match.
 */
void
log4g_string_match_filter_set_string_to_match(Log4gFilter *base,
        const gchar *string);

/**
 * \param base [in] A string match filter object.
 *
 * \return The current string to match for \e base.
 */
const gchar *
log4g_string_match_filter_get_string_to_match(Log4gFilter *base);

/**
 * \brief Set the accept-on-match property.
 *
 * \param base [in] A string match filter object.
 * \param accept [in] The new accept on match value.
 */
void
log4g_string_match_filter_set_accept_on_match(Log4gFilter *base,
        gboolean accept);

/**
 * \brief Retrieve the accept-on-match property.
 *
 * \param base [in] A string match filter object.
 *
 * \return The current accept on match value for \e base.
 */
gboolean
log4g_string_match_filter_get_accept_on_match(Log4gFilter *base);

G_END_DECLS

#endif /* LOG4G_STRING_MATCH_FILTER_H */
