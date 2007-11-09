#!/bin/sh

BASEDIR=$PWD
PREFIXDIR=$PWD/builddir/pkg-config

rm -r builddir
mkdir builddir
mkdir builddir/pkg-config

export PKG_CONFIG_PATH=$BASEDIR/builddir/pkg-config/lib/pkgconfig

# xf86dri
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

# glproto
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

# randrproto
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

# renderproto
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

# fixesproto
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

# damageproto
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

# xcmiscproto
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

# xextproto
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

# xproto
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

# xtrans
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

# xf86miscproto
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

# xf86vidmodeproto
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

# xf86bigfontproto
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

# scrnsaverproto
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

# bigreqsproto
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

# resourceproto
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

# fontsproto
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

# inputproto
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

# xf86dgaproto
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

# videoproto
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

# compositeproto
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

# trapproto
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

# recordproto
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

# xineramaproto
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

# ice
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

# sm
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

# xau
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

# kbproto
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

# Xdmcp
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

# x11
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

# xt
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

# evieext evieproto
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

# xkbfile
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

# fontcacheproto
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

# fontenc
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

# xfont
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

# Xext
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

# xmu
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

# xkbui
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

# xxf86misc
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

# xxf86vm
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

# xpm
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

# xaw7
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

# libdrm - special
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

# xorg-server
cd xorg-server-X11R7.1-1.1.0
./configure --prefix=$PREFIXDIR --enable-xglx --with-mesa-source=$BASEDIR/builddir/Mesa-6.5
if ! test $? -eq 0
then
  echo "error xorg-server"
  exit 1;
fi
make
make install
cd ..
