
-- quvi
-- Copyright (C) 2010  Lionel Elie Mamane <lionel@mamane.lu>
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
    r.domain     = "collegehumor.com"
    r.formats    = "default"
-- default and hq seem to be the same. see also 'hq' parsing.
--    r.formats    = "default|best|hq"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    local u      = collegehumorify(self.page_url)
    r.handles    = U.handles(u, {r.domain}, {"/video%:%d+/?"})
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "collegehumor"

    self.page_url, self.id = collegehumorify(self.page_url)
    self.id = self.id or error("no match: video id")

    -- http://www.collegehumor.com/video:1942317 - OK
    -- http://www.collegehumor.com/video:6463979 - FAILS

    local page  = quvi.fetch(
        "http://www.collegehumor.com/moogaloop/video:" .. self.id,
        {fetch_type = 'config'})

    local _,_,sd_url = page:find('<file><!%[%w+%[(.-)%]')
    local _,_,hq_url = page:find('<hq><!%[%w+%[(.-)%]')

    local url = sd_url or hq_url -- default to 'sd'
    url = url or error("no match: video url") -- we need this at least

    local r = self.requested_format
    url = ((r == 'hq' or r == 'best') and hq_url) and hq_url or sd_url

    self.url = {url}

    local _,_,s = page:find('<caption>(.-)<')
    self.title  = s or error("no match: video title")

    local _,_,s = page:find('<thumbnail><!%[%w+%[(.-)%]')
    self.thumbnail_url = s or ""

    return self
end

function collegehumorify(url)
    if not url then return url end
    local _,_,id = url:find('collegehumor%.com/video[/:](%d+)')
    if id then
        url = "http://www.collegehumor.com/video:" .. id
    end
    return url,id
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
