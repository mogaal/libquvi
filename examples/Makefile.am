SUBDIRS=lib
noinst_PROGRAMS=$(EXAMPLE_PROGS)

examples_common_cppflags=\
  -I$(top_srcdir)/examples/lib\
  -I$(top_srcdir)/src/quvi-0.9/\
  $(glib_CFLAGS)\
  $(AM_CPPFLAGS)

examples_common_ldflags=\
  $(AM_LDFLAGS)

examples_common_ldadd=\
  $(top_builddir)/examples/lib/libconvenience_example.la\
  $(top_builddir)/src/libquvi-0.9.la

EXAMPLE_PROGS=supports

supports_SOURCES=supports.c
supports_CPPFLAGS=$(examples_common_cppflags)
supports_LDFLAGS=$(examples_common_ldflags)
supports_LDADD=$(examples_common_ldadd)
supports_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=playlist

playlist_SOURCES=playlist.c
playlist_CPPFLAGS=$(examples_common_cppflags)
playlist_LDFLAGS=$(examples_common_ldflags)
playlist_LDADD=$(examples_common_ldadd)
playlist_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=version

version_SOURCES=version.c
version_CPPFLAGS=$(examples_common_cppflags)
version_LDFLAGS=$(examples_common_ldflags)
version_LDADD=$(examples_common_ldadd)
version_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=resolve

resolve_SOURCES=resolve.c
resolve_CPPFLAGS=$(examples_common_cppflags)
resolve_LDFLAGS=$(examples_common_ldflags)
resolve_LDADD=$(examples_common_ldadd)
resolve_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=script

script_SOURCES=script.c
script_CPPFLAGS=$(examples_common_cppflags)
script_LDFLAGS=$(examples_common_ldflags)
script_LDADD=$(examples_common_ldadd)
script_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=media

media_SOURCES=media.c
media_CPPFLAGS=$(examples_common_cppflags)
media_LDFLAGS=$(examples_common_ldflags)
media_LDADD=$(examples_common_ldadd)
media_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=scan

scan_SOURCES=scan.c
scan_CPPFLAGS=$(examples_common_cppflags)
scan_LDFLAGS=$(examples_common_ldflags)
scan_LDADD=$(examples_common_ldadd)
scan_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=http_metainfo

http_metainfo_SOURCES=http_metainfo.c
http_metainfo_CPPFLAGS=$(examples_common_cppflags)
http_metainfo_LDFLAGS=$(examples_common_ldflags)
http_metainfo_LDADD=$(examples_common_ldadd)
http_metainfo_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=subtitle

subtitle_SOURCES=subtitle.c
subtitle_CPPFLAGS=$(examples_common_cppflags)
subtitle_LDFLAGS=$(examples_common_ldflags)
subtitle_LDADD=$(examples_common_ldadd)
subtitle_CFLAGS=$(AM_CFLAGS)

EXAMPLE_PROGS+=file_ext

file_ext_SOURCES=file_ext.c
file_ext_CPPFLAGS=$(examples_common_cppflags)
file_ext_LDFLAGS=$(examples_common_ldflags)
file_ext_LDADD=$(examples_common_ldadd)
file_ext_CFLAGS=$(AM_CFLAGS)

# vim: set ts=2 sw=2 tw=72 expandtab:
