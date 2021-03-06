/**************************************************************************
 * 
 * Copyright 2003 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 **************************************************************************/

#ifndef _INTEL_INIT_H_
#define _INTEL_INIT_H_

#include <sys/time.h>
#include "xmlconfig.h"
#include "dri_util.h"
#include "intel_rotate.h"
#include "i830_common.h"


<<<<<<< intel_screen.h
/* This roughly corresponds to a gl_renderbuffer (Mesa 6.4) */
=======
/* XXX: change name or eliminate to avoid conflict with "struct
 * intel_region"!!!
 */
>>>>>>> 1.5.2.3
typedef struct {
   drm_handle_t handle;
   drmSize size;        /* region size in bytes */
   char *map;           /* memory map */
   int offset;          /* from start of video mem, in bytes */
   int pitch;           /* row stride, in bytes */
} intelRegion;

typedef struct 
{
   intelRegion front;
   intelRegion back;
   intelRegion rotated;
   intelRegion depth;
   intelRegion tex;
   
   int deviceID;
   int width;
   int height;
   int mem;         /* unused */
   
   int cpp;         /* for front and back buffers */
<<<<<<< intel_screen.h
   int fbFormat;
=======
   int bitsPerPixel;  /* unused */
   int fbFormat;  /* XXX FBO: this is obsolete - remove after i830 updates */
>>>>>>> 1.5.2.3

   int logTextureGranularity;
   
   __DRIscreenPrivate *driScrnPriv;
   unsigned int sarea_priv_offset;

   int drmMinor;

   int irq_active;
   int allow_batchbuffer;

   struct matrix23 rotMatrix;

   int current_rotation;  /* 0, 90, 180 or 270 */
   int rotatedWidth, rotatedHeight;

   /**
   * Configuration cache with default values for all contexts
   */
   driOptionCache optionCache;
} intelScreenPrivate;


extern GLboolean
intelMapScreenRegions(__DRIscreenPrivate *sPriv);

extern void
intelUnmapScreenRegions(intelScreenPrivate *intelScreen);

extern void
intelUpdateScreenFromSAREA(intelScreenPrivate *intelScreen,
                           drmI830Sarea *sarea);

extern void
intelDestroyContext(__DRIcontextPrivate *driContextPriv);

extern GLboolean
intelUnbindContext(__DRIcontextPrivate *driContextPriv);

extern GLboolean
intelMakeCurrent(__DRIcontextPrivate *driContextPriv,
                 __DRIdrawablePrivate *driDrawPriv,
                 __DRIdrawablePrivate *driReadPriv);

<<<<<<< intel_screen.h
extern void
intelSwapBuffers(__DRIdrawablePrivate *dPriv);

=======
>>>>>>> 1.5.2.3
#endif
