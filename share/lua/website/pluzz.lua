
-- quvi
-- Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
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

-- NOTE: pluzz.fr videos expire 7 days after their original broadcast
-- on France Televisions

-- Identify the script.
function ident (self)
    local t      = {}
    t.domain     = "pluzz.fr"
    t.formats    = "default"
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    t.categories = C.proto_mms
    -- TODO: Use quvi/util:handles instead
    t.handles    =
        (self.page_url ~= nil and self.page_url:find (t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (self)
    self.host_id = "pluzz"

    local page   = quvi.fetch (self.page_url)
    local _,_,s  = page:find('<div id="playerCtnr">.-<a href="(.-)"')

    local errmsg = 
        "no match: video id (note: pluzz.fr videos expire 7 days after "
        .. "their original broadcast on France Televisions)"

    self.redirect_url = s or error (errmsg)

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
