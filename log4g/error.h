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

#ifndef LOG4G_ERROR_H
#define LOG4G_ERROR_H

G_BEGIN_DECLS

#define LOG4G_ERROR log4g_error_quark()

/**
 * Log4gError:
 * @LOG4G_ERROR_FAILURE: General Log4g failure
 * @LOG4G_ERROR_MAX: Sentinel value
 */
typedef enum {
    LOG4G_ERROR_FAILURE,
    LOG4G_ERROR_MAX
} Log4gError;

GQuark
log4g_error_quark(void);

G_END_DECLS

#endif /* LOG4G_ERROR_H */
