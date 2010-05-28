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
 * \brief Implements the API in file-appender.h
 * \author Mike Steinert
 * \date 2-9-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "file-appender.h"

enum _properties_t {
    PROP_O = 0,
    PROP_FILE,
    PROP_APPEND,
    PROP_BUFFERED_IO,
    PROP_BUFFER_SIZE,
    PROP_MAX
};

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_FILE_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean append;
    gchar *file;
    gboolean buffered;
    guint size;
    GMutex *lock;
};

G_DEFINE_DYNAMIC_TYPE(Log4gFileAppender, log4g_file_appender,
        LOG4G_TYPE_WRITER_APPENDER)

static void
log4g_file_appender_init(Log4gFileAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->append = TRUE;
    priv->file = NULL;
    priv->buffered = FALSE;
    priv->size = 8 * 1024;
    if (g_thread_supported()) {
        priv->lock = g_mutex_new();
    } else {
        priv->lock = NULL;
    }
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_appender_close(LOG4G_APPENDER(base));
    g_free(priv->file);
    priv->file = NULL;
    if (priv->lock) {
        g_mutex_free(priv->lock);
        priv->lock = NULL;
    }
    G_OBJECT_CLASS(log4g_file_appender_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    gboolean buffered;
    switch (id) {
    case PROP_FILE:
        g_mutex_lock(priv->lock);
        g_free(priv->file);
        const gchar *file = g_value_get_string(value);
        if (file) {
            priv->file = g_strdup(file);
            if (priv->file) {
                g_strstrip(priv->file);
            }
        } else {
            priv->file = NULL;
        }
        g_mutex_unlock(priv->lock);
        break;
    case PROP_APPEND:
        priv->append = g_value_get_boolean(value);
        break;
    case PROP_BUFFERED_IO:
        buffered = g_value_get_boolean(value);
        g_atomic_int_set(&priv->buffered, buffered);
        if (buffered) {
            g_object_set(base, "immediate-flush", FALSE, NULL);
        }
        break;
    case PROP_BUFFER_SIZE:
        priv->size = g_value_get_uint(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
set_file_full(Log4gAppender *base, const gchar *file, gboolean append,
        gboolean buffered, guint size)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->buffered) {
        g_object_set(base, "immediate-flush", FALSE, NULL);
    }
    log4g_writer_appender_reset(base);
    g_object_set(base, "file", file, NULL);
    g_mutex_lock(priv->lock);
    priv->append = append;
    priv->buffered = buffered;
    priv->size = size;
    g_mutex_unlock(priv->lock);
    log4g_appender_activate_options(base);
}

static void
set_qw_for_files(Log4gAppender *base, FILE *file)
{
    GObject *error = log4g_appender_get_error_handler(base);
    Log4gQuietWriter *writer = log4g_quiet_writer_new(file, error);
    if (writer) {
        log4g_writer_appender_set_quiet_writer(base, writer);
        g_object_unref(writer);
    }
}

static void
reset(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_file_appender_close_file(base);
    g_free(priv->file);
    priv->file = NULL;
    LOG4G_WRITER_APPENDER_CLASS(log4g_file_appender_parent_class)->reset(base);
}

static void
activate_options(Log4gAppender *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_mutex_lock(priv->lock);
    if (G_UNLIKELY(!priv->file)) {
        log4g_log_warn(Q_("file option not set for appender [%s]"),
                log4g_appender_get_name(LOG4G_APPENDER(base)));
        log4g_log_warn(Q_("are you using FileAppender instead "
                "of ConsoleAppender?"));
        goto exit;
    }
    FILE *ostream = fopen(priv->file, (priv->append ? "a" : "w"));
    if (!ostream) {
        log4g_log_error("%s: %s", priv->file, g_strerror(errno));
        goto exit;
    }
    if (priv->buffered) {
        if (setvbuf(ostream, NULL, _IOFBF, priv->size)) {
            log4g_log_error("%s: %s", priv->file, g_strerror(errno));
        }
    } else {
        if (setvbuf(ostream, NULL, _IONBF, 0)) {
            log4g_log_error("%s: %s", priv->file, g_strerror(errno));
        }
    }
    log4g_file_appender_set_qw_for_files(LOG4G_APPENDER(base), ostream);
    log4g_writer_appender_write_header(LOG4G_APPENDER(base));
exit:
    g_mutex_unlock(priv->lock);
}

static void
log4g_file_appender_class_init(Log4gFileAppenderClass *klass)
{
    /* initialize GObjectClass */
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gAppenderClass */
    Log4gAppenderClass *appender_class = LOG4G_APPENDER_CLASS(klass);
    appender_class->activate_options = activate_options;
    /* initialize Log4gWriterAppenderClass */
    Log4gWriterAppenderClass *writer_class =
        LOG4G_WRITER_APPENDER_CLASS(klass);
    writer_class->reset = reset;
    /* initialize Log4gFileAppenderClass */
    klass->set_file_full = set_file_full;
    klass->set_qw_for_files = set_qw_for_files;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_FILE,
            g_param_spec_string("file", Q_("File"),
                    Q_("Output file name"), NULL, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_APPEND,
            g_param_spec_boolean("append", Q_("Append"),
                    Q_("Append or overwrite file"), TRUE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_BUFFERED_IO,
            g_param_spec_boolean("buffered-io", Q_("Buffered I/O"),
                    Q_("Buffer log output"), FALSE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_BUFFER_SIZE,
            g_param_spec_uint("buffer-size", Q_("Buffer Size"),
                    Q_("Size of the output buffer"),
                    0, G_MAXUINT, 8 * 1024, G_PARAM_WRITABLE));
}

static void
log4g_file_appender_class_finalize(Log4gFileAppenderClass *klass)
{
    /* do nothing */
}

void
log4g_file_appender_register(GTypeModule *module)
{
    log4g_quiet_writer_register(module);
    log4g_file_appender_register_type(module);
}

void
log4g_file_appender_set_file_full(Log4gAppender *base, const gchar *file,
        gboolean append, gboolean buffered, guint size)
{
    g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
    LOG4G_FILE_APPENDER_GET_CLASS(base)->
        set_file_full(base, file, append, buffered, size);
}

void
log4g_file_appender_set_qw_for_files(Log4gAppender *base, FILE *file)
{
    g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
    LOG4G_FILE_APPENDER_GET_CLASS(base)->set_qw_for_files(base, file);
}

void
log4g_file_appender_close_file(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_FILE_APPENDER(base));
    Log4gQuietWriter *writer = log4g_writer_appender_get_quiet_writer(base);
    if (writer) {
        log4g_quiet_writer_close(writer);
    }
}

const gchar *
log4g_file_appender_get_file(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), NULL);
    return GET_PRIVATE(base)->file;
}

gboolean
log4g_file_appender_get_buffered_io(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), FALSE);
    return GET_PRIVATE(base)->buffered;
}

guint
log4g_file_appender_get_buffer_size(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_FILE_APPENDER(base), FALSE);
    return GET_PRIVATE(base)->size;
}
