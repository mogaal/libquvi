
-- quvi
-- Copyright (C) 2011  Bastien Nocera <hadess@hadess.net>
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
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "ted.com"
    r.formats    = "default|best|hd"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/talks/.+$"})
    return r
end

-- Parse video URL.
function parse(self)
    self.host_id  = "ted"

    local page = quvi.fetch(self.page_url)

    local _,_,s = page:find('<span id="altHeadline">(.-)</span>')
    local U      = require 'quvi/util'
    self.title  = U.unescape(s) or error("no match: video title")

    local _,_,s = page:find('&amp;su=(.-)&amp;')
    self.thumbnail_url = s or ""

    local _,_,s = page:find('<h3>Video</h3>.-dl class="downloads"(.-)</dl>')
    local metadata = s or error("no match: metadata")

    local _,_,s =
        metadata:find('<a href="([^ .]-)">Download video to desktop')

    local path  = s or error("no match: standard src") -- our 'default'

    local r = self.requested_format
    r = (r == 'best') and 'hd' or r -- 'best' is an alias for 'hd'

    if r == 'hd' then
        local _,_,s =
            metadata:find('<a href="([^ .]-)">Watch high%-res video')
        path = s or path -- fallback to 'default'
    end

    self.url = {"http://www.ted.com" .. path}

    local _,_,s = path:find("talk/(.+)$")
    self.id = s or error("no match: video id")

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
