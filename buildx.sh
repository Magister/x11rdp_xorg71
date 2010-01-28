#!/bin/sh

export CPPFLAGS=-D_GNU_SOURCE

print_help()
{
  echo ""
  echo "X11 build script"
  echo "sh buildx.sh <path to build>"
  echo "example sh buildx.sh /usr"
  echo "example sh buildx.sh /usr/local"
  echo "example sh buildx.sh /opt/X11rdp"
  echo ""
  return 0
}

if test $# -eq 0
then
  print_help
  exit 0
else
  PREFIXDIR=$1
fi

if ! test -d $PREFIXDIR
then
  echo "error directory $PREFIXDIR does not exist"
  exit 1
fi

if ! test -w $PREFIXDIR
then
  echo "error directory $PREFIXDIR is not writable"
  exit 0
fi

SRCDIR=$PWD
PCFILEDIR=$PREFIXDIR/lib/pkgconfig
export PKG_CONFIG_PATH=$PCFILEDIR

# xf86dri
if ! test -f $PCFILEDIR/xf86driproto.pc
then
  cd xf86driproto-X11R7.0-2.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86dri"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86driproto"
fi

# glproto
if ! test -f $PCFILEDIR/glproto.pc
then
  cd glproto-X11R7.1-1.4.7
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error glproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping glproto"
fi

# randrproto
if ! test -f $PCFILEDIR/randrproto.pc
then
  cd randrproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error randrproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping randrproto"
fi

# renderproto
if ! test -f $PCFILEDIR/renderproto.pc
then
  cd renderproto-X11R7.0-0.9.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error renderproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping renderproto"
fi

# fixesproto
if ! test -f $PCFILEDIR/fixesproto.pc
then
  cd fixesproto-X11R7.1-4.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fixesproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping fixesproto"
fi

# damageproto
if ! test -f $PCFILEDIR/damageproto.pc
then
  cd damageproto-X11R7.0-1.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error damageproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping damageproto"
fi

# xcmiscproto
if ! test -f $PCFILEDIR/xcmiscproto.pc
then
  cd xcmiscproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xcmiscproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xcmiscproto"
fi

# xextproto
if ! test -f $PCFILEDIR/xextproto.pc
then
  cd xextproto-X11R7.0-7.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xextproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xextproto"
fi

# xproto
if ! test -f $PCFILEDIR/xproto.pc
then
  cd xproto-X11R7.1-7.0.5
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xproto"
fi

# xtrans
if ! test -f $PCFILEDIR/xtrans.pc
then
  cd xtrans-X11R7.0-1.0.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xtrans"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xtrans"
fi

# xf86miscproto
if ! test -f $PCFILEDIR/xf86miscproto.pc
then
  cd xf86miscproto-X11R7.0-0.9.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86miscproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86miscproto"
fi

# xf86vidmodeproto
if ! test -f $PCFILEDIR/xf86vidmodeproto.pc
then
  cd xf86vidmodeproto-X11R7.0-2.2.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86vidmodeproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86vidmodeproto"
fi

# xf86bigfontproto
if ! test -f $PCFILEDIR/xf86bigfontproto.pc
then
  cd xf86bigfontproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86bigfontproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86bigfontproto"
fi

# scrnsaverproto
if ! test -f $PCFILEDIR/scrnsaverproto.pc
then
  cd scrnsaverproto-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error scrnsaverproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping scrnsaverproto"
fi

# bigreqsproto
if ! test -f $PCFILEDIR/bigreqsproto.pc
then
  cd bigreqsproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error bigreqsproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping bigreqsproto"
fi

# resourceproto
if ! test -f $PCFILEDIR/resourceproto.pc
then
  cd resourceproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error resourceproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping resourceproto"
fi

# fontsproto
if ! test -f $PCFILEDIR/fontsproto.pc
then
  cd fontsproto-X11R7.0-2.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontsproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping fontsproto"
fi

# inputproto
if ! test -f $PCFILEDIR/inputproto.pc
then
  cd inputproto-X11R7.0-1.3.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error inputproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping inputproto"
fi

# xf86dgaproto
if ! test -f $PCFILEDIR/xf86dgaproto.pc
then
  cd xf86dgaproto-X11R7.0-2.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xf86dgaproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xf86dgaproto"
fi

# videoproto
if ! test -f $PCFILEDIR/videoproto.pc
then
  cd videoproto-X11R7.0-2.2.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error videoproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping videoproto"
fi

# compositeproto
if ! test -f $PCFILEDIR/compositeproto.pc
then
  cd compositeproto-X11R7.1-0.3.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error compositeproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping compositeproto"
fi

# trapproto
if ! test -f $PCFILEDIR/trapproto.pc
then
  cd trapproto-X11R7.0-3.4.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error trapproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping trapproto"
fi

# recordproto
if ! test -f $PCFILEDIR/recordproto.pc
then
  cd recordproto-X11R7.0-1.13.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error recordproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping recordproto"
fi

# xineramaproto
if ! test -f $PCFILEDIR/xineramaproto.pc
then
  cd xineramaproto-X11R7.0-1.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xineramaproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xineramaproto"
fi

# ice
if ! test -f $PCFILEDIR/ice.pc
then
  cd libICE-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error ice"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping ice"
fi

# sm
if ! test -f $PCFILEDIR/sm.pc
then
  cd libSM-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error sm"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping sm"
