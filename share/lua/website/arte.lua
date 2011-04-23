
-- quvi
-- Copyright (C) 2011  Raphaël Droz <raphael.droz+floss@gmail.com>
--
-- This file is part of quvi <http://quvi.googlecode.com/>.
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

-- Identify the script.
function ident(self)
    local t      = {}
    t.domain     = "arte.tv"
    t.formats    = "default|hd|de|hd-de|best"
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    t.categories = C.proto_rtmp
    t.handles    =
        (self.page_url ~= nil and self.page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse(self)
    self.host_id = 'arte'
    local page   = quvi.fetch(self.page_url)

    -- local _,_,_,s,t =
    --    page:find('addToPlaylistOpen(.-)/videos/([%w_]+)%-(%d+)%.html')

    -- id will be overriden by per-language id
    -- self.id = t or error('no match: id')
    -- filename is of no use yet
    -- self.filename = s or error('no match: filename')

    -- language-agnostic configuration:
    -- http://videos.arte.tv/fr/do_delegate/videos
    -- /<title>-<id>,view,asPlayerXml.xml
    local _,_,s      = page:find('videorefFileUrl = "(.-)"')
    local config_url = s or error('no match: config url')
    local config     = quvi.fetch(config_url, {fetch_type = 'config'})

    -- per-language XML configuration URL
    local _,_,s         = config:find('lang="fr" ref="(.-)"')
    local config_url_fr = s or error("no match: config url: fr")

    local _,_,s         = config:find('lang="de" ref="(.-)"')
    local config_url_de = s or config_url_fr -- fallback to "fr" URL

    -- 'best' is just an alias for 'hd'. Assume it to be the 'best'
    -- (in quality) of all known and available formats.
    local r = self.requested_format
    r = (r == "best") and "hd" or r

    -- select the URL according to the language requested (default to 'fr')
    local config_url =
        (r ~= "default" and r:find("de")) and config_url_de or config_url_fr

    -- per-language configuration file:
    local config = quvi.fetch(config_url, {fetch_type = 'config'})
    local _,_,s  = config:find('<name>(.-)</name>')
    self.title   = s or error('no match: title')

    -- "sd" is our "default".
    local q =
        (r:find("hd") and config:find('<url quality="hd">')) and "hd" or "sd"

    -- language-specific video ID and URL
    local _,_,s = config:find('<video id="(%d+)"')
    self.id     = s or error('no match: video id')

    local _,_,s = config:find('<url quality="' ..q.. '">(.-)</url>')
    self.url    = {s or error('no match: video url')}

    return self
end

-- Local variables:
-- mode: lua
-- fill-column: 80
-- auto-fill-mode: 1
-- lua-indent-level: 4
-- indent-tabs-mode: nil
-- End:
-- vim: set ts=4 sw=4 tw=72 expandtab:
