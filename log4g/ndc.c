/* Copyright 2010, 2011 Michael Steinert
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
 * SECTION: ndc
 * @short_description: Nested data context
 *
 * The NDC class implements nested data contexts. A nested data context (NDC
 * for short) is an instrument to distinguish interleaved log output from
 * different sources. An example of interleaved log output may occur when
 * a server handles multiple simultaneous connections.
 *
 * Nested diagnostic contexts are handled on a per-thread basis. Calling
 * log4g_ndc_push(), log4g_ndc_pop(), and log4g_ndc_clear() do not affect the
 * NDCs of other threads.
 *
 * Contexts are nested. When enting a context, call log4g_ndc_push(). If there
 * is currently no NDC for the current thread, one is created as a side-effect.
 *
 * When leaving a context call log4g_ndc_pop().
 *
 * To clear the current context call log4g_ndc_clear().
 *
 * <note><para>
 * It is not necessary to call log4g_ndc_clear() when exiting a thread. NDCs
 * are automatically removed when a thread exits.
 * </para></note>
 *
 * #Log4gPatternLayout & #Log4gTTCCLayout may be configured to  automatically
 * retrieve the the nested data context for the current thread without user
 * intervention.
 *
 * Child threads do not automatically inherit the NDC of their parent. To
 * force a thread to inherit a nested data context use log4g_ndc_clone() &
 * log4g_ndc_inherit().
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "log4g/helpers/thread.h"
#include "log4g/ndc.h"

G_DEFINE_TYPE(Log4gNDC, log4g_ndc, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_NDC, struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gNDC *)instance)->priv)

/**
 * Private:
 * @stack: The NDC stack
 * @pop: The most recently popped complete context
 */
struct Private {
	GArray *stack;
	gchar *pop;
};

/**
 * Log4gDiagnosticContext:
 * @message: The current diagnostic context
 * @full: The complete diagnostic context
 */
typedef struct Log4gDiagnosticContext_ {
	gchar *message;
	gchar *full;
} Log4gDiagnosticContext;

/**
 * log4g_diagnostic_context_destroy:
 * @self: A diagnostic context object.
 *
 * Free dynamic resources used by a diagnostic context object.
 */
static void
log4g_diagnostic_context_destroy(Log4gDiagnosticContext *self)
{
	if (self) {
		g_free(self->message);
		g_free(self->full);
		g_slice_free(Log4gDiagnosticContext, self);
	}
}

/**
 * log4g_diagnostic_context_new:
 * @parent: The parent diagnostic context.
 * @message: The current diagnostic context message.
 * @ap: Format parameters for @message.
 *
 * Create a new diagnostic context object.
 *
 * Returns: A new diagnostic context object.
 */
static Log4gDiagnosticContext *
log4g_diagnostic_context_new(Log4gDiagnosticContext *parent,
		const gchar *message, va_list ap)
{
	Log4gDiagnosticContext *self = g_slice_new0(Log4gDiagnosticContext);
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
 * log4g_diagnostic_context_clone:
 * @self: The object to clone.
 *
 * Clone a diagnostic context object.
 *
 * Returns: An exact copy of @self.
 */
static Log4gDiagnosticContext *
log4g_diagnostic_context_clone(const Log4gDiagnosticContext *self)
{
	Log4gDiagnosticContext *clone = g_slice_new0(Log4gDiagnosticContext);
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

/** Thread specific data */
static GPrivate *priv = NULL;

static void
log4g_ndc_init(Log4gNDC *self)
{
	self->priv = ASSIGN_PRIVATE(self);
}

static GObject *
constructor(GType type, guint n, GObjectConstructParam *params)
{
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
free_array(GArray *data, gboolean free)
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
	struct Private *priv = GET_PRIVATE(base);
	if (priv->stack) {
		free_array(priv->stack, TRUE);
		priv->stack = NULL;
	}
	g_free(priv->pop);
	priv->pop = NULL;
	G_OBJECT_CLASS(log4g_ndc_parent_class)->finalize(base);
}

/**
 * log4g_ndc_get_instance:
 *
 * Retrieve the nested data context object for the current thread.
 *
 * Returns: A nested data context object.
 */
static Log4gNDC *
log4g_ndc_get_instance(void)
{
	Log4gNDC *self = g_private_get(priv);
	if (!self) {
		self = g_object_new(LOG4G_TYPE_NDC, NULL);
	}
	return self;
}

/**
 * log4g_ndc_get_current_stack:
 *
 * Get the current NDC stack.
 *
 * Returns: The current NDC stack or NULL if none exists.
 */
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
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->constructor = constructor;
	object_class->finalize = finalize;
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_ndc_clear:
 *
 * Clear current nested data context.
 *
 * This function is useful when a thread is used again in a different
 * unrelated context (e.g. thread pools).
 *
 * Since: 0.1
 */
void
log4g_ndc_clear(void)
{
	GArray *stack = log4g_ndc_get_current_stack();
	if (stack) {
		free_array(stack, FALSE);
		g_array_set_size(stack, 0);
	}
}

/**
 * log4g_ndc_clone:
 *
 * Clone the current nested data context.
 *
 * Another thread may inherit the value returned by this function by calling
 * log4g_ndc_inherit().
 *
 * <note><para>
 * The caller is responsible for calling g_array_free() or log4g_ndc_inherit()
 * for the returned value.
 * </para></note>
 *
 * Returns: (transfer container): A clone of the current nested data context.
 * Since: 0.1
 */
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
		Log4gDiagnosticContext *copy =
			log4g_diagnostic_context_clone(context);
		if (!copy) {
			goto error;
		}
		g_array_insert_val(clone, i, copy);
	}
	return clone;
error:
	if (clone) {
		free_array(clone, TRUE);
	}
	return NULL;
}

