
-- quvi
-- Copyright (C) 2010,2011  quvi project
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
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "video.globo.com"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/Videos/"})
    return r
end

-- Query available formats.
function query_formats(self)
    self.formats = 'default'
    return self
end

-- Parse media URL.
function parse (self)
    self.host_id = "globo"
    local page   = quvi.fetch(self.page_url)

    local _,_,s = page:find('midiaId: (.-),')
    self.id     = s or error ("no match: media id")

    local _,_,s = page:find('<title>.*-.*- (.-)</title>')
    self.title  = s or error ("no match: media title")

    local config_url =
        "http://playervideo.globo.com/webmedia/GMCPlayListASX"
        .. "?flash=true&midiaId="
        .. self.id

    -- Unless user-agent is set here to 'iphone', URL verification
    -- *will* fail (HTTP/403) later. Fetching below itself does not
    -- need it, just the URL verification. We set it here to be safe.

    local opts    = {fetch_type = 'config', user_agent = 'iphone'}
    local xml     = quvi.fetch(config_url, opts)
    local _,_,d,s = xml:find('<video duration="(.-)" src="(.-)%?')
    self.duration = tonumber(d) or 0
    self.url      = {s or error('no match: media url')}

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
