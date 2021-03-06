/* $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86xvmc.c,v 1.6 2003/10/08 22:31:59 mvojkovi Exp $ */

/*
 * Copyright (c) 2001-2003 by The XFree86 Project, Inc.
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

#ifdef HAVE_XORG_CONFIG_H
#include <xorg-config.h>
#endif

#include "misc.h"
#include "xf86.h"
#include "xf86_OSproc.h"

#include <X11/X.h>
#include <X11/Xproto.h>
#include "scrnintstr.h"
#include "resource.h"
#include "dixstruct.h"

#ifdef XFree86LOADER
#include "xvmodproc.h"
#endif

#include "xf86xvpriv.h"
#include "xf86xvmc.h"

typedef int (*XvMCScreenInitProcPtr)(ScreenPtr, int, XvMCAdaptorPtr);
#ifdef XFree86LOADER
_X_EXPORT XvMCScreenInitProcPtr XvMCScreenInitProc = NULL;
#else
_X_EXPORT XvMCScreenInitProcPtr XvMCScreenInitProc = XvMCScreenInit;
#endif


typedef struct {
  CloseScreenProcPtr CloseScreen; 
  int num_adaptors;
  XF86MCAdaptorPtr *adaptors;
  XvMCAdaptorPtr dixinfo;
} xf86XvMCScreenRec, *xf86XvMCScreenPtr;

static unsigned long XF86XvMCGeneration = 0;
static int XF86XvMCScreenIndex = -1;

#define XF86XVMC_GET_PRIVATE(pScreen) \
   (xf86XvMCScreenPtr)((pScreen)->devPrivates[XF86XvMCScreenIndex].ptr)


static int 
xf86XvMCCreateContext (
  XvPortPtr pPort,
  XvMCContextPtr pContext,
  int *num_priv,
  CARD32 **priv
)
{
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    pContext->port_priv = (XvPortRecPrivatePtr)(pPort->devPriv.ptr);

    return (*pScreenPriv->adaptors[pContext->adapt_num]->CreateContext)(
		pScrn, pContext, num_priv, priv);
}

static void 
xf86XvMCDestroyContext ( XvMCContextPtr pContext)
{
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    (*pScreenPriv->adaptors[pContext->adapt_num]->DestroyContext)(
                				pScrn, pContext);
}

static int 
xf86XvMCCreateSurface (
  XvMCSurfacePtr pSurface,
  int *num_priv,
  CARD32 **priv
)
{
    XvMCContextPtr pContext = pSurface->context;
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    return (*pScreenPriv->adaptors[pContext->adapt_num]->CreateSurface)(
                pScrn, pSurface, num_priv, priv);
}

static void 
xf86XvMCDestroySurface (XvMCSurfacePtr pSurface)
{
    XvMCContextPtr pContext = pSurface->context;
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    (*pScreenPriv->adaptors[pContext->adapt_num]->DestroySurface)(
                                                pScrn, pSurface);
}

static int 
xf86XvMCCreateSubpicture (
  XvMCSubpicturePtr pSubpicture,
  int *num_priv,
  CARD32 **priv
)
{
    XvMCContextPtr pContext = pSubpicture->context;
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    return (*pScreenPriv->adaptors[pContext->adapt_num]->CreateSubpicture)(
                                  pScrn, pSubpicture, num_priv, priv);
}

static void
xf86XvMCDestroySubpicture (XvMCSubpicturePtr pSubpicture)
{
    XvMCContextPtr pContext = pSubpicture->context;
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pContext->pScreen);
    ScrnInfoPtr pScrn = xf86Screens[pContext->pScreen->myNum];

    (*pScreenPriv->adaptors[pContext->adapt_num]->DestroySubpicture)(
                                                pScrn, pSubpicture);
}


static Bool
xf86XvMCCloseScreen (int i, ScreenPtr pScreen)
{
    xf86XvMCScreenPtr pScreenPriv = XF86XVMC_GET_PRIVATE(pScreen);

    pScreen->CloseScreen = pScreenPriv->CloseScreen;

    xfree(pScreenPriv->dixinfo);
    xfree(pScreenPriv);

    return (*pScreen->CloseScreen)(i, pScreen);
}

_X_EXPORT Bool xf86XvMCScreenInit(
   ScreenPtr pScreen, 
   int num_adaptors,
   XF86MCAdaptorPtr *adaptors
)
{
   XvMCAdaptorPtr pAdapt;
   xf86XvMCScreenPtr pScreenPriv;
   XvScreenPtr pxvs = 
	(XvScreenPtr)(pScreen->devPrivates[XF86XvScreenIndex].ptr);

   int i, j;

   if(!XvMCScreenInitProc) return FALSE;

   if(XF86XvMCGeneration != serverGeneration) {
	if((XF86XvMCScreenIndex = AllocateScreenPrivateIndex()) < 0)
	   return FALSE;
	XF86XvMCGeneration = serverGeneration;
   }

   if(!(pAdapt = xalloc(sizeof(XvMCAdaptorRec) * num_adaptors)))
	return FALSE;

   if(!(pScreenPriv = xalloc(sizeof(xf86XvMCScreenRec)))) {
	xfree(pAdapt);
	return FALSE;
   }

   pScreen->devPrivates[XF86XvMCScreenIndex].ptr = (pointer)pScreenPriv; 

   pScreenPriv->CloseScreen = pScreen->CloseScreen;
   pScreen->CloseScreen = xf86XvMCCloseScreen;

   pScreenPriv->num_adaptors = num_adaptors;
   pScreenPriv->adaptors = adaptors;
   pScreenPriv->dixinfo = pAdapt;

   for(i = 0; i < num_adaptors; i++) {
	pAdapt[i].xv_adaptor = NULL;
	for(j = 0; j < pxvs->nAdaptors; j++) {
	   if(!strcmp((*adaptors)->name, pxvs->pAdaptors[j].name)) {
		pAdapt[i].xv_adaptor = &(pxvs->pAdaptors[j]); 
		break;
	   }
	}
	if(!pAdapt[i].xv_adaptor) {
	    /* no adaptor by that name */
	    xfree(pAdapt);
	    return FALSE;
	}
	pAdapt[i].num_surfaces = (*adaptors)->num_surfaces;
	pAdapt[i].surfaces = (XvMCSurfaceInfoPtr*)((*adaptors)->surfaces);
	pAdapt[i].num_subpictures = (*adaptors)->num_subpictures;
	pAdapt[i].subpictures = (XvImagePtr*)((*adaptors)->subpictures);
	pAdapt[i].CreateContext = xf86XvMCCreateContext;
	pAdapt[i].DestroyContext = xf86XvMCDestroyContext;
	pAdapt[i].CreateSurface = xf86XvMCCreateSurface;
	pAdapt[i].DestroySurface = xf86XvMCDestroySurface;
	pAdapt[i].CreateSubpicture = xf86XvMCCreateSubpicture;
	pAdapt[i].DestroySubpicture = xf86XvMCDestroySubpicture;
	adaptors++;
   }

   if(Success != (*XvMCScreenInitProc)(pScreen, num_adaptors, pAdapt))
	return FALSE;

   return TRUE;
}

_X_EXPORT XF86MCAdaptorPtr xf86XvMCCreateAdaptorRec (void)
{
   return xcalloc(1, sizeof(XF86MCAdaptorRec));
}

_X_EXPORT void xf86XvMCDestroyAdaptorRec(XF86MCAdaptorPtr adaptor)
{
   xfree(adaptor);
}
