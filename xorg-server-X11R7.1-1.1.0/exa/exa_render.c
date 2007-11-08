 /*
 * Copyright © 2001 Keith Packard
 *
 * Partly based on code that is Copyright © The XFree86 Project Inc.
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

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdlib.h>

#include "exa_priv.h"

#ifdef RENDER
#include "mipict.h"

#if DEBUG_TRACE_FALL
static void exaCompositeFallbackPictDesc(PicturePtr pict, char *string, int n)
{
    char format[20];
    char size[20];
    char loc;
    int temp;

    if (!pict) {
	snprintf(string, n, "None");
	return;
    }

    switch (pict->format)
    {
    case PICT_a8r8g8b8:
	snprintf(format, 20, "ARGB8888");
	break;
    case PICT_r5g6b5:
	snprintf(format, 20, "RGB565  ");
	break;
    case PICT_x1r5g5b5:
	snprintf(format, 20, "RGB555  ");
	break;
    case PICT_a8:
	snprintf(format, 20, "A8      ");
	break;
    case PICT_a1:
	snprintf(format, 20, "A1      ");
	break;
    default:
	snprintf(format, 20, "0x%x", (int)pict->format);
	break;
    }

    loc = exaGetOffscreenPixmap(pict->pDrawable, &temp, &temp) ? 's' : 'm';

    snprintf(size, 20, "%dx%d%s", pict->pDrawable->width,
	     pict->pDrawable->height, pict->repeat ?
	     " R" : "");

    snprintf(string, n, "0x%lx:%c fmt %s (%s)", (long)pict->pDrawable, loc, format, size);
}

static void
exaPrintCompositeFallback(CARD8 op,
			  PicturePtr pSrc,
			  PicturePtr pMask,
			  PicturePtr pDst)
{
    char sop[20];
    char srcdesc[40], maskdesc[40], dstdesc[40];

    switch(op)
    {
    case PictOpSrc:
	sprintf(sop, "Src");
	break;
    case PictOpOver:
	sprintf(sop, "Over");
	break;
    default:
	sprintf(sop, "0x%x", (int)op);
	break;
    }

    exaCompositeFallbackPictDesc(pSrc, srcdesc, 40);
    exaCompositeFallbackPictDesc(pMask, maskdesc, 40);
    exaCompositeFallbackPictDesc(pDst, dstdesc, 40);

    ErrorF("Composite fallback: op %s, \n"
	   "                    src  %s, \n"
	   "                    mask %s, \n"
	   "                    dst  %s, \n",
	   sop, srcdesc, maskdesc, dstdesc);
}
#endif /* DEBUG_TRACE_FALL */

static Bool
exaOpReadsDestination (CARD8 op)
{
    /* FALSE (does not read destination) is the list of ops in the protocol
     * document with "0" in the "Fb" column and no "Ab" in the "Fa" column.
     * That's just Clear and Src.  ReduceCompositeOp() will already have
     * converted con/disjoint clear/src to Clear or Src.
     */
    switch (op) {
    case PictOpClear:
    case PictOpSrc:
	return FALSE;
    default:
	return TRUE;
    }
}


static Bool
exaGetPixelFromRGBA(CARD32	*pixel,
		    CARD16	red,
		    CARD16	green,
		    CARD16	blue,
		    CARD16	alpha,
		    CARD32	format)
{
    int rbits, bbits, gbits, abits;
    int rshift, bshift, gshift, ashift;

    *pixel = 0;

    if (!PICT_FORMAT_COLOR(format))
	return FALSE;

    rbits = PICT_FORMAT_R(format);
    gbits = PICT_FORMAT_G(format);
    bbits = PICT_FORMAT_B(format);
    abits = PICT_FORMAT_A(format);

    if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
	bshift = 0;
	gshift = bbits;
	rshift = gshift + gbits;
	ashift = rshift + rbits;
    } else {  /* PICT_TYPE_ABGR */
	rshift = 0;
	gshift = rbits;
	bshift = gshift + gbits;
	ashift = bshift + bbits;
    }

    *pixel |=  ( blue >> (16 - bbits)) << bshift;
    *pixel |=  (  red >> (16 - rbits)) << rshift;
    *pixel |=  (green >> (16 - gbits)) << gshift;
    *pixel |=  (alpha >> (16 - abits)) << ashift;

    return TRUE;
}

static Bool
exaGetRGBAFromPixel(CARD32	pixel,
		    CARD16	*red,
		    CARD16	*green,
		    CARD16	*blue,
		    CARD16	*alpha,
		    CARD32	format)
{
    int rbits, bbits, gbits, abits;
    int rshift, bshift, gshift, ashift;

    if (!PICT_FORMAT_COLOR(format))
	return FALSE;

    rbits = PICT_FORMAT_R(format);
    gbits = PICT_FORMAT_G(format);
    bbits = PICT_FORMAT_B(format);
    abits = PICT_FORMAT_A(format);

    if (PICT_FORMAT_TYPE(format) == PICT_TYPE_ARGB) {
	bshift = 0;
	gshift = bbits;
	rshift = gshift + gbits;
	ashift = rshift + rbits;
    } else {  /* PICT_TYPE_ABGR */
	rshift = 0;
	gshift = rbits;
	bshift = gshift + gbits;
	ashift = bshift + bbits;
    }

    *red = ((pixel >> rshift ) & ((1 << rbits) - 1)) << (16 - rbits);
    while (rbits < 16) {
	*red |= *red >> rbits;
	rbits <<= 1;
    }

    *green = ((pixel >> gshift ) & ((1 << gbits) - 1)) << (16 - gbits);
    while (gbits < 16) {
	*green |= *green >> gbits;
	gbits <<= 1;
    }

    *blue = ((pixel >> bshift ) & ((1 << bbits) - 1)) << (16 - bbits);
    while (bbits < 16) {
	*blue |= *blue >> bbits;
	bbits <<= 1;
    }

    if (abits) {
	*alpha = ((pixel >> ashift ) & ((1 << abits) - 1)) << (16 - abits);
	while (abits < 16) {
	    *alpha |= *alpha >> abits;
	    abits <<= 1;
	}
    } else
	*alpha = 0xffff;

    return TRUE;
}

static int
exaTryDriverSolidFill(PicturePtr	pSrc,
		      PicturePtr	pDst,
		      INT16		xSrc,
		      INT16		ySrc,
		      INT16		xDst,
		      INT16		yDst,
		      CARD16		width,
		      CARD16		height)
{
    ExaScreenPriv (pDst->pDrawable->pScreen);
    RegionRec region;
    BoxPtr pbox;
    int nbox;
    int dst_off_x, dst_off_y;
    PixmapPtr pSrcPix, pDstPix;
    CARD32 pixel;
    CARD16 red, green, blue, alpha;
    ExaMigrationRec pixmaps[1];

    xDst += pDst->pDrawable->x;
    yDst += pDst->pDrawable->y;
    xSrc += pSrc->pDrawable->x;
    ySrc += pSrc->pDrawable->y;

    if (!miComputeCompositeRegion (&region, pSrc, NULL, pDst,
				   xSrc, ySrc, 0, 0, xDst, yDst,
				   width, height))
	return 1;

    pSrcPix = exaGetDrawablePixmap (pSrc->pDrawable);
    pixel = exaGetPixmapFirstPixel (pSrcPix);

    pixmaps[0].as_dst = TRUE;
    pixmaps[0].as_src = FALSE;
    pixmaps[0].pPix = exaGetDrawablePixmap (pDst->pDrawable);
    exaDoMigration(pixmaps, 1, TRUE);

    pDstPix = exaGetOffscreenPixmap (pDst->pDrawable, &dst_off_x, &dst_off_y);
    if (!pDstPix) {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 0;
    }

    if (!exaGetRGBAFromPixel(pixel, &red, &green, &blue, &alpha,
			 pSrc->format))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    exaGetPixelFromRGBA(&pixel, red, green, blue, alpha,
			pDst->format);

    if (!(*pExaScr->info->PrepareSolid) (pDstPix, GXcopy, 0xffffffff, pixel))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    nbox = REGION_NUM_RECTS(&region);
    pbox = REGION_RECTS(&region);
    while (nbox--)
    {
	(*pExaScr->info->Solid) (pDstPix,
				 pbox->x1 + dst_off_x, pbox->y1 + dst_off_y,
				 pbox->x2 + dst_off_x, pbox->y2 + dst_off_y);
	pbox++;
    }

    (*pExaScr->info->DoneSolid) (pDstPix);
    exaMarkSync(pDst->pDrawable->pScreen);
    exaDrawableDirty (pDst->pDrawable);

    REGION_UNINIT(pDst->pDrawable->pScreen, &region);
    return 1;
}

