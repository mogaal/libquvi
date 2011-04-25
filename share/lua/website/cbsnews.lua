
-- quvi
-- Copyright (C) 2010,2011  quvi project
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
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "cbsnews.com"
-- m4v, seem to be available for most videos
-- mp4, appear to be for newer videos
-- Available height property can vary greatly per video.
    r.formats    =
        "default|best|m4v_216p|m4v_360p|m4v_480p|mp4_180p|mp4_240p|mp4_480p"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/video/watch/"})
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "cbsnews"
    local page   = quvi.fetch(self.page_url)

    -- Need "? because some videos have the " and some don't
    local _,_,s = page:find('CBSVideo.setVideoId%("?(.-)"?%);')
    self.id     = s or error ("no match: video id")

    -- Goto cnet and get the url for the xml of the available video formats
    s =  "http://api.cnet.com/restApi/v1.0/videoSearch?videoIds="
        ..s.. "&iod=videoMedia"

    local xml = quvi.fetch (s, {fetch_type = 'config'})

    -- Grab title from the XML.
    local _,_,s = xml:find ('<Title>.-CDATA%[(.-)%]')
    self.title  = s or error ("no match: video title")

    -- Go over the URLs in the XML.
    local r_fmt = self.requested_format
    local r_type,r_height = from_fmt_id(r_fmt)
--    print(r_type,r_height)

    local p = '<Height>(%d+)<.-<BitRate>(%d+)<.-<DeliveryUrl>.-CDATA%[(.-)%]'
    local t = {}
    local url

    for h,b,u in xml:gfind(p) do
--        print(h,b,u)
        url  = (not url) and u or url
        h    = tonumber(h)
        t[u] = {height=h, bitrate=tonumber(b)}
        if r_fmt ~= "best" then
            if h == r_height and u:find("%."..r_type) then
                url = u
                break
            end
        end
    end

    if r_fmt == 'best' then
        -- Note: container (m4v/mp4) is ignored. Compare height and
        -- bitrate properties. If the same height, then higher bitrate
        -- decides.
        local best = {h=0,b=0}
        for u,v in pairs(t) do
            if best.h <= v.height and best.b < v.bitrate then
                best.h = v.height
                best.b = v.bitrate
                url = u
            end
        end
    end

    self.url = {url}

    return self
end

function from_fmt_id(s)
    local _,_,t,h = s:find('(%w+)_(%d+)p')
    if not t or not h then
        -- Default to "m4v_216p" which seems to be available for most.
        -- Note that anything that matches "%w+_%d+p" but does not
        -- exist in config, will be ignored and whatever is found
        -- in the config in first, will be used instead.
        t,h = from_fmt_id("m4v_216p")
    end
    return t,tonumber(h)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
