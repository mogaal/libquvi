
-- quvi
-- Copyright (C) 2011  Toni Gundogdu <legatvs@gmail.com>
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

-- NOTE: Most videos expire some (7?) days after their original broadcast

local Arte = {} -- Utility functions unique to to this script.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "videos.arte.tv"
    r.formats    = "default|best"
    r.categories = C.proto_rtmp
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, {"/%w+/videos/"})
    return r
end

-- Query available formats.
function query_formats(self)
    local config  = Arte.get_config(self)
    local U       = require 'quvi/util'
    local formats = Arte.iter_formats(config, U)

    local t = {}
    for _,v in pairs(formats) do
        table.insert(t, Arte.to_s(v))
    end

    table.sort(t)
    self.formats = table.concat(t, "|")

    return self
end

-- Parse media URL.
function parse(self)
    self.host_id  = 'arte'
    local config  = Arte.get_config(self)
    local U       = require 'quvi/util'
    local formats = Arte.iter_formats(config, U)
    local format  = U.choose_format(self, formats,
                                    Arte.choose_best,
                                    Arte.choose_default,
                                    Arte.to_s)
    self.title         = format.title or error('no match: title')
    self.id            = format.id or error('no match: id')
    self.thumbnail_url = format.thumb or ''
    self.url           = {format.url or error("no match: media url")}

    return self
end

--
-- Utility functions
--

function Arte.get_config(self)
    local page       = quvi.fetch(self.page_url)
    local _,_,s      = page:find('videorefFileUrl = "(.-)"')
    local config_url = s or error('no match: config url')
    local config     = quvi.fetch(config_url, {fetch_type = 'config'})
    return Arte.get_lang_config(config)
end

function Arte.get_lang_config(config)
    local t = {}
    for lang,url in config:gfind('<video lang="(%w+)" ref="(.-)"') do
        table.insert(t, {lang=lang,
                         config=quvi.fetch(url, {fetch_type = 'config'})})
    end
    return t
end

function Arte.iter_lang_formats(lang_config, t, U)

    local p = '<video id="(%d+)" lang="(%w+)"'
           .. '.-<name>(.-)<'
           .. '.-<firstThumbnailUrl>(.-)<'
           .. '.-<dateExpiration>(.-)<'
           .. '.-<dateVideo>(.-)<'

    local config = lang_config.config

    local id,lang,title,thumb,exp,date = config:match(p)
    if not id then error("no match: media id, etc.") end

    if lang ~= lang_config.lang then
        error("no match: lang")
    end

    if Arte.has_expired(exp, U) then
        error('error: media no longer available (expired)')
    end

    local urls = config:match('<urls>(.-)</urls>')
                  or error('no match: urls')

    for q,u in urls:gfind('<url quality="(%w+)">(.-)<') do
--        print(q,u)
        table.insert(t, {lang=lang,   quality=q,   url=u,
                         thumb=thumb, title=title, id=id})
    end
end

function Arte.iter_formats(config, U)
    local t = {}
    for _,v in pairs(config) do
        Arte.iter_lang_formats(v, t, U)
    end
    return t
end

function Arte.has_expired(s, U)
    return U.to_timestamp(s) - os.time() < 0
end

function Arte.choose_best(formats) -- Whatever matches 'hd' first
    local r
    for _,v in pairs(formats) do
        if Arte.to_s(v):find('hd') then
            return v
        end
    end
    return r
end

function Arte.choose_default(formats) -- Whatever matches 'sd' first
    local r
    for _,v in pairs(formats) do
        if Arte.to_s(v):find('sd') then
            return v
        end
    end
    return r
end

function Arte.to_s(t)
    return string.format("%s_%s", t.quality, t.lang)
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
