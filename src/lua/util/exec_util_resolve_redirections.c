/* libquvi
 * Copyright (C) 2012  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "config.h"

#include <lauxlib.h>
#include <lualib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_media_s.h"
/* -- */
#include "lua/load_util_script.h"
#include "lua/setfield.h"
#include "lua/def.h"

static const gchar script_fname[]= "resolve_redirections.lua";
static const gchar script_func[] = "resolve_redirections";

/* Resolve URL redirections with exception rules. */
gchar *l_exec_util_resolve_redirections(_quvi_t q, const gchar *url)
{
  lua_State *l;
  gchar *r;

  q->status.rc = l_load_util_script(q, script_fname, script_func);

  if (quvi_ok(q) == QUVI_FALSE)
    return (NULL);

  l = q->handle.lua;
  l_setfield_s(l, US_INPUT_URL, url, -1); /* Set as qargs.input_url */

  /*
   * 1=qargs [qargs: set in l_load_util_script]
   * 1=returns a string
   */
  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(q->status.errmsg, lua_tostring(l, -1));

      /* Keep error code if it was set by a callback: quvi.resolve
       * calling the network callback responsible for resolving URL
       * redirections. The error is most likely a network error. */
      if (q->status.rc != QUVI_ERROR_CALLBACK)
        q->status.rc = QUVI_ERROR_SCRIPT;

      return (NULL);
    }

  r = NULL;

  if (lua_isstring(l, -1))
    {
      const gchar *s = lua_tostring(l, -1);
      if (g_strcmp0(s, url) != 0) /* Ignore, unless it is different. */
        r = g_strdup(s);
    }
  else
    luaL_error(l, "%s: did not return a string", script_func);

  lua_pop(l, 1);

  /* quvi.resolve which is called from the script sets q->status.rc . */
  return (r);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
