
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

-- w/ HD: <http://vimeo.com/1485507>
-- no HD: <http://vimeo.com/10772672>

local Vimeo = {} -- Utility functions unique to this script.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "vimeo.com"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/%d+$"})
    return r
end

-- Query available formats.
function query_formats(self)
    local config  = Vimeo.get_config(self)
    local formats = Vimeo.iter_formats(self, config)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Vimeo.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id  = "vimeo"
    local config  = Vimeo.get_config(self)

    local _,_,s = config:find("<caption>(.-)</")
    self.title  = s or error("no match: media title")

    local _,_,s = config:find('<duration>(%d+)')
    self.duration = (tonumber(s) or 0) * 1000 -- to msec

    local _,_,s = config:find('<thumbnail>(.-)<')
    self.thumbnail_url = s or ''

    local formats = Vimeo.iter_formats(self, config)
    local U       = require 'quvi/util'
    self.url      = {U.choose_format(self, formats,
                                     Vimeo.choose_best,
                                     Vimeo.choose_default,
                                     Vimeo.to_s).url
                        or error("no match: media url")}
    return self
end

--
-- Utility functions
--

function Vimeo.normalize(url)
    url = url:gsub("player.", "") -- player.vimeo.com
    url = url:gsub("/video/", "/") -- player.vimeo.com
    return url
end

function Vimeo.get_config(self)
    self.page_url = Vimeo.normalize(self.page_url)

    local _,_,s   = self.page_url:find('vimeo.com/(%d+)')
    self.id       = s or error("no match: media id")

    local config_url = "http://vimeo.com/moogaloop/load/clip:" .. self.id
    local config = quvi.fetch(config_url, {fetch_type = 'config'})

    if config:find('<error>') then
        local _,_,s = config:find('<message>(.-)[\n<]')
        error( (not s) and "no match: error message" or s )
    end

    return config
end

function Vimeo.iter_formats(self, config)
    local _,_,s = config:find('<isHD>(%d+)')
    local isHD  = tonumber(s) or 0

    local t = {}
    Vimeo.add_format(self, config, t, 'sd')
    if isHD == 1 then
        Vimeo.add_format(self, config, t, 'hd')
    end

    return t
end

function Vimeo.add_format(self, config, t, quality)
    table.insert(t,
        {quality=quality,
         url=Vimeo.to_url(self, config, quality)})
end

function Vimeo.choose_best(formats) -- Last is 'best'
    local r
    for _,v in pairs(formats) do r = v end
    return r
end

function Vimeo.choose_default(formats) -- First is 'default'
    for _,v in pairs(formats) do return v end
end

function Vimeo.to_url(self, config, quality)
    local _,_,s = config:find("<request_signature>(.-)</")
    local sign  = s or error("no match: request signature")

    local _,_,s = config:find("<request_signature_expires>(.-)</")
    local exp   = s or error("no match: request signature expires")

    local fmt_s = "http://vimeo.com/moogaloop/play/clip:%s/%s/%s/?q=%s"

    return string.format(fmt_s, self.id, sign, exp, quality)
end

function Vimeo.to_s(t)
    return string.format("%s", t.quality)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
