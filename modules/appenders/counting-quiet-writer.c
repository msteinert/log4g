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
 * \brief Implements the API in counting-quiet-writer.h
 * \author Mike Steinert
 * \date 2-17-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "counting-quiet-writer.h"
#include "log4g/interface/error-handler.h"
#include <string.h>

G_DEFINE_DYNAMIC_TYPE(Log4gCountingQuietWriter, log4g_counting_quiet_writer,
              LOG4G_TYPE_QUIET_WRITER)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_COUNTING_QUIET_WRITER, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gulong count;
};

static void
log4g_counting_quiet_writer_init(Log4gCountingQuietWriter *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->count = 0;
}

static void
_write(Log4gQuietWriter *self, const gchar *string)
{
    LOG4G_QUIET_WRITER_CLASS(
            log4g_counting_quiet_writer_parent_class)->write(self, string);
    GET_PRIVATE(self)->count += strlen(string);
}

static void
log4g_counting_quiet_writer_class_init(Log4gCountingQuietWriterClass *klass)
{
    Log4gQuietWriterClass *qw_class = LOG4G_QUIET_WRITER_CLASS(klass);
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gQuietWriter class */
    qw_class->write = _write;
}

static void
log4g_counting_quiet_writer_class_finalize(
        Log4gCountingQuietWriterClass *klass)
{
    /* do nothing */
}

void
log4g_counting_quiet_writer_register(GTypeModule *module)
{
    log4g_counting_quiet_writer_register_type(module);
}

Log4gQuietWriter *
log4g_counting_quiet_writer_new(FILE *file, GObject *error)
{
    Log4gQuietWriter *self;
    g_return_val_if_fail(file, NULL);
    g_return_val_if_fail(error, NULL);
    self = g_object_new(LOG4G_TYPE_COUNTING_QUIET_WRITER, NULL);
    if (!self) {
        return NULL;
    }
    log4g_quiet_writer_set_error_handler(self, error);
    log4g_quiet_writer_set_file(self, file);
    return self;
}

gulong
log4g_counting_quiet_writer_get_count(Log4gQuietWriter *base)
{
    g_return_val_if_fail(LOG4G_IS_COUNTING_QUIET_WRITER(base), 0);
    return GET_PRIVATE(base)->count;
}

void
log4g_counting_quiet_writer_set_count(Log4gQuietWriter *base, gulong count)
{
    g_return_if_fail(LOG4G_IS_COUNTING_QUIET_WRITER(base));
    GET_PRIVATE(base)->count = count;
}
