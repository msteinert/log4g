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
 * SECTION: logging-event
 * @short_description: The internal representation of logging events
 *
 * Once an affirmative decision is made to log an event a logging event
 * instance is created. This instance is passed to appenders and filters to
 * perform actual logging.
 *
 * <note><para>
 * This class is only useful to those wishing to extend Log4g.
 * </para></note>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include "log4g/helpers/thread.h"
#include "log4g/logging-event.h"
#include "log4g/mdc.h"
#include "log4g/ndc.h"

G_DEFINE_TYPE(Log4gLoggingEvent, log4g_logging_event, G_TYPE_OBJECT)

#define ASSIGN_PRIVATE(instance) \
	(G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_LOGGING_EVENT, \
		struct Private))

#define GET_PRIVATE(instance) \
	((struct Private *)((Log4gLoggingEvent *)instance)->priv)

struct Private {
	gchar *logger;
	Log4gLevel *level;
	gchar *message;
	GTimeVal timestamp;
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
	self->priv = ASSIGN_PRIVATE(self);
	struct Private *priv = GET_PRIVATE(self);
	priv->thread_lookup_required = TRUE;
	priv->ndc_lookup_required = TRUE;
	priv->mdc_lookup_required = TRUE;
}

static void
dispose(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	if (priv->level) {
		g_object_unref(priv->level);
		priv->level = NULL;
	}
	G_OBJECT_CLASS(log4g_logging_event_parent_class)->dispose(base);
}

static void
finalize(GObject *base)
{
	struct Private *priv = GET_PRIVATE(base);
	g_free(priv->logger);
	g_free(priv->message);
	g_free(priv->ndc);
	g_free(priv->fullinfo);
	g_free(priv->thread);
	if (priv->mdc) {
		g_hash_table_destroy(priv->mdc);
	}
	if (priv->keys) {
		g_array_free(priv->keys, TRUE);
	}
	G_OBJECT_CLASS(log4g_logging_event_parent_class)->finalize(base);
}

static void
log4g_logging_event_class_init(Log4gLoggingEventClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);
	object_class->dispose = dispose;
	object_class->finalize = finalize;
	GTimeVal start;
	g_get_current_time(&start);
	klass->start = (start.tv_sec * 1000) + (start.tv_usec * 0.001);
	g_type_class_add_private(klass, sizeof(struct Private));
}

/**
 * log4g_logging_event_new:
 * @logger: The name of the logger that is creating this event.
 * @level: The log level of this event.
 * @function: The function where this event was logged.
 * @file: The file where this event was logged.
 * @line: The line in @file where this event was logged.
 * @message: A printf formatted log message.
 * @ap: Format parameters.
 *
 * Create a new logging event.
 *
 * Returns: A new logging event object.
 * Since: 0.1
 */
Log4gLoggingEvent *
log4g_logging_event_new(const gchar *logger, Log4gLevel *level,
		const gchar *function, const gchar *file, const gchar *line,
		const gchar *message, va_list ap)
{
	Log4gLoggingEvent *self = g_object_new(LOG4G_TYPE_LOGGING_EVENT, NULL);
	if (!self) {
		return NULL;
	}
	struct Private *priv = GET_PRIVATE(self);
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
	g_get_current_time(&priv->timestamp);
	return self;
error:
	g_object_unref(self);
	return NULL;
}

/**
 * log4g_logging_event_get_level:
 * @self: A logging event object.
 *
 * Calls the @get_level function from the #Log4gLoggingEventClass of @self.
 *
 * Returns: (transfer none): The log level of @self.
 * Since: 0.1
 */
Log4gLevel *
log4g_logging_event_get_level(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	return priv->level;
}

