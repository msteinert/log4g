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
 * \brief Implements the API in log4g/appender/rolling_file-appender.h
 * \author Mike Steinert
 * \date 2-17-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "log4g/appender/rolling-file-appender.h"
#include "log4g/helpers/counting-quiet-writer.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

enum _properties_t {
    PROP_O = 0,
    PROP_MAX_BACKUP_INDEX,
    PROP_MAXIMUM_FILE_SIZE,
    PROP_MAX
};

G_DEFINE_TYPE(Log4gRollingFileAppender, log4g_rolling_file_appender,
        LOG4G_TYPE_FILE_APPENDER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_ROLLING_FILE_APPENDER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    guint backup; /**< The number of backup indexes to keep */
    gulong max; /**< The maximum file size (default is 10MB) */
    gulong next;
};

static void
log4g_rolling_file_appender_init(Log4gRollingFileAppender *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->backup = 1;
    priv->max = 10 * 1024 * 1024; /* 10MB */
    priv->next = 0;
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_MAX_BACKUP_INDEX:
        priv->backup = g_value_get_uint(value);
        break;
    case PROP_MAXIMUM_FILE_SIZE:
        priv->max = g_value_get_ulong(value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static void
sub_append(Log4gAppender *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    Log4gQuietWriter *writer = log4g_writer_appender_get_quiet_writer(base);
    LOG4G_WRITER_APPENDER_CLASS(log4g_rolling_file_appender_parent_class)->
            sub_append(base, event);
    if (log4g_file_appender_get_file(base) && writer) {
        gulong size = log4g_counting_quiet_writer_get_count(writer);
        if ((size >= priv->max) && (size >= priv->next)) {
            log4g_rolling_file_appender_roll_over(base);
        }
    }
}

static void
set_file_full(Log4gAppender *base, const gchar *file, gboolean append,
        gboolean buffered, guint size)
{
    LOG4G_FILE_APPENDER_CLASS(log4g_rolling_file_appender_parent_class)->
            set_file_full(base, file, append, buffered, size);
    if (append) {
        struct stat buf;
        Log4gQuietWriter *qw = log4g_writer_appender_get_quiet_writer(base);
        if (stat(file, &buf)) {
            if (ENOENT != errno) {
                log4g_log_error("stat(): %s", g_strerror(errno));
                return;
            }
        }
        log4g_counting_quiet_writer_set_count(qw, buf.st_size);
    }
}

static void
set_qw_for_files(Log4gAppender *base, FILE *file)
{
    GObject *error = log4g_appender_get_error_handler(base);
    Log4gQuietWriter *writer = log4g_counting_quiet_writer_new(file, error);
    if (writer) {
        log4g_writer_appender_set_quiet_writer(base, writer);
        g_object_unref(writer);
    }
}

static void
roll_over(Log4gAppender *base)
{
    const gchar *file;
    Log4gQuietWriter *writer;
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    writer = log4g_writer_appender_get_quiet_writer(base);
    if (writer) {
        gulong size = log4g_counting_quiet_writer_get_count(writer);
        priv->next = size + priv->max;
    }
    if (priv->max > 0) {
        guint i;
        GString *source;
        GString *target;
        source = g_string_sized_new(128);
        if (G_UNLIKELY(!source)) {
            return;
        }
        target = g_string_sized_new(128);
        if (G_UNLIKELY(!target)) {
            g_string_free(source, TRUE);
            return;
        }
        file = log4g_file_appender_get_file(base);
        for (i = priv->backup - 1; i >= 1; --i) {
            g_string_printf(source, "%s.%u", file, i);
            g_string_printf(target, "%s.%u", file, i + 1);
            rename(source->str, target->str);
        }
        g_string_free(source, TRUE);
        g_string_printf(target, "%s.%u", file, 1);
        if (rename(file, target->str) ? FALSE : TRUE) {
            g_string_printf(target, "%s", file);
            log4g_file_appender_set_file_full(base, target->str, TRUE,
                    log4g_file_appender_get_buffered_io(base),
                    log4g_file_appender_get_buffer_size(base));
        }
        g_string_free(target, TRUE);
    }
    priv->next = 0;
}

static void
log4g_rolling_file_appender_class_init(Log4gRollingFileAppenderClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gWriterAppenderClass *writer_class = LOG4G_WRITER_APPENDER_CLASS(klass);
    Log4gFileAppenderClass *file_class = LOG4G_FILE_APPENDER_CLASS(klass);
    /* initialize GObject class */
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gWriterAppender class */
    writer_class->sub_append = sub_append;
    /* initialize Log4gaFileAppender class */
    file_class->set_file_full = set_file_full;
    file_class->set_qw_for_files = set_qw_for_files;
    /* initialize Log4gRollingFileAppender class */
    klass->roll_over = roll_over;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_MAX_BACKUP_INDEX,
            g_param_spec_uint("max-backup-index", Q_("Maximum Backup Index"),
                    Q_("Maximum number of backup files"),
                    0, G_MAXUINT, 1, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_MAXIMUM_FILE_SIZE,
            g_param_spec_ulong("maximum-file-size", Q_("Maximum File Size"),
                    Q_("Maximum size a log file may grow to"),
                    0, G_MAXLONG, 10 * 1024 * 1024, G_PARAM_WRITABLE));
}

Log4gAppender *
log4g_rolling_file_appender_new(Log4gLayout *layout, const gchar *file,
        gboolean append, gboolean buffered)
{
    Log4gAppender *self;
    g_return_val_if_fail(layout, NULL);
    g_return_val_if_fail(file, NULL);
    self = g_object_new(LOG4G_TYPE_ROLLING_FILE_APPENDER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_appender_set_layout(self, layout);
    log4g_file_appender_set_file_full(self, file, append, buffered,
            log4g_file_appender_get_buffer_size(self));
    return self;
}

guint
log4g_rolling_file_appender_get_max_backup_index(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base), 0);
    return GET_PRIVATE(base)->backup;
}

void
log4g_rolling_file_appender_set_max_backup_index(Log4gAppender *base,
        guint backup)
{
    g_return_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base));
    g_object_set(base, "max-backup-index", backup, NULL);
}

gulong
log4g_rolling_file_appender_get_maximum_file_size(Log4gAppender *base)
{
    g_return_val_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base), 0);
    return GET_PRIVATE(base)->max;
}

void
log4g_rolling_file_appender_set_maximum_file_size(Log4gAppender *base,
        gulong max)
{
    g_return_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base));
    g_object_set(base, "maximum-file-size", max, NULL);
}

void
log4g_rolling_file_appender_roll_over(Log4gAppender *base)
{
    g_return_if_fail(LOG4G_IS_ROLLING_FILE_APPENDER(base));
    LOG4G_ROLLING_FILE_APPENDER_GET_CLASS(base)->roll_over(base);
}
