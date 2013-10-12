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

/*
 * NOTE: The error messages produced in these functions are intended for
 *       developers. They would typically be seen when a new script is
 *       being developed or an old one is being maintained.
 *
 *       These messages should be clear, indicating the actual error,
 *       minimizing the time spent on locating the problem in the script.
 */

#include "config.h"

#include <lauxlib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_export_s.h"
#include "_quvi_subtitle_s.h"
#include "_quvi_script_s.h"
/* -- */
#include "misc/subtitle_export.h"
#include "lua/setfield.h"
#include "lua/chk.h"
#include "lua/def.h"

static const gchar script_func[] = "export";

static QuviError _chk_result(lua_State *l, _quvi_subtitle_export_t qse,
                             const gchar *script_fpath)
{
  lua_pushnil(l);
  while (lua_next(l, LI_KEY))
    {
      l_chk_assign_s(l, SUES_DATA, qse->data, FALSE);
      lua_pop(l, 1);
    }

  if (qse->data->len ==0)
    {
      luaL_error(l, "%s: %s: must return `qargs.data'",
                 script_fpath, script_func);
    }
  lua_pop(l, 1);
  return (QUVI_OK);
}

QuviError l_exec_subtitle_export_script_export(gpointer p, GSList *sl)
{
  _quvi_subtitle_export_t qse;
  _quvi_script_t qs;
  lua_State *l;

  qse = (_quvi_subtitle_export_t) p;
  l = qse->handle.quvi->handle.lua;

  qs = (_quvi_script_t) sl->data;
  lua_getglobal(l, script_func);

  if (!lua_isfunction(l, -1))
    {
      luaL_error(l, "%s: the function `%s' was not found",
                 qs->fpath->str, script_func);
    }

  lua_newtable(l);
  l_set_reg_userdata(l, USERDATA_QUVI_T, (gpointer) qse->handle.quvi);
  l_setfield_s(l, SUES_INPUT_URL, qse->url.input->str, -1);
  l_setfield_n(l, SUES_FROM_FORMAT, qse->format.from);

  if (lua_pcall(l, 1, 1, 0))
    {
      g_string_assign(qse->handle.quvi->status.errmsg, lua_tostring(l, -1));
      return (QUVI_ERROR_SCRIPT);
    }

  if (!lua_istable(l, -1))
    {
      luaL_error(l, "%s: %s: must return a dictionary, this is "
                 "typically the `qargs'", qs->fpath->str, script_func);
    }
  return (_chk_result(l, qse, qs->fpath->str));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
