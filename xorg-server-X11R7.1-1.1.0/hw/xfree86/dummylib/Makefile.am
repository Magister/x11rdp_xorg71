# libdummy.a contains just those bits used in the server itself
# libdummy-nonserver.a contains additional routines normally found in the
# server for use in building the utilities like scanpci & the config tools

noinst_LIBRARIES = libdummy.a libdummy-nonserver.a

INCLUDES = $(XORG_INCS)

AM_CFLAGS = $(XORG_CFLAGS) @SERVER_DEFINES@ @LOADER_DEFINES@

libdummy_a_SOURCES = getvalidbios.c getemptypci.c \
                     pcitestmulti.c xf86allocscripi.c \
                     xf86addrestolist.c xf86drvmsg.c xf86drvmsgverb.c \
                     xf86getverb.c \
                     xf86opt.c xf86screens.c xf86servisinit.c xf86verbose.c \
                     #xf86errorf.c xf86errorfverb.c xf86msg.c xf86msgverb.c \
                     #logvwrite.c verrorf.c xf86info.c xalloc.c fatalerror.c \
                     #$(srcdir)/../os-support/shared/sigiostubs.c

libdummy_nonserver_a_SOURCES = \
	fatalerror.c \
	getvalidbios.c \
	getemptypci.c \
	logvwrite.c \
	pcitestmulti.c \
	verrorf.c \
	xalloc.c \
	xf86allocscripi.c \
	xf86addrestolist.c \
	xf86drvmsg.c \
	xf86drvmsgverb.c \
	xf86errorf.c \
	xf86errorfverb.c \
	xf86getpagesize.c \
	xf86getverb.c \
	xf86info.c \
	xf86msg.c \
	xf86msgverb.c \
	xf86opt.c \
	xf86screens.c \
	xf86servisinit.c \
	xf86verbose.c \
	$(srcdir)/../os-support/shared/sigiostubs.c

EXTRA_DIST = README dummylib.h
