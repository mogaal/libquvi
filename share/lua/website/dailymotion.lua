
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

local domains = {
    "dailymotion.",
    "dai.ly"
}

-- Check whether the domain is handled
function is_handled (page_url)
    if page_url == nil then
        return false
    end
    for k,domain in pairs(domains) do
        if page_url:find(domain) ~= nil then
            return true
        end
    end
    return false
end

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "dailymotion."
    t.formats = "default|best|hq|hd"
    t.handles = is_handled(page_url)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id  = "dailymotion"
    video.page_url = normalize (video.page_url)

    local opts  = { arbitrary_cookie = 'family_filter=off' }
    local page  = quvi.fetch(video.page_url, opts)

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

function normalize (url)
    if (url:find ("/swf/")) then
        url = url:gsub ("/video/", "/")
        url = url:gsub ("/swf/", "/video/")
    end
    return url
end