/**
 * log4g_logging_event_get_logger_name:
 * @self: A logging event object.
 *
 * Retrieve the name of the logger that created a logging event.
 *
 * Returns: The name of the logger that created @self.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_logger_name(Log4gLoggingEvent *self)
{
	return GET_PRIVATE(self)->logger;
}

/**
 * log4g_logging_event_get_rendered_message:
 * @self: A logging event object.
 *
 * Retrieve the rendered logging message.
 *
 * See: log4g_logging_event_get_message()
 *
 * Returns: The rendered logging message.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_rendered_message(Log4gLoggingEvent *self)
{
	return GET_PRIVATE(self)->message;
}

/**
 * log4g_logging_event_get_message:
 * @self: A logging event object.
 *
 * Retrieve the log message.
 *
 * This function is equivalent to log4g_logging_event_get_rendered_message().
 *
 * Returns: The log message.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_message(Log4gLoggingEvent *self)
{
	return GET_PRIVATE(self)->message;
}

/**
 * log4g_logging_event_get_mdc:
 * @self: A logging event object.
 * @key: A mapped data context key.
 *
 * Retrieve a mapped data context value for a logging event.
 *
 * See: #Log4gMDC
 *
 * Returns: The MDC value for @key.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_mdc(Log4gLoggingEvent *self, const gchar *key)
{
	struct Private *priv = GET_PRIVATE(self);
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

/**
 * log4g_logging_event_get_time_stamp:
 * @self: A logging event object.
 *
 * Retrieve the timestamp of a logging event.
 *
 * Returns: (transfer none): The timestamp of @self.
 * Since: 0.1
 */
const GTimeVal *
log4g_logging_event_get_time_stamp(Log4gLoggingEvent *self)
{
	return &GET_PRIVATE(self)->timestamp;
}

/**
 * log4g_logging_event_get_thread_name:
 * @self: A logging event object.
 *
 * Retrieve the name of the thread where a logging event was logged.
 *
 * Returns: The name of the thread where @self was logged.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_thread_name(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->thread) {
		return priv->thread;
	}
	if (priv->thread_lookup_required) {
		return log4g_thread_get_name();
	}
	return NULL;
}

/**
 * log4g_logging_event_get_ndc:
 * @self: A logging event object.
 *
 * Retrieve the nested data context for a logging event.
 *
 * See: #Log4gNDC
 *
 * Returns: The rendered NDC string for @self.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_ndc(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->ndc) {
		return priv->ndc;
	}
	if (priv->ndc_lookup_required) {
		return log4g_ndc_get();
	}
	return NULL;
}

/**
 * get_keys_:
 * @key: The hash table key.
 * @value: The hash table value (unused).
 * @user_data: An array to append @key to.
 *
 * Callback for g_hash_table_foreach().
 */
static void
get_keys_(gpointer key, G_GNUC_UNUSED gpointer value, gpointer user_data)
{
	g_array_append_val((GArray *)user_data, key);
}

/**
 * get_property_key_set_:
 * @self: A logging event object.
 * @mdc: The hash table to get keys from.
 *
 * Construct a key set from an MDC hash table.
 */
static void
get_property_key_set_(Log4gLoggingEvent *self, GHashTable *mdc)
{
	guint size = g_hash_table_size((GHashTable *)mdc);
	struct Private *priv = GET_PRIVATE(self);
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
	g_hash_table_foreach(mdc, get_keys_, priv->keys);
}

/**
 * log4g_logging_event_get_property_key_set:
 * @self: A logging event object.
 *
 * Get the MDC keys (if any) for this event.
 *
 * See: #Log4gMDC
 *
 * Returns: An array of keys, or %NULL if no keys exist.
 * Since: 0.1
 */
const GArray *
log4g_logging_event_get_property_key_set(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (priv->mdc) {
		if (!priv->keys) {
			get_property_key_set_(self, priv->mdc);
		}
	} else {
		const GHashTable *mdc = log4g_mdc_get_context();
		if (mdc) {
			get_property_key_set_(self, (GHashTable *)mdc);
		}
	}
	return priv->keys;
}

/**
 * mdc_copy_:
 * @key: The hash table key.
 * @value: The hash table value (unused).
 * @user_data: A hash table to insert @key & @value into.
 *
 * Callback for g_hash_table_foreach().
 */
static void
mdc_copy_(gpointer key, gpointer value, gpointer user_data)
{
	g_hash_table_insert((GHashTable *)user_data, key, value);
}

