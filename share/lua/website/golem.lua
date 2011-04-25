
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
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "video.golem.de"
    r.formats    = "default|best|ipod|high"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/[%w-_]+/%d+/"})
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "golem"
    local page   = quvi.fetch(self.page_url)

    local _,_,s = page:find('"id", "(.-)"')
    self.id     = s or error ("no match: video id")

    local config_url =
        string.format("http://video.golem.de/xml/%s", self.id)

    local config = quvi.fetch (config_url, {fetch_type = 'config'})
    local _,_,s  = config:find("<title>(.-)</")
    self.title   = s or error ("no match: video title")

    video_url =
        string.format("http://video.golem.de/download/%s", self.id)

    format = "medium" -- This is the default.

    if (self.requested_format == "best") then
        format = "high"
    else
        for _,v in pairs({"ipod","high"}) do
            if (v == self.requested_format) then
                format = v
                break
            end
        end
    end

    self.url = {video_url .. "?q=" .. format}

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
