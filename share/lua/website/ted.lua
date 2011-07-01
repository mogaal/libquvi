
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
    self.host_id  = "ted"

    local page = quvi.fetch(self.page_url)

    local _,_,s = page:find('ti:"(%d+)"')
    self.id = s or error("no match: media id")

    local _,_,s = page:find('<span id="altHeadline">(.-)</span>')
    local U      = require 'quvi/util'
    self.title  = U.unescape(s) or error("no match: media title")

    local _,_,s = page:find('&amp;su=(.-)&amp;')
    self.thumbnail_url = s or ""

    local formats = Ted.iter_formats(page)
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
    --
    -- Assume first mp4 to be the 'sd' and the following (if any) the
    -- 'hd'. Some media have also audio tracks (e.g. 'mp3').
    --
    local have_sd = false
    local t = {}
    for src,c in page:gfind('href="(.-)">.-%((%w+)%)') do
        if src:find('/download/') then
            local u,q = 'http://www.ted.com' .. src
            c         = string.lower(c)
            if c == 'mp4' then
                if not have_sd then
                    have_sd = true
                    q = 'sd'
                else
                    q = 'hd'
                end
            end
--            print(u,c,q)
            table.insert(t, {url=u, container=c, quality=q})
        end
    end
    return t
end

function Ted.choose_best(formats) -- Last 'mp4' is the 'best'
    local r = Ted.choose_default(formats)
    for _,v in pairs(formats) do
        if v.container == 'mp4' then
            r = v
        end
    end
    return r
end

function Ted.choose_default(formats) -- First 'mp4' is the 'default'
    local r -- Return this if mp4 is not found for some reason
    for _,v in pairs(formats) do
        if v.container == 'mp4' then
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
