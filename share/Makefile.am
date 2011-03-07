
DIST_lua = \
lua/README \
lua/util/content_type.lua \
lua/util/charset.lua \
lua/util/trim.lua \
lua/website/README \
lua/website/quvi/bit.lua \
lua/website/quvi/const.lua \
lua/website/quvi/util.lua \
lua/website/academicearth.lua \
lua/website/bloomberg.lua \
lua/website/break.lua \
lua/website/buzzhumor.lua \
lua/website/cbsnews.lua \
lua/website/collegehumor.lua \
lua/website/charlierose.lua \
lua/website/clipfish.lua \
lua/website/dailymotion.lua \
lua/website/gaskrank.lua \
lua/website/francetelevisions.lua \
lua/website/funnyhub.lua \
lua/website/funnyordie.lua \
lua/website/globo.lua \
lua/website/golem.lua \
lua/website/google.lua \
lua/website/liveleak.lua \
lua/website/pluzz.lua \
lua/website/publicsenat.lua \
lua/website/sapo.lua \
lua/website/sevenload.lua \
lua/website/spiegel.lua \
lua/website/tmunderground.lua \
lua/website/theonion.lua \
lua/website/vimeo.lua \
lua/website/youtube.lua

if ENABLE_NSFW
DIST_lua += \
lua/website/tube8.lua \
lua/website/xvideos.lua \
lua/website/youjizz.lua
endif

if ENABLE_BROKEN
DIST_lua +=
endif

EXTRA_DIST = $(DIST_lua)

nobase_dist_pkgdata_DATA = $(DIST_lua) $(DIST_howtos)
