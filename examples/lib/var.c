/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <stdio.h>
#include <quvi.h>

quvi_http_metainfo_t qmi = NULL;
quvi_subtitle_t qsub = NULL;
quvi_playlist_t qp = NULL;
quvi_resolve_t qr = NULL;
quvi_media_t qm = NULL;
quvi_scan_t qs = NULL;
quvi_t q = NULL;

/* vim: set ts=2 sw=2 tw=72 expandtab: */
