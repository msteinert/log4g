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
 * \brief Implements the API in log4g/dom-configurator.h
 * \author Mike Steinert
 * \date 2-23-2010
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <errno.h>
#include <gmodule.h>
#include <libxml/parser.h>
#include "log4g/dom-configurator.h"
#include "log4g/error.h"
#include "log4g/interface/appender-attachable.h"
#include "log4g/log-manager.h"
#include <stdlib.h>

#define GET_PRIVATE(instance) \
    (G_TYPE_INSTANCE_GET_PRIVATE(instance, LOG4G_TYPE_DOM_CONFIGURATOR, \
            struct Log4gPrivate))

struct Log4gPrivate {
    xmlParserCtxtPtr ctx; /**< LibXML-2.0 parser context */
    GModule *module; /**< module handle to find *_get_type() functions */
    GString *scratch; /**< scratch buffer */
    gboolean debug; /**< enables XML parser debugging */
    GHashTable *appenders; /**< store named appenders */
};

static void
parse_property(Log4gConfigurator *base, xmlNodePtr node, gpointer object)
{
    xmlChar *name = NULL;
    xmlChar *value = NULL;
    GParamSpec *spec = NULL;
    name = xmlGetProp(node, (const xmlChar *)"name");
    if (!name) {
        log4g_log_error(Q_("properties must have a `name' attribute"));
        goto exit;
    }
    value = xmlGetProp(node, (const xmlChar *)"value");
    if (!value) {
        log4g_log_error(Q_("properties must have a `value' attribute"));
        goto exit;
    }
    spec = g_object_class_find_property(
                G_OBJECT_GET_CLASS(object), (gchar *)name);
    if (!spec) {
        log4g_log_error(Q_("object does not have the property `%s'"),
                (gchar *)name);
        goto exit;
    }
    if (G_TYPE_STRING == spec->value_type) {
        g_object_set(object, (const gchar *)name, (const gchar *)value, NULL);
    } else if (G_TYPE_BOOLEAN == spec->value_type) {
        if (!g_ascii_strcasecmp((const gchar *)value, "true")) {
            g_object_set(object, (const gchar *)name, TRUE, NULL);
        } else if (!g_ascii_strcasecmp((const gchar *)value, "false")) {
            g_object_set(object, (const gchar *)name, FALSE, NULL);
        } else {
            log4g_log_error(Q_("%s: not a boolean value (true|false)"), value);
            goto exit;
        }
    } else if (G_TYPE_CHAR == spec->value_type) {
        if (1 != strlen((const char *)value)) {
            log4g_log_error(Q_("%s: not a char"), value);
            goto exit;
        }
        g_object_set(object, (const gchar *)value, *value, NULL);
    } else if (G_TYPE_UCHAR == spec->value_type) {
        if (1 != strlen((const char *)value)) {
            log4g_log_error(Q_("%s: not a uchar"), value);
            goto exit;
        }
        g_object_set(object, (const gchar *)value, *value, NULL);
    } else if (G_TYPE_INT == spec->value_type) {
        gint64 i;
        errno = 0;
        i = g_ascii_strtoll((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not an int"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, (gint)i, NULL);
    } else if (G_TYPE_UINT == spec->value_type) {
        guint64 i;
        errno = 0;
        i = g_ascii_strtoull((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not an uint"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, (guint)i, NULL);
    } else if (G_TYPE_LONG== spec->value_type) {
        gint64 i;
        errno = 0;
        i = g_ascii_strtoll((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not a long"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, (glong)i, NULL);
    } else if (G_TYPE_ULONG== spec->value_type) {
        guint64 i;
        errno = 0;
        i = g_ascii_strtoull((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not an ulong"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, (gulong)i, NULL);
    } else if (G_TYPE_INT64== spec->value_type) {
        gint64 i;
        errno = 0;
        i = g_ascii_strtoll((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not a uint64"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, (glong)i, NULL);
    } else if (G_TYPE_UINT64== spec->value_type) {
        guint64 i;
        errno = 0;
        i = g_ascii_strtoull((const char *)value, NULL, 10);
        if (errno) {
            log4g_log_error(Q_("%s: not an uint64"), value);
            goto exit;
        }
        g_object_set(object, (const char *)value, i, NULL);
    } else if (G_TYPE_FLOAT == spec->value_type) {
        gdouble d;
        errno = 0;
        d = g_ascii_strtod((gchar *)value, NULL);
        if (errno) {
            log4g_log_error(Q_("%s: not a float"), value);
            goto exit;
        }
        g_object_set(object, (const gchar *)name, (gfloat)d, NULL);
    } else if (G_TYPE_DOUBLE == spec->value_type) {
        gdouble d;
        errno = 0;
        d = g_ascii_strtod((gchar *)value, NULL);
        if (errno) {
            log4g_log_error(Q_("%s: not a double"), value);
            goto exit;
        }
        g_object_set(object, (const gchar *)name, d, NULL);
    } else {
        log4g_log_warn(Q_("%s: property cannot be set via DOM configuration"),
                name);
        goto exit;
    }
exit:
    if (spec) {
        g_param_spec_unref(spec);
    }
    if (name) {
        xmlFree(name);
    }
    if (value) {
        xmlFree(value);
    }
}

static Log4gLayout *
parse_layout(Log4gConfigurator *base, xmlNodePtr node)
{
    gpointer function;
    xmlChar *type = NULL;
    GType (*get_type)(void);
    Log4gLayout *layout = NULL;
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    type = xmlGetProp(node, (const xmlChar *)"type");
    if (!type) {
        log4g_log_error(Q_("layouts must have a `type'"));
        goto exit;
    }
    g_string_set_size(priv->scratch, 0);
    g_string_append(priv->scratch, (gchar *)type);
    g_string_append(priv->scratch, "_get_type");
    if (!g_module_symbol(priv->module, priv->scratch->str, &function)) {
        log4g_log_error(Q_("%s: invalid `type'"), type);
        goto exit;
    }
    get_type = function;
    layout = g_object_new(get_type(), NULL);
    if (!layout) {
        log4g_log_error(Q_("%s: g_object_new() returned NULL"), type);
        goto exit;
    }
    if (!LOG4G_IS_LAYOUT(layout)) {
        log4g_log_error(Q_("%s: not an instance of log4g_layout"), type);
        g_object_unref(layout);
        layout = NULL;
        goto exit;
    }
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"property")) {
            parse_property(base, node, layout);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
    log4g_layout_activate_options(layout);
exit:
    if (type) {
        xmlFree(type);
    }
    return layout;
}

static Log4gFilter *
parse_filter(Log4gConfigurator *base, xmlNodePtr node)
{
    gpointer function;
    xmlChar *type = NULL;
    GType (*get_type)(void);
    Log4gFilter *filter = NULL;
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    type = xmlGetProp(node, (const xmlChar *)"type");
    if (!type) {
        log4g_log_error(Q_("filters must have a `type'"));
        goto exit;
    }
    g_string_set_size(priv->scratch, 0);
    g_string_append(priv->scratch, (gchar *)type);
    g_string_append(priv->scratch, "_get_type");
    if (!g_module_symbol(priv->module, priv->scratch->str, &function)) {
        log4g_log_error(Q_("%s: invalid `type'"), type);
        goto exit;
    }
    get_type = function;
    filter = g_object_new(get_type(), NULL);
    if (!filter) {
        log4g_log_error(Q_("%s: g_object_new() returned NULL"), type);
        goto exit;
    }
    if (!LOG4G_IS_FILTER(filter)) {
        log4g_log_error(Q_("%s: not an instance of log4g_filter"), type);
        g_object_unref(filter);
        filter = NULL;
        goto exit;
    }
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"property")) {
            parse_property(base, node, filter);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
    log4g_filter_activate_options(filter);
exit:
    if (type) {
        xmlFree(type);
    }
    return filter;
}

static Log4gAppender *
parse_appender(Log4gConfigurator *base, xmlNodePtr node)
{
    xmlChar *type = NULL;
    xmlChar *name = NULL;
    Log4gAppender *appender = NULL;
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    type = xmlGetProp(node, (const xmlChar *)"type");
    if (type) {
        gpointer function;
        GType (*get_type)(void);
        g_string_set_size(priv->scratch, 0);
        g_string_append(priv->scratch, (gchar *)type);
        g_string_append(priv->scratch, "_get_type");
        if (!g_module_symbol(priv->module, priv->scratch->str, &function)) {
            log4g_log_error(Q_("%s: invalid `type'"), type);
            goto exit;
        }
        get_type = function;
        appender = g_object_new(get_type(), NULL);
        if (!appender) {
            log4g_log_error(Q_("%s: g_object_new() returned NULL"), type);
            goto exit;
        }
        if (!LOG4G_IS_APPENDER(appender)) {
            log4g_log_error(Q_("%s: not an instance of log4g_appender"), type);
            g_object_unref(appender);
            appender = NULL;
            goto exit;
        }
    }
    name = xmlGetProp(node, (const xmlChar *)"name");
    if (appender) {
        if (name) {
            g_object_ref(appender);
            g_hash_table_insert(priv->appenders, name, appender);
            name = NULL;
        }
    } else {
        if (name) {
            appender =
                LOG4G_APPENDER(g_hash_table_lookup(priv->appenders, name));
            if (!appender) {
                log4g_log_error(Q_("%s: no such appender"), name);
                goto exit;
            }
            g_object_ref(appender);
        } else {
            log4g_log_error(Q_("appenders without a `type' must "
                        "have a `name'"));
            goto exit;
        }
    }
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"property")) {
            parse_property(base, node, appender);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"layout")) {
            Log4gLayout *layout = parse_layout(base, node);
            if (layout) {
                log4g_appender_set_layout(appender, layout);
                g_object_unref(layout);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"filter")) {
            Log4gFilter *filter = parse_filter(base, node);
            if (filter) {
                log4g_appender_add_filter(appender, filter);
                g_object_unref(filter);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"appender")) {
            if (LOG4G_IS_APPENDER_ATTACHABLE(appender)) {
                Log4gAppender *child = parse_appender(base, node);
                if (child) {
                    log4g_appender_attachable_add_appender(
                            LOG4G_APPENDER_ATTACHABLE(appender), child);
                    g_object_unref(child);
                }
            } else {
                log4g_log_error(Q_("%s: does not implement "
                        "log4g_appender_attachable"), type);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
    if (log4g_appender_requires_layout(appender)) {
        if (!log4g_appender_get_layout(appender)) {
            log4g_log_error(Q_("%s: appender requires a layout"), type);
            g_object_unref(appender);
            appender = NULL;
            goto exit;
        }
    }
    log4g_appender_activate_options(appender);
exit:
    if (type) {
        xmlFree(type);
    }
    if (name) {
        xmlFree(name);
    }
    return appender;
}

static void
parse_level(Log4gConfigurator *base, xmlNodePtr node, Log4gLogger *logger)
{
    gpointer function;
    xmlChar *type = NULL;
    xmlChar *value = NULL;
    Log4gLevel *level = NULL;
    Log4gLevelClass *klass = NULL;
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    type = xmlGetProp(node, (const xmlChar *)"type");
    if (type) {
        GType (*get_type)(void);
        g_string_set_size(priv->scratch, 0);
        g_string_append(priv->scratch, (gchar *)type);
        g_string_append(priv->scratch, "_get_type");
        if (!g_module_symbol(priv->module, priv->scratch->str, &function)) {
            log4g_log_error(Q_("%s: invalid `type'"), type);
            goto exit;
        }
        get_type = function;
        klass = g_type_class_ref(get_type());
    } else {
        klass = g_type_class_ref(LOG4G_TYPE_LEVEL);
    }
    if (!klass) {
        log4g_log_error(Q_("g_type_class_ref() returned NULL"));
        goto exit;
    }
    value = xmlGetProp(node, (const xmlChar *)"value");
    if (!value) {
        log4g_log_error(Q_("levels require a `value'"));
        goto exit;
    }
    if (!klass->string_to_level) {
        log4g_log_error(Q_("Log4gLevel virtual method "
                    "string_to_level() is NULL"));
        goto exit;
    }
    level = klass->string_to_level((gchar *)value);
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"property")) {
            parse_property(base, node, level);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
    log4g_logger_set_level(logger, level);
exit:
    if (klass) {
        g_type_class_unref(klass);
    }
    if (type) {
        xmlFree(type);
    }
    if (value) {
        xmlFree(value);
    }
}

static void
parse_logger(Log4gConfigurator *base, xmlNodePtr node)
{
    Log4gLogger *logger;
    xmlChar *additivity;
    xmlChar *name = NULL;
    name = xmlGetProp(node, (const xmlChar *)"name");
    if (!name) {
        log4g_log_error(Q_("loggers require a `name'"));
        goto exit;
    }
    logger = log4g_log_manager_get_logger((const gchar *)name);
    if (!logger) {
        goto exit;
    }
    additivity = xmlGetProp(node, (const xmlChar *)"additivity");
    if (additivity) {
        if (!xmlStrcmp(additivity, (const xmlChar *)"true")) {
            log4g_logger_set_additivity(logger, TRUE);
        } else if (!xmlStrcmp(additivity , (const xmlChar *)"false")) {
            log4g_logger_set_additivity(logger, FALSE);
        } else {
            log4g_log_error(Q_("%s: `additivity' must be a boolean value"),
                    additivity);
        }
        xmlFree(additivity);
    } else {
        log4g_logger_set_additivity(logger, TRUE);
    }
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"level")) {
            parse_level(base, node, logger);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"appender")) {
            Log4gAppender *appender = parse_appender(base, node);
            if (appender) {
                log4g_logger_add_appender(logger, appender);
                g_object_unref(appender);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
exit:
    if (name) {
        xmlFree(name);
    }
}

static void
parse_root(Log4gConfigurator *base, xmlNodePtr node)
{
    Log4gLogger *logger = log4g_log_manager_get_root_logger();
    if (!logger) {
        return;
    }
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"property")) {
            parse_property(base, node, logger);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"level")) {
            parse_level(base, node, logger);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"appender")) {
            Log4gAppender *appender = parse_appender(base, node);
            if (appender) {
                log4g_logger_add_appender(logger, appender);
                g_object_unref(appender);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
}

static void
error_handler(void *ctx, const char *format, ...)
{
    GString *string = (GString *)ctx;
    va_list ap;
    va_start(ap, format);
    g_string_append_vprintf(string, format, ap);
    if (string->str[string->len - 1] == '\n') {
        string->str[string->len - 1] = '\0';
        log4g_log_warn(string->str);
        g_string_set_size(string, 0);
    }
    va_end(ap);
}

static gboolean
do_configure(Log4gConfigurator *base, const char *uri,
        Log4gLoggerRepository *repository, GError **error)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    gboolean status = TRUE;
    xmlNodePtr node;
    xmlDocPtr doc;
    gint options;
    xmlChar *att;
    gchar *env;
    GString *string;
    string = g_string_sized_new(128);
    if (!string) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                Q_("g_string_new() returned NULL"));
        return FALSE;
    }
    xmlSetGenericErrorFunc(string, error_handler);
    options = XML_PARSE_NOWARNING | XML_PARSE_NOERROR | XML_PARSE_NOBLANKS;
    env = getenv("LOG4G_PARSE_DTDVALID");
    if (env) {
        options |= XML_PARSE_DTDVALID;
    }
    /* read XML file */
    doc = xmlCtxtReadFile(priv->ctx, uri, NULL, options);
    if (!doc) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                Q_("failed to parse configuration"));
        status = FALSE;
        goto exit;
    }
    /* check root element */
    node = xmlDocGetRootElement(doc);
    if (!node) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                Q_("invalid document: document is empty"));
        status = FALSE;
        goto exit;
    }
    if (xmlStrcmp(node->name, (const xmlChar *)"configuration")) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                Q_("%s: invalid root element (expected log4g:configuration)"),
                node->name);
        status = FALSE;
        goto exit;
    }
    /* parse root attributes */
    att = xmlGetProp(node, (const xmlChar *)"debug");
    if (att) {
        if (!xmlStrcmp(att, (const xmlChar *)"true")) {
            priv->debug = TRUE;
        } else if (!xmlStrcmp(att, (const xmlChar *)"false")) {
            priv->debug = FALSE;
        } else if (!xmlStrcmp(att, (const xmlChar *)"null")) {
            log4g_log_warn(Q_("%s: ignoring `debug' attribute"), att);
        } else {
            log4g_log_error(Q_("%s: invalid value for attribute `debug'"),
                    att);
        }
        xmlFree(att);
    }
    att = xmlGetProp(node, (const xmlChar *)"reset");
    if (att) {
        if (!xmlStrcmp(att, (const xmlChar *)"true")) {
            log4g_log_manager_reset_configuration();
        }
        xmlFree(att);
    }
    att = xmlGetProp(node, (const xmlChar *)"threshold");
    if (att) {
        Log4gLevel *level;
        Log4gLoggerRepository *repository =
            log4g_log_manager_get_logger_repository();
        if (!xmlStrcmp(att, (const xmlChar *)"all")) {
            level = log4g_level_ALL();
        } else if (!xmlStrcmp(att, (const xmlChar *)"trace")) {
            level = log4g_level_TRACE();
        } else if (!xmlStrcmp(att, (const xmlChar *)"debug")) {
            level = log4g_level_DEBUG();
        } else if (!xmlStrcmp(att, (const xmlChar *)"info")) {
            level = log4g_level_INFO();
        } else if (!xmlStrcmp(att, (const xmlChar *)"warn")) {
            level = log4g_level_WARN();
        } else if (!xmlStrcmp(att, (const xmlChar *)"error")) {
            level = log4g_level_ERROR();
        } else if (!xmlStrcmp(att, (const xmlChar *)"fatal")) {
            level = log4g_level_FATAL();
        } else if (!xmlStrcmp(att, (const xmlChar *)"off")) {
            level = log4g_level_OFF();
        } else if (!xmlStrcmp(att, (const xmlChar *)"null")) {
            level = NULL;
        } else {
            log4g_log_error(Q_("%s: invalid repository threshold"), att);
            level = NULL;
        }
        if (level) {
            log4g_logger_repository_set_threshold(repository, level);
        }
        xmlFree(att);
    }
    /* parse document */
    node = node->xmlChildrenNode;
    while (node) {
        if (!xmlStrcmp(node->name, (const xmlChar *)"appender")) {
            Log4gAppender *appender = parse_appender(base, node);
            if (appender) {
                g_object_unref(appender);
            }
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"logger")) {
            parse_logger(base, node);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"root")) {
            parse_root(base, node);
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"text")) {
            log4g_log_warn(Q_("invalid text element"));
        } else if (!xmlStrcmp(node->name, (const xmlChar *)"comment")) {
            /* do nothing */
        } else {
            log4g_log_warn(Q_("%s: invalid element"), node->name);
        }
        node = node->next;
    }
