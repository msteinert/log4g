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
 * \brief Implements the API in log4g/logging-event.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "log4g/helpers/thread.h"
#include "log4g/logging-event.h"
#include "log4g/mdc.h"
#include "log4g/ndc.h"
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

G_DEFINE_TYPE(Log4gLoggingEvent, log4g_logging_event, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LOGGING_EVENT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gchar *logger;
    Log4gLevel *level;
    gchar *message;
    struct timeval timestamp;
    gboolean thread_lookup_required;
    gchar *thread;
    gboolean ndc_lookup_required;
    gchar *ndc;
    gboolean mdc_lookup_required;
    GHashTable *mdc;
    const gchar *function;
    const gchar *file;
    const gchar *line;
    gchar *fullinfo;
    GArray *keys;
};

static void
log4g_logging_event_init(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->logger = NULL;
    priv->level = NULL;
    priv->message = NULL;
    priv->function = NULL;
    priv->file = NULL;
    priv->line = NULL;
    priv->fullinfo = NULL;
    memset(&priv->timestamp, 0, sizeof(priv->timestamp));
    priv->thread_lookup_required = TRUE;
    priv->thread = NULL;
    priv->ndc_lookup_required = TRUE;
    priv->ndc = NULL;
    priv->mdc_lookup_required = TRUE;
    priv->mdc = NULL;
    priv->keys = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->logger) {
        g_free(priv->logger);
        priv->logger = NULL;
    }
    if (priv->level) {
        g_object_unref(priv->level);
        priv->level = NULL;
    }
    if (priv->message) {
        g_free(priv->message);
        priv->message = NULL;
    }
    if (priv->ndc) {
        g_free(priv->ndc);
        priv->ndc = NULL;
    }
    if (priv->fullinfo) {
        g_free(priv->fullinfo);
        priv->fullinfo = NULL;
    }
    if (priv->thread) {
        g_free(priv->thread);
        priv->thread = NULL;
    }
    if (priv->mdc) {
        g_hash_table_destroy(priv->mdc);
        priv->mdc = NULL;
    }
    if (priv->keys) {
        g_array_free(priv->keys, TRUE);
        priv->keys = NULL;
    }
    G_OBJECT_CLASS(log4g_logging_event_parent_class)->finalize(base);
}

static void
log4g_logging_event_class_init(Log4gLoggingEventClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    struct timeval start;
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize class data */
    if (gettimeofday(&start, NULL)) {
        klass->start = 0;
        log4g_log_error("gettimeofday(): %s", g_strerror(errno));
    } else {
        klass->start = (start.tv_sec * 1000) + (start.tv_usec * 0.001);
    }
}

Log4gLoggingEvent *
log4g_logging_event_new(const gchar *logger, Log4gLevel *level,
        const gchar *function, const gchar *file, const gchar *line,
        const gchar *message, va_list ap)
{
    Log4gLoggingEvent *self = g_object_new(LOG4G_TYPE_LOGGING_EVENT, NULL);
    struct Log4gPrivate *priv;
    if (!self) {
        return NULL;
    }
    priv = GET_PRIVATE(self);
    if (logger) {
        priv->logger = g_strdup(logger);
        if (!priv->logger) {
            goto error;
        }
    }
    if (level) {
        g_object_ref(level);
        priv->level = level;
    }
    if (message) {
        priv->message = g_strdup_vprintf(message, ap);
        if (!priv->message) {
            goto error;
        }
    }
    priv->function = function;
    priv->file = file;
    priv->line = line;
    gettimeofday(&priv->timestamp, NULL);
    return self;
error:
    g_object_unref(self);
    return NULL;
}

Log4gLevel *
log4g_logging_event_get_level(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    return priv->level;
}

const gchar *
log4g_logging_event_get_logger_name(Log4gLoggingEvent *self)
{
    return GET_PRIVATE(self)->logger;
}

const gchar *
log4g_logging_event_get_rendered_message(Log4gLoggingEvent *self)
{
    return GET_PRIVATE(self)->message;
}

const gchar *
log4g_logging_event_get_message(Log4gLoggingEvent *self)
{
    return GET_PRIVATE(self)->message;
}

const gchar *
log4g_logging_event_get_mdc(Log4gLoggingEvent *self, const gchar *key)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->mdc) {
        const gchar *value = g_hash_table_lookup(priv->mdc, key);
        if (value) {
            return value;
        }
    }
    if (priv->mdc_lookup_required) {
        return log4g_mdc_get(key);
    }
    return NULL;
}

