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
 * \brief Implements the API in log4g/level.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/level.h"
#include <string.h>
#include <syslog.h>

G_DEFINE_TYPE(Log4gLevel, log4g_level, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LEVEL, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gint level;
    gchar *string;
    gint syslog;
};

static void
log4g_level_init(Log4gLevel *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->level = LOG4G_LEVEL_DEBUG_INT;
    priv->string = NULL; /* DEBUG ? */
    priv->syslog = 7;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->string) {
        g_free(priv->string);
    }
    G_OBJECT_CLASS(log4g_level_parent_class)->finalize(base);
}

static void
log4g_level_class_init(Log4gLevelClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* intialize Log4gLevel */
    klass->string_to_level = log4g_level_string_to_level;
    klass->string_to_level_default = log4g_level_string_to_level_default;
    klass->int_to_level = log4g_level_int_to_level;
    klass->int_to_level_default = log4g_level_int_to_level_default;
    /* initialize default log levels */
    klass->ALL = log4g_level_new(LOG4G_LEVEL_ALL_INT, "ALL", LOG_DEBUG);
    klass->TRACE = log4g_level_new(LOG4G_LEVEL_TRACE_INT, "TRACE", LOG_DEBUG);
    klass->DEBUG = log4g_level_new(LOG4G_LEVEL_DEBUG_INT, "DEBUG", LOG_DEBUG);
    klass->INFO = log4g_level_new(LOG4G_LEVEL_INFO_INT, "INFO", LOG_INFO);
    klass->WARN = log4g_level_new(LOG4G_LEVEL_WARN_INT, "WARN", LOG_WARNING);
    klass->ERROR = log4g_level_new(LOG4G_LEVEL_ERROR_INT, "ERROR", LOG_ERR);
    klass->FATAL = log4g_level_new(LOG4G_LEVEL_FATAL_INT, "FATAL", LOG_EMERG);
    klass->OFF = log4g_level_new(LOG4G_LEVEL_OFF_INT, "OFF", LOG_EMERG);
}

Log4gLevel *
log4g_level_new(gint level, const gchar *string, gint syslog)
{
    Log4gLevel *self = g_object_new(LOG4G_TYPE_LEVEL, NULL);
    struct Log4gPrivate *priv;
    if (!self) {
        return NULL;
    }
    priv = GET_PRIVATE(self);
    priv->level = level;
    priv->string = g_strdup(string);
    if (!priv->string) {
        g_object_unref(self);
        return NULL;
    }
    priv->syslog = syslog;
    return self;
}

gboolean
log4g_level_equals(Log4gLevel *self, Log4gLevel *level)
{
    if (GET_PRIVATE(self)->level == GET_PRIVATE(level)->level) {
        return TRUE;
    }
    return FALSE;
}

gint
log4g_level_get_syslog_equivalent(Log4gLevel *self)
{
    return GET_PRIVATE(self)->syslog;
}

gboolean
log4g_level_is_greater_or_equal(Log4gLevel *self, Log4gLevel *level)
{
    if (GET_PRIVATE(self)->level >= GET_PRIVATE(level)->level) {
        return TRUE;
    } else {
        return FALSE;
    }
}

const gchar *
log4g_level_to_string(Log4gLevel *self)
{
    return GET_PRIVATE(self)->string;
}

gint
log4g_level_to_int(Log4gLevel *self)
{
    return GET_PRIVATE(self)->level;
}

Log4gLevel *
log4g_level_string_to_level(const gchar *level)
{
    Log4gLevel *to = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    to = log4g_level_string_to_level_default(level, klass->DEBUG);
    g_type_class_unref(klass);
    return to;
}

Log4gLevel *
log4g_level_string_to_level_default(const gchar *level, Log4gLevel *def)
{
    Log4gLevel *to = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    if (!g_ascii_strcasecmp(level, "ALL")) {
        to = klass->ALL;
    } else if (!g_ascii_strcasecmp(level, "DEBUG")) {
        to = klass->DEBUG;
    } else if (!g_ascii_strcasecmp(level, "INFO")) {
        to = klass->INFO;
    } else if (!g_ascii_strcasecmp(level, "WARN")) {
        to = klass->WARN;
    } else if (!g_ascii_strcasecmp(level, "ERROR")) {
        to = klass->ERROR;
    } else if (!g_ascii_strcasecmp(level, "FATAL")) {
        to = klass->FATAL;
    } else if (!g_ascii_strcasecmp(level, "OFF")) {
        to = klass->OFF;
    } else if (!g_ascii_strcasecmp(level, "TRACE")) {
        to = klass->TRACE;
    } else {
        to = def;
    }
    g_type_class_unref(klass);
    return to;
}

Log4gLevel *
log4g_level_int_to_level(gint level)
{
    Log4gLevel *to = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    to = log4g_level_int_to_level_default(level, klass->DEBUG);
    g_type_class_unref(klass);
    return to;
}

Log4gLevel *
log4g_level_int_to_level_default(gint level, Log4gLevel *def)
{
    Log4gLevel *to = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return def;
    }
    if (level == LOG4G_LEVEL_ALL_INT) {
        to = klass->ALL;
    } else if (level == LOG4G_LEVEL_TRACE_INT) {
        to = klass->TRACE;
    } else if (level == LOG4G_LEVEL_DEBUG_INT) {
        to = klass->DEBUG;
    } else if (level == LOG4G_LEVEL_INFO_INT) {
        to = klass->INFO;
    } else if (level == LOG4G_LEVEL_WARN_INT) {
        to = klass->WARN;
    } else if (level == LOG4G_LEVEL_ERROR_INT) {
        to = klass->ERROR;
    } else if (level == LOG4G_LEVEL_FATAL_INT) {
        to = klass->FATAL;
    } else if (level == LOG4G_LEVEL_OFF_INT) {
        to = klass->OFF;
    } else {
        to = def;
    }
    g_type_class_unref(klass);
    return to;
}

Log4gLevel *
log4g_level_ALL(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->ALL;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_TRACE(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->TRACE;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_DEBUG(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->DEBUG;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_INFO(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->INFO;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_WARN(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->WARN;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_ERROR(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->ERROR;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_FATAL(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->FATAL;
    g_type_class_unref(klass);
    return level;
}

Log4gLevel *
log4g_level_OFF(void)
{
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    if (!klass) {
        return NULL;
    }
    level = klass->OFF;
    g_type_class_unref(klass);
    return level;
}
