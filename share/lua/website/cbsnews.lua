-- Copyright (C) 2010 quvi team.
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
-- 
-- http://www.cbsnews.com/video/watch/?id=7118769n
-- http://mediasd.cbsnews.com/media/2010/12/04/48_1204_FULLMOON_1100.m4v    

-- Identify the script.
function ident (page_url)
    local t   = {}
    t.domain  = "cbsnews.com"
    t.formats = "default|best|1100k|598k|386k"
    t.handles = (page_url ~= nil and page_url:find(t.domain) ~= nil)
    return t
end

-- Parse video URL.
function parse (video)
    video.host_id = "cbsnews"
    local page    = quvi.fetch(video.page_url)

    -- Need "? because some videos have the " and some don't
    local _,_,s = page:find('CBSVideo.setVideoId%("?(.-)"?%);')
    video.id    = s or error ("no match: video id")

    -- Goto cnet and get the url for the xml of the available video formats
    s =  "http://api.cnet.com/restApi/v1.0/videoSearch?videoIds="
        .. s
        .. "&iod=videoMedia"

    local xml  = quvi.fetch(s, 'config')

    -- Grab title from the XML.
    local _,_,s = xml:find ('<Title>.-CDATA%[(.-)%]')
    video.title = s or error ("no match: video title")

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

    if (video.requested_format == 'best') then
       url = t[best]
    else

        -- Try to match requested_format to known IDs. Strip non-digits.
        local _,_,f = video.requested_format:find ('(%d+)')
        table.foreach (t,
            function (k,v) if (tonumber (f) == k) then url = v end end)

        -- Use whatever is in the table as our 'default' if above failed.
        if (url == nil) then
            table.foreach (t,
                function (k,v) if (url == nil) then url = v end end)
        end
    end

    video.url = {url}

    return video
end
