
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
    t.domain  = "dailymotion."
    t.formats = "default|best|hq|hd"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "dailymotion"
    local page    = quvi.fetch(video.page_url)

    if (page:find('SWFObject("http:")') ~= nil) then
        error ("Looks like a partner video. Refusing to continue.")
    end

    local _,_,s = page:find('title="(.-)"')
    video.title = s or error ("no match: video title")

    local _,_,s = page:find("video/(.-)_")
    video.id    = s or error ("no match: video id")

    local best    = nil
    local req_fmt = video.requested_format

    if (req_fmt == "default") then
        req_fmt = "sd" -- Dailymotion defaults to this.
    end

    for id,path in page:gfind("%%22(%w%w)URL%%22%%3A%%22(.-)%%22") do
        path = path:gsub("%%5C","")
        path = quvi.unescape(path)
        best = path
        if (req_fmt == id) then
            video.url = {path}
            break
        end
    end

    if (best ~= nil and video.requested_format == "best") then
        video.url = {best}
    end

    if (video.url == nil) then
        error ("no match: format id, path")
    end

    return video
end


