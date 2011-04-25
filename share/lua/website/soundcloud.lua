
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
    r.domain     = "soundcloud.com"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/.+/.+$"})
    return r
end

-- Parse video URL.
function parse(self)
    self.host_id  = "soundcloud"

    local page = quvi.fetch(self.page_url)

    local _,_,s = page:find("window%.SC%.bufferTracks%.push(%(.-%);)")
    local metadata = s or error("no match: metadata")

    local _,_,s = metadata:find('"uid":"(%w-)"')
    self.id = s or error("no match: video id")

    local _,_,s = metadata:find('"title":"(.-)"')
    local title  = s or error("no match: video title")
    -- Unescape the Unicode strings if any
    -- the HTML will be unescaped by quvi itself
    self.title = string.gsub(title, "\\u(%d+)",
        function (h)
            return string.char(tonumber(h, 16))
        end)

    local _,_,s = page:find('content="([:/%w%?%.-]-)" property="og:image"')
    self.thumbnail_url = s or ""

    local _,_,s = metadata:find('"duration":(%d-),')
    self.duration = tonumber(s) or 0

    local _,_,s = metadata:find('"streamUrl":"(.-)"')
    self.url  = { s }  or error("no match: stream URL")

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
