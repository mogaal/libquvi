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
    t.domain = "golem.de"

    -- This is my formats-string.
    t.formats = "default|best|ipod|high"

    -- This is my response to "Will you handle this URL?"
    -- Note that page_url may be nil.
    t.will_handle = (page_url ~= nil and page_url:find(t.domain) ~= nil)

    -- Here are my details.
    return t

end

-- Fetches video page and parses the video URL.
function parse (video)

    -- This is my "host ID".
    video.host_id = "golem"

    -- Fetch video page.
    local page = quvi.fetch(video.page_url)

    -- This is my video ID.
    local _,_,s = page:find('"id", "(.-)"')
    video.id    = s or error ("no match: video id")

    -- Fetch config.

    local config_url =
        string.format("http://video.golem.de/xml/%s", video.id)

    local config = quvi.fetch(config_url, "config")

    -- This is my video title.
    local _,_,s = config:find("<title>(.-)</")
    video.title = s or error ("no match: video title")

    -- This is my video URL.
    video_url =
        string.format("http://video.golem.de/download/%s", video.id)

    -- Choose correct format.
    format = "medium" -- This is the default.

    if (video.requested_format == "best") then
        format = "high"
    else
        for _,v in pairs({"ipod","high"}) do
            if (v == video.requested_format) then
                format = v
                break
            end
        end
    end

    -- Append format string.
    video.url = {video_url .. "?q=" .. format}

    -- Return the updated video properties.
    return video

end


