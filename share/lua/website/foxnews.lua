
-- quvi
-- Copyright (C) 2011  Lionel Elie Mamane <lionel@mamane.lu>
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

local Foxnews = {} -- Utility functions unique to this script.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "video.foxnews.com"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/v/%d+"})
                    or U.handles(self.page_url, {r.domain}, {"/v/embed%.js"},
                        {"id%=%d+"})
                    or U.handles(self.page_url, {r.domain},
                        {"/v/video%-embed%.html"}, {"video_id=%d+"})
    return r
end

-- Query available formats.
function query_formats(self)
    local U       = require 'quvi/util'
    local js      = Foxnews.fetch_feed_js(self, U)
    local formats = Foxnews.iter_formats_js(js, U)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Foxnews.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "foxnews"

    local U    = require 'quvi/util'
    local js   = Foxnews.fetch_feed_js(self, U)

    local _,_,item = js:find('"item":(%b{})')
    if not item then
        error("no match: item")
    end

    local _,_,s = js:find('"title":"(.-)"')
    self.title  = s or error("no match: media title")

    do
        local thumbs = Foxnews.iter_thumbnails_js(item)
        local best_thumb_res = -1
        local best_thumb_url = ''
        for _,thumb in pairs(thumbs) do
            local res = thumb.height*thumb.width
            if res > best_thumb_res then
                best_thumb_url = thumb.url
                best_thumb_res = res
            end
        end
        self.thumbnail_url = best_thumb_url
    end

    local formats = Foxnews.iter_formats_js(item, U)
    self.url      = {U.choose_format(self, formats,
                                     Foxnews.choose_best,
                                     Foxnews.choose_default,
                                     Foxnews.to_s).url
                        or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Foxnews.fetch_feed_js(self, U)
    self.page_url = Foxnews.normalize(self)

    return quvi.fetch('http://video.foxnews.com/v/feed/video/'
            .. self.id .. '.js?template=grab')
end

function Foxnews.normalize(self) -- "Normalize" embedded URLs
    if self.page_url:find("/v/embed.js?id=",1,true) then
        self.page_url = self.page_url:gsub("/v/embed%.js%?id=(%d+)", "/v/%1/")
    elseif self.page_url:find("/v/video-embed.html?video_id=",1,true) then
        self.page_url =
            self.page_url:gsub("/v/video%-embed%.html%?video_id=(%d+)",
                               "/v/%1/")
    end
    local _, ie, s = self.page_url:find("/v/(%d+)")
    self.id = s or error("no match: media id")
    self.page_url = self.page_url:sub(1, ie)
end

function Foxnews.iter_formats_js(page, U)
    local formats = Foxnews.find_set(page, '"media%-content"')
    if not formats then
        error("no match: media-content")
    end

    local t = {}
    for attrs in formats:gfind('"@attributes":(%b{})') do
        local format = {}
        for key,value in attrs:gfind('"(.-)":"(.-)"') do
            format[key]=value
	    end
        if format.rel == "stream" then
            local a = {"framerate","bitrate","height","duration","width"}
            for _, key in pairs(a) do
                if format[key] then
                    format[key]=tonumber(format[key])
                end
            end
            local _,_,s = format.url:find('FNC_([%w%p]-)%.')
            if s then
                format.description=string.lower(s)
            end
            table.insert(t, format)
        end
    end

    return t
end

function Foxnews.iter_thumbnails_js(page, U)
    local thumbs = Foxnews.find_set(page, '"media%-thumbnail"')
    if not thumbs then
        return {}
    end

    local t = {}
    for attrs in thumbs:gfind('"@attributes":(%b{})') do
        local thumb = {}
        for key,value in attrs:gfind('"(.-)":"(.-)"') do
            thumb[key]=value
	    end
        table.insert(t, thumb)
    end

    return t
end

function Foxnews.choose_default(formats) -- Lowest quality available
    local r = {width=0xffff, height=0xffff, url=nil, bitrate=0xfffffffffff}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_lower_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

function Foxnews.choose_best(formats) -- Highest quality available
    local r = {width=0, height=0, url=nil, bitrate=-1}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_higher_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

function Foxnews.to_s(t)
    if t.description then
        return t.description
    else
        return string.format("%s_%sp", t.width, t.height)
    end
end

function Foxnews.find_set(str, label)
    local _, _, r = str:find(label .. ':(%b[])')
    if not r then
        _, _, r = str:find(label .. ':(%b{})')
    end
    return r
end

-- Local Variables: **
-- indent-tabs-mode: () **
-- lua-indent-level: 4 **
-- End: **
-- vim: set ts=4 sw=4 tw=72 expandtab:
