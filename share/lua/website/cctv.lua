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
    t.domain = "cctv.com"

    -- This is my formats-string.
    t.formats = "default"

    -- This is my response to "Will you handle this URL?"
    -- Note that page_url may be nil.
    t.will_handle = (page_url ~= nil and page_url:find(t.domain) ~= nil)

    -- Here are my details.
    return t

end

-- Fetches video page and parses the video URL.
function parse (video)

    -- This is my "host ID".
    video.host_id = "cctv"

    -- Fetch video page.
    local page = quvi.fetch(video.page_url)

    -- This is my video title.
    local _,_,s = page:find('<meta name="description"%s+content="(.-)"')
    video.title = s or error ("no match: video title")

    -- This is my video ID.
    local _,_,s = page:find("videoId=(.-)&")
    video.id    = s or error ("no match: video id")

    -- This is the domain string used to fetch the config.
    local _,_,s = video.page_url:find("http://(.-)/")
    local domain = s or error ("no match: domain")

    -- This is the config URL.
    local config_url =
        string.format("http://%s/playcfg/flv_info_new.jsp?videoId=%s",
            domain, video.id)

    -- Fetch the config.
    local config = quvi.fetch(config_url, "config")

    -- These are my other details used to construct the video URL.
    local _,_,s    = config:find('"chapters":%[(.-)%]')
    local chapters = s or error ("no match: chapters")
    video.url      = {} -- Array. Make a mental note of this.

    -- Some of the cctv videos have "segments", include all of them.
    for path in chapters:gfind('{"url":"(.-)",') do
        table.insert(video.url,
            string.format("http://v.cctv.com/flash/%s", path))
    end

    -- We should have at least one in the array.
    if (table.getn(video.url) == 0) then
        error ("no match: chapter url path")
    end

    -- Return the updated video properties.
    return video

end


