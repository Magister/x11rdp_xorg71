module_LTLIBRARIES = libshadowfb.la
libshadowfb_la_LDFLAGS = -avoid-version
libshadowfb_la_SOURCES = sfbmodule.c shadow.c

sdk_HEADERS = shadowfb.h

INCLUDES = $(XORG_INCS)

AM_CFLAGS = $(XORG_CFLAGS) @SERVER_DEFINES@ @MODULE_DEFINES@ @LOADER_DEFINES@
