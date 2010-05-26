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
 * \brief Implements the API in log4g/layout/pattern-layout.h
 * \author Mike Steinert
 * \date 2-12-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/layout/pattern-layout.h"

enum _properties_t {
    PROP_O = 0,
    PROP_CONVERSION_PATTERN,
    PROP_MAX
};

static void
activate_options(Log4gOptionHandler *base)
{
    /* do nothing */
}

static void
option_handler_init(Log4gOptionHandlerInterface *interface)
{
    interface->activate_options = activate_options;
}

G_DEFINE_DYNAMIC_TYPE_EXTENDED(Log4gPatternLayout, log4g_pattern_layout,
        LOG4G_TYPE_LAYOUT, 0,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_OPTION_HANDLER, option_handler_init))

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_PATTERN_LAYOUT, \
            struct Log4gPrivate))

/** \brief The default size of a string buffer */
#define BUF_SIZE (256)

/** \brief The maximum size of a string buffer */
#define MAX_CAPACITY (1024)

struct Log4gPrivate {
    gchar *pattern;
    GString *string;
    Log4gPatternConverter *head;
};

static void
log4g_pattern_layout_init(Log4gPatternLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->pattern = NULL;
    priv->string = NULL;
    priv->head = NULL;
}

static void
dispose(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->head) {
        g_object_unref(priv->head);
        priv->head = NULL;
    }
    G_OBJECT_CLASS(log4g_pattern_layout_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    g_free(priv->pattern);
    priv->pattern = NULL;
    if (priv->string) {
        g_string_free(priv->string, TRUE);
        priv->string = NULL;
    }
    G_OBJECT_CLASS(log4g_pattern_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    const gchar *pattern;
    switch (id) {
    case PROP_CONVERSION_PATTERN:
        pattern = g_value_get_string(value);
        g_free(priv->pattern);
        if (priv->head) {
            g_object_unref(priv->head);
            priv->head = NULL;
        }
        priv->pattern = g_strdup((pattern ? pattern : "%m%n"));
        if (!priv->pattern) {
            break;
        }
        Log4gPatternParser *parser =
            log4g_pattern_layout_create_pattern_parser(LOG4G_LAYOUT(base),
                    priv->pattern);
        if (!parser) {
            g_free(priv->pattern);
            priv->pattern = NULL;
            break;
        }
        priv->head = log4g_pattern_parser_parse(parser);
        g_object_unref(parser);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(base, id, pspec);
        break;
    }
}

static gchar *
format(Log4gLayout *base, Log4gLoggingEvent *event)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->string) {
        if (priv->string->allocated_len > MAX_CAPACITY) {
            g_string_free(priv->string, TRUE);
            priv->string = NULL;
        }
    }
    if (!priv->string) {
        priv->string = g_string_sized_new(BUF_SIZE);
        if (!priv->string) {
            return NULL;
        }
    } else {
        g_string_set_size(priv->string, 0);
    }
    for (Log4gPatternConverter *c = priv->head; c != NULL;
            c = log4g_pattern_converter_get_next(c)) {
        log4g_pattern_converter_format(c, priv->string, event);
    }
    return priv->string->str;
}

static Log4gPatternParser *
create_pattern_parser(Log4gLayout *base, const gchar *pattern)
{
    return log4g_pattern_parser_new(pattern);
}

static void
log4g_pattern_layout_class_init(Log4gPatternLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->dispose = dispose;
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayout class */
    layout_class->format = format;
    /* initialize Log4gPatternLayout class */
    klass->create_pattern_parser = create_pattern_parser;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_CONVERSION_PATTERN,
            g_param_spec_string("conversion-pattern", Q_("Conversion Pattern"),
                    Q_("String that controls formatting"),
                    NULL, G_PARAM_WRITABLE));
}

static void
log4g_pattern_layout_class_finalize(Log4gPatternLayoutClass *klass)
{
    /* do nothing */
}

void
log4g_pattern_layout_register(GTypeModule *module)
{
    log4g_pattern_layout_register_type(module);
}

Log4gLayout *
log4g_pattern_layout_new(const gchar *pattern)
{
    Log4gLayout *self = g_object_new(LOG4G_TYPE_PATTERN_LAYOUT, NULL);
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    log4g_pattern_layout_set_conversion_pattern(self, pattern);
    if (!priv->head) {
        g_object_unref(self);
        return NULL;
    }
    return self;
}

void
log4g_pattern_layout_set_conversion_pattern(Log4gLayout *base,
        const gchar *pattern)
{
    g_return_if_fail(LOG4G_IS_PATTERN_LAYOUT(base));
    g_object_set(base, "conversion-pattern", pattern, NULL);
}

const gchar *
log4g_pattern_layout_get_conversion_pattern(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_PATTERN_LAYOUT(base), NULL);
    return GET_PRIVATE(base)->pattern;
}

Log4gPatternParser *
log4g_pattern_layout_create_pattern_parser(Log4gLayout *base,
        const gchar *pattern)
{
    g_return_val_if_fail(LOG4G_IS_PATTERN_LAYOUT(base), NULL);
    Log4gPatternLayoutClass *klass =
        LOG4G_PATTERN_LAYOUT_GET_CLASS(base);
    return klass->create_pattern_parser(base, pattern);
}
