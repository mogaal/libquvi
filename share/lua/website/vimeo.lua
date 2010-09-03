
-- Copyright (C) 2010 Toni Gundogdu.
--
-- This file is part of quvi <http://quvi.googlecode.com/>.
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    local page    = quvi.fetch(video.page_url)

    local _,_,s = page:find('clip_id=(.-)"')
    video.id    = s or error ("no match: video id")

    local config_url = "http://vimeo.com/moogaloop/load/clip:" .. video.id
    local config = quvi.fetch(config_url, "config")

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


