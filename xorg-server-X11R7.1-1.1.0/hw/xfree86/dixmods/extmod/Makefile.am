sdk_HEADERS = dgaproc.h

extsmoduledir = $(moduledir)/extensions
extsmodule_LTLIBRARIES = libextmod.la

if DGA
DGA_SRCS = xf86dga.c xf86dga2.c dgaproc.h xf86dgaext.h
endif

if XV
XV_SRCS = xvmod.c xvmodproc.h
endif

AM_CFLAGS = @XORG_CFLAGS@ @SERVER_DEFINES@ @LOADER_DEFINES@
INCLUDES = @XORG_INCS@ \
           -I$(top_srcdir)/afb \
           -I$(top_srcdir)/mfb \
           -I$(top_srcdir)/cfb \
           -I$(top_srcdir)/dbe \
           -I$(top_srcdir)/hw/xfree86/loader \
           -I$(top_srcdir)/miext/shadow

libextmod_la_LDFLAGS = -avoid-version
libextmod_la_SOURCES = modinit.c \
                       modinit.h \
                       $(DGA_SRCS) \
                       xf86misc.c \
                       xf86miscproc.h \
                       xf86vmode.c \
                       vidmodeproc.h \
                       $(XV_SRCS)
libextmod_la_LIBADD = $(top_builddir)/Xext/libXextmodule.la
