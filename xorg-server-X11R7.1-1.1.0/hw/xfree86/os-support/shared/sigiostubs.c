/* $XFree86: xc/programs/Xserver/hw/xfree86/os-support/shared/sigiostubs.c,v 1.4 2003/08/24 17:37:06 dawes Exp $ */
/*
 * Copyright (c) 1999-2003 by The XFree86 Project, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */
/* $XdotOrg: xserver/xorg/hw/xfree86/os-support/shared/sigiostubs.c,v 1.6 2006/03/25 19:52:04 ajax Exp $ */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#ifdef XFree86Server
#ifdef __UNIXOS2__
# define I_NEED_OS2_H
#endif
# include <X11/X.h>
# include "xf86.h"
# include "xf86Priv.h"
# include "xf86_OSlib.h"
#else
# include <unistd.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/time.h>
# include <errno.h>
#endif

_X_EXPORT int
xf86InstallSIGIOHandler(int fd, void (*f)(int, void *), void *closure)
{
    return 0;
}

_X_EXPORT int
xf86RemoveSIGIOHandler(int fd)
{
    return 0;
}

_X_EXPORT int
xf86BlockSIGIO (void)
{
    return 0;
}

_X_EXPORT void
xf86UnblockSIGIO (int wasset)
{
}

#ifdef XFree86Server
void
xf86AssertBlockedSIGIO (char *where)
{
}
#endif

/* XXX This is a quick hack for the benefit of xf86SetSilkenMouse() */
Bool
xf86SIGIOSupported ()
{
    return FALSE;
}

