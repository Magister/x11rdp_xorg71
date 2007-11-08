/* $XFree86: xc/programs/Xserver/hw/xfree86/dummylib/xf86opt.c,v 1.1 2000/02/13 03:06:42 dawes Exp $ */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include "os.h"
#include "xf86.h"
#include "xf86Priv.h"

/*
 * Utility functions required by libxf86_os. 
 */

_X_EXPORT void
xf86ProcessOptions(int i, pointer p, OptionInfoPtr o)
{
}

_X_EXPORT Bool
xf86GetOptValBool(const OptionInfoRec *o, int i, Bool *b)
{
    return FALSE;
}

