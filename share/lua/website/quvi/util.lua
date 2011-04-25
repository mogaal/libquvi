
-- quvi
-- Copyright (C) 2010  Toni Gundogdu <legatvs@gmail.com>
--
-- This file is part of quvi <http://quvi.sourceforge.net/>.
--
-- This library is free software; you can redistribute it and/or
-- modify it under the terms of the GNU Lesser General Public
-- License as published by the Free Software Foundation; either
-- version 2.1 of the License, or (at your option) any later version.
--
-- This library is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
-- Lesser General Public License for more details.
--
-- You should have received a copy of the GNU Lesser General Public
-- License along with this library; if not, write to the Free Software
-- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
-- 02110-1301  USA
--

local M = {}

-- http://www.lua.org/pil/20.3.html
function M.decode (s)
    r = {}
    for n,v in s:gfind ("([^&=]+)=([^&=]+)") do
        n = M.unescape (n)
        r[n] = v
    end
    return r
end

-- http://www.lua.org/pil/20.3.html
function M.unescape (s)
    s = s:gsub ('+', ' ')
    s = s:gsub ('%%(%x%x)', function (h)
            return string.char (tonumber (h, 16))
        end)
    return s
end

-- handles
--  Check whether a website script can "handle" the specified URL
-- Params:
--  url     .. video page URL
--  domains .. table of domain names
--  paths   .. table of URL path patterns to match
--  queries .. table of URL query patterns to match
function M.handles(url, domains, paths, queries)
    if not url or not domains then
        return false
    end
    local U = require 'quvi/url'
    local t = U.parse(url)
--    for k,v in pairs(t) do print(k,v) end
    local r = M.contains(t.host, domains)
    if r then
        if paths then
            r = M.contains(t.path, paths)
        end
        if r then
            if queries then
                if t.query then
                    r = M.contains(t.query, queries)
                else
                    r = false
                end
            end
        end
    end
    return r
end

function M.contains(s,t)
    if not s then return false end
    for k,v in pairs(t) do
        if s:find(v) then return true end
    end
    return false
end

function M.ends(s,e) -- http://lua-users.org/wiki/StringRecipes
    return e == '' or s:sub(-#e) == e
end

return M

-- vim: set ts=4 sw=4 tw=72 expandtab:
