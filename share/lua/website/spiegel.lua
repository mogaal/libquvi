
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

-- Formats.
local lookup = {
    default   = "VP6_388",
    best      = "H264_1400",
    ["576k"]  = "VP6_576",
    ["928k"]  = "VP6_928",
    ["1400k"] = "H264_1400"
}

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "spiegel.de"
    t.formats = ""
    for k,_ in pairs (lookup) do
        t.formats = t.formats .."|".. k
    end
    t.formats = t.formats:gsub("^%|","")
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "spiegel"

    local _,_,s = video.page_url:find("/video/video%-(.-)%.")
    video.id    = s or error ("no match: video id")

    local playlist_url = string.format(
        "http://www1.spiegel.de/active/playlist/fcgi/playlist.fcgi/"
        .. "asset=flashvideo/mode=id/id=%s", video.id)

    local playlist = quvi.fetch(playlist_url, "playlist")

    local _,_,s = playlist:find("<headline>(.-)</")
    video.title = s or error ("no match: video title")

    local config_url = string.format(
        "http://video.spiegel.de/flash/%s.xml", video.id)

    local config = quvi.fetch(config_url, "config")

    local format = lookup[default]

    for k,v in pairs (lookup) do
        if (k == video.requested_format) then
            format = v
            break
        end
    end

    -- Match the format string to a link in config.
    local pattern = "<filename>(%d+)_(%d+)x(%d+)_(%w+)_(%d+).(%w+)"
    for id,w,h,c,b,s in config:gfind(pattern) do
        fname = string.format("%s_%sx%s_%s_%s.%s",id,w,h,c,b,s)
        if (format == string.format("%s_%s",c,b)) then
            video.url = {"http://video.spiegel.de/flash/"..fname}
            break
        end
    end

    return video
end


