noinst_LTLIBRARIES = librender.la

AM_CFLAGS = $(DIX_CFLAGS) @SERVER_DEFINES@ @LOADER_DEFINES@

librender_la_SOURCES =	\
	animcur.c	\
	filter.c	\
	glyph.c		\
	miglyph.c	\
	miindex.c	\
	mipict.c	\
	mirect.c	\
	mitrap.c	\
	mitri.c		\
	picture.c	\
	render.c	\
	renderedge.c

if XORG
sdk_HEADERS = picture.h mipict.h glyphstr.h picturestr.h renderedge.h
endif
