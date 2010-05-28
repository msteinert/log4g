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
 * \file
 * \brief Quickly configure Log4g.
 * \author Mike Steinert
 * \date 2-22-2010
 *
 * For file based configuration, see log4g/dom-configurator.h.
 */

#ifndef LOG4G_BASIC_CONFIGURATOR_H
#define LOG4G_BASIC_CONFIGURATOR_H

#include <log4g/appender.h>

G_BEGIN_DECLS

/**
 * \brief Quickly configure Log4g.
 *
 * This function adds a console appender to the
 * \ref log4g/root-logger.h "root logger" that uses the TTCC layout and
 * prints to \e stdout.
 */
void
log4g_basic_configurator_configure(void);

/**
 * \brief Add an appender to the root category.
 *
 * \param appender [in] The appender to add to the root category.
 */
void
log4g_basic_configurator_configure_appender(Log4gAppender *appender);

/**
 * \brief Reset configuration.
 *
 * Reset the default hierarchy to its default state.
 */
void
log4g_basic_configurator_reset_configuration(void);

G_END_DECLS

#endif /* LOG4G_BASIC_CONFIGURATOR_H */
