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

#ifndef quvi_llst_h
#define quvi_llst_h

/* Title: llst.h */

/*
 * Typedef: Handles
 *
 * quvi_llst_node_t Node handle
 */
typedef void *quvi_llst_node_t;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  QUVIcode         quvi_llst_append(quvi_llst_node_t *node, void *data);
  size_t           quvi_llst_size(quvi_llst_node_t node);
  quvi_llst_node_t quvi_llst_next(quvi_llst_node_t node);
  void            *quvi_llst_data(quvi_llst_node_t node);
  void             quvi_llst_free(quvi_llst_node_t *node);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* quvi_llst_h */

/* vim: set ts=2 sw=2 tw=72 expandtab: */
