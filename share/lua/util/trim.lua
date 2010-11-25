
-- Copyright (C) 2010 Toni Gundogdu.
--
-- This file is part of quvi <http://quvi.googlecode.com/>.
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


