SUBDIRS = doc pcl pcl-mono raster ps etc

bin_PROGRAMS = Xprt

Xprt_CFLAGS = @SERVER_DEFINES@ @DIX_CFLAGS@ @XPRINT_CFLAGS@ \
	-DXPRINT -DPRINT_ONLY_SERVER -D_XP_PRINT_SERVER_  \
	-DXPRINTDIR=\"$(prefix)/X11/xserver\"    \
	-DXPRASTERDDX -DXPPCLDDX -DXPMONOPCLDDX -DXPPSDDX

Xprt_LDFLAGS = -L$(top_srcdir)
Xprt_LDADD = @XPRINT_LIBS@ ps/libps.la raster/libraster.la  \
	pcl/libpcl.la pcl-mono/libpcl.la  \
	../mfb/libmfb.la ../mi/libmi.la  \
	../cfb32/libcfb32.la ../cfb/libcfb.la ../Xext/libXext.la \
	@FREETYPE_LIBS@

miinitext-wrapper.c:
	echo "#include \"$(top_srcdir)/mi/miinitext.c\"" >> $@

dpmsstubs-wrapper.c:
	echo "#include \"$(top_srcdir)/Xext/dpmsstubs.c\"" >> $@

Xprt_SOURCES =			\
	attributes.c		\
	attributes.h		\
	AttrValid.c		\
	AttrValid.h		\
	ddxInit.c		\
	DiPrint.h		\
	Init.c			\
	mediaSizes.c		\
	Oid.c			\
	OidDefs.h		\
	Oid.h			\
	OidStrs.h		\
	spooler.c		\
	spooler.h		\
	Util.c			\
	miinitext-wrapper.c	\
	dpmsstubs-wrapper.c

EXTRA_DIST = ValTree.c