/**
 * log4g_ndc_inherit
 * @stack: A cloned nested data context.
 *
 * Inherit a cloned nested data context.
 *
 * <note><para>
 * After calling this function the caller should no longer reference @stack.
 * </para></note>
 *
 * See: log4g_ndc_clone()
 *
 * Since: 0.1
 */
void
log4g_ndc_inherit(GArray *stack)
{
	Log4gNDC *self = log4g_ndc_get_instance();
	if (!self) {
		return;
	}
	g_return_if_fail(stack);
	struct Private *priv = GET_PRIVATE(self);
	if (priv->stack) {
		free_array(priv->stack, TRUE);
	}
	priv->stack = stack;
}

/**
 * log4g_ndc_get:
 *
 * Retrieve the current diagnostic context formatted as a string.
 *
 * <note><para>
 * You should call log4g_logging_event_get_ndc() instead of this function.
 * </para></note>
 *
 * Returns: The current diagnostic context.
 * Since: 0.1
 */
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
	return g_array_index(stack, Log4gDiagnosticContext *,
			stack->len - 1)->full;
}

/**
 * log4g_ndc_size:
 *
 * Retrieve the size (depth) of the current nested data context.
 *
 * Returns: The number of elements on the nested data context stack.
 * Since: 0.1
 */
guint
log4g_ndc_size(void)
{
	GArray *stack = log4g_ndc_get_current_stack();
	if (!stack) {
		return 0;
	}
	return stack->len;
}

/**
 * log4g_ndc_pop:
 *
 * Call this function before leaving a diagnostic context.
 *
 * The returned value is the the value that most recently added with
 * log4g_ndc_push(). If no context is available, this function returns
 * %NULL.
 *
 * Returns: The innermost diagnostic context.
 * Since: 0.1
 */
const gchar *
log4g_ndc_pop(void)
{
	Log4gNDC *self = log4g_ndc_get_instance();
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->stack || !priv->stack->len) {
		return NULL;
	}
	Log4gDiagnosticContext *context = g_array_index(priv->stack,
			Log4gDiagnosticContext *, priv->stack->len - 1);
	if (context) {
		g_free(priv->pop);
		priv->pop = context->message;
		context->message = NULL;
		log4g_diagnostic_context_destroy(context);
	}
	g_array_remove_index(priv->stack, priv->stack->len - 1);
	return priv->pop;
}

/**
 * log4g_ndc_peek:
 *
 * Look at the innermost diagnostic context without removing it.
 *
 * The returned value is the the value that most recently added with
 * log4g_ndc_push(). If no context is available, this function returns
 * %NULL.
 *
 * Returns: The innermost diagnostic context.
 * Since: 0.1
 */
const gchar *
log4g_ndc_peek(void)
{
	GArray *stack = log4g_ndc_get_current_stack();
	Log4gDiagnosticContext *context;
	if (!stack || !stack->len) {
		return NULL;
	}
	context = g_array_index(stack, Log4gDiagnosticContext *,
			stack->len - 1);
	return context->message;
}

/**
 * log4g_ndc_push:
 * @message: A NDC message (accepts printf formats).
 * @...: Format parameters.
 *
 * Push new diagnostic context information for the current thread.
 *
 * Since: 0.1
 */
void
log4g_ndc_push(const char *message, ...)
{
	Log4gNDC *self = log4g_ndc_get_instance();
	va_list ap;
	if (!self) {
		return;
	}
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->stack) {
		Log4gDiagnosticContext *context;
		va_start(ap, message);
		context = log4g_diagnostic_context_new(NULL, message, ap);
		va_end(ap);
		if (!context) {
			return;
		}
		priv->stack = g_array_sized_new(FALSE, TRUE,
				sizeof(gchar *), 1);
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

/**
 * log4g_ndc_remove:
 *
 * Remove all diagnostic context for the current thread.
 *
 * Since: 0.1
 */
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

/**
 * log4g_ndc_set_max_depth:
 * @maxdepth: The new size to truncate the context stack to.
 *
 * Set the maximum depth of the current diagnostic context.
 *
 * If the current depth is smaller or equal to @maxdepth then no action
 * is taken.
 *
 * This function is a convenient alternative to calling log4g_ndc_pop()
 * multiple times. The following code example will preserve the depth of the
 * diagnostic context stack after a complex sequence of calls:
 *
 * |[
 * void foo()
 * {
 *     guint depth = log4g_ndc_get_size();
 *     ... complex sequence of calls ...
 *     log4g_ndc_set_max_depth(depth);
 * }
 * ]|
 *
 * Since: 0.1
 */
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
