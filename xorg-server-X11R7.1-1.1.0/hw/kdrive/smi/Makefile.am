INCLUDES = 					\
	-I$(top_srcdir)/hw/kdrive/fbdev		\
	-I$(top_srcdir)/hw/kdrive/vesa		\
	@KDRIVE_INCS@				\
	@KDRIVE_CFLAGS@

bin_PROGRAMS = Xsmi

if TSLIB
TSLIB_FLAG = -lts
endif

noinst_LIBRARIES = libsmi.a

#	smivideo.c	# not ready yet

libsmi_a_SOURCES = 	\
	smi.c		\
	smi.h		\
	smidraw.c	\
	smidraw.h

Xsmi_SOURCES = \
	smistub.c

SMI_LIBS =					\
	libsmi.a 				\
	$(top_builddir)/hw/kdrive/fbdev/libfbdev.a \
	$(top_builddir)/hw/kdrive/vesa/libvesa.a \
	@KDRIVE_LIBS@

Xsmi_LDADD = \
	$(SMI_LIBS)				\
	@KDRIVE_LIBS@                          \
	@XSERVER_LIBS@					\
	$(TSLIB_FLAG)

Xsmi_DEPENDENCIES = $(SMI_LIBS) @KDRIVE_LIBS@
