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

--[[
NOTES
* This script fetches "video info" first
  - Advantages: requires less bandwidth, works around sign-in-to-view
  - Disadvantages: fails frequently for still unknown reason
* If the above fails
  - Dump server returned error to stdout as a warning
  - Fetch and parse video page instead
  - Advantages: works for most (unrestricted) videos
  - Disadvantages:
    - Requires (a lot) more bandwidth
    - Does not work with videos that require signing-in-to-view
]]--

-- These are my formats.
local lookup = {
    mobile   = "17", --   3gp
    sd_270p  = "18", --   480x270
    sd_360p  = "34", --   640x360
    hq_480p  = "35", --   854x480
    hd_720p  = "22", --  1280x720
    hd_1080p = "37"  -- 1920x1080
}

-- Returns script details.
function ident (page_url)
    
    -- This is what I return.
    local t = {}

    -- This is my domain.
    t.domain = "youtube.com"

    -- This is my formats-string.
    t.formats = ""
    for k,_ in pairs (lookup) do
        t.formats = t.formats .."|".. k
    end
    t.formats = "default|best" .. t.formats

    -- This is my response to "Will you handle this URL?"
    -- Note that page_url may be nil.
    if (page_url ~= nil) then
        page_url = youtubify(page_url)
    end
    t.will_handle = (page_url ~= nil and page_url:find(t.domain) ~= nil)

    -- Here are my details.
    return t

end

-- Fetches video page and parses the video URL.
function parse (video)

    -- This is my "host ID".
    video.host_id = "youtube"

    -- Page URL.
    local page_url = youtubify(video.page_url)

    -- This is my video ID.
    local _,_,s = page_url:find("v=([%w-_]+)")
    video.id    = s or error ("no match: video id")

    -- Fetch and pray.
    video,t,best = get_video_info(video)
    if (video.title == nil) then
        video,t,best = old_faithful(page_url, video)
    end

    -- Construct the video URL.
    local video_url = 
        string.format(
            "http://youtube.com/get_video?video_id=%s&t=%s",
                video.id, quvi.unescape(t))

    -- Choose correct format ID.
    if (best == nil and video.requested_format == "best") then
        print ("  > Warning: Unable to find `best' format. Use `default'.")
    end

    local fmt_id = nil

    if (video.requested_format == "best") then
        fmt_id = best or fmt_id
    else
        for k,v in pairs (lookup) do
            if (k == video.requested_format) then
                fmt_id = v
                break
            end
        end
    end

    if (fmt_id ~= nil) then
        video_url = video_url .."&fmt=".. fmt_id
    end

    -- Set my video URL.
    video.url = {video_url}

    -- Return the updated video properties.
    return video

end

-- Youtube video page URL unwrangler.
function youtubify (url)
    url = url:gsub("-nocookie", "")    -- youtube-nocookie.com
    url = url:gsub("/v/", "/watch?v=") -- embedded
    return url
end

-- The preferred method, uses less bandwidth, fails for some videos.
-- See also the NOTES above.
function get_video_info (video)

    -- Fetch video info.
    local config_url = string.format(
        "http://www.youtube.com/get_video_info?&video_id=%s"
         .. "&el=detailpage&ps=default&eurl=&gl=US&hl=en", video.id)

    local config = quvi.unescape( quvi.fetch(config_url, "config") )

    -- Check response. For still unknown reasons, the above
    -- does not work for all videos that I've tried so far.
    local _,_,s = config:find("&reason=(.-)[?:&]?$")
    if (s ~= nil) then
        print ("  > Warning: get_video_info: " .. s:gsub("+"," "))
        print ("  > Warning: Revert to fetch video page instead.")
        return video -- This one's for the Old Faithful.
    end

    -- This is my video title.
    local _,_,s = config:find("&title=(.-)&")
    video.title = s or error ("no match: video title")
    video.title = video.title:gsub("+"," ")

    -- This is my t(oken) param used to construct the video URL.
    local _,_,s = config:find("&token=(.-)&")
    local t     = s or error ("no match: token parameter")

    -- Best format.
    local _,_,best = config:find("&fmt_map=(%d+)")

    -- Return parsed details.
    return video, t, best

end

-- Fetch video page from the user specified URL and parse.
-- See also the NOTES above.
function old_faithful (page_url, video)

    -- Fetch video page.
    local page = quvi.fetch(page_url)

    -- This is my video title.
    local _,_,s = page:find('<meta name="title" content="(.-)"')
    video.title = s or error ("no match: video title")

    -- This is my t param used to construct the video URL.
    local _,_,s = page:find('&t=(.-)&')
    local t     = s or error ("no match: t param")

    -- Best format
    local _,_,best = page:find("&fmt_map=(%d+)")

    -- Return parsed details.
    return video, t, best

end


