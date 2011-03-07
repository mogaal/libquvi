
-- quvi
-- Copyright (C) 2010  quvi project
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
    r.domain     = "cbsnews.com"
    r.formats    = "default|best|1100k|598k|386k"
    r.categories = C.proto_http
    r.handles    =
        (self.page_url ~= nil and self.page_url:find(r.domain) ~= nil)
    return r
end

-- Parse video URL.
function parse (self)
    self.host_id = "cbsnews"
    local page   = quvi.fetch(self.page_url)

    -- Need "? because some videos have the " and some don't
    local _,_,s = page:find('CBSVideo.setVideoId%("?(.-)"?%);')
    self.id     = s or error ("no match: video id")

    -- Goto cnet and get the url for the xml of the available video formats
    s =  "http://api.cnet.com/restApi/v1.0/videoSearch?videoIds="
        .. s
        .. "&iod=videoMedia"

    local xml = quvi.fetch (s, {fetch_type = 'config'})

    -- Grab title from the XML.
    local _,_,s = xml:find ('<Title>.-CDATA%[(.-)%]')
    self.title  = s or error ("no match: video title")

    -- Go over the URLs. Figure out 'best' based on bitrate.
    local t    = {}
    local best = 0

    for url in xml:gfind ('<DeliveryUrl>.-CDATA%[(.-)%]') do
        local _,_,s = url:find('_(%d+)%.%w+$')
        local n     = tonumber (s)
        if (best < n) then best = n end
        t[n]        = url
    end

    local url = nil

    if (self.requested_format == 'best') then
       url = t[best]
    else

        -- Try to match requested_format to known IDs. Strip non-digits.
        local _,_,f = self.requested_format:find ('(%d+)')
        table.foreach (t,
            function (k,v) if (tonumber (f) == k) then url = v end end)

        -- Use whatever is in the table as our 'default' if above failed.
        if (url == nil) then
            table.foreach (t,
                function (k,v) if (url == nil) then url = v end end)
        end
    end

    self.url = {url}

    return self
end

-- vim: set ts=4 sw=4 tw=72 expandtab:
