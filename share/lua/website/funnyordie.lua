
-- quvi
-- Copyright (C) 2011  Toni Gundogdu
-- Copyright (C) 2010 quvi project
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

local FunnyOrDie = {} -- Utility functions unique to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "funnyordie.com"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/videos/"})
    return r
end

-- Query available formats.
function query_formats(self)
    local page    = quvi.fetch(self.page_url)
    local formats = FunnyOrDie.iter_formats(page)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, FunnyOrDie.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "funnyordie"
    local page   = quvi.fetch(self.page_url)

    self.title = page:match('"og:title" content="(.-)">')
                    or error ("no match: media title")

    self.id = page:match('key:%s+"(.-)"')
                or error ("no match: media id")

    self.thumbnail_url = page:match('"og:image" content="(.-)"') or ''

    local formats = FunnyOrDie.iter_formats(page)
    local U       = require 'quvi/util'
    self.url      = {U.choose_format(self, formats,
                                     FunnyOrDie.choose_best,
                                     FunnyOrDie.choose_default,
                                     FunnyOrDie.to_s).url
                        or error('no match: media url')}
    return self
end

--
-- Utility functions
--

function FunnyOrDie.iter_formats(page)
    local t = {}
    for u in page:gfind("'src',%s+'(.-)'") do
        local q,c = u:match('(%w+)%.(%w+)$')
        table.insert(t, {url=u, quality=q, container=c})
--        print(u,c)
    end
    return t
end

function FunnyOrDie.choose_best(formats) -- Last is 'best'
    local r = FunnyOrDie.choose_default(formats)
    for _,v in pairs(formats) do
        r = v
    end
    return r
end

function FunnyOrDie.choose_default(formats) -- First is 'default'
    for _,v in pairs(formats) do
        return v
    end
end

function FunnyOrDie.to_s(t)
    return string.format("%s_%s", t.container, t.quality)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
