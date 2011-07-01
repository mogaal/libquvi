
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

local CBSNews = {} -- Utility functions unique to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "cbsnews.com"
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/video/watch/"})
    return r
end

-- Query available formats.
function query_formats(self)
    local U       = require 'quvi/util'
    local config  = CBSNews.get_config(self)
    local formats = CBSNews.iter_formats(config)

    local t = {}
    for k,v in pairs(formats) do
        table.insert(t, CBSNews.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "cbsnews"
    local config = CBSNews.get_config(self)

    local _,_,s  = config:find ('<Title>.-CDATA%[(.-)%]')
    self.title   = s or error ("no match: media title")

    local formats = CBSNews.iter_formats(config)
    local U       = require 'quvi/util'
    self.url      = {U.choose_format(self, formats,
                                     CBSNews.choose_best,
                                     CBSNews.choose_default,
                                     CBSNews.to_s).url
                        or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function CBSNews.get_config(self)
    local page  = quvi.fetch(self.page_url)

    -- Need "? because some videos have the " and some don't
    local _,_,s = page:find('CBSVideo.setVideoId%("?(.-)"?%);')
    self.id     = s or error("no match: media id")

    local s_fmt = "http://api.cnet.com/restApi/v1.0/videoSearch?videoIds=%s"
                    .. "&iod=videoMedia"

    local config_url = string.format(s_fmt, self.id)

    return quvi.fetch(config_url, {fetch_type = 'config'})
end

function CBSNews.iter_formats(config) -- Iterate available formats
    local p = '<Width>(%d+)<'
           .. '.-<Height>(%d+)<'
           .. '.-<BitRate>(%d+)<'
           .. '.-<DeliveryUrl>.-'
           .. 'CDATA%[(.-)%]'
    local t = {}
    for w,h,b,u in config:gfind(p) do
        local _,_,s = u:find('%.(%w+)$')
--        print(w,h,b,s,u)
        table.insert(t,
            {width=tonumber(w),
             height=tonumber(h),
             bitrate=tonumber(b),
             url=u,
             container=s})
    end
    return t
end

function CBSNews.choose_best(formats) -- Highest quality available
    local r = {width=0, height=0, bitrate=0, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(formats) do
        if U.is_higher_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

function CBSNews.choose_default(t) -- Lowest quality available
    local r = {width=0xffff, height=0xffff, bitrate=0xffff, url=nil}
    local U = require 'quvi/util'
    for _,v in pairs(t) do
        if U.is_lower_quality(v,r) then
            r = v
        end
    end
--    for k,v in pairs(r) do print(k,v) end
    return r
end

function CBSNews.to_s(t)
    return string.format("%s_%sk_%sp", t.container, t.bitrate, t.height)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
