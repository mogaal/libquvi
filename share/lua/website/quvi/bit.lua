
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

local M = {}

function M.bit_or (x, y) -- http://is.gd/iVg4x
    local p = 1
    while p < x do p = p + p end
    while p < y do p = p + p end
    local z = 0
    repeat
        if p <= x or p <= y then
            z = z + p
            if p <= x then x = x - p end
            if p <= y then y = y - p end
        end
        p = p * 0.5
    until p < 1
    return z
end

-- 1-based indexing
function M.bit (p) return 2 ^ (p - 1) end

-- e.g. "if has_bit (foo, bit (n)) then ..."
function M.has_bit(x, p) return x % (p + p) >= p end

return M

-- vim: set ts=4 sw=4 tw=72 expandtab:
