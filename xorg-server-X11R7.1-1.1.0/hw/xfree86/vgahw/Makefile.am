module_LTLIBRARIES = libvgahw.la
libvgahw_la_LDFLAGS = -avoid-version
libvgahw_la_SOURCES = vgaHW.c vgaHWmodule.c
INCLUDES = $(XORG_INCS) -I$(srcdir)/../ddc -I$(srcdir)/../i2c
AM_CFLAGS = $(XORG_CFLAGS) @SERVER_DEFINES@ @MODULE_DEFINES@ @LOADER_DEFINES@

sdk_HEADERS = vgaHW.h

EXTRA_DIST = vgaCmap.c
