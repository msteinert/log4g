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
 * SECTION: log4g
 * @short_description: The main Log4g interface
 * @title: Log4g
 * @section_id: Log4g
 * @see_also: #Log4gNDC, #Log4gMDC, #Log4gDOMConfigurator,
 *            #Log4gBasicConfigurator, #Log4gThread, #Log4gAsyncAppender,
 *            #Log4gLogger, #Log4gRootLogger
 *
 * The file defines the API that most users of Log4g will want to use.
 * Sticking to this API will allow you to easily compile Log4g out of your
 * application by defining LOG4G_DISABLE before including this file (i.e. in
 * config.h, etc).
 *
 * You may also wish to refer to the documentation for NDC (nested data
 * contexts) & MDC (mapped data contexts). The NDC & MDC APIs can also be
 * compiled out of your code.
 *
 * Defining LOG4G_LOG_DOMAIN before you include log4g/log4g.h will set the
 * log domain for the current file. Log statements that do not specify a
 * logger will use the LOG4G_LOG_DOMAIN logger by default.
 *
 * Example (log a message to the "org.gnome.test" logger):
 *
 * |[
 * #define LOG4G_LOG_DOMAIN "org.gnome.test"
 * #include <log4g/log4g.h>
 *
 * void
 * foo(void)
 * {
 *     log4g_warn("you had better watch out");
 * }
 * ]|
 *
 * If LOG4G_LOG_DOMAIN is not defined then log statements will use the root
 * logger by default. You may also explicitly specify a logger when you log
 * a message.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <glib.h>
#include <locale.h>
#include "log4g/basic-configurator.h"
#include "log4g/dom-configurator.h"
#include "log4g/helpers/thread.h"
#include "log4g/log-manager.h"
#include "log4g/log4g.h"
#include <stdlib.h>

/* Indicates if log4g_init() has been called */
static gboolean initialized = FALSE;

/* Option flags */
typedef enum {
	LOG4G_FLAG_DEBUG, /* Enable debug output */
	LOG4G_FLAG_QUIET /* Enable quiet mode */
} Log4gFlag;

/* Option flag definitions */
static const GDebugKey const flags[] = {
	{ "debug", LOG4G_FLAG_DEBUG },
	{ "quiet", LOG4G_FLAG_QUIET }
};

/* Configuration options */
typedef struct Options_ {
	gchar *configuration; /* Configuration file name */
	gint flags; /* Configuration flags */
	gchar *thread; /* Main thread name */
} Options;

static Options *
options_new(void)
{
	Options *self = g_slice_new0(Options);
	if (!self) {
		return NULL;
	}
	self->configuration = NULL;
	self->flags = 0;
	self->thread = NULL;
	return self;
}

static void
options_destroy(gpointer object)
{
	Options *self = (Options *)object;
	if (self) {
		g_free(self->configuration);
		g_free(self->thread);
		g_slice_free(Options, self);
	}
}

static gboolean
log4g_arg_configuration_cb(G_GNUC_UNUSED const gchar *key, const gchar *value,
		gpointer data)
{
	Options *opt = (Options *)data;
	opt->configuration = g_strdup(value);
	if (!opt->configuration) {
		return FALSE;
	}
	return TRUE;
}

static gboolean
log4g_arg_flags_cb(G_GNUC_UNUSED const gchar *key, const gchar *value,
		gpointer data)
{
	Options *opt = (Options *)data;
	opt->flags |= g_parse_debug_string(value, flags, G_N_ELEMENTS(flags));
	return TRUE;
}

static gboolean
log4g_arg_main_thread_cb(G_GNUC_UNUSED const gchar *key, const gchar *value,
		gpointer data)
{
	Options *opt = (Options *)data;
	opt->thread = g_strdup(value);
	if (!opt->thread) {
		return FALSE;
	}
	return TRUE;
}

static const GOptionEntry const log4g_args[] = {
	{ "log4g-configuration", '\0', G_OPTION_FLAG_FILENAME,
		G_OPTION_ARG_CALLBACK, log4g_arg_configuration_cb,
		N_("Configuration file to load"), N_("FILE.xml") },
	{ "log4g-flags", '\0', 0,
		G_OPTION_ARG_CALLBACK, log4g_arg_flags_cb,
		N_("Log4g debugging flags to set"), N_("FLAGS") },
	{ "log4g-main-thread", '\0', 0,
		G_OPTION_ARG_CALLBACK, log4g_arg_main_thread_cb,
		N_("Set the name of the main thread"), N_("NAME") },
	{ NULL, '\0', 0, G_OPTION_ARG_NONE, NULL, NULL, NULL }
};

static gboolean
pre_parse_hook(G_GNUC_UNUSED GOptionContext *context,
		G_GNUC_UNUSED GOptionGroup *group,
		gpointer data, G_GNUC_UNUSED GError **error)
{
	Options *opt = (Options *)data;
	if (initialized) {
		return TRUE;
	}
	const gchar *env = g_getenv("LOG4G_DEBUG");
	if (env) {
		opt->flags =
			g_parse_debug_string(env, flags, G_N_ELEMENTS(flags));
	}
	env = g_getenv("LOG4G_CONFIGURATION");
	if (env) {
		opt->configuration = g_strdup(env);
		if (!opt->configuration) {
			return FALSE;
		}
	}
	return TRUE;
}

static gboolean
post_parse_hook(G_GNUC_UNUSED GOptionContext *context,
		G_GNUC_UNUSED GOptionGroup *group,
		gpointer data, G_GNUC_UNUSED GError **error)
{
	Options *opt = (Options *)data;
	if (initialized) {
		return TRUE;
	}
	if (opt->flags & LOG4G_FLAG_DEBUG) {
		log4g_set_internal_debugging(TRUE);
	}
	if (opt->flags & LOG4G_FLAG_QUIET) {
		log4g_set_quiet_mode(TRUE);
	}
	gboolean cfg = FALSE;
	log4g_thread_set_name((opt->thread ? opt->thread : "main"));
	if (opt->configuration) {
		GError *error = NULL;
		cfg = log4g_dom_configurator_configure(opt->configuration,
				&error);
		if (!cfg) {
			log4g_log_warn("%s: %s", opt->configuration,
					error->message);
			log4g_log_warn(Q_("using basic configurator for "
						"configuration"));
			g_error_free(error);
		}
	}
	if (!cfg) {
		log4g_basic_configurator_configure();
	}
	initialized = TRUE;
	return TRUE;
}

/**
 * log4g_get_option_group: (skip):
 *
 * Retrieve the option group used by the Log4g package.
 *
 * If your application is using the GLib option parser then you may call
 * this function to retrieve the option group parsed by Log4g.
 *
 * Example (initialize Log4g via a GOptionGroup):
 *
 * |[
 * #include <log4g/log4g.h>
 * #include <stdlib.h>
 *
 * #define GETTEXT_PACKAGE "test-app"
 * #include <glib/gi18n-lib.h>
 *
 * int
 * main(int argc, char **argv)
 * {
 *     GError *error = NULL;
 *     GOptionContext *context;
 *     GOptionEntry entries[] = {
 *         { NULL }
 *     };
 *     g_thread_init(NULL);
 *     context = g_option_context_new("- demonstrate Log4g initialization");
 *     if (!context) {
 *         return EXIT_FAILURE;
 *     }
 *     g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
 *     g_option_context_add_group(context, log4g_get_option_group())
 *     if (!g_option_context_parse(context, &argc, &argv, &error)) {
 *         log4g_error("failed to parse options: %s", error->mesasge);
 *         g_error_free(error);
 *         return EXIT_FAILURE;
 *     }
 *     g_option_context_free();
 *     log4g_info("Log4g is initialized & ready to go!");
 *     log4g_finalize();
 *     return EXIT_SUCCESS;
 * }
 * ]|
 *
 * If you use the returned option group in a GLib option parser you do not
 * need to call log4g_init().
 *
 * Returns: A GLib option group.
 * Since: 0.1
 */
GOptionGroup *
log4g_get_option_group(void)
{
	if (!initialized) {
		if (!setlocale(LC_ALL, "")) {
			log4g_log_warn(Q_("Locale not supported by C "
						"library.\n\tUsing the "
						"fallback 'C' locale.\n"));
		}
#ifdef ENABLE_NLS
		if (!bindtextdomain(GETTEXT_PACKAGE, LOG4G_LOCALEDIR)) {
			log4g_log_error("bindtextdomain(): %s",
					g_strerror(errno));
		}
		if (!bindtextdomain(GETTEXT_PACKAGE "-properties",
					LOG4G_LOCALEDIR)) {
			log4g_log_error("bindtextdomain(): %s",
					g_strerror(errno));
		}
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
		if (!bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8")) {
			log4g_log_error("bind_textdomain_codeset(): %s",
					g_strerror(errno));
		}
		if (!bind_textdomain_codeset(GETTEXT_PACKAGE "-properties",
					"UTF-8")) {
			log4g_log_error("bind_textdomain_codeset(): %s",
					g_strerror(errno));
		}
#endif
#endif
	}
	Options *opt = options_new();
	if (!opt) {
		return NULL;
	}
	GOptionGroup *group = g_option_group_new("log4g", Q_("Log4g Options"),
			Q_("Show Log4g Options"), opt, options_destroy);
	if (!group) {
		return NULL;
	}
	g_option_group_set_parse_hooks(group, pre_parse_hook, post_parse_hook);
	g_option_group_add_entries(group, log4g_args);
	g_option_group_set_translation_domain(group, GETTEXT_PACKAGE);
	return group;
}

