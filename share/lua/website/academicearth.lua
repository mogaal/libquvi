
-- quvi
-- Copyright (C) 2010,2011  Toni Gundogdu <legatvs@gmail.com>
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

-- academicearth.org hosts videos at either blip.tv or youtube.com
-- This webscript uses the "redirect_url" to point to the source.

local AcademicEarth = {} -- Utility functions specific to this script

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "academicearth.org"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/lectures/"})
    return r
end

-- Query available formats.
function query_formats(self)
    return AcademicEarth.get_redirect_url(self)
end

-- Parse media URL.
function parse(self)
    return AcademicEarth.get_redirect_url(self)
end

--
-- Utility functions
--

function AcademicEarth.get_redirect_url(self)
    local page = quvi.fetch(self.page_url)

    local _,_,s = page:find('ytID = "(.-)"')
    if s then
        self.redirect_url = 'http://youtube.com/e/' .. s
    else
        local _,_,s = page:find('embed src="(.-)"') -- blip
        if s then
            self.redirect_url = s
        else
            error('no match: blip or youtube pattern')
        end
    end
    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
