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
 * \brief Implements the API in log4g/filter/deny-all-filter.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/filter/deny-all-filter.h"

G_DEFINE_DYNAMIC_TYPE(Log4gDenyAllFilter, log4g_deny_all_filter,
        LOG4G_TYPE_FILTER)

static void
log4g_deny_all_filter_init(Log4gDenyAllFilter *self)
{
    /* do nothing */
}

static Log4gFilterDecision
decide(Log4gFilter *self, Log4gLoggingEvent *event)
{
    return LOG4G_FILTER_DENY;
}

static void
log4g_deny_all_filter_class_init(Log4gDenyAllFilterClass *klass)
{
    Log4gFilterClass *filter_class = LOG4G_FILTER_CLASS(klass);
    /* initialize Log4gFilter class */
    filter_class->decide = decide;
}

static void
log4g_deny_all_filter_class_finalize(Log4gDenyAllFilterClass *klass)
{
    /* do nothing */
}

void
log4g_deny_all_filter_register(GTypeModule *module)
{
    log4g_deny_all_filter_register_type(module);
}

Log4gFilter *log4g_deny_all_filter_new(void)
{
    return g_object_new(LOG4G_TYPE_DENY_ALL_FILTER, NULL);
}
