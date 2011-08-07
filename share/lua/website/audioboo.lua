
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

-- Identify the script.
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "audioboo.fm"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/boos/%d+%-"})
    return r
end

-- Query available formats.
function query_formats(self)
    self.formats = 'default'
    return self
end

-- Parse media URL.
function parse (self)
    self.host_id = "audioboo"

    local oe_url =
        "http://audioboo.fm/publishing/oembed.json?url=" .. self.page_url

    local oe = quvi.fetch(oe_url, {fetch_type='config'})

    self.title = oe:match('"title":"(.-)"')
                    or error('no match: media title')

    self.thumbnail_url = oe:match('"thumbnail_url":"(.-)"') or ''

    self.id = oe:match('id=."boo_embed_(.-)."')
                or error('no match: media id')

    self.url = {oe:match('a href=."(.-)."')
                    or error('no match: media url')}

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
