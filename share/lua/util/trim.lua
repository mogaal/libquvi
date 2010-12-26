
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

function trim_fields (video)
    for k,v in pairs (video) do
        if (type (v) == 'table') then
            video[k] = trim_fields (v)
        else
            video[k] = trim (v)
        end
    end
    return video
end

function trim (s) -- Based on http://lua-users.org/wiki/StringTrim (trim1)
    s = (s:gsub ("^%s*(.-)%s*$", "%1"))
    return (s:gsub ("%s%s+", " "))
end


