/* $XFree86: xc/programs/Xserver/hw/xfree86/dummylib/xalloc.c,v 1.1 2000/02/13 03:06:39 dawes Exp $ */

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

_X_EXPORT pointer
Xalloc(unsigned long n)
{
    if (!n)
	n = 1;
    return malloc(n);
}

_X_EXPORT pointer
Xrealloc(pointer p, unsigned long n)
{
    if (!n)
	n = 1;
    return realloc(p, n);
}

_X_EXPORT pointer
Xcalloc(unsigned long n)
{
    pointer r;

    r = Xalloc(n);
    memset(r, 0, n);
    return r;
}

_X_EXPORT pointer
XNFalloc(unsigned long n)
{
    pointer r;

    r = Xalloc(n);
    if (!r)
	FatalError("XNFalloc failed\n");
    return r;
   
}

_X_EXPORT pointer
XNFrealloc(pointer p, unsigned long n)
{
    pointer r;

    r = Xrealloc(p, n);
    if (!r)
	FatalError("XNFrealloc failed\n");
    return r;
   
}

_X_EXPORT pointer
XNFcalloc(unsigned long n)
{
    pointer r;

    r = Xcalloc(n);
    if (!r)
	FatalError("XNFcalloc failed\n");
    return r;
   
}

_X_EXPORT void
Xfree(pointer p)
{
    free(p);
}

_X_EXPORT char *
Xstrdup(const char *s)
{
    char *sd;

    if (s == NULL)
	return NULL;

    sd = (char *)Xalloc(strlen(s) + 1);
    if (sd != NULL)
	strcpy(sd, s);
    return sd;
}
