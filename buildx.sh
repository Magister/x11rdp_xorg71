#!/bin/sh

BASEDIR=$PWD
PREFIXDIR=$PWD/builddir/pkg-config

if ! [ -a builddir ]
then
  mkdir builddir
fi

if ! [ -a builddir/pkg-config ]
then
  mkdir builddir/pkg-config
fi

export PKG_CONFIG_PATH=$BASEDIR/builddir/pkg-config/lib/pkgconfig

# xf86dri
pkg-config --exists 'xf86driproto'
if ! test $? -eq 0
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
fi

# glproto
pkg-config --exists 'glproto'
if ! test $? -eq 0
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
fi

# randrproto
pkg-config --exists 'randrproto'
if ! test $? -eq 0
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
fi

# renderproto
pkg-config --exists 'renderproto'
if ! test $? -eq 0
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
fi

# fixesproto
pkg-config --exists 'fixesproto'
if ! test $? -eq 0
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
fi

# damageproto
pkg-config --exists 'damageproto'
if ! test $? -eq 0
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
fi

# xcmiscproto
pkg-config --exists 'xcmiscproto'
if ! test $? -eq 0
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
fi

# xextproto
pkg-config --exists 'xextproto'
if ! test $? -eq 0
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
fi

# xproto
pkg-config --exists 'xproto'
if ! test $? -eq 0
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
fi

# xtrans
pkg-config --exists 'xtrans'
if ! test $? -eq 0
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
fi

# xf86miscproto
pkg-config --exists 'xf86miscproto'
if ! test $? -eq 0
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
fi

# xf86vidmodeproto
pkg-config --exists 'xf86vidmodeproto'
if ! test $? -eq 0
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
fi

# xf86bigfontproto
pkg-config --exists 'xf86bigfontproto'
if ! test $? -eq 0
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
fi

# scrnsaverproto
pkg-config --exists 'scrnsaverproto'
if ! test $? -eq 0
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
fi

# bigreqsproto
pkg-config --exists 'bigreqsproto'
if ! test $? -eq 0
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
fi

# resourceproto
pkg-config --exists 'resourceproto'
if ! test $? -eq 0
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
fi

# fontsproto
pkg-config --exists 'fontsproto'
if ! test $? -eq 0
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
fi

# inputproto
pkg-config --exists 'inputproto'
if ! test $? -eq 0
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
fi

# xf86dgaproto
pkg-config --exists 'xf86dgaproto'
if ! test $? -eq 0
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
fi

# videoproto
pkg-config --exists 'videoproto'
if ! test $? -eq 0
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
fi

# compositeproto
pkg-config --exists 'compositeproto'
if ! test $? -eq 0
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
fi

# trapproto
pkg-config --exists 'trapproto'
if ! test $? -eq 0
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
fi

# recordproto
pkg-config --exists 'recordproto'
if ! test $? -eq 0
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
fi

# xineramaproto
pkg-config --exists 'xineramaproto'
if ! test $? -eq 0
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
fi

# ice
pkg-config --exists 'ice'
if ! test $? -eq 0
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
fi

# sm
pkg-config --exists 'sm'
if ! test $? -eq 0
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
fi

# xau
pkg-config --exists 'xau'
if ! test $? -eq 0
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
fi

# kbproto
pkg-config --exists 'kbproto'
if ! test $? -eq 0
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
fi

# Xdmcp
pkg-config --exists 'xdmcp'
if ! test $? -eq 0
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
fi

# x11
pkg-config --exists 'x11'
if ! test $? -eq 0
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
fi

# xt
pkg-config --exists 'xt'
if ! test $? -eq 0
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
fi

# evieext evieproto
pkg-config --exists 'evieproto'
if ! test $? -eq 0
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
fi

# xkbfile
pkg-config --exists 'xkbfile'
if ! test $? -eq 0
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
fi

# fontcacheproto
pkg-config --exists 'fontcacheproto'
if ! test $? -eq 0
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
fi

# fontenc
pkg-config --exists 'fontenc'
if ! test $? -eq 0
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
fi

# xfont
pkg-config --exists 'xfont'
if ! test $? -eq 0
then
  cd libXfont-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xfont"
    exit 1;
  fi
  make
  make install
  cd ..
fi

# Xext
pkg-config --exists 'xext'
if ! test $? -eq 0
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
fi

# xmu
pkg-config --exists 'xmu'
if ! test $? -eq 0
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
fi

# xkbui
pkg-config --exists 'xkbui'
if ! test $? -eq 0
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
fi

# xxf86misc
pkg-config --exists 'xxf86misc'
if ! test $? -eq 0
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
fi

# xxf86vm
pkg-config --exists 'xxf86vm'
if ! test $? -eq 0
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
fi

# xpm
pkg-config --exists 'xpm'
if ! test $? -eq 0
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
fi

# xaw7
pkg-config --exists 'xaw7'
if ! test $? -eq 0
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
fi

# libdrm - special
pkg-config --exists 'libdrm'
if ! test $? -eq 0
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
fi

# xorg-server
pkg-config --exists 'xorg-server'
if ! test $? -eq 0
then
  cd xorg-server-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR --enable-xglx --with-mesa-source=$BASEDIR/Mesa-6.5
  if ! test $? -eq 0
  then
    echo "error xorg-server"
    exit 1;
  fi
  make
  make install
  cd ..
fi

echo "all ok"
