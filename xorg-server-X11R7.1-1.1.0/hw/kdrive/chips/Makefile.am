INCLUDES = 					\
	@KDRIVE_INCS@				\
	-I$(top_srcdir)/hw/kdrive/vesa		\
	@KDRIVE_CFLAGS@

bin_PROGRAMS = Xchips

if TSLIB
TSLIB_FLAG = -lts
endif

noinst_LIBRARIES = libchips.a

libchips_a_SOURCES = 	\
	chipsdraw.c	\
	chips.c		\
	chips.h	

Xchips_SOURCES = \
	chipsstub.c

CHIPS_LIBS =					\
	libchips.a 				\
	$(top_builddir)/hw/kdrive/vesa/libvesa.a \
	@KDRIVE_LIBS@

Xchips_LDADD = \
	$(CHIPS_LIBS)				\
	@KDRIVE_LIBS@                          \
	@XSERVER_LIBS@				\
	$(TSLIB_FLAG)

Xchips_DEPENDENCIES = $(CHIPS_LIBS) @KDRIVE_LIBS@
