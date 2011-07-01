
-- quvi
-- Copyright (C) 2011 Paul Kocialkowski <contact@paulk.fr>
-- Based on http://userscripts.org/scripts/show/87011
-- Which is (C) sebaro, released under the GNU GPL
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

local Megavideo = {} -- Utility functions unique to this script.

-- Identify the script.
function ident(self)
    package.path = self.script_dir .. '/?.lua'
    local C      = require 'quvi/const'
    local r      = {}
    r.domain     = "megavideo.com"
    r.formats    = "default"
    r.categories = C.proto_http
    local U      = require 'quvi/util'
    r.handles    = U.handles(self.page_url, {r.domain}, nil, {'v=[%w]+'})
    return r
end

-- Query available formats.
function query_formats(self)
    self.formats = 'default'
    return self
end

-- Parse media URL.
function parse(self)
    self.host_id = "megavideo"

    local _,_,s = self.page_url:find('?v=([^&]+)')
    self.id     = s or error ("no match: media id")

    local videolink_url =
        "http://www.megavideo.com/xml/videolink.php?v=" .. self.id

    local videolink_url_page =
        quvi.fetch(videolink_url, {fetch_type = 'config'})

    local _,_,s = videolink_url_page:find(' title="([^"]+)"')
    self.title  = s or error ("no match: media title")
    self.title  = string.gsub(self.title, "+", " ")

    local _,_,s = videolink_url_page:find(' un="([^"]+)"')
    local str   = s or error ("no match: str")

    local _,_,s = videolink_url_page:find(' k1="([^"]+)"')
    local k1    = s or error ("no match: k1")

    local _,_,s = videolink_url_page:find(' k2="([^"]+)"')
    local k2    = s or error ("no match: k2")

    local _,_,s = videolink_url_page:find(' s="([^"]+)"')
    local ser   = s or error ("no match: s")

    local _,_,s = videolink_url_page:find('(error="1")')
    local err   = s

    if err then
        local _,_,s     = videolink_url_page:find('errortext="([^"]+)"')
        local err_txt   = s

        error (err_txt)
    end

    local loc1 = {}
    local loc6 = {}
    local loc7 = {}
    local loc3
    local loc12
    local loc9
    local loc2 = {}

    for loc3 = 1,string.len(str)
    do
        local s = "000"
          .. Megavideo.decimal2binary(
                Megavideo.hexchar2decimal(string.sub(str,loc3,loc3)))
        table.insert(loc1, string.sub(s, string.len(s)-3))
    end

    loc1 = Megavideo.splitall(table.concat(loc1))

    for loc3 = 0, 384-1
    do
        k1 = (k1 * 11 + 77213) % 81371;
		k2 = (k2 * 17 + 92717) % 192811;
		loc6[loc3+1] = (k1 + k2) % 128;
    end

    loc3 = 256
    while loc3 >= 0
    do
		local loc5 = loc6[loc3+1];
		local loc4 = loc3 % 128;
		local loc8 = loc1[loc5+1];
		loc1[loc5+1] = loc1[loc4+1];
		loc1[loc4+1] = loc8;

        loc3=loc3-1
    end

    for loc3=0, 128-1
    do
        loc1[loc3+1] =
          Megavideo.binaryxor(loc1[loc3 + 1], loc6[loc3 + 1 + 256]) % 2
    end

    loc12 = table.concat(loc1)

    loc3 = 0
	while loc3 < string.len(loc12)
    do
        loc9 = string.sub(loc12, loc3+1, loc3+4)
        table.insert(loc7, loc9)
        loc3 = loc3+4
	end

    for loc3 = 0,table.getn(loc7)-1
    do
        table.insert(loc2, Megavideo.decimal2hex(
                            Megavideo.binary2decimal(tonumber(loc7[loc3+1]))) )
    end

    self.url = {
        string.format("http://www%s.megavideo.com/files/%s/video.flv",
            ser, table.concat(loc2))
    }

    return self
end

--
-- Utility functions
--

function Megavideo.hexchar2decimal(s)
    local n = tonumber(s) or string.byte(string.upper(s)) - 55

    if n > 15 then
        error("Hexadecimal value is too high.")
    end

    return n
end

function Megavideo.decimal2hex(s)
    return string.format("%x", s)
end

function Megavideo.decimal2binary(s)
    local bin   = 0
    local i     = 0

    while s > 0
    do
        bin=bin + (s%2) * math.pow(10, i)
		s=(s-s%2)/2
        i=i+1
    end

    return bin
end

function Megavideo.binary2decimal(s)
    local dec   = 0
    local i     = 0

    while s > 0
    do
        dec = dec + (s%10) * math.pow(2, i)
        s=(s-(s % 10)) / 10
        i=i+1
    end

    return dec
end

function Megavideo.splitall(s)
    local c = ""
    local i = 1
    local t = {}

    while i <= string.len(s)
    do
        c = string.sub(s, i, i)
        table.insert(t, c)
        i=i+1
    end

    return t
end

function Megavideo.binaryxor(s1, s2)
    local r     = 0
    local t     = 1
    local i     = 0
    local l     = math.abs(s1 - s2)

    while t > 0
    do
        if ((s1%2) + (s2%2)) == 1
        then
            r=r+math.pow(2, i)
        end

		s1=(s1-s1%2)/2
		s2=(s2-s2%2)/2

        i=i+1
        t=s1+s2
    end

    return r
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
