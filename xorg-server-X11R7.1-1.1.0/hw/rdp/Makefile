
OBJS = rdpmain.o rdpdraw.o rdpinput.o rdpmisc.o rdpup.o miinitext.o fbcmap.o

LIBS = ../../dbe/.libs/libdbe.a \
	../../dix/.libs/libdix.a \
	../../fb/.libs/libfb.a \
	../../GL/glx/.libs/libglx.a \
	../../GL/mesa/.libs/libGLcore.a \
	../../mfb/.libs/libmfb.a \
	../../mi/.libs/libmi.a \
	../../os/.libs/libos.a \
	../../randr/.libs/librandr.a \
	../../record/.libs/librecord.a \
	../../render/.libs/librender.a \
	../../xkb/.libs/libxkb.a \
	../../XTrap/.libs/libxtrap.a \
	../../Xext/.libs/libXext.a \
	../../Xi/.libs/libXi.a \
	librdp.a \
	-lfreetype -lz -lm -lXfont -lXau -lXdmcp

CFLAGS = -O2 -fno-strength-reduce -ansi -pedantic \
	-I../../include -I../../cfb -I../../mfb -I../../mi \
	-I../../../fontsproto-X11R7.0-2.0.2 -I../../../xproto-X11R7.1-7.0.5 \
	-I../../../kbproto-X11R7.0-1.0.2 \
	-I../../../builddir/pkg-config/include \
	-I../../os -I../../render -I../xfree86/common -I../xfree86/os-support \
	-Dlinux -D__i386__ -D_POSIX_C_SOURCE=199309L -D_POSIX_SOURCE \
	-D_XOPEN_SOURCE=500L -D_BSD_SOURCE -D_SVID_SOURCE -D_REENTRANT \
	-DSHAPE -DBIGREQS -DGLXEXT -DGLX_USE_MESA -DMITMISC -DXTEST \
	-DPANORAMIX -DRANDR -DRENDER -DXCMISC -DXSYNC -DXRECORD \
	-D_GNU_SOURCE -DXAPPGROUP -DXCSECURITY -DTOGCUP \
	-DXF86BIGFONT -DDPMSExtension -DSINGLEDEPTH -DXvExtension \
	-DXFree86Server -DXvMCExtension -DSMART_SCHEDULE -DXResExtension \
	-DX_BYTE_ORDER=X_LITTLE_ENDIAN -DXTRAP -DMITSHM -DDBE -DEVI \
	-DSCREENSAVER -DXV -DXVMC -DFONTCACHE \
	-DGCCUSESGAS -DSTATIC_COLOR -DAVOID_GLYPHBLT -DPIXPRIV \
	-DNDEBUG -DFUNCPROTO=15 -DNARROWPROTO -DDDXOSINIT \
	-DSERVER_LOCK -DDDXOSFATALERROR -DPART_NET \
	-DDDXTIME -D_HAVE_XALLOC_DECLS

LDFLAGS = -L../../../builddir/pkg-config/lib

all: rdp

rdp: $(OBJS)
	$(AR) rvu librdp.a $(OBJS)
	ranlib librdp.a
	$(CC) $(LDFLAGS) -o X11rdp $(LIBS) $(LIBS)

clean:
	rm -f $(OBJS) librdp.a

miinitext.o: ../../mi/miinitext.c
	$(CC) $(CFLAGS) -c ../../mi/miinitext.c

fbcmap.o: ../../fb/fbcmap.c
	$(CC) $(CFLAGS) -c ../../fb/fbcmap.c
