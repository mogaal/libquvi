
-- quvi
-- Copyright (C) 2010,2011  Toni Gundogdu <legatvs@gmail.com>
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

function M.slash_unescape (s)
    s = s:gsub ('\\(.)', '%1')
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

function M.is_higher_quality(a,b)
    if a.height > b.height then
        if a.width > b.width then
            if a['bitrate'] then -- Optional: bitrate
                if a.bitrate > b.bitrate then
                    return true
                end
            else
                return true
            end
        end
    end
    return false
end

function M.is_lower_quality(a,b)
    if a.height < b.height then
        if a.width < b.width then
            if a['bitrate'] then -- Optiona: bitrate
                if a.bitrate < b.bitrate then
                    return true
                end
            else
                return true
            end
        end
    end
    return false
end

function M.choose_format(self,
                         formats,
                         callback_choose_best,
                         callback_choose_default,
                         callback_to_s)
    local r_fmt = self.requested_format

    if r_fmt == 'best' then
        return callback_choose_best(formats)
    end

    for _,v in pairs(formats) do
        if r_fmt == callback_to_s(v) then
            return v
        end
    end

    return callback_choose_default(formats)
end

function M.to_timestamp(s) -- Based on <http://is.gd/ee9ZTD>
    local p = "%a+, (%d+) (%a+) (%d+) (%d+):(%d+):(%d+)"

    local d,m,y,hh,mm,ss = s:match(p)
    if not d then error('no match: date') end

    local MON = {Jan=1, Feb=2, Mar=3, Apr=4, May=5, Jun=6, Jul=7, Aug=8,
                 Sep=9, Oct=10, Nov=11, Dec=12}

    local m = MON[m]

    local offset = os.time() - os.time(os.date("!*t"))

    return os.time({day=d,month=m,year=y,
                    hour=hh,min=mm,sec=ss}) + offset
end

return M

-- vim: set ts=4 sw=4 tw=72 expandtab:
