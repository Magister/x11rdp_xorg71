/* $XFree86: xc/programs/Xserver/xkb/ddxPrivate.c,v 1.2 2003/04/03 16:20:22 dawes Exp $ */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <stdio.h>
#define NEED_EVENTS 1
#include <X11/X.h>
#include "windowstr.h"
#define XKBSRV_NEED_FILE_FUNCS
#include <X11/extensions/XKBsrv.h>

#include "xf86.h"

int
XkbDDXPrivate(DeviceIntPtr dev,KeyCode key,XkbAction *act)
{
    XkbAnyAction *xf86act = &(act->any);
    char msgbuf[XkbAnyActionDataSize+1];

    if (xf86act->type == XkbSA_XFree86Private) {
        memcpy(msgbuf, xf86act->data, XkbAnyActionDataSize);
        msgbuf[XkbAnyActionDataSize]= '\0';
        if (strcmp(msgbuf, "-vmode")==0)
            xf86ProcessActionEvent(ACTION_PREV_MODE, NULL);
        else if (strcmp(msgbuf, "+vmode")==0)
            xf86ProcessActionEvent(ACTION_NEXT_MODE, NULL);
        else if (strcmp(msgbuf, "ungrab")==0)
            xf86ProcessActionEvent(ACTION_DISABLEGRAB, NULL);
        else if (strcmp(msgbuf, "clsgrb")==0)
            xf86ProcessActionEvent(ACTION_CLOSECLIENT, NULL);
        else
            xf86ProcessActionEvent(ACTION_MESSAGE, (void *) msgbuf);
    }

    return 0;
}
