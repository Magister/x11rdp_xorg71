noinst_LTLIBRARIES = libss.la

AM_CFLAGS = \
	$(DIX_CFLAGS) \
	@SERVER_DEFINES@ \
	@MODULE_DEFINES@ \
	@LOADER_DEFINES@ \
	@GLX_DEFINES@

INCLUDES = -I@MESA_SOURCE@/include \
           -I../X \
           -I../array_cache \
           -I../glapi \
           -I../main \
           -I../math \
           -I../shader \
           -I../swrast \
           -I../swrast_setup \
           -I../tnl \
           -I.. \
           -I$(top_srcdir)/hw/xfree86/os-support

nodist_libss_la_SOURCES = ss_context.c ss_triangle.c
