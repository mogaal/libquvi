
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

-- Parse file suffix (extension) from content-type string.
function suffix_from_contenttype (ctype)

    -- Ideally, we'd parse these from /etc/mime-types.
    -- In reality, we need a more cross-platform solution.

    if (ctype:find ("text/html")) then
        error (
            'content-type cannot be "' ..ctype.. '" for a video. '
            .. 'The rule script for this website is either buggy or incomplete.'
        )
    end

    local _,_,s = ctype:find("/(.-)$")
    s = s or error ("no match: content type")
    s = s:gsub("^x%-","")

    if (s:find("octet")
        or s:find("swf")
        or s:find("flash")
        or s:find("plain"))
    then
        s = "flv"
    end

    return s
end


