/* quvi
 * Copyright (C) 2010  Toni Gundogdu <legatvs@gmail.com>
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

#include "config.h"

#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

#include "llst.h"
#include "quvi/quvi.h"

int llst_add(llst_node_t * node, void *data)
{
  struct _llst_node_s *n = 0;

  assert(node);

  n = calloc(1, sizeof(*n));
  if (!n)
    return (QUVI_MEM);

  if (*node)                    /* insert after the last. */
    {
      llst_node_t curr = *node;
      while (curr->next)
        curr = curr->next;
      curr->next = n;
      /* without the above, we would add the node to the beginning,
         see the else-block below. */
    }
  else                          /* add first node. */
    {
      n->next = *node;
      *node = n;
    }

  n->data = data;

  return (QUVI_OK);
}

int llst_size(llst_node_t head)
{
  llst_node_t curr = head;
  int n = 0;
  while (curr)
    {
      curr = curr->next;
      ++n;
    }
  return (n);
}

void llst_free(llst_node_t * head)
{
  llst_node_t curr = *head;
  assert(head);
  while (curr)
    {
      llst_node_t next = curr->next;
      free(curr->data);
      curr->data = 0;
      free(curr);
      curr = 0;
      curr = next;
    }
  *head = 0;
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
