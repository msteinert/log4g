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
 * \brief Implements the API in log4g/helpers/quiet-writer.h
 * \author Mike Steinert
 * \date 1-29-2010
 */

#include "config.h"
#include <errno.h>
#include "log4g/helpers/quiet-writer.h"
#include "log4g/interface/error-handler.h"
#include <string.h>

G_DEFINE_TYPE(Log4gQuietWriter, log4g_quiet_writer, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_QUIET_WRITER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    FILE *file;
    gpointer error;
};

static void
log4g_quiet_writer_init(Log4gQuietWriter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->file = NULL;
    priv->error = NULL;
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    log4g_quiet_writer_close(LOG4G_QUIET_WRITER(base));
    if (priv->error) {
        g_object_unref(priv->error);
        priv->error = NULL;
    }
    G_OBJECT_CLASS(log4g_quiet_writer_parent_class)->finalize(base);
}

void
_write(Log4gQuietWriter *self, const char *string)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->file) {
        return;
    }
    if (EOF == fputs(string, priv->file)) {
        log4g_error_handler_error(priv->error, NULL,
                Q_("filed to write [%s]: %s"), string, g_strerror(errno));
    }
}

static void
log4g_quiet_writer_class_init(Log4gQuietWriterClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize QuietWriter class */
    klass->write = _write;
}

Log4gQuietWriter *
log4g_quiet_writer_new(FILE *file, gpointer error)
{
    Log4gQuietWriter *self;
    g_return_val_if_fail(file, NULL);
    g_return_val_if_fail(error, NULL);
    g_return_val_if_fail(LOG4G_IS_ERROR_HANDLER(error), NULL);
    self = g_object_new(LOG4G_TYPE_QUIET_WRITER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_quiet_writer_set_error_handler(self, error);
    GET_PRIVATE(self)->file = file;
    return self;
}

void
log4g_quiet_writer_close(Log4gQuietWriter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->file) {
        return;
    }
    if (EOF == fclose(priv->file)) {
        log4g_error_handler_error(priv->error, NULL,
                Q_("failed to close writer: %s"), g_strerror(errno));
    }
    priv->file = NULL;
}

void
log4g_quiet_writer_write(Log4gQuietWriter *self, const char *string)
{
    g_return_if_fail(LOG4G_IS_QUIET_WRITER(self));
    LOG4G_QUIET_WRITER_GET_CLASS(self)->write(self, string);
}

void
log4g_quiet_writer_flush(Log4gQuietWriter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->file) {
        return;
    }
    if (EOF == fflush(priv->file)) {
        log4g_error_handler_error(priv->error, NULL,
                Q_("filed to flush writer: %s"), g_strerror(errno));
    }
}

void
log4g_quiet_writer_set_error_handler(Log4gQuietWriter *self, gpointer error)
{
    if (!error) {
        log4g_warn(Q_("attempted to set NULL error handler"));
    } else {
        struct Log4gPrivate *priv;
        g_return_if_fail(LOG4G_IS_ERROR_HANDLER(error));
        priv = GET_PRIVATE(self);
        if (priv->error) {
            g_object_unref(priv->error);
        }
        g_object_ref(error);
        priv->error = error;
    }
}

void
log4g_quiet_writer_set_file(Log4gQuietWriter *self, FILE *file)
{
    GET_PRIVATE(self)->file = file;
}
