
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
-- Commented out due to limited hd support
--    r.formats  = "default|best|hd"
    r.categories = C.proto_http
    r.handles    =
        (self.page_url ~= nil and self.page_url:find(r.domain) ~= nil)
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "collegehumor"
   
    local success, _, vid =
        self.page_url:find("^http://www%.collegehumor%.com/video:(%d+)")

    if ( success == nil ) then
        error("expected URL beginning with 'http://collegehumor.com/video:'")
    end

    self.id = vid

    local page = quvi.fetch(self.page_url)
    local _,_,s = page:find('<meta name="title" content="(.-)"')
    self.title = s or error("no match: video title")

    local page =
        quvi.fetch("http://www.collegehumor.com/moogaloop/video:" .. vid,
            {fetch_type = 'config'})

    local _,_,s = page:find('<file>([%w%p]+)</file>')
    local default = s or error("no match: default quality URL")

    local _,_,s = page:find('<hq>([%w%p]+)</hq>')
    local hq = s

    self.url = { default }
    if (self.requested_format == "hd" or self.requested_format == "best") then
        if ( hq ~= nil ) then
	        self.url = { hq }
	    end
    end

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
