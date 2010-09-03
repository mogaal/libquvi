
DIST_lua = \
lua/README \
lua/util/content_type.lua \
lua/util/charset.lua \
lua/website/README \
lua/website/break.lua \
lua/website/buzzhumor.lua \
lua/website/charlierose.lua \
lua/website/clipfish.lua \
lua/website/dailymotion.lua \
lua/website/evisor.lua \
lua/website/funnyhub.lua \
lua/website/funnyordie.lua \
lua/website/golem.lua \
lua/website/google.lua \
lua/website/liveleak.lua \
lua/website/myubo.lua \
lua/website/sevenload.lua \
lua/website/spiegel.lua \
lua/website/theonion.lua \
lua/website/vimeo.lua \
lua/website/youtube.lua

if ENABLE_SMUT
DIST_lua += \
lua/website/tube8.lua \
lua/website/xvideos.lua \
lua/website/youjizz.lua
endif

if ENABLE_BROKEN
DIST_lua += \
lua/website/cctv.lua
endif

EXTRA_DIST = $(DIST_lua)

nobase_dist_pkgdata_DATA = $(DIST_lua) $(DIST_howtos)