/**
 * log4g_init:
 * @argc: A pointer to main's @argc variable.
 * @argv: A pointer to main's @argv variable.
 *
 * Initialize the Log4g package.
 *
 * Log4g accepts some standard command line arguments (all prepended with
 * "log4g"). Calling this function will parse out the arguments Log4g
 * understands and removes them from the argument list. If you call
 * log4g_init() before parsing application options your application will
 * not see the Log4g arguments.
 *
 * Example (initialize Log4g):
 *
 * |[
 * #include &lt;log4g/log4g.h&gt;
 * #include &lt;stdlib.h&gt;
 *
 * int
 * main(int argc, char **argv)
 * {
 *     g_thread_init(NULL);
 *     log4g_init(&argc, &argv);
 *     log4g_info("Log4g is initialized & ready to go!");
 *     return EXIT_SUCCESS;
 * }
 * ]|
 *
 * Log4g accepts the following command line arguments:
 *
 * <emphasis>--log4g-configuration=&lt;FILE.xml&gt;</emphasis>
 *
 * Specify an XML configuration file to load. If the XML configuration file
 * could not be loaded (or if none was specified) then Log4g will be
 * configured using the basic configurator.
 *
 * <emphasis>--log4g-flags=&lt;FLAGS&gt;</emphasis>
 *
 * Specify flags that modify the behavior of Log4g. Currently Log4g
 * understands two flags.
 * <itemizedlist>
 * <listitem><para>debug: enable debug output on stdout</para></listitem>
 * <listitem><para>quiet: disable all error and debug output</para></listitem>
 * </itemizedlist>
 *
 * <emphasis>--log4g-main-thread=&lt;NAME&gt;</emphasis>
 *
 * Set the name of the main thread (the default is "main").
 *
 * After calling this function the Log4g API is ready for use within your
 * application.
 *
 * Since: 0.1
 */
void
log4g_init(int *argc, char ***argv)
{
	if (initialized) {
		return;
	}
	GOptionContext *context = g_option_context_new(NULL);
	if (!context) {
		return;
	}
	g_option_context_set_ignore_unknown_options(context, TRUE);
	g_option_context_set_help_enabled(context, FALSE);
	GOptionGroup *group = log4g_get_option_group();
	if (group) {
		g_option_context_set_main_group(context, group);
		GError *error = NULL;
		if (!g_option_context_parse(context, argc, argv, &error)) {
			log4g_log_warn("%s", error->message);
			g_error_free(error);
		}
	}
	g_option_context_free(context);
}

/**
 * log4g_finalize:
 *
 * Finalize the Log4g package.
 *
 * It is not strictly necessary to call this function to use Log4g.
 *
 * Calling this function will destroy the logger hierarchy and any cleanup
 * any instance data used by Log4g. One advantage of calling log4g_finalize()
 * is to allow asynchronous appenders a chance to flush any pending log
 * messages before your application exits.
 *
 * Example (initialize & cleanup Log4g):
 *
 * |[
 * #include &lt;log4g/log4g.h&gt;
 * #include &lt;stdlib.h&gt;
 *
 * int
 * main(int argc, char **argv)
 * {
 *     g_thread_init(NULL);
 *     log4g_init(&argc, &argv);
 *     log4g_info("Log4g is initialized & ready to go!");
 *     log4g_finalize();
 *     return EXIT_SUCCESS;
 * }
 * ]|
 *
 * If you call this function you must call log4g_init() again before using
 * the Log4g API.
 *
 * Since: 0.1
 */
void
log4g_finalize(void)
{
	log4g_log_manager_shutdown();
	log4g_log_manager_remove_instance();
}
