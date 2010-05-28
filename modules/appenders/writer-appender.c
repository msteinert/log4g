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
 * \brief Implements the API in writer-appender.h
 * \author Mike Steinert
 * \date 2-8-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/interface/error-handler.h"
#include "writer-appender.h"

enum _properties_t {
    PROP_O = 0,
    PROP_IMMEDIATE_FLUSH,
    PROP_WRITER,
    PROP_MAX
};

G_DEFINE_DYNAMIC_TYPE(Log4gWriterAppender, log4g_writer_appender,
        LOG4G_TYPE_APPENDER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_WRITER_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean flush;
    Log4gQuietWriter *writer;
    GMutex *lock;
};

static void
log4g_writer_appender_init(Log4gWriterAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->flush = TRUE;
    priv->writer = NULL;
    if (g_thread_supported()) {
        priv->lock = g_mutex_new();
    } else {
        priv->lock = NULL;
    }
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->writer) {
        g_object_unref(priv->writer);
        priv->writer = NULL;
    }
    G_OBJECT_CLASS(log4g_writer_appender_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_appender_close(LOG4G_APPENDER(base));
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    G_OBJECT_CLASS(log4g_writer_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_IMMEDIATE_FLUSH:
        priv->flush = g_value_get_boolean(value);
        break;
    case PROP_WRITER:
        g_mutex_lock(priv->lock);
        log4g_writer_appender_reset(LOG4G_APPENDER(base));
        FILE *file = g_value_get_pointer(value);
        GObject *error =
            log4g_appender_get_error_handler(LOG4G_APPENDER(base));
        priv->writer = log4g_quiet_writer_new(file, error);
        if (priv->writer) {
            log4g_writer_appender_write_header(LOG4G_APPENDER(base));
        }
        g_mutex_unlock(priv->lock);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    if (!log4g_writer_appender_check_entry_conditions(base)) {
        return;
    }
    log4g_writer_appender_sub_append(base, event);
}

static void
_close(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!log4g_appender_get_closed(base)) {
        g_mutex_lock(priv->lock);
        log4g_appender_set_closed(base, TRUE);
        log4g_writer_appender_write_footer(base);
        log4g_writer_appender_reset(base);
        g_mutex_unlock(priv->lock);
    }
}

static gboolean
requires_layout(Log4gAppender *self)
{
    return TRUE;
}

void
sub_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    Log4gLayout *layout = log4g_appender_get_layout(base);
    const char *message = log4g_layout_format(layout, event);
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_quiet_writer_write(priv->writer, message);
    if (priv->flush) {
        log4g_quiet_writer_flush(priv->writer);
    }
}

static void
close_writer(Log4gAppender *base)
{
    log4g_quiet_writer_close(GET_PRIVATE(base)->writer);
}

static void
reset(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (!priv->writer) {
        return;
    }
    log4g_writer_appender_close_writer(base);
    if (priv->writer) {
        g_object_unref(priv->writer);
        priv->writer = NULL;
    }
}

static void
log4g_writer_appender_class_init(Log4gWriterAppenderClass *klass)
{
    /* initialize GObjectClass */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->append = append;
    appender_class->close = _close;
    appender_class->requires_layout = requires_layout;
    /* initialize Log4gWriterAppenderClass */
    klass->sub_append = sub_append;
    klass->close_writer = close_writer;
    klass->reset = reset;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_IMMEDIATE_FLUSH,
            g_param_spec_boolean("immediate-flush", Q_("Immediate Flush"),
                    Q_("Flush immediately after writing"),
                    TRUE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_WRITER,
            g_param_spec_pointer("writer", Q_("Writer"),
                    Q_("Set the stdio(3) stream to use"), G_PARAM_WRITABLE));
}

static void
log4g_writer_appender_class_finalize(Log4gWriterAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_writer_appender_register(GTypeModule *module)
{
    log4g_quiet_writer_register(module);
    log4g_writer_appender_register_type(module);
}

gboolean
log4g_writer_appender_check_entry_conditions(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_WRITER_APPENDER(base), FALSE);
    if (log4g_appender_get_closed(base)) {
        log4g_log_warn(Q_("not allowed to write to a closed appender"));
        return FALSE;
    }
    if (!GET_PRIVATE(base)->writer) {
        Log4gErrorHandler *error = (Log4gErrorHandler *)
            log4g_appender_get_error_handler(base);
        if (error) {
            log4g_error_handler_error(error, NULL,
                Q_("no output stream or file set for the appender named [%s]"),
                log4g_appender_get_name(base));
        }
        return FALSE;
    }
    if (!log4g_appender_get_layout(base)) {
        Log4gErrorHandler *error = (Log4gErrorHandler *)
            log4g_appender_get_error_handler(base);
        if (error) {
            log4g_error_handler_error(error, NULL,
                Q_("no layout set for the appender named [%s]"),
                log4g_appender_get_name(base));
        }
        return FALSE;
    }
    return TRUE;
}

void
log4g_writer_appender_close_writer(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    LOG4G_WRITER_APPENDER_GET_CLASS(base)->close_writer(base);
}

void
log4g_writer_appender_set_writer(Log4gAppender *base, FILE *file)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    g_object_set(base, "writer", file, NULL);
}

void
log4g_writer_appender_sub_append(Log4gAppender *base,
        Log4gLoggingEvent *event)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    LOG4G_WRITER_APPENDER_GET_CLASS(base)->sub_append(base, event);
}

void
log4g_writer_appender_reset(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    LOG4G_WRITER_APPENDER_GET_CLASS(base)->reset(base);
}

void
log4g_writer_appender_write_footer(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    Log4gLayout *layout = log4g_appender_get_layout(base);
    if (layout) {
        struct Log4gPrivate *priv = GET_PRIVATE(base);
        const gchar *footer = log4g_layout_get_footer(layout);
        if (footer && priv->writer) {
            log4g_quiet_writer_write(priv->writer, footer);
            log4g_quiet_writer_flush(priv->writer);
        }
    }
}

void
log4g_writer_appender_write_header(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    Log4gLayout *layout = log4g_appender_get_layout(base);
    if (layout) {
        struct Log4gPrivate *priv = GET_PRIVATE(base);
        const gchar *header = log4g_layout_get_header(layout);
        if (header && priv->writer) {
            log4g_quiet_writer_write(priv->writer, header);
        }
    }
}

Log4gQuietWriter *
log4g_writer_appender_get_quiet_writer(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_WRITER_APPENDER(base), NULL);
    return GET_PRIVATE(base)->writer;
}

void
log4g_writer_appender_set_quiet_writer(Log4gAppender *base,
        Log4gQuietWriter *writer)
{
    g_return_if_fail(LOG4G_IS_WRITER_APPENDER(base));
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->writer) {
        g_object_unref(priv->writer);
    }
    g_object_ref(writer);
    priv->writer = writer;
}