exit:
    if (string) {
        g_string_free(string, TRUE);
    }
    if (doc) {
        xmlFreeDoc(doc);
    }
    return status;
}

static void
configurator_init(Log4gConfiguratorInterface *interface, gpointer data)
{
    interface->do_configure = do_configure;
}

G_DEFINE_TYPE_WITH_CODE(Log4gDOMConfigurator, log4g_dom_configurator,
        G_TYPE_OBJECT,
        G_IMPLEMENT_INTERFACE(LOG4G_TYPE_CONFIGURATOR, configurator_init))

static void
log4g_dom_configurator_init(Log4gDOMConfigurator *self)
{
    struct Log4gPrivate *priv = GET_PRIVATE(self);
    memset(priv, 0, sizeof(*priv));
    /* allocate resources */
    priv->scratch = g_string_sized_new(128);
    if (!priv->scratch) {
        return;
    }
    priv->module = g_module_open(NULL, G_MODULE_BIND_LAZY);
    if (!priv->module) {
        return;
    }
    priv->ctx = xmlNewParserCtxt();
    if (!priv->ctx) {
        return;
    }
    priv->appenders = g_hash_table_new_full(g_str_hash, g_str_equal,
                            xmlFree, g_object_unref);
    if (!priv->appenders) {
        return;
    }
}

