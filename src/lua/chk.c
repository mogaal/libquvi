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
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "lua/chk.h"
#include "lua/def.h"
#include "misc/re.h"

/*
 * NOTE: The error messages produced in these functions are intended for
 * developers. They would typically be seen when a new script is being
 * developed.
 *
 * The messages should be clear, indicating the actual error, minimizing
 * the time spent on locating the actual problem in the script.
 */

gboolean l_chk_can_parse_url(lua_State *l, _quvi_script_t qs,
                             const gchar *k_can_parse_url,
                             const gchar *k_domains,
                             const gchar *script_func)
{
  gboolean r = FALSE;

  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      l_chk_assign_s(l, k_domains, qs->domains, TRUE);
      l_chk_assign_b(l, k_can_parse_url, &r);
      lua_pop(l, 1);
    }
  if (qs->domains->len ==0)
    {
      luaL_error(l, "%s: %s: the returned dictionary must contain "
                 "a string value `%s'",
                 qs->fpath->str, script_func, k_domains);
    }
  return (r);
}

/*
 * Return the value of the named (`w') string. The value is trimmed
 * of any extra whitespace (e.g. leading, trailing).
 *
 * NOTE: g_free the returned value when done using it.
 */
gboolean l_chk_s(lua_State *l, const gchar *w, gchar **v, gboolean trim_flag)
{
  if (lua_isstring(l, LI_KEY) && lua_isstring(l, LI_VALUE))
    {
      if (g_strcmp0(lua_tostring(l, LI_KEY), w) == 0)
        {
          const gchar *s = lua_tostring(l, LI_VALUE);
          *v = (trim_flag == TRUE)
               ? m_trim_ws(s)
               : g_strdup(s);
          return (TRUE);
        }
    }
  return (FALSE);
}

gboolean l_chk_assign_s(lua_State *l, const gchar *k, GString *v,
                        gboolean trim_flag)
{
  gchar *s = NULL;
  if (l_chk_s(l, k, &s, trim_flag) == TRUE)
    {
      g_string_assign(v, s);
      g_free(s);
      return (TRUE);
    }
  return (FALSE);
}

gboolean l_chk_n(lua_State *l, const gchar *w, gdouble *v)
{
  if (lua_isstring(l, LI_KEY) && lua_isnumber(l, LI_VALUE))
    {
      if (g_strcmp0(lua_tostring(l, LI_KEY), w) == 0)
        {
          *v = lua_tonumber(l, LI_VALUE);
          return (TRUE);
        }
    }
  return (FALSE);
}

gboolean l_chk_assign_n(lua_State *l, const gchar *k, gdouble *v)
{
  gdouble n = 0;
  if (l_chk_n(l, k, &n) == TRUE)
    {
      *v = n;
      return (TRUE);
    }
  return (FALSE);
}

gboolean l_chk_b(lua_State *l, const gchar *w, gboolean *v)
{
  if (lua_isstring(l, LI_KEY) && lua_isboolean(l, LI_VALUE))
    {
      if (g_strcmp0(lua_tostring(l, LI_KEY), w) == 0)
        {
          *v = lua_toboolean(l, LI_VALUE);
          return (TRUE);
        }
    }
  return (FALSE);
}

gboolean l_chk_assign_b(lua_State *l, const gchar *k, gboolean *v)
{
  gboolean b = 0;
  if (l_chk_b(l, k, &b) == TRUE)
    {
      *v = b;
      return (TRUE);
    }
  return (FALSE);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
