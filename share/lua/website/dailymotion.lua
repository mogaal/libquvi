
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

-- "http://dai.ly/cityofscars",
-- "http://www.dailymotion.com/video/xdpig1_city-of-scars_shortfilms",

local Dailymotion = {} -- Utility functions unique to this script.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "dailymotion."
    r.formats    = "default|best"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain, "dai.ly"},
                    {"/video/","/%w+$","/family_filter"})
    return r
end

-- Query available formats.
function query_formats(self)
    local U       = require 'quvi/util'
    local page    = Dailymotion.fetch_page(self, U)
    local formats = Dailymotion.iter_formats(page, U)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Dailymotion.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "dailymotion"

    local U    = require 'quvi/util'
    local page = Dailymotion.fetch_page(self, U)

    local _,_,s = page:find('title="(.-)"')
    self.title  = s or error("no match: media title")

    local _,_,s = page:find("video/(.-)_")
    self.id     = s or error("no match: media id")

    local _,_,s = page:find('"og:image" content="(.-)"')
    self.thumbnail_url = s or ''

    local formats = Dailymotion.iter_formats(page, U)
    self.url      = {U.choose_format(self, formats,
                                     Dailymotion.choose_best,
                                     Dailymotion.choose_default,
                                     Dailymotion.to_s).url
                        or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Dailymotion.fetch_page(self, U)
    self.page_url = Dailymotion.normalize(self.page_url)

    local _,_,s = self.page_url:find('/family_filter%?urlback=(.+)')
    if s then
        self.page_url = 'http://dailymotion.com' .. U.unescape(s)
    end

    local opts = {arbitrary_cookie = 'family_filter=off'}
    return quvi.fetch(self.page_url, opts)
end

function Dailymotion.normalize(page_url) -- "Normalize" embedded URLs
    if page_url:find("/swf/") then
        page_url = page_url:gsub("/video/", "/")
        page_url = page_url:gsub("/swf/", "/video/")
    end
    return page_url
end

function Dailymotion.iter_formats(page, U)
    local _,_,seq = page:find('"sequence",%s+"(.-)"')
    if not seq then
        local e = "no match: sequence"
        if page:find("_partnerplayer") then
            e = e .. ": looks like a partner video which we do not support"
        end
        error(e)
    end
    seq = U.unescape(seq)

    local _,_,msg = seq:find('"message":"(.-)[<"]')
    if msg then
        msg = msg:gsub('+',' ')
        error(msg:gsub('\\',''))
    end

    local _,_,vpp = seq:find('"videoPluginParameters":{(.-)}')
    if not vpp then
        -- See also <http://sourceforge.net/apps/trac/clive/ticket/4>
        error("no match: video plugin params")
    end

    local t = {}
    for url in vpp:gfind('%w+URL":"(.-)"') do
        url = url:gsub("\\/", "/")
        local _,_,c,w,h,cn = url:find('(%w+)%-(%d+)x(%d+).-%.(%w+)')
        if not c then
            error('no match: codec, width, height, container')
        end
--        print(c,w,h,cn)
        table.insert(t, {width=tonumber(w), height=tonumber(h),
                         container=cn,      codec=string.lower(c),
                         url=url})
    end

    return t
end

function Dailymotion.choose_default(formats) -- Lowest quality available
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

function Dailymotion.choose_best(formats) -- Highest quality available
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

function Dailymotion.to_s(t)
    return string.format("%s_%sp", t.container, t.height)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
