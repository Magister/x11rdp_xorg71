#!/bin/sh

BASEDIR=$PWD
PREFIXDIR=$PWD/builddir/pkg-config

if ! test -d builddir
then
  mkdir builddir
fi

if ! test -d builddir/pkg-config
then
  mkdir builddir/pkg-config
fi

export PKG_CONFIG_PATH=$BASEDIR/builddir/pkg-config/lib/pkgconfig

# xf86dri
if ! test -f builddir/pkg-config/lib/pkgconfig/xf86driproto.pc
then
  cd xf86driproto-X11R7.0-2.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86dri"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86driproto"
fi

# glproto
if ! test -f builddir/pkg-config/lib/pkgconfig/glproto.pc
then
  cd glproto-X11R7.1-1.4.7
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error glproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping glproto"
fi

# randrproto
if ! test -f builddir/pkg-config/lib/pkgconfig/randrproto.pc
then
  cd randrproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error randrproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping randrproto"
fi

# renderproto
if ! test -f builddir/pkg-config/lib/pkgconfig/renderproto.pc
then
  cd renderproto-X11R7.0-0.9.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error renderproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping renderproto"
fi

# fixesproto
if ! test -f builddir/pkg-config/lib/pkgconfig/fixesproto.pc
then
  cd fixesproto-X11R7.1-4.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fixesproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping fixesproto"
fi

# damageproto
if ! test -f builddir/pkg-config/lib/pkgconfig/damageproto.pc
then
  cd damageproto-X11R7.0-1.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error damageproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping damageproto"
fi

# xcmiscproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xcmiscproto.pc
then
  cd xcmiscproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xcmiscproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xcmiscproto"
fi

# xextproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xextproto.pc
then
  cd xextproto-X11R7.0-7.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xextproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xextproto"
fi

# xproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xproto.pc
then
  cd xproto-X11R7.1-7.0.5
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xproto"
fi

# xtrans
if ! test -f builddir/pkg-config/lib/pkgconfig/xtrans.pc
then
  cd xtrans-X11R7.0-1.0.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xtrans"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xtrans"
fi

# xf86miscproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xf86miscproto.pc
then
  cd xf86miscproto-X11R7.0-0.9.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86miscproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86miscproto"
fi

# xf86vidmodeproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xf86vidmodeproto.pc
then
  cd xf86vidmodeproto-X11R7.0-2.2.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86vidmodeproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86vidmodeproto"
fi

# xf86bigfontproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xf86bigfontproto.pc
then
  cd xf86bigfontproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86bigfontproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86bigfontproto"
fi

# scrnsaverproto
if ! test -f builddir/pkg-config/lib/pkgconfig/scrnsaverproto.pc
then
  cd scrnsaverproto-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error scrnsaverproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping scrnsaverproto"
fi

# bigreqsproto
if ! test -f builddir/pkg-config/lib/pkgconfig/bigreqsproto.pc
then
  cd bigreqsproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error bigreqsproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping bigreqsproto"
fi

# resourceproto
if ! test -f builddir/pkg-config/lib/pkgconfig/resourceproto.pc
then
  cd resourceproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error resourceproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping resourceproto"
fi

# fontsproto
if ! test -f builddir/pkg-config/lib/pkgconfig/fontsproto.pc
then
  cd fontsproto-X11R7.0-2.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontsproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping fontsproto"
fi

# inputproto
if ! test -f builddir/pkg-config/lib/pkgconfig/inputproto.pc
then
  cd inputproto-X11R7.0-1.3.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error inputproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping inputproto"
fi

# xf86dgaproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xf86dgaproto.pc
then
  cd xf86dgaproto-X11R7.0-2.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86dgaproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86dgaproto"
fi

# videoproto
if ! test -f builddir/pkg-config/lib/pkgconfig/videoproto.pc
then
  cd videoproto-X11R7.0-2.2.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error videoproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping videoproto"
fi

# compositeproto
if ! test -f builddir/pkg-config/lib/pkgconfig/compositeproto.pc
then
  cd compositeproto-X11R7.1-0.3.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error compositeproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping compositeproto"
fi

# trapproto
if ! test -f builddir/pkg-config/lib/pkgconfig/trapproto.pc
then
  cd trapproto-X11R7.0-3.4.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error trapproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping trapproto"
fi

# recordproto
if ! test -f builddir/pkg-config/lib/pkgconfig/recordproto.pc
then
  cd recordproto-X11R7.0-1.13.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error recordproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping recordproto"
fi

