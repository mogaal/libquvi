--[[
/* 
* Copyright (C) 2010 Toni Gundogdu.
*
* This file is part of quvi, see http://quvi.googlecode.com/
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
]]--

-- If you make improvements to this script, drop a line. Thanks.
--   <http://quvi.googlecode.com/>

-- Returns script details.
function ident (page_url)
    
    -- This is what I return.
    local t = {}

    -- This is my domain.
    t.domain = "dailymotion."

    -- This is my formats-string.
    t.formats = "default|best|hq|hd"

    -- This is my response to "Will you handle this URL?"
    -- Note that page_url may be nil.
    t.will_handle = (page_url ~= nil and page_url:find(t.domain) ~= nil)

    -- Here are my details.
    return t

end

-- Fetches video page and parses the video URL.
function parse (video)

    -- This is my "host ID".
    video.host_id = "dailymotion"

    -- Fetch video page.
    local page = quvi.fetch(video.page_url)

    -- Check if we can even dl this video.
    if (page:find('SWFObject("http:")') ~= nil) then
        error ("Looks like a dailymotion partner video."
            .. "Refusing to continue.")
    end

    -- This is my video title.
    local _,_,s = page:find('title="(.-)"')
    video.title = s or error ("no match: video title")

    -- This is my video ID.
    local _,_,s = page:find("video/(.-)_")
    video.id    = s or error ("no match: video id")

    -- Other relevant details.
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

    -- Return the updated video properties.
    return video

end


