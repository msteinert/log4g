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
 * \brief Implements the API in log4g/ndc.h
 * \author Mike Steinert
 * \date 2-3-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/thread.h"
#include "log4g/ndc.h"

G_DEFINE_TYPE(Log4gNDC, log4g_ndc, G_TYPE_OBJECT)

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_NDC, \
            struct Log4gPrivate))

struct Log4gPrivate {
    GArray *stack; /**< The NDC stack */
    gchar *pop; /**< The most recently popped complete context */
};

/** \brief A diagnostic context node. */
typedef struct _Log4gDiagnosticContext {
    gchar *message; /**< The current diagnostic context */
    gchar *full; /**< The complete diagnostic context */
} Log4gDiagnosticContext;

/**
 * \brief Free dynamic resources used by a diagnostic context object.
 *
 * \param self [in] A diagnostic context object.
 */
static void
log4g_diagnostic_context_destroy(Log4gDiagnosticContext *self)
{
    if (self) {
        g_free(self->message);
        g_free(self->full);
        g_free(self);
    }
}

/**
 * \brief Create a new diagnostic context object.
 *
 * \param parent [in] The parent diagnostic context.
 * \param message [in] The current diagnostic context message.
 * \param ap [in] Format parameters for \e message.
 *
 * \return A new diagnostic context object.
 */
static Log4gDiagnosticContext *
log4g_diagnostic_context_new(Log4gDiagnosticContext *parent,
        const gchar *message, va_list ap)
{
    Log4gDiagnosticContext *self = g_try_malloc(sizeof(*self));
    if (!self) {
        return NULL;
    }
    self->message = g_strdup_vprintf(message, ap);
    if (!self->message) {
        goto error;
    }
    if (parent) {
        self->full = g_strjoin(" ", parent->full, self->message, NULL);
        if (!self->full) {
            goto error;
        }
    } else {
        self->full = g_strdup(self->message);
        if (!self->full) {
            goto error;
        }
    }
    return self;
error:
    log4g_diagnostic_context_destroy(self);
    return NULL;
}

/**
 * \brief Clone a diagnostic context object.
 *
 * \param self [in] The object to clone.
 *
 * \return An exact copy of \e self.
 */
static Log4gDiagnosticContext *
log4g_diagnostic_context_clone(const Log4gDiagnosticContext *self)
{
    Log4gDiagnosticContext *clone = g_try_malloc(sizeof(*clone));
    if (!clone) {
        return NULL;
    }
    if (self->message) {
        clone->message = g_strdup(self->message);
        if (!clone->message) {
            goto error;
        }
    }
    if (self->full) {
        clone->full = g_strdup(self->full);
        if (!clone->full) {
            goto error;
        }
    }
    return clone;
error:
    log4g_diagnostic_context_destroy(clone);
    return NULL;
}

/** \brief Thread specific data */
static GPrivate *priv = NULL;

/**
 * \brief Get the current NDC stack.
 *
 * \return The current NDC stack or NULL if none exists.
 */
static GArray *log4g_ndc_get_current_stack(void);

static void
log4g_ndc_init(Log4gNDC *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    priv->stack = NULL;
    priv->pop = NULL;
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
{
    if (g_thread_supported()) {
        static gsize once = 0;
        if (g_once_init_enter(&once)) {
            priv = g_private_new(g_object_unref);
            if (!priv) {
                return NULL;
            }
            g_once_init_leave(&once, 1);
        }
    }
    GObject *self = g_private_get(priv);
    if (!self) {
        self = G_OBJECT_CLASS(log4g_ndc_parent_class)->
            constructor(type, n, params);
        if (!self) {
            return NULL;
        }
        g_private_set(priv, self);
    } else {
        g_object_ref(self);
    }
    return self;
}

static void
_free_array(GArray *data, gboolean free)
{
    GArray *array = (GArray *)data;
    for (guint i = 0; i < array->len; ++i) {
        Log4gDiagnosticContext *context =
            g_array_index(array, Log4gDiagnosticContext *, i);
        log4g_diagnostic_context_destroy(context);
    }
    if (free) {
        g_array_free(array, TRUE);
    }
}

static void
finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->stack) {
        _free_array(priv->stack, TRUE);
        priv->stack = NULL;
    }
    g_free(priv->pop);
    priv->pop = NULL;
    G_OBJECT_CLASS(log4g_ndc_parent_class)->finalize(base);
}

static GArray *
log4g_ndc_get_current_stack(void)
{
    Log4gNDC *self = log4g_ndc_get_instance();
    if (!self) {
        return NULL;
    }
    return GET_PRIVATE(self)->stack;
}

