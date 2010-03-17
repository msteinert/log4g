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
 * \brief Implements the API in log4g/layout/ttcc-layout.h
 * \author Mike Steinert
 * \date 2-9-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/layout/ttcc-layout.h"

enum _properties_t {
    PROP_O = 0,
    PROP_THREAD_PRINTING,
    PROP_CATEGORY_PREFIXING,
    PROP_CONTEXT_PRINTING,
    PROP_MAX
};

G_DEFINE_TYPE(Log4gTTCCLayout, log4g_ttcc_layout, LOG4G_TYPE_DATE_LAYOUT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_TTCC_LAYOUT, \
            struct Log4gPrivate))

struct Log4gPrivate {
    gboolean thread;
    gboolean category;
    gboolean context;
    GString *string;
};

static void
log4g_ttcc_layout_init(Log4gTTCCLayout *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->thread = TRUE;
    priv->category = TRUE;
    priv->context = TRUE;
    priv->string = g_string_sized_new(256);
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->string) {
        g_string_free(priv->string, TRUE);
        priv->string = NULL;
    }
    G_OBJECT_CLASS(log4g_ttcc_layout_parent_class)->finalize(base);
}

static void
set_property(GObject *base, guint id, const GValue *value, GParamSpec *pspec)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    switch (id) {
    case PROP_THREAD_PRINTING:
        priv->thread = g_value_get_boolean(value);
        break;
    case PROP_CATEGORY_PREFIXING:
        priv->category = g_value_get_boolean(value);
        break;
    case PROP_CONTEXT_PRINTING:
        priv->context = g_value_get_boolean(value);
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
    g_string_set_size(priv->string, 0);
    log4g_date_layout_date_format(base, priv->string, event);
    g_string_append_c(priv->string, ' ');
    if (priv->thread) {
        g_string_append_c(priv->string, '[');
        g_string_append_printf(priv->string, "%s",
                log4g_logging_event_get_thread_name(event));
        g_string_append(priv->string, "] ");
    }
    g_string_append(priv->string,
            log4g_level_to_string(log4g_logging_event_get_level(event)));
    g_string_append_c(priv->string, ' ');
    if (priv->category) {
        g_string_append(priv->string,
                log4g_logging_event_get_logger_name(event));
        g_string_append_c(priv->string, ' ');
    }
    if (priv->context) {
        const gchar *ndc = log4g_logging_event_get_ndc(event);
        if (ndc) {
            g_string_append(priv->string, ndc);
            g_string_append_c(priv->string, ' ');
        }
    }
    g_string_append(priv->string, "- ");
    g_string_append(priv->string,
            log4g_logging_event_get_rendered_message(event));
    g_string_append(priv->string,
            LOG4G_LAYOUT_CLASS(log4g_ttcc_layout_parent_class)->LINE_SEP);
    return priv->string->str;
}

static void
log4g_ttcc_layout_class_init(Log4gTTCCLayoutClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    Log4gLayoutClass *layout_class = LOG4G_LAYOUT_CLASS(klass);
    /* initialize GObject class */
    gobject_class->finalize = finalize;
    gobject_class->set_property = set_property;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
    /* initialize Log4gLayout class */
    layout_class->format = format;
    /* install properties */
    g_object_class_install_property(gobject_class, PROP_THREAD_PRINTING,
            g_param_spec_boolean("thread-printing", Q_("Thread Printing"),
                    Q_("Toggle thread printing"), TRUE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_CATEGORY_PREFIXING,
            g_param_spec_boolean("category-prefixing",
                    Q_("Category Prefixing"), Q_("Toggle category prefixing"),
                    TRUE, G_PARAM_WRITABLE));
    g_object_class_install_property(gobject_class, PROP_CONTEXT_PRINTING,
            g_param_spec_boolean("context-printing", Q_("Context Printing"),
                    Q_("Toggle context printing"), TRUE, G_PARAM_WRITABLE));
}

Log4gLayout *
log4g_ttcc_layout_new(const char *format)
{
    Log4gLayout *self = g_object_new(LOG4G_TYPE_TTCC_LAYOUT, NULL);
    if (!self) {
        return NULL;
    }
    log4g_date_layout_set_date_format(self, format);
    return self;
}

void
log4g_ttcc_set_thread_printing(Log4gLayout *base, gboolean thread)
{
    g_return_if_fail(LOG4G_IS_TTCC_LAYOUT(base));
    g_object_set(base, "thread-printing", thread, NULL);
}

gboolean
log4g_ttcc_get_thread_printing(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_TTCC_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->thread;
}

void
log4g_ttcc_set_category_prefixing(Log4gLayout *base, gboolean category)
{
    g_return_if_fail(LOG4G_IS_TTCC_LAYOUT(base));
    g_object_set(base, "category-prefixing", category, NULL);
}

gboolean
log4g_ttcc_get_category_prefixing(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_TTCC_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->category;
}

void
log4g_ttcc_set_context_printing(Log4gLayout *base, gboolean context)
{
    g_return_if_fail(LOG4G_IS_TTCC_LAYOUT(base));
    g_object_set(base, "context-printing", context, NULL);
}

gboolean
log4g_ttcc_get_context_printing(Log4gLayout *base)
{
    g_return_val_if_fail(LOG4G_IS_TTCC_LAYOUT(base), FALSE);
    return GET_PRIVATE(base)->context;
}
