/* 
* Copyright (C) 2010 Toni Gundogdu.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef llst_h
#define llst_h

struct _llst_node_s {
  struct _llst_node_s *next;
  struct _llst_node_s *prev;
  void *data;
};

typedef struct _llst_node_s *llst_node_t;

int llst_add(llst_node_t *, void *);

int llst_size(llst_node_t);

void llst_free(llst_node_t *);

#endif
