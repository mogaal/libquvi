
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

local YouTube = {} -- Utility functions unique to this script

-- <http://en.wikipedia.org/wiki/YouTube#Quality_and_codecs>

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "youtube.com"
    r.formats    = "default|best"
    r.categories  = C.proto_http
    self.page_url = YouTube.normalize(self.page_url)
    local U       = require 'quvi/util'
    r.handles     = U.handles(self.page_url,
                        {r.domain}, {"/watch"}, {"v=[%w-_]+"})
    return r
end

-- Query available formats.
function query_formats(self)
    local config,U  = YouTube.get_config(self)
    local formats   = YouTube.iter_formats(config, U)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, YouTube.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse URL.
function parse(self)
    self.host_id   = "youtube"
    local page_url = YouTube.normalize(self.page_url)

    local _,_,s = page_url:find('#a?t=(.+)')
    self.start_time = s or ''

    return YouTube.get_video_info(self)
end

--
-- Utility functions
--

function YouTube.normalize(url)
    if not url then return url end
    url = url:gsub("-nocookie", "")        -- youtube-nocookie.com
    url = url:gsub("/v/", "/watch?v=")     -- embedded
    url = url:gsub("/embed/", "/watch?v=") -- embedded
    url = url:gsub("/e/", "/watch?v=")     -- embedded
    url = url:gsub("youtu%.be/","youtube.com/watch?v=") -- youtu.be
    return url
end

function YouTube.get_config(self)
    local _,_,s  = self.page_url:find('^(%w+)://')
    local scheme = s or error("no match: scheme")

    local page_url = YouTube.normalize(self.page_url)

    local _,_,s = page_url:find("v=([%w-_]+)")
    self.id = s or error("no match: media id")

    local s_fmt = "%s://www.youtube.com/get_video_info?&video_id=%s"
                    .. "&el=detailpage&ps=default&eurl=&gl=US&hl=en"

    local config_url = string.format(s_fmt, scheme, self.id)

    local U      = require 'quvi/util'
    local config = U.decode(quvi.fetch(config_url, {fetch_type='config'}))

    if config['reason'] then
        local reason = U.unescape(config['reason'])
        local code   = config['errorcode']
        error(string.format("%s (code=%s)", reason, code))
    end

    return config,U
end

function YouTube.iter_formats(config, U)
    local fmt_stream_map = config['url_encoded_fmt_stream_map']
                        or error("no match: url_encoded_fmt_stream_map")

    fmt_stream_map = U.unescape(fmt_stream_map) .. ","

    local urls = {}
    for f in fmt_stream_map:gfind('([^,]*),') do
        local d = U.decode(f)
        if d['itag'] and d['url'] then
            urls[U.unescape(d['itag'])] = U.unescape(d['url'])
        end
    end

    local fmt_map = config['fmt_list'] or error("no match: fmt_list")
    fmt_map = U.unescape(fmt_map)

    local r = {}
    for f,w,h in fmt_map:gfind('(%d+)/(%d+)x(%d+)') do
--        print(f,w,h)
        table.insert(r, {fmt_id=tonumber(f),    url=urls[f],
                          width=tonumber(w), height=tonumber(h)})
    end

    return r
end

function YouTube.get_video_info(self)
    local config,U = YouTube.get_config(self)

    self.title = config['title'] or error('no match: media title')
    self.title = U.unescape(self.title)

    self.thumbnail_url = config['thumbnail_url'] or ''
    if #self.thumbnail_url > 0 then
        self.thumbnail_url = U.unescape(self.thumbnail_url)
    end
    
    self.duration = (config['length_seconds'] or 0)*1000 -- to msec

    self.requested_format =
        YouTube.convert_deprecated_id(self.requested_format)

    local formats = YouTube.iter_formats(config, U)
    local url     = U.choose_format(self, formats,
                                    YouTube.choose_best,
                                    YouTube.choose_default,
                                    YouTube.to_s).url
                        or error("no match: media url")

    if url and #self.start_time > 0 then
        local min, sec = self.start_time:match("^(%d+)m(%d+)s$")
        min = tonumber(min) or 0
        sec = tonumber(sec) or 0
        local msec = (min * 60000) + (sec * 1000)
        if msec > 0 then
            url = url .. "&begin=" .. msec
        end
    end

    self.url = {url}

    return self
end

function YouTube.choose_best(formats) -- Highest quality available
    local r = {width=0, height=0, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_higher_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

function YouTube.choose_default(formats) -- Lowest quality available
    local r = {width=0xffff, height=0xffff, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_lower_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

YouTube.conv_table = { -- Deprecated.
    -- flv
    flv_240p =  '5',
    flv_360p = '34',
    flv_480p = '35',
    -- mp4
     mp4_360p = '18',
     mp4_720p = '22',
    mp4_1080p = '37',
    mp4_3072p = '38'
}

function YouTube.convert_deprecated_id(r_fmt)
    if YouTube.conv_table[r_fmt] then
        local s = string.format("fmt%02d_", YouTube.conv_table[r_fmt])
        r_fmt = r_fmt:gsub("^(%w+)_", s)
    end
    return r_fmt
end

function YouTube.to_s(t)
    return string.format("fmt%02d_%sp", t.fmt_id, t.height)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
