/* $XdotOrg: xserver/xorg/hw/xfree86/common/xf86RandR.c,v 1.26 2006/03/25 19:52:03 ajax Exp $ */
/*
 * $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86RandR.c,v 1.7tsi Exp $
 *
 * Copyright © 2002 Keith Packard, member of The XFree86 Project, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Keith Packard not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Keith Packard makes no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include <X11/X.h>
#include "os.h"
#include "mibank.h"
#include "globals.h"
#include "xf86.h"
#include "xf86Priv.h"
#include "xf86DDC.h"
#include "mipointer.h"
#include <randrstr.h>

typedef struct _xf86RandRInfo {
    CreateScreenResourcesProcPtr    CreateScreenResources;
    CloseScreenProcPtr		    CloseScreen;
    int				    virtualX;
    int				    virtualY;
    int				    mmWidth;
    int				    mmHeight;
    Rotation			    rotation;
} XF86RandRInfoRec, *XF86RandRInfoPtr;

static int	    xf86RandRIndex = -1;
static int	    xf86RandRGeneration;

#define XF86RANDRINFO(p)    ((XF86RandRInfoPtr) (p)->devPrivates[xf86RandRIndex].ptr)

static int
xf86RandRModeRefresh (DisplayModePtr mode)
{
    if (mode->VRefresh)
	return (int) (mode->VRefresh + 0.5);
    else
	return (int) (mode->Clock * 1000.0 / mode->HTotal / mode->VTotal + 0.5);
}

static Bool
xf86RandRGetInfo (ScreenPtr pScreen, Rotation *rotations)
{
    RRScreenSizePtr	    pSize;
    ScrnInfoPtr		    scrp = XF86SCRNINFO(pScreen);
    XF86RandRInfoPtr	    randrp = XF86RANDRINFO(pScreen);
    DisplayModePtr	    mode;
    int			    refresh0 = 60;
    xorgRRModeMM	    RRModeMM;

    *rotations = RR_Rotate_0;

    for (mode = scrp->modes; ; mode = mode->next)
    {
	int refresh = xf86RandRModeRefresh (mode);

	if (mode == scrp->modes)
	    refresh0 = refresh;

	RRModeMM.mode = mode;
	RRModeMM.virtX = randrp->virtualX;
	RRModeMM.virtY = randrp->virtualY;
	RRModeMM.mmWidth = randrp->mmWidth;
	RRModeMM.mmHeight = randrp->mmHeight;

	if(scrp->DriverFunc) {
	   (*scrp->DriverFunc)(scrp, RR_GET_MODE_MM, &RRModeMM);
	}

	pSize = RRRegisterSize (pScreen,
				mode->HDisplay, mode->VDisplay,
				RRModeMM.mmWidth, RRModeMM.mmHeight);
	if (!pSize)
	    return FALSE;
	RRRegisterRate (pScreen, pSize, refresh);
	if (mode == scrp->currentMode &&
	    mode->HDisplay == scrp->virtualX && mode->VDisplay == scrp->virtualY)
	    RRSetCurrentConfig (pScreen, randrp->rotation, refresh, pSize);
	if (mode->next == scrp->modes)
	    break;
    }
    if (scrp->currentMode->HDisplay != randrp->virtualX ||
	scrp->currentMode->VDisplay != randrp->virtualY)
    {
	mode = scrp->modes;

	RRModeMM.mode = NULL;
	RRModeMM.virtX = randrp->virtualX;
	RRModeMM.virtY = randrp->virtualY;
	RRModeMM.mmWidth = randrp->mmWidth;
	RRModeMM.mmHeight = randrp->mmHeight;

	if(scrp->DriverFunc) {
	   (*scrp->DriverFunc)(scrp, RR_GET_MODE_MM, &RRModeMM);
	}

	pSize = RRRegisterSize (pScreen,
				randrp->virtualX, randrp->virtualY,
				RRModeMM.mmWidth, RRModeMM.mmHeight);
	if (!pSize)
	    return FALSE;
	RRRegisterRate (pScreen, pSize, refresh0);
	if (scrp->virtualX == randrp->virtualX &&
	    scrp->virtualY == randrp->virtualY)
	{
	    RRSetCurrentConfig (pScreen, randrp->rotation, refresh0, pSize);
	}
    }

    /* If there is driver support for randr, let it set our supported rotations */
    if(scrp->DriverFunc) {
	xorgRRRotation RRRotation;

	RRRotation.RRRotations = *rotations;
	if (!(*scrp->DriverFunc)(scrp, RR_GET_INFO, &RRRotation))
	    return TRUE;
	*rotations = RRRotation.RRRotations;
    }

    return TRUE;
}

static Bool
xf86RandRSetMode (ScreenPtr	    pScreen,
		  DisplayModePtr    mode,
		  Bool		    useVirtual,
		  int		    mmWidth,
		  int		    mmHeight)
{
    ScrnInfoPtr		scrp = XF86SCRNINFO(pScreen);
    XF86RandRInfoPtr	randrp = XF86RANDRINFO(pScreen);
    int			oldWidth = pScreen->width;
    int			oldHeight = pScreen->height;
    int			oldmmWidth = pScreen->mmWidth;
    int			oldmmHeight = pScreen->mmHeight;
    WindowPtr		pRoot = WindowTable[pScreen->myNum];
    Bool		ret = TRUE;

    if (pRoot)
	(*scrp->EnableDisableFBAccess) (pScreen->myNum, FALSE);
    if (useVirtual)
    {
	scrp->virtualX = randrp->virtualX;
	scrp->virtualY = randrp->virtualY;
    }
    else
    {
	scrp->virtualX = mode->HDisplay;
	scrp->virtualY = mode->VDisplay;
    }
    if(randrp->rotation & (RR_Rotate_90 | RR_Rotate_270))
    {
	/* If the screen is rotated 90 or 270 degrees, swap the sizes. */
	pScreen->width = scrp->virtualY;
	pScreen->height = scrp->virtualX;
	pScreen->mmWidth = mmHeight;
	pScreen->mmHeight = mmWidth;
    }
    else
    {
	pScreen->width = scrp->virtualX;
	pScreen->height = scrp->virtualY;
	pScreen->mmWidth = mmWidth;
	pScreen->mmHeight = mmHeight;
    }
    if (!xf86SwitchMode (pScreen, mode))
    {
	scrp->virtualX = pScreen->width = oldWidth;
	scrp->virtualY = pScreen->height = oldHeight;
	pScreen->mmWidth = oldmmWidth;
	pScreen->mmHeight = oldmmHeight;
	ret = FALSE;
    }
    /*
     * Make sure the layout is correct
     */
    xf86ReconfigureLayout();

    /*
     * Make sure the whole screen is visible
     */
    xf86SetViewport (pScreen, pScreen->width, pScreen->height);
    xf86SetViewport (pScreen, 0, 0);
    if (pRoot)
	(*scrp->EnableDisableFBAccess) (pScreen->myNum, TRUE);
    return ret;
}

static Bool
xf86RandRSetConfig (ScreenPtr		pScreen,
		    Rotation		rotation,
		    int			rate,
		    RRScreenSizePtr	pSize)
{
    ScrnInfoPtr		    scrp = XF86SCRNINFO(pScreen);
    XF86RandRInfoPtr	    randrp = XF86RANDRINFO(pScreen);
    DisplayModePtr	    mode;
    int			    px, py;
    Bool		    useVirtual = FALSE;
    Rotation		    oldRotation = randrp->rotation;

    miPointerPosition (&px, &py);
    for (mode = scrp->modes; ; mode = mode->next)
    {
	if (mode->HDisplay == pSize->width &&
	    mode->VDisplay == pSize->height &&
	    (rate == 0 || xf86RandRModeRefresh (mode) == rate))
	    break;
	if (mode->next == scrp->modes)
	{
	    if (pSize->width == randrp->virtualX &&
		pSize->height == randrp->virtualY)
	    {
		mode = scrp->modes;
		useVirtual = TRUE;
		break;
	    }
	    return FALSE;
	}
    }

    if (randrp->rotation != rotation) {

        /* Have the driver do its thing. */
	if (scrp->DriverFunc) {
	    xorgRRRotation RRRotation;
	    RRRotation.RRConfig.rotation = rotation;
	    RRRotation.RRConfig.rate = rate;
	    RRRotation.RRConfig.width = pSize->width;
	    RRRotation.RRConfig.height = pSize->height;

	    /*
	     * Currently we need to rely on HW support for rotation.
	     */
	    if (!(*scrp->DriverFunc)(scrp, RR_SET_CONFIG, &RRRotation))
		return FALSE;
	} else
	    return FALSE;

	randrp->rotation = rotation;
    }

    if (!xf86RandRSetMode (pScreen, mode, useVirtual, pSize->mmWidth, pSize->mmHeight)) {
	if(randrp->rotation != oldRotation) {
	   /* Have the driver undo its thing. */
	   if (scrp->DriverFunc) {
	       xorgRRRotation RRRotation;
	       RRRotation.RRConfig.rotation = oldRotation;
	       RRRotation.RRConfig.rate = xf86RandRModeRefresh (scrp->currentMode);
	       RRRotation.RRConfig.width = pScreen->width;
	       RRRotation.RRConfig.height = pScreen->height;
	       (*scrp->DriverFunc)(scrp, RR_SET_CONFIG, &RRRotation);
	   }

	   randrp->rotation = oldRotation;
	}
	return FALSE;
    }
    /*
     * Move the cursor back where it belongs; SwitchMode repositions it
     */
    if (pScreen == miPointerCurrentScreen ())
    {
	px = (px >= pScreen->width ? (pScreen->width - 1) : px);
	py = (py >= pScreen->height ? (pScreen->height - 1) : py);

        xf86SetViewport(pScreen, px, py);

        (*pScreen->SetCursorPosition) (pScreen, px, py, FALSE);
    }

    return TRUE;
}

