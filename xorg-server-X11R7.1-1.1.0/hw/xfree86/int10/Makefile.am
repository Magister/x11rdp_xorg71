module_LTLIBRARIES = libint10.la

sdk_HEADERS = xf86int10.h

EXTRA_CFLAGS = @SERVER_DEFINES@ @MODULE_DEFINES@ @LOADER_DEFINES@

libint10_la_LDFLAGS = -avoid-version

if INT10_VM86
AM_CFLAGS = -D_PC -D_VM86_LINUX $(XORG_CFLAGS) $(EXTRA_CFLAGS)
INCLUDES = $(XORG_INCS)
libint10_la_SOURCES = \
	helper_exec.c \
	helper_mem.c \
	pci.c \
	xf86int10.c \
	xf86int10module.c \
	$(srcdir)/../os-support/linux/int10/vm86/linux_vm86.c \
	$(srcdir)/../os-support/linux/int10/linux.c
endif

if INT10_X86EMU
AM_CFLAGS = -D_X86EMU -DNO_SYS_HEADERS -D_PC $(XORG_CFLAGS) $(EXTRA_CFLAGS)
INCLUDES = $(XORG_INCS) -I$(srcdir)/../x86emu
libint10_la_SOURCES = pci.c xf86int10module.c helper_exec.c helper_mem.c \
	xf86int10.c xf86x86emu.c generic.c x86emu.c
endif

if INT10_STUB
AM_CFLAGS = -D_PC -D_VM86_LINUX $(XORG_CFLAGS) $(EXTRA_CFLAGS)
libint10_la_SOURCES = stub.c xf86int10module.c
endif

EXTRA_DIST = xf86x86emu.h INT10.HOWTO