/**
 * log4g_logging_event_get_thread_copy:
 * @self: A logging event object.
 *
 * Copy the current thread name into a logging object.
 *
 * Asynchronous appenders should call this function.
 *
 * See: #Log4gThreadClass
 */
void
log4g_logging_event_get_thread_copy(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->thread_lookup_required) {
		return;
	}
	priv->thread_lookup_required = FALSE;
	priv->thread = g_strdup(log4g_thread_get_name());
}

/**
 * log4g_logging_event_get_mdc_copy:
 * @self: A logging event object.
 *
 * Copy the current mapped data context into a logging event.
 *
 * Asynchronous appenders should call this function.
 *
 * See #Log4gMDC
 *
 * Since: 0.1
 */
void
log4g_logging_event_get_mdc_copy(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->mdc_lookup_required) {
		return;
	}
	const GHashTable *mdc;
	priv->mdc_lookup_required = FALSE;
	mdc = log4g_mdc_get_context();
	if (!mdc) {
		return;
	}
	priv->mdc = g_hash_table_new_full(g_str_hash, g_str_equal,
			g_free, g_free);
	if (!priv->mdc) {
		return;
	}
	g_hash_table_foreach((GHashTable *)mdc, mdc_copy_, priv->mdc);
}

/**
 * log4g_logging_event_get_ndc_copy:
 * @self: A logging event object.
 *
 * Copy the current nested data context into a logging event.
 *
 * Asynchronous appenders should call this function.
 *
 * See #Log4gNDC
 *
 * Since: 0.1
 */
void
log4g_logging_event_get_ndc_copy(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->ndc_lookup_required) {
		return;
	}
	priv->ndc_lookup_required = FALSE;
	priv->ndc = g_strdup(log4g_ndc_get());
}

/**
 * log4g_logging_event_get_function_name:
 * @self: A logging event object.
 *
 * Retrieve the function where a logging event was logged.
 *
 * Returns: The function where @self was logged.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_function_name(Log4gLoggingEvent *self)
{
	const gchar *function = GET_PRIVATE(self)->function;
	return (function ? function : "?");
}

/**
 * log4g_logging_event_get_file_name:
 * @self: A logging event object.
 *
 * Retrieve the file where a logging event was logged.
 *
 * Returns: The file where @self was logged.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_file_name(Log4gLoggingEvent *self)
{
	const gchar *file = GET_PRIVATE(self)->file;
	return (file ? file : "?");
}

/**
 * log4g_logging_event_get_line_number:
 * @self: A logging event object.
 *
 * Retrieve the line number where a logging event was logged.
 *
 * Returns: The line number where @self was logged.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_line_number(Log4gLoggingEvent *self)
{
	const gchar *line = GET_PRIVATE(self)->line;
	return (line ? line : "?");
}

/**
 * log4g_logging_event_get_full_info:
 * @self: A logging event object.
 *
 * Retrieve the full location information where a logging event was logged.
 *
 * The full location information is in the format:
 *
 * |[
 * function(file:line)
 * ]|
 *
 * Returns: The full log location information for @self.
 * Since: 0.1
 */
const gchar *
log4g_logging_event_get_full_info(Log4gLoggingEvent *self)
{
	struct Private *priv = GET_PRIVATE(self);
	if (!priv->fullinfo) {
		priv->fullinfo = g_strdup_printf("%s(%s:%s)",
				(priv->function ? priv->function : "?"),
				(priv->file ? priv->file : "?"),
				(priv->line ? priv->line : "?"));
	}
	return priv->fullinfo;
}

/**
 * log4g_logging_event_get_start_time:
 *
 * Retrieve the time when the log system was initialized.
 *
 * Returns: The number of seconds elapsed since the Unix epoch when the log
 *         system was initialized
 * Since: 0.1
 */
glong
log4g_logging_event_get_start_time(void)
{
	Log4gLoggingEventClass *klass =
		g_type_class_peek(LOG4G_TYPE_LOGGING_EVENT);
	return klass->start;
}