static int
exaTryDriverComposite(CARD8		op,
		      PicturePtr	pSrc,
		      PicturePtr	pMask,
		      PicturePtr	pDst,
		      INT16		xSrc,
		      INT16		ySrc,
		      INT16		xMask,
		      INT16		yMask,
		      INT16		xDst,
		      INT16		yDst,
		      CARD16		width,
		      CARD16		height)
{
    ExaScreenPriv (pDst->pDrawable->pScreen);
    RegionRec region;
    BoxPtr pbox;
    int nbox;
    int src_off_x, src_off_y, mask_off_x, mask_off_y, dst_off_x, dst_off_y;
    PixmapPtr pSrcPix, pMaskPix = NULL, pDstPix;
    struct _Pixmap scratch;
    ExaMigrationRec pixmaps[3];

    /* Bail if we might exceed coord limits by rendering from/to these.  We
     * should really be making some scratch pixmaps with offsets and coords
     * adjusted to deal with this, but it hasn't been done yet.
     */
    if (pSrc->pDrawable->width > pExaScr->info->maxX ||
	pSrc->pDrawable->height > pExaScr->info->maxY ||
	pDst->pDrawable->width > pExaScr->info->maxX ||
	pDst->pDrawable->height > pExaScr->info->maxY || 
	(pMask && (pMask->pDrawable->width > pExaScr->info->maxX ||
		   pMask->pDrawable->height > pExaScr->info->maxY)))
    {
	return -1;
    }

    xDst += pDst->pDrawable->x;
    yDst += pDst->pDrawable->y;

    if (pMask) {
	xMask += pMask->pDrawable->x;
	yMask += pMask->pDrawable->y;
    }

    xSrc += pSrc->pDrawable->x;
    ySrc += pSrc->pDrawable->y;

    if (!miComputeCompositeRegion (&region, pSrc, pMask, pDst,
				   xSrc, ySrc, xMask, yMask, xDst, yDst,
				   width, height))
	return 1;

    if (pExaScr->info->CheckComposite &&
	!(*pExaScr->info->CheckComposite) (op, pSrc, pMask, pDst))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    pixmaps[0].as_dst = TRUE;
    pixmaps[0].as_src = exaOpReadsDestination(op);
    pixmaps[0].pPix = exaGetDrawablePixmap (pDst->pDrawable);
    pixmaps[1].as_dst = FALSE;
    pixmaps[1].as_src = TRUE;
    pixmaps[1].pPix = exaGetDrawablePixmap (pSrc->pDrawable);
    if (pMask) {
	pixmaps[2].as_dst = FALSE;
	pixmaps[2].as_src = TRUE;
	pixmaps[2].pPix = exaGetDrawablePixmap (pMask->pDrawable);
	exaDoMigration(pixmaps, 3, TRUE);
    } else {
	exaDoMigration(pixmaps, 2, TRUE);
    }

    pSrcPix = exaGetOffscreenPixmap (pSrc->pDrawable, &src_off_x, &src_off_y);
    if (pMask)
	pMaskPix = exaGetOffscreenPixmap (pMask->pDrawable, &mask_off_x,
					  &mask_off_y);
    pDstPix = exaGetOffscreenPixmap (pDst->pDrawable, &dst_off_x, &dst_off_y);

    if (!pDstPix) {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 0;
    }

    if (!pSrcPix && (!pMask || pMaskPix) && pExaScr->info->UploadToScratch) {
	pSrcPix = exaGetDrawablePixmap (pSrc->pDrawable);
	if ((*pExaScr->info->UploadToScratch) (pSrcPix, &scratch))
	    pSrcPix = &scratch;
    } else if (pSrcPix && pMask && !pMaskPix && pExaScr->info->UploadToScratch) {
	pMaskPix = exaGetDrawablePixmap (pMask->pDrawable);
	if ((*pExaScr->info->UploadToScratch) (pMaskPix, &scratch))
	    pMaskPix = &scratch;
    }

    if (!pSrcPix || (pMask && !pMaskPix)) {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return 0;
    }

    if (!(*pExaScr->info->PrepareComposite) (op, pSrc, pMask, pDst, pSrcPix,
					     pMaskPix, pDstPix))
    {
	REGION_UNINIT(pDst->pDrawable->pScreen, &region);
	return -1;
    }

    nbox = REGION_NUM_RECTS(&region);
    pbox = REGION_RECTS(&region);

    xMask -= xDst;
    yMask -= yDst;

    xSrc -= xDst;
    ySrc -= yDst;

    while (nbox--)
    {
	(*pExaScr->info->Composite) (pDstPix,
				     pbox->x1 + xSrc + src_off_x,
				     pbox->y1 + ySrc + src_off_y,
				     pbox->x1 + xMask + mask_off_x,
				     pbox->y1 + yMask + mask_off_y,
				     pbox->x1 + dst_off_x,
				     pbox->y1 + dst_off_y,
				     pbox->x2 - pbox->x1,
				     pbox->y2 - pbox->y1);
	pbox++;
    }

    (*pExaScr->info->DoneComposite) (pDstPix);
    exaMarkSync(pDst->pDrawable->pScreen);
    exaDrawableDirty (pDst->pDrawable);

    REGION_UNINIT(pDst->pDrawable->pScreen, &region);
    return 1;
}


