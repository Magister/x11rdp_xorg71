noinst_LTLIBRARIES = libxkb.la libxkbstubs.la

AM_CFLAGS = $(DIX_CFLAGS) @SERVER_DEFINES@ @LOADER_DEFINES@ \
	-DHAVE_XKB_CONFIG_H

DDX_SRCS = \
        ddxBeep.c \
        ddxCtrls.c \
        ddxFakeBtn.c \
        ddxFakeMtn.c \
        ddxInit.c \
        ddxKeyClick.c \
        ddxLEDs.c \
        ddxLoad.c \
        ddxList.c \
        ddxDevBtn.c

DIX_SRCS = \
        xkb.c \
        xkbUtils.c \
        xkbEvents.c \
        xkbAccessX.c \
        xkbSwap.c \
        xkbLEDs.c \
        xkbInit.c \
        xkbActions.c \
        xkbPrKeyEv.c

# this should be replaced by a common library or something, ideally -d
XKBFILE_SRCS = \
        maprules.c \
        xkmread.c \
        xkbfmisc.c \
        xkberrs.c

X11_SRCS = \
        XKBMisc.c \
        XKBAlloc.c \
        XKBGAlloc.c \
        XKBMAlloc.c

XI_SRCS = xkbPrOtherEv.c

libxkb_la_SOURCES = $(DDX_SRCS) $(DIX_SRCS) $(XI_SRCS) $(XKBFILE_SRCS) \
                    $(X11_SRCS)
libxkbstubs_la_SOURCES = ddxVT.c ddxPrivate.c ddxKillSrv.c

EXTRA_DIST = xkb.h xkbDflts.h

xkbcompileddir = $(XKB_COMPILED_DIR)
dist_xkbcompiled_DATA = README.compiled