static void
log4g_ndc_class_init(Log4gNDCClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->constructor = constructor;
    gobject_class->finalize = finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gNDC *
log4g_ndc_get_instance(void)
{
    Log4gNDC *self = g_private_get(priv);
    if (!self) {
        self = g_object_new(LOG4G_TYPE_NDC, NULL);
    }
    return self;
}

void
log4g_ndc_clear(void)
{
    GArray *stack = log4g_ndc_get_current_stack();
    if (stack) {
        _free_array(stack, FALSE);
        g_array_set_size(stack, 0);
    }
}

GArray *
log4g_ndc_clone(void)
{
    GArray *stack = log4g_ndc_get_current_stack();
    if (!stack) {
        return NULL;
    }
    GArray *clone =
        g_array_sized_new(FALSE, TRUE, sizeof(gchar *), stack->len);
    if (!clone) {
        return NULL;
    }
    for (guint i = 0; i < stack->len; ++i) {
        Log4gDiagnosticContext *context =
            g_array_index(stack, Log4gDiagnosticContext *, i);
        if (!context) {
            continue;
        }
        Log4gDiagnosticContext *copy = log4g_diagnostic_context_clone(context);
        if (!copy) {
            goto error;
        }
        g_array_insert_val(clone, i, copy);
    }
    return clone;
error:
    if (clone) {
        _free_array(clone, TRUE);
    }
    return NULL;
}

void
log4g_ndc_inherit(GArray *stack)
{
    Log4gNDC *self = log4g_ndc_get_instance();
    if (!self) {
        return;
    }
    g_return_if_fail(stack);
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (priv->stack) {
        _free_array(priv->stack, TRUE);
    }
    priv->stack = stack;
}

const gchar *
log4g_ndc_get(void)
{
    GArray *stack = log4g_ndc_get_current_stack();
    if (!stack) {
        return NULL;
    }
    if (!stack->len) {
        return NULL;
    }
    return g_array_index(
            stack, Log4gDiagnosticContext *, stack->len - 1)->full;
}

guint
log4g_ndc_size(void)
{
    GArray *stack = log4g_ndc_get_current_stack();
    if (!stack) {
        return 0;
    }
    return stack->len;
}

const gchar *
log4g_ndc_pop(void)
{
    Log4gNDC *self = log4g_ndc_get_instance();
    if (!self) {
        return NULL;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->stack || !priv->stack->len) {
        return NULL;
    }
    Log4gDiagnosticContext *context =
        g_array_index(priv->stack, Log4gDiagnosticContext *,
                priv->stack->len - 1);
    if (context) {
        g_free(priv->pop);
        priv->pop = context->message;
        context->message = NULL;
        log4g_diagnostic_context_destroy(context);
    }
    g_array_remove_index(priv->stack, priv->stack->len - 1);
    return priv->pop;
}

const gchar *
log4g_ndc_peek(void)
{
    GArray *stack = log4g_ndc_get_current_stack();
    Log4gDiagnosticContext *context;
    if (!stack || !stack->len) {
        return NULL;
    }
    context = g_array_index(stack, Log4gDiagnosticContext *, stack->len - 1);
    return context->message;
}

void
log4g_ndc_push(const char *message, ...)
{
    Log4gNDC *self = log4g_ndc_get_instance();
    va_list ap;
    if (!self) {
        return;
    }
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    if (!priv->stack) {
        Log4gDiagnosticContext *context;
        va_start(ap, message);
        context = log4g_diagnostic_context_new(NULL, message, ap);
        va_end(ap);
        if (!context) {
            return;
        }
        priv->stack = g_array_sized_new(FALSE, TRUE, sizeof(gchar *), 1);
        if (!priv->stack) {
            log4g_diagnostic_context_destroy(context);
            return;
        }
        g_array_append_val(priv->stack, context);
    } else if (!priv->stack->len) {
        Log4gDiagnosticContext *context;
        va_start(ap, message);
        context = log4g_diagnostic_context_new(NULL, message, ap);
        va_end(ap);
        if (!context) {
            return;
        }
        g_array_append_val(priv->stack, context);
    } else {
        Log4gDiagnosticContext *context;
        Log4gDiagnosticContext *parent =
            g_array_index(priv->stack, Log4gDiagnosticContext *,
                    priv->stack->len - 1);
        va_start(ap, message);
        context = log4g_diagnostic_context_new(parent, message, ap);
        va_end(ap);
        if (!context) {
            return;
        }
        g_array_append_val(priv->stack, context);
    }
}

void
log4g_ndc_remove(void)
{
    if (g_atomic_pointer_get(&priv)) {
        Log4gNDC *self = g_private_get(priv);
        if (self) {
            g_object_unref(self);
            g_private_set(priv, NULL);
        }
    }
}

void
log4g_ndc_set_max_depth(guint maxdepth)
{
    GArray *stack = log4g_ndc_get_current_stack();
    if (!stack || (stack->len < maxdepth)) {
        return;
    }
    for (guint i = stack->len - 1; i > maxdepth - 1; --i) {
        Log4gDiagnosticContext *context =
            g_array_index(stack, Log4gDiagnosticContext *, i);
        log4g_diagnostic_context_destroy(context);
    }
    g_array_set_size(stack, maxdepth);
}
