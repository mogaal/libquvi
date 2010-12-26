
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

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "sevenload.com"
    t.formats = "default"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "sevenload"
    local page    = quvi.fetch(video.page_url)

    local _,_,s = page:find('<meta name="title" content="(.-)"')
    video.title = s or error ("no match: video title")

    local _,_,s      = page:find('configPath=(.-)"')
    local config_url = s or error ("no match: config")

    config_url       = quvi.unescape(config_url)
    local config     = quvi.fetch (config_url, {fetch_type = 'config'})

    local _,_,s = config_url:find("itemId=(%w+)")
    video.id    = s or error ("no match: video id")

    local _,_,s = config:find('<location seeking="yes">(.-)</')
    video.url   = {s or error ("no match: location")}

    return video
end


