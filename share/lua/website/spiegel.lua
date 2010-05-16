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

-- These are my formats.
local lookup = {
    default   = "VP6_388",
    best      = "H264_1400",
    ["576k"]  = "VP6_576",
    ["928k"]  = "VP6_928",
    ["1400k"] = "H264_1400"
}

-- Returns script details.
function ident (page_url)
    
    -- This is what I return.
    local t = {}

    -- This is my domain.
    t.domain = "spiegel.de"

    -- This is my formats-string.
    t.formats = ""
    for k,_ in pairs (lookup) do
        t.formats = t.formats .."|".. k
    end
    t.formats = t.formats:gsub("^%|","")

    -- This is my response to "Will you handle this URL?"
    -- Note that page_url may be nil.
    t.will_handle = (page_url ~= nil and page_url:find(t.domain) ~= nil)

    -- Here are my details.
    return t

end

-- Fetches video page and parses the video URL.
function parse (video)

    -- This is my "host ID".
    video.host_id = "spiegel"

    -- This is my video ID.
    local _,_,s = video.page_url:find("/video/video%-(.-)%.")
    video.id    = s or error ("no match: video id")

    -- Skip video page. Fetch playlist.
    local playlist_url = string.format(
        "http://www1.spiegel.de/active/playlist/fcgi/playlist.fcgi/"
        .. "asset=flashvideo/mode=id/id=%s", video.id)

    local playlist = quvi.fetch(playlist_url, "playlist")

    -- This is my video title.
    local _,_,s = playlist:find("<headline>(.-)</")
    video.title = s or error ("no match: video title")

    -- Fetch config.
    local config_url = string.format(
        "http://video.spiegel.de/flash/%s.xml", video.id)

    local config = quvi.fetch(config_url, "config")

    -- Choose correct format.
    local format = lookup[default]

    for k,v in pairs (lookup) do
        if (k == video.requested_format) then
            format = v
            break
        end
    end

    -- Match format string to link in config.
    local pattern = "<filename>(%d+)_(%d+)x(%d+)_(%w+)_(%d+).(%w+)"
    for id,w,h,c,b,s in config:gfind(pattern) do
        fname = string.format("%s_%sx%s_%s_%s.%s",id,w,h,c,b,s)
        if (format == string.format("%s_%s",c,b)) then
            video.url = {"http://video.spiegel.de/flash/"..fname}
            break
        end
    end

    -- Return the updated video properties.
    return video

end