fi

# xau
if ! test -f $PCFILEDIR/xau.pc
then
  cd libXau-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xau"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xau"
fi

# kbproto
if ! test -f $PCFILEDIR/kbproto.pc
then
  cd kbproto-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error kbproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping kbproto"
fi

# Xdmcp
if ! test -f $PCFILEDIR/xdmcp.pc
then
  cd libXdmcp-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error Xdmcp"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xdmcp"
fi

# x11
if ! test -f $PCFILEDIR/x11.pc
then
  cd libX11-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error x11"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping x11"
fi

# xt
if ! test -f $PCFILEDIR/xt.pc
then
  cd libXt-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xt"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xt"
fi

# evieext evieproto
if ! test -f $PCFILEDIR/evieproto.pc
then
  cd evieext-X11R7.0-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error evieext"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping evieproto"
fi

# xkbfile
if ! test -f $PCFILEDIR/xkbfile.pc
then
  cd libxkbfile-X11R7.1-1.0.3
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xkbfile"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xkbfile"
fi

# fontcacheproto
if ! test -f $PCFILEDIR/fontcacheproto.pc
then
  cd fontcacheproto-X11R7.0-0.1.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontcacheproto"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping fontcacheproto"
fi

# fontenc
if ! test -f $PCFILEDIR/fontenc.pc
then
  cd libfontenc-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error fontenc"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping fontenc"
fi

# freetype
if ! test -f $PCFILEDIR/freetype2.pc
then
  cd freetype-2.1.10
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error freetype"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping freetype"
fi

# xfont
if ! test -f $PCFILEDIR/xfont.pc
then
  cd libXfont-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xfont"
    exit 1
  fi
  make
  if ! test $? -eq 0
  then
    echo "error make xfont"
    exit 1
  fi
  make install
  cd ..
else
  echo "skipping xfont"
fi

# Xext
if ! test -f $PCFILEDIR/xext.pc
then
  cd libXext-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error Xext"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xext"
fi

# xmu
if ! test -f $PCFILEDIR/xmu.pc
then
  cd libXmu-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xmu"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xmu"
fi

# xkbui
if ! test -f $PCFILEDIR/xkbui.pc
then
  cd libxkbui-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xkbui"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xkbui"
fi

# xxf86misc
if ! test -f $PCFILEDIR/xxf86misc.pc
then
  cd libXxf86misc-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xxf86misc"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xxf86misc"
fi

# xxf86vm
if ! test -f $PCFILEDIR/xxf86vm.pc
then
  cd libXxf86vm-X11R7.1-1.0.1
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xxf86vm"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xxf86vm"
fi

# xpm
if ! test -f $PCFILEDIR/xpm.pc
then
  cd libXpm-X11R7.1-3.5.5
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xpm"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xpm"
fi

# xaw7
if ! test -f $PCFILEDIR/xaw7.pc
then
  cd libXaw-X11R7.1-1.0.2
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error xaw7"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping xaw7"
fi

# libdrm - special
if ! test -f $PCFILEDIR/libdrm.pc
then
  cd libdrm-2.3.0
  ./configure --prefix=$PREFIXDIR
  if ! test $? -eq 0
  then
    echo "error libdrm"
    exit 1
  fi
  make
  make install
  cd ..
else
  echo "skipping libdrm"
fi

# xorg-server
if ! test -f $PCFILEDIR/xorg-server.pc
then
  cd xorg-server-X11R7.1-1.1.0
  ./configure --prefix=$PREFIXDIR --enable-xglx --with-mesa-source=$SRCDIR/Mesa-6.5 --disable-dmx
  if ! test $? -eq 0
  then
    echo "error xorg-server"
    exit 1
  fi
  make
  if ! test $? -eq 0
  then
    echo "error make xorg-server"
    exit 1
  fi
  make install
  cd ..
else
  echo "skipping xorg-server"
fi

# X11rdp
cd xorg-server-X11R7.1-1.1.0/hw/rdp
export X11RDPBASE=$PREFIXDIR
make
if ! test $? -eq 0
then
  echo "error make X11rdp"
  exit 1
fi

echo "all ok"

# this will copy the build X server with the other X server binaries
cp $SRCDIR/xorg-server-X11R7.1-1.1.0/hw/rdp/X11rdp $X11RDPBASE/bin
strip $X11RDPBASE/bin/X11rdp

# make a symboloc link to your local xkbcomp
if ! test -f $X11RDPBASE/bin/xkbcomp
then
  if test -f /usr/bin/xkbcomp
  then
    ln -s /usr/bin/xkbcomp $X11RDPBASE/bin/xkbcomp
  elif test -f /usr/X11R6/bin/xkbcomp
  then
    ln -s /usr/X11R6/bin/xkbcomp $X11RDPBASE/bin/xkbcomp
  fi
fi

# make a symbolic link to your local font directory
if ! test -d $X11RDPBASE/lib/X11/fonts
then
  if test -d /usr/share/fonts/X11
  then
    ln -s /usr/share/fonts/X11 $X11RDPBASE/lib/X11/fonts
  elif test -d /usr/share/X11/fonts
  then
    ln -s /usr/share/X11/fonts $X11RDPBASE/lib/X11/fonts
  elif test -d /usr/X11R6/lib/X11/fonts
  then
    ln -s /usr/X11R6/lib/X11/fonts $X11RDPBASE/lib/X11/fonts
  fi
fi
