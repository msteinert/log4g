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
 * \brief Implements the API in log4g/appender/null-appender.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "null-appender.h"

G_DEFINE_DYNAMIC_TYPE(Log4gNullAppender, log4g_null_appender,
        LOG4G_TYPE_APPENDER)

static void
log4g_null_appender_init(Log4gNullAppender *self)
{
    /* do nothing */
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    /* do nothing */
}

static void
do_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    /* do nothing */
}

static void
_close(Log4gAppender *base)
{
    /* do nothing */
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return FALSE;
}

static void
log4g_null_appender_class_init(Log4gNullAppenderClass *klass)
{
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->append = append;
    appender_class->do_append = do_append;
    appender_class->requires_layout = requires_layout;
    appender_class->close = _close;
}

static void
log4g_null_appender_class_finalize(Log4gNullAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_null_appender_register(GTypeModule *module)
{
    log4g_null_appender_register_type(module);
}
