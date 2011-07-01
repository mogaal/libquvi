
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

--
-- NOTE: mp4s do not appear to be available (404) even if listed in the
--       config xml, this is the case at least with the test URL
--

local Spiegel = {} -- Utility functions unique to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "spiegel.de"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/video/"})
    return r
end

-- Query available formats.
function query_formats(self)
    Spiegel.get_media_id(self)

    local config  = Spiegel.get_config(self)
    local formats = Spiegel.iter_formats(config)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Spiegel.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "¦")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "spiegel"

    Spiegel.get_media_id(self)

    local playlist = Spiegel.get_playlist(self)

    local _,_,s = playlist:find("<headline>(.-)</")
    self.title  = s or error ("no match: media title")

    local config  = Spiegel.get_config(self)
    local formats = Spiegel.iter_formats(config)

    local U       = require 'quvi/util'
    local format  = U.choose_format(self, formats,
                                    Spiegel.choose_best,
                                    Spiegel.choose_default,
                                    Spiegel.to_s)

    self.duration = (format.duration or 0) * 1000 -- to msec
    self.url      = {format.url or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Spiegel.get_media_id(self)
    local _,_,s = self.page_url:find("/video/video%-(.-)%.")
    self.id     = s or error ("no match: media id")
end

function Spiegel.get_playlist(self)
    local fmt_s = "http://www1.spiegel.de/active/playlist/fcgi/playlist.fcgi/"
               .. "asset=flashvideo/mode=id/id=%s"

    local playlist_url = string.format(fmt_s, self.id)

    return quvi.fetch(playlist_url, {fetch_type = 'playlist'})
end

function Spiegel.get_config(self)
    local fmt_s      = "http://video.spiegel.de/flash/%s.xml"
    local config_url = string.format(fmt_s, self.id)
    return quvi.fetch(config_url, {fetch_type = 'config'})
end

function Spiegel.iter_formats(config)
    local p = '<filename>(.-)<'
           .. '.-<codec>(.-)<'
           .. '.-<totalbitrate>(%d+)'
           .. '.-<width>(%d+)'
           .. '.-<height>(%d+)'
           .. '.-<duration>(%d+)'
    local t = {}
    for fn,c,b,w,h,d in config:gfind(p) do
        local _,_,cn = fn:find('%.(%w+)$') 
        local u = 'http://video.spiegel.de/flash/' .. fn
--        print(u,c,b,w,h,cn,d)
        table.insert(t, {codec=string.lower(c), url=u,
                         width=tonumber(w),     height=tonumber(h),
                         bitrate=tonumber(b),   duration=tonumber(d),
                         container=cn})
    end
    return t
end

function Spiegel.choose_best(formats) -- Highest quality available
    local r = {width=0, height=0, bitrate=0, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_higher_quality(v,r) then
            r = v
        end
    end
    return r
end

function Spiegel.choose_default(formats) -- Lowest quality available
    local r = {width=0xffff, height=0xffff, bitrate=0xffff, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_lower_quality(v,r) then
            r = v
        end
    end
    return r
end

function Spiegel.to_s(t)
    return string.format('%s_%s_%sk_%sp',
        t.container, t.codec, t.bitrate, t.height)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