/*
 * Wait until the screen is initialized before whacking the
 * sizes around; otherwise the screen pixmap will be allocated
 * at the current mode size rather than the maximum size
 */
static Bool
xf86RandRCreateScreenResources (ScreenPtr pScreen)
{
    XF86RandRInfoPtr	    randrp = XF86RANDRINFO(pScreen);
#if 0
    ScrnInfoPtr		    scrp = XF86SCRNINFO(pScreen);
    DisplayModePtr	    mode;
#endif

    pScreen->CreateScreenResources = randrp->CreateScreenResources;
    if (!(*pScreen->CreateScreenResources) (pScreen))
	return FALSE;

#if 0
    mode = scrp->currentMode;
    if (mode)
	xf86RandRSetMode (pScreen, mode, TRUE);
#endif

    return TRUE;
}

/*
 * Reset size back to original
 */
static Bool
xf86RandRCloseScreen (int index, ScreenPtr pScreen)
{
    ScrnInfoPtr		    scrp = XF86SCRNINFO(pScreen);
    XF86RandRInfoPtr	    randrp = XF86RANDRINFO(pScreen);

    scrp->virtualX = pScreen->width = randrp->virtualX;
    scrp->virtualY = pScreen->height = randrp->virtualY;
    scrp->currentMode = scrp->modes;
    pScreen->CloseScreen = randrp->CloseScreen;
    xfree (randrp);
    pScreen->devPrivates[xf86RandRIndex].ptr = 0;
    return (*pScreen->CloseScreen) (index, pScreen);
}

_X_EXPORT Rotation
xf86GetRotation(ScreenPtr pScreen)
{
    if (xf86RandRIndex == -1)
       return RR_Rotate_0;

    return XF86RANDRINFO(pScreen)->rotation;
}

/* Function to change RandR's idea of the virtual screen size */
_X_EXPORT Bool
xf86RandRSetNewVirtualAndDimensions(ScreenPtr pScreen,
	int newvirtX, int newvirtY, int newmmWidth, int newmmHeight,
	Bool resetMode)
{
    XF86RandRInfoPtr randrp;

    if (xf86RandRIndex == -1)
	return FALSE;

    randrp = XF86RANDRINFO(pScreen);
    if (randrp == NULL)
	return FALSE;

    if (newvirtX > 0)
	randrp->virtualX = newvirtX;

    if (newvirtY > 0)
	randrp->virtualY = newvirtY;

    if (newmmWidth > 0)
	randrp->mmWidth = newmmWidth;

    if (newmmHeight > 0)
	randrp->mmHeight = newmmHeight;

    /* This is only for during server start */
    if (resetMode) {
	return (xf86RandRSetMode(pScreen,
		  XF86SCRNINFO(pScreen)->currentMode,
		  TRUE,
		  pScreen->mmWidth, pScreen->mmHeight));
    }

    return TRUE;
}

Bool
xf86RandRInit (ScreenPtr    pScreen)
{
    rrScrPrivPtr	rp;
    XF86RandRInfoPtr	randrp;
    ScrnInfoPtr		scrp = XF86SCRNINFO(pScreen);

#ifdef PANORAMIX
    /* XXX disable RandR when using Xinerama */
    if (!noPanoramiXExtension)
	return TRUE;
#endif
    if (xf86RandRGeneration != serverGeneration)
    {
	xf86RandRIndex = AllocateScreenPrivateIndex();
	xf86RandRGeneration = serverGeneration;
    }

    randrp = xalloc (sizeof (XF86RandRInfoRec));
    if (!randrp)
	return FALSE;

    if (!RRScreenInit (pScreen))
    {
	xfree (randrp);
	return FALSE;
    }
    rp = rrGetScrPriv(pScreen);
    rp->rrGetInfo = xf86RandRGetInfo;
    rp->rrSetConfig = xf86RandRSetConfig;

    randrp->virtualX = scrp->virtualX;
    randrp->virtualY = scrp->virtualY;
    randrp->mmWidth = pScreen->mmWidth;
    randrp->mmHeight = pScreen->mmHeight;

    randrp->CreateScreenResources = pScreen->CreateScreenResources;
    pScreen->CreateScreenResources = xf86RandRCreateScreenResources;

    randrp->CloseScreen = pScreen->CloseScreen;
    pScreen->CloseScreen = xf86RandRCloseScreen;

    randrp->rotation = RR_Rotate_0;

    pScreen->devPrivates[xf86RandRIndex].ptr = randrp;
    return TRUE;
}