static void
log4g_dom_configurator_finalize(GObject *base)
{
    struct Log4gPrivate *priv = GET_PRIVATE(base);
    if (priv->scratch) {
        g_string_free(priv->scratch, TRUE);
        priv->scratch = NULL;
    }
    if (priv->ctx) {
        xmlFreeParserCtxt(priv->ctx);
        priv->ctx = NULL;
    }
    if (priv->module) {
        g_module_close(priv->module);
        priv->module = NULL;
    }
    if (priv->appenders) {
        g_hash_table_destroy(priv->appenders);
        priv->appenders = NULL;
    }
    xmlCleanupParser();
    G_OBJECT_CLASS(log4g_dom_configurator_parent_class)->finalize(base);
}

static void
log4g_dom_configurator_class_init(Log4gDOMConfiguratorClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    /* initialize GObject */
    gobject_class->finalize = log4g_dom_configurator_finalize;
    /* initialize private data */
    g_type_class_add_private(klass, sizeof(struct Log4gPrivate));
}

Log4gConfigurator *
log4g_dom_configurator_new(void)
{
    Log4gConfigurator *self;
    gchar *debug;
    LIBXML_TEST_VERSION
    self = g_object_new(LOG4G_TYPE_DOM_CONFIGURATOR, NULL);
    if (!self) {
        return NULL;
    }
    debug = getenv("LOG4G_DEBUG");
    if (debug) {
        struct Log4gPrivate *priv = GET_PRIVATE(self);
        priv->debug = TRUE;
    }
    return self;
}

gboolean
log4g_dom_configurator_configure(const gchar *uri, GError **error)
{
    gboolean status;
    struct Log4gPrivate *priv;
    Log4gLoggerRepository *repository;
    Log4gConfigurator *self = log4g_dom_configurator_new();
    if (!self) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                "log4g_dom_configurator_new() returned NULL");
        return FALSE;
    }
    repository = log4g_log_manager_get_logger_repository();
    if (!repository) {
        g_set_error(error, LOG4G_ERROR, LOG4G_ERROR_FAILURE,
                "log4g_log_manager_get_logger_repository() returned NULL");
        g_object_unref(self);
        return FALSE;
    }
    priv = GET_PRIVATE(self);
    status = log4g_configurator_do_configure(self, uri, repository, error);
    g_object_unref(self);
    return status;
}
