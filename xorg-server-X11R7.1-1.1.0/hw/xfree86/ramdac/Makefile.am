module_LTLIBRARIES = libramdac.la

libramdac_la_LDFLAGS = -avoid-version

libramdac_la_SOURCES = xf86RamDacMod.c xf86RamDac.c xf86RamDacCmap.c \
                      xf86Cursor.c xf86HWCurs.c IBM.c BT.c TI.c \
		      xf86BitOrder.c

sdk_HEADERS = BT.h IBM.h TI.h xf86Cursor.h xf86RamDac.h

DISTCLEANFILES = xf86BitOrder.c
EXTRA_DIST = BTPriv.h IBMPriv.h TIPriv.h xf86CursorPriv.h xf86RamDacPriv.h \
	CURSOR.NOTES

AM_CFLAGS = -DXAAReverseBitOrder=xf86ReverseBitOrder -DRAMDAC_MODULE \
            $(XORG_CFLAGS) @SERVER_DEFINES@ @MODULE_DEFINES@ @LOADER_DEFINES@
INCLUDES = $(XORG_INCS)

xf86BitOrder.c:
	echo "#define XAAReverseBitOrder xf86ReverseBitOrder" > $@
	echo "#include \"$(srcdir)/../xaa/xaaBitOrder.c\"" >> $@
