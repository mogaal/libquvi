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
    t.domain = "liveleak.com"

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
    video.host_id = "liveleak"

    -- Fetch video page.
    local page = quvi.fetch(video.page_url)

    -- This is my video title.
    local _,_,s = page:find("<title>LiveLeak.com%s+-%s+(.-)</title>")
    video.title = s or error ("no match: video title")

    -- This is my video ID.
    local _,_,s = page:find("token=(.-)['&]")
    video.id    = s or error ("no match: video id")

    -- Fetch config.
    local _,_,s      = page:find("'config','(.-)'")
    local config_url = s or error ("no match: config")
    local config     = quvi.fetch(quvi.unescape(config_url), "config")

    -- Fetch playlist.
    local _,_,s        = config:find("<file>(.-)</")
    local playlist_url = s or error ("no match: playlist")
    local playlist     = quvi.fetch(playlist_url, "playlist")

    -- This is my video URL.
    local _,_,s = playlist:find("<location>(.-)</")
    video.url   = {s or error ("no match: location")}

    -- Return the updated video properties.
    return video

end


