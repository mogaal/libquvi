
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

local Golem = {} -- Utility functions unique to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "video.golem.de"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/[%w-_]+/%d+/"})
    return r
end

-- Query available formats.
function query_formats(self)
    local config  = Golem.get_config(self)
    local formats = Golem.iter_formats(config)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Golem.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "golem"
    local config = Golem.get_config(self)

    local _,_,s  = config:find("<title>(.-)</")
    self.title   = s or error("no match: media title")

    local _,_,s  = config:find('<teaser.-<url>(.-)<.-</teaser>')
    if s then
        self.thumbnail_url = string.format('http://video.golem.de%s', s)
    end

    local formats = Golem.iter_formats(config)
    local U       = require 'quvi/util'
    self.url      = {U.choose_format(self, formats,
                                     Golem.choose_best,
                                     Golem.choose_default,
                                     Golem.to_s).url
                        or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Golem.get_config(self)
    local _,_,s = self.page_url:find('/[%w-_]+/(%d+)/')
    self.id = s or error("no match: media id")

    local config_url = "http://video.golem.de/xml/" .. self.id
    return quvi.fetch(config_url, {fetch_type = 'config'})
end

function Golem.iter_formats(config)
    local p = '<(%w+) width="(%d+)" height="(%d+)">'
           .. '.-<filename>.-%.(%w+)<'
           .. '.-<url>(.-)<'
           .. '.-</teaser>'
    local t = {}
    for id,w,h,c,u in config:gfind(p) do
            u = 'http://video.golem.de' .. u
--            print(id,w,h,c,u)
            table.insert(t, {width=tonumber(w), height=tonumber(h),
                             container=c, url=u, id=id})
    end

    return t
end

function Golem.choose_best(formats) -- Highest quality available
    local r = {width=0, height=0, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_higher_quality(v,r) then
            r = v
        end
    end
    return r
end

function Golem.choose_default(formats)
    local r = {width=0xffff, height=0xffff, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_lower_quality(v,r) then
            r = v
        end
    end
    return r
end

function Golem.to_s(t)
    return string.format("%s_%s_%sp", t.container, t.id, t.height)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
