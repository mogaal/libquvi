
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

-- arte videos reportedly expire after ~7 days.

-- Default ('default', 'best') to "fr". In order to access the German
-- ('de') videos, use 'de' or 'hd-de'.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "videos.arte.tv"
    r.formats    = "default|hd|de|hd-de|best"
    r.categories = C.proto_rtmp
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/%w+/videos/"})
    return r
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

    -- per-language XML configuration URL (default to 'fr')
    local _,_,conf_url_fr = config:find('lang="fr" ref="(.-)"')
    local _,_,conf_url_de = config:find('lang="de" ref="(.-)"')

    local config_url = conf_url_fr or conf_url_de

    -- 'best' is just an alias for 'hd'. Assume it to be the 'best'
    -- (in quality) of all known and available formats.
    local r = self.requested_format
    r = (r == "best") and "hd" or r

    -- user requested specifically 'de'.
    if r ~= 'default' and r:find('de') then
        if not conf_url_de then
            error("no match: config url: de")
        else
            config_url = conf_url_de
        end
    end

    if not config_url then
        error("no match: config url: fr or de "
          .. "(note: videos expire 7 days after their original "
          .. "broadcast)")
    end

    -- per-language configuration file:
    local config = quvi.fetch(config_url, {fetch_type = 'config'})
    local _,_,s  = config:find('<name>(.-)</name>')
    self.title   = s or error('no match: title')

    -- check which quality is available.
    local _,_,sd_url = config:find('url quality="sd">(.-)<')
    local _,_,hd_url = config:find('url quality="hd">(.-)<')

    local url = sd_url or hd_url -- default to sd.
    url = (hd_url and r:find('hd')) and hd_url or url

    self.url = {url or error("no match: media url")}

    -- language-specific video ID and URL
    local _,_,s = config:find('<video id="(%d+)"')
    self.id     = s or error('no match: video id')

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
