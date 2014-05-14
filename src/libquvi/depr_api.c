/* libquvi
 * Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

/*
 * Title: quvi API: Deprecated
 */

#include "quvi/quvi.h"

/*
 * Function: quvi_next_videolink
 *
 * Deprecated since 0.2.15. Removed in 0.4.0, use
 * <quvi_next_media_url> instead.
 */
QUVIcode quvi_next_videolink(void *handle);

/*
 * Function: quvi_next_host
 *
 * Deprecated since 0.2.0. Removed in 0.4.0, use
 * <quvi_next_supported_website> instead.
 */
QUVIcode quvi_next_host(char **domain, char **formats);

/* vim: set ts=2 sw=2 tw=72 expandtab: */