struct timeval *
log4g_logging_event_get_time_stamp(Log4gLoggingEvent *self)
{
    return &GET_PRIVATE(self)->timestamp;
}

const gchar *
log4g_logging_event_get_thread_name(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->thread) {
        return priv->thread;
    }
    if (priv->thread_lookup_required) {
        return log4g_thread_get_name();
    }
    return NULL;
}

const gchar *
log4g_logging_event_get_ndc(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->ndc) {
        return priv->ndc;
    }
    if (priv->ndc_lookup_required) {
        return log4g_ndc_get();
    }
    return NULL;
}

/**
 * \brief Callback for g_hash_table_foreach().
 *
 * \param key [in] The hash table key.
 * \param value [in] The hash table value (unused).
 * \param user_data [in] An array to append \e key to.
 */
static void
_get_keys(gpointer key, gpointer value, gpointer user_data)
{
    g_array_append_val((GArray *)user_data, key);
}

/**
 * \brief Construct a key set from an MDC hash table.
 *
 * \param self [in] A logging event object.
 * \param mdc [in] The hash table to get keys from.
 */
static void
_get_property_key_set(Log4gLoggingEvent *self, GHashTable *mdc)
{
    guint size;
    size = g_hash_table_size((GHashTable *)mdc);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!size) {
        return;
    }
    if (priv->keys) {
        g_array_free(priv->keys, TRUE);
    }
    priv->keys = g_array_sized_new(FALSE, FALSE, sizeof(gchar *), size);
    if (!priv->keys) {
        return;
    }
    g_hash_table_foreach(mdc, _get_keys, priv->keys);
}

const GArray *
log4g_logging_event_get_property_key_set(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->mdc) {
        if (!priv->keys) {
            _get_property_key_set(self, priv->mdc);
        }
    } else {
        const GHashTable *mdc = log4g_mdc_get_context();
        if (mdc) {
            _get_property_key_set(self, (GHashTable *)mdc);
        }
    }
    return priv->keys;
}

/**
 * \brief Callback for g_hash_table_foreach().
 *
 * \param key [in] The hash table key.
 * \param value [in] The hash table value (unused).
 * \param user_data [in] A hash table to insert \e key & \e value into.
 */
static void
_mdc_copy(gpointer key, gpointer value, gpointer user_data)
{
    g_hash_table_insert((GHashTable *)user_data, key, value);
}

void
log4g_logging_event_get_thread_copy(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->thread_lookup_required) {
        return;
    }
    priv->thread_lookup_required = FALSE;
    priv->thread = g_strdup(log4g_thread_get_name());
}

void
log4g_logging_event_get_mdc_copy(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->mdc_lookup_required) {
        return;
    }
    const GHashTable *mdc;
    priv->mdc_lookup_required = FALSE;
    mdc = log4g_mdc_get_context();
    if (!mdc) {
        return;
    }
    priv->mdc = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    if (!priv->mdc) {
        return;
    }
    g_hash_table_foreach((GHashTable *)mdc, _mdc_copy, priv->mdc);
}

void
log4g_logging_event_get_ndc_copy(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->ndc_lookup_required) {
        return;
    }
    priv->ndc_lookup_required = FALSE;
    priv->ndc = g_strdup(log4g_ndc_get());
}

glong
log4g_logging_event_get_start_time(void)
{
    Log4gLoggingEventClass *klass =
        g_type_class_peek(LOG4G_TYPE_LOGGING_EVENT);
    return klass->start;
}

const gchar *
log4g_logging_event_get_function_name(Log4gLoggingEvent *self)
{
    const gchar *function = GET_PRIVATE(self)->function;
    return (function ? function : "?");
}

const gchar *
log4g_logging_event_get_file_name(Log4gLoggingEvent *self)
{
    const gchar *file = GET_PRIVATE(self)->file;
    return (file ? file : "?");
}

const gchar *
log4g_logging_event_get_line_number(Log4gLoggingEvent *self)
{
    const gchar *line = GET_PRIVATE(self)->line;
    return (line ? line : "?");
}

const gchar *
log4g_logging_event_get_full_info(Log4gLoggingEvent *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->fullinfo) {
        priv->fullinfo =
            g_strdup_printf("%s(%s:%s)",
                    (priv->function ? priv->function : "?"),
                    (priv->file ? priv->file : "?"),
                    (priv->line ? priv->line : "?"));
    }
    return priv->fullinfo;
}
