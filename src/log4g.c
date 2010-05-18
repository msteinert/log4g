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
 * \brief Implements the API in log4g/log4g.h
 * \author Mike Steinert
 * \date 2-4-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <glib.h>
#include "log4g/basic-configurator.h"
#include "log4g/dom-configurator.h"
#include "log4g/helpers/thread.h"
#include "log4g/log-manager.h"
#include "log4g/log4g.h"
#include <stdlib.h>

/** \brief Indicates if log4g_init() has been called */
static gboolean initialized = FALSE;

/** \brief Option flags */
typedef enum {
    LOG4G_FLAG_DEBUG, /**< Enable debug output */
    LOG4G_FLAG_QUIET /**< Enable quiet mode */
} Log4gFlag;

/** \brief Option flag definitions */
static const GDebugKey flags[] = {
    { "debug", LOG4G_FLAG_DEBUG },
    { "quiet", LOG4G_FLAG_QUIET }
};

/** \brief Configuration options */
typedef struct _Options {
    gchar *configuration; /**< Configuration file name */
    gint flags; /**< Configuration flags */
    gchar *thread; /**< Main thread name */
} Options;

static Options *
options_new(void)
{
    Options *self = g_try_malloc(sizeof(*self));
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
        if (self->configuration) {
            g_free(self->configuration);
        }
        if (self->thread) {
            g_free(self->thread);
        }
        g_free(self);
    }
}

static gboolean
log4g_arg_configuration_cb(const gchar *key, const gchar *value,
        gpointer user_data)
{
    Options *opt = (Options *)user_data;
    opt->configuration = g_strdup(value);
    if (!opt->configuration) {
        return FALSE;
    }
    return TRUE;
}

static gboolean
log4g_arg_flags_cb(const gchar *key, const gchar *value, gpointer user_data)
{
    Options *opt = (Options *)user_data;
    opt->flags |= g_parse_debug_string(value, flags, G_N_ELEMENTS(flags));
    return TRUE;
}

static gboolean
log4g_arg_main_thread_cb(const gchar *key, const gchar *value,
        gpointer user_data)
{
    Options *opt = (Options *)user_data;
    opt->thread = g_strdup(value);
    if (!opt->thread) {
        return FALSE;
    }
    return TRUE;
}

G_GNUC_NORETURN static gboolean
log4g_arg_version_cb(const gchar *key, const gchar *value, gpointer user_data)
{
    g_print("Log4g %s\n\n"
            "Copyright 2010 Michael Steinert\n"
            "This is free software; see the source "
            "for copying conditions. There is NO\n"
            "warranty; not even for MERCHANTABILITY "
            "or FITNESS FOR A PARTICULAR PURPOSE.\n" , VERSION);
    exit(EXIT_SUCCESS);
}

static const GOptionEntry log4g_args[] = {
    { "log4g-configuration", '\0', G_OPTION_FLAG_FILENAME,
      G_OPTION_ARG_CALLBACK, log4g_arg_configuration_cb,
      N_("Configuration file to load"), N_("FILE.xml") },
    { "log4g-flags", '\0', 0,
      G_OPTION_ARG_CALLBACK, log4g_arg_flags_cb,
      N_("Log4g debugging flags to set"), N_("FLAGS") },
    { "log4g-main-thread", '\0', 0,
      G_OPTION_ARG_CALLBACK, log4g_arg_main_thread_cb,
      N_("Set the name of the main thread"), N_("NAME") },
    { "log4g-version", '\0', G_OPTION_FLAG_NO_ARG,
      G_OPTION_ARG_CALLBACK, log4g_arg_version_cb,
      N_("Display the Log4g version"), NULL },
    { NULL }
};

static gboolean
pre_parse_hook(GOptionContext *context, GOptionGroup *group,
        gpointer data, GError **error)
{
    Options *opt = (Options *)data;
    if (initialized) {
        return TRUE;
    }
    const gchar *env = g_getenv("LOG4G_DEBUG");
    if (env) {
        opt->flags = g_parse_debug_string(env, flags, G_N_ELEMENTS(flags));
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
post_parse_hook(GOptionContext *context, GOptionGroup *group,
        gpointer data, GError **error)
{
    Options *opt = (Options *)data;
    if (initialized) {
        return TRUE;
    }
    g_type_init();
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
        cfg = log4g_dom_configurator_configure(opt->configuration, &error);
        if (!cfg) {
            log4g_log_warn("%s: %s", opt->configuration, error->message);
            log4g_log_warn(Q_("using basic configurator for configuration"));
            g_error_free(error);
        }
    }
    if (!cfg) {
        log4g_basic_configurator_configure();
    }
    initialized = TRUE;
    return TRUE;
}

GOptionGroup *
log4g_get_option_group(void)
{
    if (!initialized) {
        if (!setlocale(LC_ALL, "")) {
            log4g_log_warn(Q_("Locale not supported by C library.\n"
                    "\tUsing the fallback 'C' locale.\n"));
        }
#ifdef ENABLE_NLS
        if (!bindtextdomain(GETTEXT_PACKAGE, LOG4G_LOCALEDIR)) {
            log4g_log_error("bindtextdomain(): %s", g_strerror(errno));
        }
        if (!bindtextdomain(GETTEXT_PACKAGE "-properties", LOG4G_LOCALEDIR)) {
            log4g_log_error("bindtextdomain(): %s", g_strerror(errno));
        }
#ifdef HAVE_BIND_TEXTDOMAIN_CODESET
        if (!bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8")) {
            log4g_log_error("bind_textdomain_codeset(): %s",
                    g_strerror(errno));
        }
        if (!bind_textdomain_codeset(GETTEXT_PACKAGE "-properties", "UTF-8")) {
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

void
log4g_finalize(void)
{
    log4g_log_manager_shutdown();
}
