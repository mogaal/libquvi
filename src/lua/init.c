/* libquvi
 * Copyright (C) 2012,2013  Toni Gundogdu <legatvs@gmail.com>
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

/* 'quvi' object functions for scripts, e.g. 'quvi.http.fetch' */

extern gint l_quvi_http_metainfo(lua_State*);
extern gint l_quvi_http_resolve(lua_State*);
extern gint l_quvi_http_cookie(lua_State*);
extern gint l_quvi_http_header(lua_State*);
extern gint l_quvi_http_fetch(lua_State*);

static const luaL_Reg quvi_reg_meth[] =
{
  {NULL, NULL}
};

static const luaL_Reg quvi_http_reg_meth[] =
{
  {"metainfo", l_quvi_http_metainfo},
  {"resolve", l_quvi_http_resolve},
  {"cookie", l_quvi_http_cookie},
  {"header", l_quvi_http_header},
  {"fetch", l_quvi_http_fetch},
  {NULL, NULL}
};

extern gint l_quvi_crypto_encrypt(lua_State*);
extern gint l_quvi_crypto_decrypt(lua_State*);
extern gint l_quvi_crypto_hash(lua_State*);

static const luaL_Reg quvi_crypto_reg_meth[] =
{
  {"encrypt", l_quvi_crypto_encrypt},
  {"decrypt", l_quvi_crypto_decrypt},
  {"hash",    l_quvi_crypto_hash},
  {NULL, NULL}
};

extern gint l_quvi_base64_encode(lua_State*);
extern gint l_quvi_base64_decode(lua_State*);

static const luaL_Reg quvi_base64_reg_meth[] =
{
  {"encode",    l_quvi_base64_encode},
  {"decode",    l_quvi_base64_decode},
  {NULL, NULL}
};

QuviError l_init(_quvi_t q)
{
  q->handle.lua = (lua_State*) luaL_newstate();
  if (q->handle.lua == NULL)
    return (QUVI_ERROR_LUA_INIT);

  luaL_openlibs(q->handle.lua);
  luaL_register(q->handle.lua, "quvi", quvi_reg_meth);
  luaL_register(q->handle.lua, "quvi.http", quvi_http_reg_meth);
  luaL_register(q->handle.lua, "quvi.crypto", quvi_crypto_reg_meth);
  luaL_register(q->handle.lua, "quvi.base64", quvi_base64_reg_meth);

  return (QUVI_OK);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
