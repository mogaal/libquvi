libquvisubincludedir=$(includedir)/quvi-0.9/quvi

libquvisubinclude_HEADERS=\
  qbool.h\
  qcallback.h\
  qdef.h\
  qerror.h\
  qfunc.h\
  qhttpmiprop.h\
  qinfo.h\
  qmediaprop.h\
  qoption.h\
  qplaylistprop.h\
  qscript.h\
  qsubtprop.h\
  qsupp.h\
  qversion.h

install-data-local:
	@if test -f $(libquvisubincludedir)/qfunc.h ; then \
		echo "** Old headers found in $(libquvisubincludedir).  You should" ; \
		echo "** remove the contents of this directory and type 'make " ; \
		echo "** install' again." ; \
		false ; \
	fi

# vim: set ts=2 sw=2 tw=72 expandtab:
