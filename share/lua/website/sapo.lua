
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

--
-- Should work with most http://videos.sapo.pt/$video_id URLs.
--

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "videos.sapo.pt"
    t.formats = "default"
    t.handles = (page_url ~= nil and page_url:find (t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)

    video.host_id = "sapo"
    local page    = quvi.fetch (video.page_url)

    if (page:find ('rtmp:%/%/')) then
        error ("video requires rtmp which we do not currently support")
    end

    local _,_,s = page:find ('class="tit">(.-)</div>')
    if (s == nil) then
        _,_,s = page:find ('<title>(.-)%s+-%s+')
    end
    video.title = s or error ("no match: video title")

    local _,_,s = page:find ('?file=(.-)/mov')
    if (s == nil) then error ("no match: video url") end
    video.url   = {s .. "/mov"}

    local _,_,s = video.url[1]:find ('.*/(.-)/mov')
    video.id    = s or error ("no match: video id")

    return video
end


