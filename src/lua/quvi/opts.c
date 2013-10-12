/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
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
#include <glib.h>
#include <curl/curl.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
/* -- */
#include "lua/quvi/opts.h"
#include "lua/def.h"
#include "misc/slst.h"

static gpointer _opt_new(const QuviObjectOption qoo, const gchar *vs,
                         const gdouble vn)
{
  l_quvi_object_opt_t o = g_new0(struct l_quvi_object_opt_s, 1);
  o->value.str = g_strdup(vs);
  o->value.n = vn;
  o->id = qoo;
  return (o);
}

/* Return a single-linked list of options passed with quvi.* function. */
GSList *l_quvi_object_opts_new(lua_State *l, gint index)
{
  l_quvi_object_opt_t o;
  GSList *r;
  gdouble k;
  gint t;

  r = NULL;

  if (!lua_istable(l, index))
    return (NULL);

  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      if (lua_isnumber(l, LI_KEY))
        {
          k = lua_tonumber(l, LI_KEY);
          t = lua_type(l, LI_VALUE);
          o = NULL;

          switch (t)
            {
            case LUA_TSTRING:
              o = _opt_new(k, lua_tostring(l, LI_VALUE), 0);
              break;
            case LUA_TNUMBER:
              o = _opt_new(k, NULL, lua_tonumber(l, LI_VALUE));
              break;
            case LUA_TBOOLEAN:
              o = _opt_new(k, NULL, (gdouble)
                           (lua_toboolean(l, LI_VALUE) ? TRUE:FALSE) );
              break;
            default:
              g_warning("[%s] ignored: unsupported lua type=0x%x",
                        __func__, t);
              break;
            }

          if (o != NULL)
            r = g_slist_prepend(r, o);
        }
      lua_pop(l, 1);
    }
  return (g_slist_reverse(r));
}

static void _opt_free(gpointer p, gpointer userdata)
{
  l_quvi_object_opt_t o = (l_quvi_object_opt_t) p;

  if (p == NULL)
    return;

  g_free(o->value.str);
  o->value.str = NULL;

  g_free(o);
  o = NULL;
}

void l_quvi_object_opts_free(GSList *p)
{
  m_slist_free_full(p, _opt_free);
}

#ifdef _UNUSED
void l_quvi_object_opts_curl(GSList *opts, _quvi_t q)
{
  l_quvi_object_opt_t o;
  GSList *p;

  if (opts == NULL)
    return;

  if (l_quvi_object_opts_is_set(q->handle.lua, opts,
                                QUVI_OBJECT_OPTION_HTTP_USER_AGENT, &p,
                                NULL, FALSE)
      == TRUE)
    {
      o = (l_quvi_object_opt_t) p->data;
      curl_easy_setopt(q->handle.curl, CURLOPT_USERAGENT, o->value.str);
    }
}
#endif /* _UNUSED */

gboolean l_quvi_object_opts_is_set(lua_State *l, GSList *opts,
                                   const QuviObjectOption qoo, GSList **dst,
                                   const gchar *w,
                                   const gboolean croak_if_error)

{
  *dst = opts;
  while (*dst != NULL)
    {
      l_quvi_object_opt_t o = (l_quvi_object_opt_t) (*dst)->data;
      if (o->id == qoo)
        return (TRUE);
      *dst = g_slist_next(*dst);
    }

  if (croak_if_error == TRUE && w != NULL)
    luaL_error(l, "%s is required", w);

  return (FALSE);
}

void l_quvi_object_opts_chk_given(lua_State *l, GSList *opts,
                                  const gchar *w)
{
  if (opts == NULL)
    luaL_error(l, "expects a table of %s options passed as an arg", w);
}

gboolean l_quvi_object_opts_croak_if_error(lua_State *l, GSList *opts)
{
  GSList *p;

  if (opts == NULL)
    return (TRUE);

  if (l_quvi_object_opts_is_set(l, opts, QUVI_OBJECT_OPTION_CROAK_IF_ERROR,
                                &p, NULL, FALSE)
      == TRUE)
    {
      l_quvi_object_opt_t o = (l_quvi_object_opt_t) p->data;
      return ((o->value.n) != 0 ? TRUE:FALSE);
    }
  return (TRUE);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