# xineramaproto
if ! test -f builddir/pkg-config/lib/pkgconfig/xineramaproto.pc
then
  cd xineramaproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xineramaproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xineramaproto"
fi

# ice
if ! test -f builddir/pkg-config/lib/pkgconfig/ice.pc
then
  cd libICE-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error ice"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping ice"
fi

# sm
if ! test -f builddir/pkg-config/lib/pkgconfig/sm.pc
then
  cd libSM-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error sm"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping sm"
fi

# xau
if ! test -f builddir/pkg-config/lib/pkgconfig/xau.pc
then
  cd libXau-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xau"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xau"
fi

# kbproto
if ! test -f builddir/pkg-config/lib/pkgconfig/kbproto.pc
then
  cd kbproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error kbproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping kbproto"
fi

# Xdmcp
if ! test -f builddir/pkg-config/lib/pkgconfig/xdmcp.pc
then
  cd libXdmcp-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error Xdmcp"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xdmcp"
fi

# x11
if ! test -f builddir/pkg-config/lib/pkgconfig/x11.pc
then
  cd libX11-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error x11"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping x11"
fi

# xt
if ! test -f builddir/pkg-config/lib/pkgconfig/xt.pc
then
  cd libXt-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xt"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xt"
fi

# evieext evieproto
if ! test -f builddir/pkg-config/lib/pkgconfig/evieproto.pc
then
  cd evieext-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error evieext"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping evieproto"
fi

# xkbfile
if ! test -f builddir/pkg-config/lib/pkgconfig/xkbfile.pc
then
  cd libxkbfile-X11R7.1-1.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xkbfile"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xkbfile"
fi

# fontcacheproto
if ! test -f builddir/pkg-config/lib/pkgconfig/fontcacheproto.pc
then
  cd fontcacheproto-X11R7.0-0.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontcacheproto"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping fontcacheproto"
fi

# fontenc
if ! test -f builddir/pkg-config/lib/pkgconfig/fontenc.pc
then
  cd libfontenc-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontenc"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping fontenc"
fi

# freetype
if ! test -f builddir/pkg-config/lib/pkgconfig/freetype2.pc
then
  cd freetype-2.1.10
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error freetype"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping freetype"
fi

# xfont
if ! test -f builddir/pkg-config/lib/pkgconfig/xfont.pc
then
  cd libXfont-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xfont"
    exit 1;
  fi
  make
  if ! test $? -eq 0
  then
    echo "error make xfont"
    exit 1;
  fi
  make install
  cd ..
else
  echo "skipping xfont"
fi

# Xext
if ! test -f builddir/pkg-config/lib/pkgconfig/xext.pc
then
  cd libXext-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error Xext"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xext"
fi

# xmu
if ! test -f builddir/pkg-config/lib/pkgconfig/xmu.pc
then
  cd libXmu-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xmu"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xmu"
fi

# xkbui
if ! test -f builddir/pkg-config/lib/pkgconfig/xkbui.pc
then
  cd libxkbui-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xkbui"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xkbui"
fi

# xxf86misc
if ! test -f builddir/pkg-config/lib/pkgconfig/xxf86misc.pc
then
  cd libXxf86misc-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xxf86misc"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xxf86misc"
fi

# xxf86vm
if ! test -f builddir/pkg-config/lib/pkgconfig/xxf86vm.pc
then
  cd libXxf86vm-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xxf86vm"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xxf86vm"
fi

# xpm
if ! test -f builddir/pkg-config/lib/pkgconfig/xpm.pc
then
  cd libXpm-X11R7.1-3.5.5
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xpm"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xpm"
fi

# xaw7
if ! test -f builddir/pkg-config/lib/pkgconfig/xaw7.pc
then
  cd libXaw-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xaw7"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping xaw7"
fi

# libdrm - special
if ! test -f builddir/pkg-config/lib/pkgconfig/libdrm.pc
then
  cd libdrm-2.3.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error libdrm"
    exit 1;
  fi
  make
  make install
  cd ..
else
  echo "skipping libdrm"
fi

# xorg-server

if ! test -f builddir/pkg-config/lib/pkgconfig/xorg-server.pc
then
  cd xorg-server-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR --with-fontdir=/usr/share/fonts/X11 --enable-xglx --with-mesa-source=$BASEDIR/Mesa-6.5
  if ! test $? -eq 0
  then
    echo "error xorg-server"
    exit 1;
  fi
  make
  if ! test $? -eq 0
  then
    echo "error make xorg-server"
    exit 1;
  fi
  make install
  cd ..
else
  echo "skipping xorg-server"
fi

echo "all ok"
