
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

-- Identify the script.
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "xvideos.com"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/video%d+/"})
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "xvideos"
    local page   = quvi.fetch(self.page_url)

    local _,_,s = page:find("<title>(.-)%s+-%s+XVID")
    self.title  = s or error ("no match: video title")

    local _,_,s = page:find("id_video=(.-)&amp;")
    self.id     = s or error ("no match: video id")

    local _,_,s = page:find("flv_url=(.-)&amp;")
    s           = s or error ("no match: flv url")
    local U     = require 'quvi/util'
    self.url    = {U.unescape(s)}

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
