
-- quvi
-- Copyright (C) 2010  quvi project
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

-- Note: Video URL verification which is done after parsing the details
-- in this script *will* fail if you are using a proxy that omits the
-- user-agent.

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "globo.com"
    t.formats = "default"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "globo"
    local page    = quvi.fetch(video.page_url)

    local _,_,s = page:find('midiaId: (.-),')
    video.id    = s or error ("no match: video id")

    local _,_,s = page:find('<title>.*-.*- (.-)</title>')
    video.title = s or error ("no match: video title")

    s = "http://playervideo.globo.com/webmedia/GMCPlayListASX"
        .. "?flash=true&midiaId="
        .. video.id

    -- Unless user-agent is set here to 'iphone', URL verification
    -- *will* fail (HTTP/403) later. Fetching below itself does not
    -- need it, just the URL verification. We set it here to be safe.

    local opts  = {fetch_type = 'config', user_agent = 'iphone'}
    local xml   = quvi.fetch (s, opts)
    local _,_,s = xml:find('<video duration=".*" src="(.-)%?')
    video.url   = {s or error ('no match: video url')}

    return video
end


