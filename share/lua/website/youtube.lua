
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

-- http://en.wikipedia.org/wiki/YouTube#Quality_and_codecs
-- $container_$maxwidth = '$fmt_id'
local fmt_id_lookup = {
    -- flv
    flv_240p =  '5',
    flv_360p = '34',
    flv_480p = '35',
    -- mp4
     mp4_360p = '18',
     mp4_720p = '22',
    mp4_1080p = '37',
    mp4_3072p = '38',
    -- webm
    webm_480p = '43',
    webm_720p = '45',
    -- 3gp, use 'tgp' since lua won't accept '3gp'
    tgp_144p  = '17'
}

local domains = {
    "youtube.com",
    "youtube-nocookie.com",
    "youtu.be"
}

-- Check whether the domain is handled
function is_handled (page_url)
    if page_url == nil then
        return false
    end
    for k,domain in pairs(domains) do
        if page_url:find(domain) ~= nil then
            return true
        end
    end
    return false
end

-- Identify the script.
function ident (self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "youtube.com"
    r.formats    = ""
    for k,_ in pairs (fmt_id_lookup) do
        r.formats = r.formats .."|".. k
    end
    r.formats = "default|best" .. r.formats
    if (self.page_url ~= nil) then
        self.page_url = youtubify(self.page_url)
    end
    r.categories = C.proto_http
    r.handles    = is_handled(self.page_url)
    return r
end

function parse (self)
    self.host_id  = "youtube"
    local page_url = youtubify(self.page_url)

    local _,_,s = page_url:find("v=([%w-_]+)")
    self.id    = s or error ("no match: video id")

    local _,_,s = page_url:find('#t=(.+)')
    self.starttime = s or ''

    return get_video_info (self)
end

-- Youtubify the URL.
function youtubify (url)
    url = url:gsub("-nocookie", "")    -- youtube-nocookie.com
    url = url:gsub("/v/", "/watch?v=") -- embedded
    return url
end

function get_video_info (self, result)

    local _,_,s  = self.page_url:find ('^(%w+)://')
    local scheme = s or error ("no match: scheme")

    local config_url = string.format (
        "%s://www.youtube.com/get_video_info?&video_id=%s"
        .. "&el=detailpage&ps=default&eurl=&gl=US&hl=en",
            scheme, self.id)

    local opts   = { fetch_type = 'config' }
    local U      = require 'quvi/util'
    local config = U.decode (quvi.fetch (config_url, opts))

    if (config['reason']) then
        local reason = U.unescape (config['reason'])
        local code   = config['errorcode']
        error (reason..' (code='..code..')')
    end

    self.title = config['title'] or error ('no match: video title')
    self.title = U.unescape (self.title)

    local fmt_url_map =
        config['fmt_url_map'] or error ("no match: fmt_url_map")

    fmt_url_map = U.unescape (fmt_url_map) .. ','

    -- Assume first found URL to be the 'best'.
    local best  = nil
    local t     = {}

    for fmt,url in fmt_url_map:gfind ('(%d+)%|(.-),') do
        if (best == nil) then best = url end
        t[fmt] = url
    end

    -- Choose URL.
    local url = nil

    if (self.requested_format == 'best') then
        url = best
    else
        url = to_url (self.requested_format, t)
        if (url == nil and self.requested_format ~= 'default') then
            -- Fallback to our 'default'.
            url = to_url ('default', t)
        end
    end

    self.url = {url or error ("no match: video url")}

    return self
end

function to_url (fmt, t)

    if (fmt == 'default') then
        fmt = 'flv_240p'
    end

    -- Match format ID alias to YouTube fmt ID.
    local id = nil

    table.foreach (fmt_id_lookup,
        function (k,v) if (k == fmt) then id = v end end)

    -- Match fmt ID to available fmt IDs.
    local url = nil

    table.foreach (t,
        function (k,v) if (id == k) then url = v end end)

    return url
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
