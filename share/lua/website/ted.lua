
-- quvi
-- Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
-- Copyright (C) 2011  Bastien Nocera <hadess@hadess.net>
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

local Ted = {} -- Utility functions unique to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "ted.com"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/talks/.+$"})
    return r
end

-- Query available formats.
function query_formats(self)
    local page    = quvi.fetch(self.page_url)
    local formats = Ted.iter_formats(page)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Ted.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse video URL.
function parse(self)
    self.host_id = "ted"
    local page   = quvi.fetch(self.page_url)

    self.id      = page:match('ti:"(%d+)"')
                    or error("no match: media id")

    self.title   = page:match('<title>(.-)%s+|')
                    or error("no match: media title")

    self.thumbnail_url = page:match('&amp;su=(.-)&amp;') or ''

    local formats = Ted.iter_formats(page)
    local U       = require 'quvi/util'
    self.url      = {U.choose_format(self, formats,
                                     Ted.choose_best,
                                     Ted.choose_default,
                                     Ted.to_s).url
                        or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Ted.iter_formats(page)
    local pp = 'http://download.ted.com'
    local p  = 'href="' ..pp.. '(.-)"'
    local t  = {}
    for u in page:gfind(p) do
        local c = u:match('%.(%w+)$') or error('no match: container')
        local q = u:match('%-(%w+)%.%w+$') -- nil is acceptable here
        u = pp .. u
        if not Ted.find_duplicate(t,u) then
            table.insert(t, {url=u, container=c, quality=q})
--            print(u,c,q)
        end
    end
    return t
end

function Ted.find_duplicate(t,u)
    for _,v in pairs(t) do
        if v.url == u then return true end
    end
    return false
end

function Ted.choose_best(formats) -- Last 'mp4' is the 'best'
    local r = Ted.choose_default(formats)
    local p = '(%d+)p'
    for _,v in pairs(formats) do
        if v.container:match('mp4') then
            if v.quality then
                local a = v.quality:match(p)
                local b = (r.quality) and r.quality:match(p) or 0
                if a and tonumber(a) > tonumber(b) then
                    r = v
                end
            else
                r = v
            end
        end
    end
    return r
end

function Ted.choose_default(formats) -- First 'mp4' is the 'default'
    local r -- Return this if mp4 is not found for some reason
    for _,v in pairs(formats) do
        if v.container:match('mp4') then
            return v
        end
        r = v
    end
    return r
end

function Ted.to_s(t)
    return (t.quality)
        and string.format("%s_%s", t.container, t.quality)
        or  string.format("%s", t.container)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