void
exaComposite(CARD8	op,
	     PicturePtr pSrc,
	     PicturePtr pMask,
	     PicturePtr pDst,
	     INT16	xSrc,
	     INT16	ySrc,
	     INT16	xMask,
	     INT16	yMask,
	     INT16	xDst,
	     INT16	yDst,
	     CARD16	width,
	     CARD16	height)
{
    ExaScreenPriv (pDst->pDrawable->pScreen);
    int ret = -1;
    Bool saveSrcRepeat = pSrc->repeat;
    Bool saveMaskRepeat = pMask ? pMask->repeat : 0;

    /* We currently don't support acceleration of gradients, or other pictures
     * with a NULL pDrawable.
     */
    if (pExaScr->swappedOut ||
	pSrc->pDrawable == NULL || (pMask != NULL && pMask->pDrawable == NULL))
    {
	ExaCheckComposite (op, pSrc, pMask, pDst, xSrc, ySrc,
			   xMask, yMask, xDst, yDst, width, height);
        return;
    }

    /* Remove repeat in source if useless */
    if (pSrc->repeat && !pSrc->transform && xSrc >= 0 &&
	(xSrc + width) <= pSrc->pDrawable->width && ySrc >= 0 &&
	(ySrc + height) <= pSrc->pDrawable->height)
	    pSrc->repeat = 0;

    if (!pMask)
    {
	if (op == PictOpSrc)
	{
	    if (pSrc->pDrawable->width == 1 &&
		pSrc->pDrawable->height == 1 && pSrc->repeat)
	    {
		ret = exaTryDriverSolidFill(pSrc, pDst, xSrc, ySrc, xDst, yDst,
					    width, height);
		if (ret == 1)
		    goto bail;
	    }
	    else if (!pSrc->repeat && !pSrc->transform &&
		     pSrc->format == pDst->format)
	    {
		RegionRec	region;

		xDst += pDst->pDrawable->x;
		yDst += pDst->pDrawable->y;
		xSrc += pSrc->pDrawable->x;
		ySrc += pSrc->pDrawable->y;

		if (!miComputeCompositeRegion (&region, pSrc, pMask, pDst,
					       xSrc, ySrc, xMask, yMask, xDst,
					       yDst, width, height))
		    goto bail;


		exaCopyNtoN (pSrc->pDrawable, pDst->pDrawable, NULL,
			     REGION_RECTS(&region), REGION_NUM_RECTS(&region),
			     xSrc - xDst, ySrc - yDst,
			     FALSE, FALSE, 0, NULL);
		REGION_UNINIT(pDst->pDrawable->pScreen, &region);
		goto bail;
	    }
	}
    }

    /* Remove repeat in mask if useless */
    if (pMask && pMask->repeat && !pMask->transform && xMask >= 0 &&
	(xMask + width) <= pMask->pDrawable->width && yMask >= 0 &&
	(yMask + height) <= pMask->pDrawable->height)
	    pMask->repeat = 0;


    if (pExaScr->info->PrepareComposite &&
	!pSrc->alphaMap && (!pMask || !pMask->alphaMap) && !pDst->alphaMap)
    {
	ret = exaTryDriverComposite(op, pSrc, pMask, pDst, xSrc, ySrc, xMask,
				    yMask, xDst, yDst, width, height);
	if (ret == 1)
	    goto bail;
    }

    if (ret != 0) {
	ExaMigrationRec pixmaps[3];
	/* failure to accelerate was not due to pixmaps being in the wrong
	 * locations.
	 */
	pixmaps[0].as_dst = TRUE;
	pixmaps[0].as_src = exaOpReadsDestination(op);
	pixmaps[0].pPix = exaGetDrawablePixmap (pDst->pDrawable);
	pixmaps[1].as_dst = FALSE;
	pixmaps[1].as_src = TRUE;
	pixmaps[1].pPix = exaGetDrawablePixmap (pSrc->pDrawable);
	if (pMask) {
	    pixmaps[2].as_dst = FALSE;
	    pixmaps[2].as_src = TRUE;
	    pixmaps[2].pPix = exaGetDrawablePixmap (pMask->pDrawable);
	    exaDoMigration(pixmaps, 3, FALSE);
	} else {
	    exaDoMigration(pixmaps, 2, FALSE);
	}
    }

#if DEBUG_TRACE_FALL
    exaPrintCompositeFallback (op, pSrc, pMask, pDst);
#endif

    ExaCheckComposite (op, pSrc, pMask, pDst, xSrc, ySrc,
		      xMask, yMask, xDst, yDst, width, height);

 bail:
    pSrc->repeat = saveSrcRepeat;
    if (pMask)
	pMask->repeat = saveMaskRepeat;
}
#endif

#define NeedsComponent(f) (PICT_FORMAT_A(f) != 0 && PICT_FORMAT_RGB(f) != 0)

/**
 * exaRasterizeTrapezoid is just a wrapper around the software implementation.
 *
 * The trapezoid specification is basically too hard to be done in hardware (at
 * the very least, without programmability), so we just do the appropriate
 * Prepare/FinishAccess for it before using fbtrap.c.
 */
void
exaRasterizeTrapezoid (PicturePtr pPicture, xTrapezoid  *trap,
		       int x_off, int y_off)
{
    ExaMigrationRec pixmaps[1];

    pixmaps[0].as_dst = TRUE;
    pixmaps[0].as_src = TRUE;
    pixmaps[0].pPix = exaGetDrawablePixmap (pPicture->pDrawable);
    exaDoMigration(pixmaps, 1, FALSE);

    exaPrepareAccess(pPicture->pDrawable, EXA_PREPARE_DEST);
    fbRasterizeTrapezoid(pPicture, trap, x_off, y_off);
    exaFinishAccess(pPicture->pDrawable, EXA_PREPARE_DEST);
}

/**
 * exaAddTriangles does migration and syncing before dumping down to the
 * software implementation.
 */
void
exaAddTriangles (PicturePtr pPicture, INT16 x_off, INT16 y_off, int ntri,
		 xTriangle *tris)
{
    ExaMigrationRec pixmaps[1];

    pixmaps[0].as_dst = TRUE;
    pixmaps[0].as_src = TRUE;
    pixmaps[0].pPix = exaGetDrawablePixmap (pPicture->pDrawable);
    exaDoMigration(pixmaps, 1, FALSE);

    exaPrepareAccess(pPicture->pDrawable, EXA_PREPARE_DEST);
    fbAddTriangles(pPicture, x_off, y_off, ntri, tris);
    exaFinishAccess(pPicture->pDrawable, EXA_PREPARE_DEST);
}

/* exaGlyphs is a slight variation on miGlyphs, to support acceleration.  The
 * issue is that miGlyphs' use of ModifyPixmapHeader makes it impossible to
 * migrate these pixmaps.  So, instead we create a pixmap at the beginning of
 * the loop and upload each glyph into the pixmap before compositing.
 */
void
exaGlyphs (CARD8	op,
	  PicturePtr	pSrc,
	  PicturePtr	pDst,
	  PictFormatPtr	maskFormat,
	  INT16		xSrc,
	  INT16		ySrc,
	  int		nlist,
	  GlyphListPtr	list,
	  GlyphPtr	*glyphs)
{
    ExaScreenPriv (pDst->pDrawable->pScreen);
    PixmapPtr	pPixmap = NULL, pScratchPixmap = NULL;
    PicturePtr	pPicture;
    PixmapPtr   pMaskPixmap = NULL;
    PicturePtr  pMask;
    ScreenPtr   pScreen = pDst->pDrawable->pScreen;
    int		width = 0, height = 0;
    int		x, y;
    int		xDst = list->xOff, yDst = list->yOff;
    int		n;
    GlyphPtr	glyph;
    int		error;
    BoxRec	extents;
    CARD32	component_alpha;

    /* If the driver doesn't support accelerated composite, there's no point in
     * going to this extra work.  Assume that no driver will be able to do
     * component-alpha, which is likely accurate (at least until we make a CA
     * helper).
     */
    if (!pExaScr->info->PrepareComposite ||
	(maskFormat && NeedsComponent(maskFormat->format))) {
	miGlyphs(op, pSrc, pDst, maskFormat, xSrc, ySrc, nlist, list, glyphs);
	return;
    }

    if (maskFormat)
    {
	GCPtr	    pGC;
	xRectangle  rect;
	
	miGlyphExtents (nlist, list, glyphs, &extents);
	
	if (extents.x2 <= extents.x1 || extents.y2 <= extents.y1)
	    return;
	width = extents.x2 - extents.x1;
	height = extents.y2 - extents.y1;
	pMaskPixmap = (*pScreen->CreatePixmap) (pScreen, width, height,
						maskFormat->depth);
	if (!pMaskPixmap)
	    return;
	component_alpha = NeedsComponent(maskFormat->format);
	pMask = CreatePicture (0, &pMaskPixmap->drawable,
			       maskFormat, CPComponentAlpha, &component_alpha,
			       serverClient, &error);
	if (!pMask)
	{
	    (*pScreen->DestroyPixmap) (pMaskPixmap);
	    return;
	}
	pGC = GetScratchGC (pMaskPixmap->drawable.depth, pScreen);
	ValidateGC (&pMaskPixmap->drawable, pGC);
	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;
	(*pGC->ops->PolyFillRect) (&pMaskPixmap->drawable, pGC, 1, &rect);
	FreeScratchGC (pGC);
	x = -extents.x1;
	y = -extents.y1;
    }
    else
    {
	pMask = pDst;
	x = 0;
	y = 0;
    }

    while (nlist--)
    {
	GCPtr pGC;
	int maxwidth = 0, maxheight = 0, i;
	ExaMigrationRec pixmaps[1];

	x += list->xOff;
	y += list->yOff;
	n = list->len;
	for (i = 0; i < n; i++) {
	    if (glyphs[i]->info.width > maxwidth)
		maxwidth = glyphs[i]->info.width;
	    if (glyphs[i]->info.height > maxheight)
		maxheight = glyphs[i]->info.height;
	}
	if (maxwidth == 0 || maxheight == 0) {
	    while (n--)
	    {
		glyph = *glyphs++;
		x += glyph->info.xOff;
		y += glyph->info.yOff;
	    }
	    list++;
	    continue;
	}

	/* Get a scratch pixmap to wrap the original glyph data */
	pScratchPixmap = GetScratchPixmapHeader (pScreen, glyphs[0]->info.width,
						 glyphs[0]->info.height, 
						 list->format->depth,
						 list->format->depth, 
						 0, (pointer) (glyphs[0] + 1));
	if (!pScratchPixmap)
	    return;

	/* Create the (real) temporary pixmap to store the current glyph in */
	pPixmap = (*pScreen->CreatePixmap) (pScreen, maxwidth, maxheight,
					    list->format->depth);
	if (!pPixmap) {
	    FreeScratchPixmapHeader (pScratchPixmap);
	    return;
	}

	/* Create a temporary picture to wrap the temporary pixmap, so it can be
	 * used as a source for Composite.
	 */
	component_alpha = NeedsComponent(list->format->format);
	pPicture = CreatePicture (0, &pPixmap->drawable, list->format,
				  CPComponentAlpha, &component_alpha, 
				  serverClient, &error);
	if (!pPicture) {
	    (*pScreen->DestroyPixmap) (pPixmap);
	    FreeScratchPixmapHeader (pScratchPixmap);
	    return;
	}

	/* Get a scratch GC with which to copy the glyph data from scratch to
	 * temporary
	 */
	pGC = GetScratchGC (list->format->depth, pScreen);
	ValidateGC (&pPixmap->drawable, pGC);

	/* Give the temporary pixmap an initial kick towards the screen, so
	 * it'll stick there.
	 */
	pixmaps[0].as_dst = TRUE;
	pixmaps[0].as_src = TRUE;
	pixmaps[0].pPix = pPixmap;
	exaDoMigration (pixmaps, 1, TRUE);

	while (n--)
	{
	    glyph = *glyphs++;
	    
	    (*pScreen->ModifyPixmapHeader) (pScratchPixmap, 
					    glyph->info.width,
					    glyph->info.height,
					    0, 0, -1, (pointer) (glyph + 1));
	    pScratchPixmap->drawable.serialNumber = NEXT_SERIAL_NUMBER;

	    /* Copy the glyph data into the proper pixmap instead of a fake.
	     * First we try to use UploadToScreen, if we can, then we fall back
	     * to a plain exaCopyArea in case of failure.
	     */
	    if (!pExaScr->info->UploadToScreen ||
		!exaPixmapIsOffscreen(pPixmap) ||
		!(*pExaScr->info->UploadToScreen) (pPixmap, 0, 0,
					glyph->info.width,
					glyph->info.height,
					pScratchPixmap->devPrivate.ptr,
					pScratchPixmap->devKind))
	    {
		exaCopyArea (&pScratchPixmap->drawable, &pPixmap->drawable, pGC,
			     0, 0, glyph->info.width, glyph->info.height, 0, 0);
	    } else {
		exaDrawableDirty (&pPixmap->drawable);
	    }

	    if (maskFormat)
	    {
		CompositePicture (PictOpAdd,
				  pPicture,
				  NULL,
				  pMask,
				  0, 0,
				  0, 0,
				  x - glyph->info.x,
				  y - glyph->info.y,
				  glyph->info.width,
				  glyph->info.height);
	    }
	    else
	    {
		CompositePicture (op,
				  pSrc,
				  pPicture,
				  pDst,
				  xSrc + (x - glyph->info.x) - xDst,
				  ySrc + (y - glyph->info.y) - yDst,
				  0, 0,
				  x - glyph->info.x,
				  y - glyph->info.y,
				  glyph->info.width,
				  glyph->info.height);
	    }
	    x += glyph->info.xOff;
	    y += glyph->info.yOff;
	}
	list++;
	FreeScratchGC (pGC);
	FreePicture ((pointer) pPicture, 0);
	(*pScreen->DestroyPixmap) (pPixmap);
	FreeScratchPixmapHeader (pScratchPixmap);
    }
    if (maskFormat)
    {
	x = extents.x1;
	y = extents.y1;
	CompositePicture (op,
			  pSrc,
			  pMask,
			  pDst,
			  xSrc + x - xDst,
			  ySrc + y - yDst,
			  0, 0,
			  x, y,
			  width, height);
	FreePicture ((pointer) pMask, (XID) 0);
	(*pScreen->DestroyPixmap) (pMaskPixmap);
    }
}
