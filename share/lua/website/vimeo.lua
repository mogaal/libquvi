
-- quvi
-- Copyright (C) 2010  Toni Gundogdu <legatvs@gmail.com>
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
function ident (page_url)
    local t   = {}
    t.domain  = "vimeo.com"
    t.formats = "default|best|hd"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "vimeo"
    
    is_player, _, video.id =
        video.page_url:find("^http://player.vimeo.com/video/(%d+)")

    if ( is_player ~= nil ) then
        video.page_url = "http://vimeo.com/" .. vid
    end

    if (video.id == nil) then
        local _,_,s = video.page_url:find('vimeo.com/(%d+)')
        video.id    = s
    end

    if (video.id == nil) then error ("no match: video") end

    local config_url = "http://vimeo.com/moogaloop/load/clip:" .. video.id
    local config = quvi.fetch (config_url, {fetch_type = 'config'})

    local _,_,s = config:find("<caption>(.-)</")
    video.title = s or error ("no match: video title")

    local _,_,s = config:find("<request_signature>(.-)</")
    local sign  = s or error ("no match: request signature")

    local _,_,s = config:find("<request_signature_expires>(.-)</")
    local exp   = s or error ("no match: request signature expires")

    local _,_,s     = config:find("<hd_button>(%d)</")
    local hd_button = s or error ("no match: hd button")

    local q = "sd" -- Same as "default".
    if (video.requested_format == "hd" or video.requested_format == "best") then
        if (hd_button == "1") then
            q = "hd"
        end
    end

    video.url = {
        string.format("http://vimeo.com/moogaloop/play/clip:%s/%s/%s/?q=%s",
            video.id, sign, exp, q)
    }

    return video
end


