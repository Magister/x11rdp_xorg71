if AIGLX
GLXDRI_LIBRARY = libglxdri.la
endif

noinst_LTLIBRARIES = libglx.la $(GLXDRI_LIBRARY)

AM_CFLAGS = \
	-I@MESA_SOURCE@/include \
	@DIX_CFLAGS@ \
	@GL_CFLAGS@ \
	@XLIB_CFLAGS@ \
	@LIBDRM_CFLAGS@ \
	@DRIPROTO_CFLAGS@ \
	-I@MESA_SOURCE@/src/mesa/glapi \
	-I@MESA_SOURCE@/src/mesa/main \
	@SERVER_DEFINES@ \
	@MODULE_DEFINES@ \
	@LOADER_DEFINES@ \
	-DNO_LIBCWRAPPER \
	@GLX_DEFINES@

# none yet
#sdk_HEADERS =

INCLUDES = \
	-I$(top_srcdir)/hw/xfree86/os-support \
	-I$(top_srcdir)/hw/xfree86/os-support/bus \
	-I$(top_srcdir)/hw/xfree86/common \
	-I$(top_srcdir)/hw/xfree86/dri \
	-I$(top_srcdir)/mi


nodist_libglx_la_SOURCES = indirect_size.h

libglxdri_la_SOURCES = \
        glxdri.c

libglx_la_SOURCES = \
        g_disptab.c \
        g_disptab.h \
        g_disptab_EXT.c \
        g_disptab_EXT.h \
        glapi.c \
        glcontextmodes.c \
        glcontextmodes.h \
        global.c \
        glthread.c \
        glxcmds.c \
        glxcmdsswap.c \
        glxcontext.h \
        glxdrawable.h \
        glxerror.h \
        glxext.c \
        glxext.h \
	glxvisuals.c \
        glxscreens.c \
        glxscreens.h \
        glxserver.h \
        glxutil.c \
        glxutil.h \
        indirect_dispatch.c \
        indirect_dispatch.h \
        indirect_dispatch_swap.c \
        indirect_reqsize.c \
        indirect_reqsize.h \
        indirect_size_get.c \
        indirect_size_get.h \
        indirect_util.c \
        indirect_util.h \
        render2.c \
        render2swap.c \
        renderpix.c \
        renderpixswap.c \
        rensize.c \
        rensizetab.c \
        single2.c \
        single2swap.c \
        singlepix.c \
        singlepixswap.c \
        singlesize.c \
        singlesize.h \
        unpack.h \
        xfont.c
