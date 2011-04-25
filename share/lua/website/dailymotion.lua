
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

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "dailymotion."
    r.formats    = "default|best|hq|hd"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
-- "http://dai.ly/cityofscars",
-- "http://www.dailymotion.com/video/xdpig1_city-of-scars_shortfilms",
    r.handles    =
        U.handles(self.page_url, {r.domain, "dai.ly"}, {"/video/","/%w+$"})
    return r
end

-- Parse video URL.
function parse(self)
    self.host_id  = "dailymotion"
    self.page_url = normalize(self.page_url)

    local U = require 'quvi/util'

    local _,_,s = self.page_url:find('/family_filter%?urlback=(.+)')
    if s then -- Set family_filter arbitrary cookie below.
        self.page_url = 'http://dailymotion.com' .. U.unescape(s)
    end

    local opts = {arbitrary_cookie = 'family_filter=off'}
    local page = quvi.fetch(self.page_url, opts)

    local _,_,s = page:find('title="(.-)"')
    self.title  = s or error("no match: video title")

    local _,_,s = page:find("video/(.-)_")
    self.id     = s or error("no match: video id")

    self.url    = get_url(self, page, U)
    if not self.url then
        error("no match: video url")
    end

    return self
end

function get_url(self, page, U)
    local _,_,seq = page:find('"sequence",%s+"(.-)"')
    if not seq then
        local e = "no match: sequence"
        if page:find("_partnerplayer") then
            e = e .. ": looks like a partner video which we do not support"
        end
        error(e)
    end
    seq = U.unescape(seq)

    local _,_,vpp = seq:find('"videoPluginParameters":{(.-)}')
    if not vpp then
        local _,_,s = page:find('"video", "(.-)"')
        if not s then
            error("no match: video plugin params")
        else
            -- some videos (that require setting family_filter cookie)
            -- may list only one link which is not found under
            -- "videoPluginParameters". See also:
            -- http://sourceforge.net/apps/trac/clive/ticket/4
            return {s}
        end
    end

    -- "sd" is our "default".
    local req_fmt = self.requested_format
    req_fmt = (req_fmt == "default") and "sd" or req_fmt

    -- choose "best" from the array, check against reported video
    -- resolution height. pick the highest available.
    local best,curr
    local best_h = 0

    for id,url in vpp:gfind('(%w+)URL":"(.-)"') do -- id=format id.
        url = url:gsub("\\/", "/")
        -- found requested format id.
        if id == req_fmt then
            curr = url
            break
        end
        -- default to whatever is the first in the array.
        if not curr then curr = url end
        -- compare heights as reported in the URLs.
        local _,_,w,h = url:find("(%d+)x(%d+)")
        if tonumber(h) > tonumber(best_h) then
            best_h = h
            best = url
        end
    end

    curr = (req_fmt == "best") and best or curr
    return {curr}
end

function normalize(page_url) -- embedded URL to "regular".
    if page_url:find("/swf/") then
        page_url = page_url:gsub("/video/", "/")
        page_url = page_url:gsub("/swf/", "/video/")
    end
    return page_url
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
