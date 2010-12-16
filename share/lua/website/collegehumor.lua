
-- Copyright (C) 2010 Lionel Elie Mamane <lionel@mamane.lu>.
--
-- This file is part of quvi <http://quvi.sourceforge.net/>.
--
-- This program is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- This program is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with this program.  If not, see <http://www.gnu.org/licenses/>.

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "collegehumor.com"
    t.formats = "default"
-- Uncomment to re-enable. See also: http://is.gd/g9b4K
--    t.formats = "default|best|hd"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "collegehumor"
   
    local success, _, vid =
        video.page_url:find("^http://www%.collegehumor%.com/video:(%d+)")

    if ( success == nil ) then
        error("expected URL beginning with 'http://collegehumor.com/video:'")
    end

    video.id = vid

    local page = quvi.fetch(video.page_url)
    local _,_,s = page:find('<meta name="title" content="(.-)"')
    video.title = s or error("no match: video title")

    local page =
        quvi.fetch("http://www.collegehumor.com/moogaloop/video:" .. vid,
            'config')

    local _,_,s = page:find('<file>([%w%p]+)</file>')
    local default = s or error("no match: default quality URL")

    local _,_,s = page:find('<hq>([%w%p]+)</hq>')
    local hq = s

    video.url = { default }
    if (video.requested_format == "hd" or video.requested_format == "best") then
        if ( hq ~= nil ) then
	        video.url = { hq }
	    end
    end

    return video
end
