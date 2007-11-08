/**************************************************************************
 * 
 * Copyright 2006 Tungsten Graphics, Inc., Cedar Park, Texas.
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

/* Provide additional functionality on top of bufmgr buffers:
 *   - 2d semantics and blit operations
 *   - refcounting of buffers for multiple images in a buffer.
 *   - refcounting of buffer mappings.
 *   - some logic for moving the buffers to the best memory pools for
 *     given operations.
 *
 * Most of this is to make it easier to implement the fixed-layout
 * mipmap tree required by intel hardware in the face of GL's
 * programming interface where each image can be specifed in random
 * order and it isn't clear what layout the tree should have until the
 * last moment.
 */

#include "intel_context.h"
#include "intel_regions.h"
#include "intel_blit.h"
#include "intel_bufmgr.h"

/* XXX: Thread safety?
 */
GLubyte *intel_region_map(struct intel_context *intel, struct intel_region *region)
{
   DBG("%s\n", __FUNCTION__);
   if (!region->map_refcount++) {
      region->map = bmMapBuffer(intel->bm, region->buffer, 0);
   }

   return region->map;
}

void intel_region_unmap(struct intel_context *intel, 
			struct intel_region *region)
{
   DBG("%s\n", __FUNCTION__);
   if (!--region->map_refcount) {
      bmUnmapBuffer(intel->bm, region->buffer);
      region->map = NULL;
   }
}

struct intel_region *intel_region_alloc( struct intel_context *intel, 
					 GLuint cpp,
					 GLuint pitch, 
					 GLuint height )
{
   struct intel_region *region = calloc(sizeof(*region), 1);

   DBG("%s\n", __FUNCTION__);

   region->cpp = cpp;
   region->pitch = pitch;
   region->height = height; 	/* needed? */
   region->refcount = 1;

   bmGenBuffers(intel->bm, 1, &region->buffer, 0);
   bmBufferData(intel->bm, region->buffer, pitch * cpp * height, NULL, 0);

   return region;
}

void intel_region_reference( struct intel_region **dst,
			     struct intel_region *src)
{
   assert(*dst == NULL);
   if (src) {
      src->refcount++;
      *dst = src;
   }
}

void intel_region_release( struct intel_context *intel,
			   struct intel_region **region )
{
   if (!*region)
      return;

   DBG("%s %d\n", __FUNCTION__, (*region)->refcount-1);

   ASSERT((*region)->refcount > 0);
   (*region)->refcount--;

   if ((*region)->refcount == 0) {
      assert((*region)->map_refcount == 0);
      bmDeleteBuffers(intel->bm, 1, &(*region)->buffer);
      free(*region);
   }
   *region = NULL;
}


struct intel_region *intel_region_create_static( struct intel_context *intel,
						 GLuint mem_type,
						 GLuint offset,
						 void *virtual,
						 GLuint cpp,
						 GLuint pitch, 
						 GLuint height )
{
   struct intel_region *region = calloc(sizeof(*region), 1);
   DBG("%s\n", __FUNCTION__);

   region->cpp = cpp;
   region->pitch = pitch;
   region->height = height; 	/* needed? */
   region->refcount = 1;

   /*
    * We use a "shared" buffer type to indicate buffers created and
    * shared by others.
    */

   bmGenBuffers(intel->bm, 1, &region->buffer, DRM_MM_TT | DRM_MM_SHARED);
   bmSetShared(intel->bm, region->buffer, DRM_MM_TT, offset, virtual);

   return region;
}



/*
 * XXX Move this into core Mesa?
 */
static void _mesa_copy_rect( GLubyte *dst,
			     GLuint cpp,
			     GLuint dst_pitch,
			     GLuint dst_x, 
			     GLuint dst_y,
			     GLuint width,
			     GLuint height,
			     GLubyte *src,
			     GLuint src_pitch,
			     GLuint src_x,
			     GLuint src_y )
{
   GLuint i;

   dst_pitch *= cpp;
   src_pitch *= cpp;
   dst += dst_x * cpp;
   src += src_x * cpp;
   dst += dst_y * dst_pitch;
   src += src_y * dst_pitch;
   width *= cpp;

   if (width == dst_pitch && 
       width == src_pitch)
      memcpy(dst, src, height * width);
   else {
      for (i = 0; i < height; i++) {
	 memcpy(dst, src, width);
	 dst += dst_pitch;
	 src += src_pitch;
      }
   }
}


/* Upload data to a rectangular sub-region.  Lots of choices how to do this:
 *
 * - memcpy by span to current destination
 * - upload data as new buffer and blit
 *
 * Currently always memcpy.
 */
void intel_region_data(struct intel_context *intel, 
		       struct intel_region *dst,
		       GLuint dst_offset,
		       GLuint dstx, GLuint dsty,
		       void *src, GLuint src_pitch,
		       GLuint srcx, GLuint srcy,
		       GLuint width, GLuint height)
{
   DBG("%s\n", __FUNCTION__);

   LOCK_HARDWARE(intel);
   
   _mesa_copy_rect(intel_region_map(intel, dst) + dst_offset,
		   dst->cpp,
		   dst->pitch,
		   dstx, dsty,
		   width, height,
		   src,
		   src_pitch,
		   srcx, srcy);      

   intel_region_unmap(intel, dst);

   UNLOCK_HARDWARE(intel);
   
}
			  
/* Copy rectangular sub-regions. Need better logic about when to
 * push buffers into AGP - will currently do so whenever possible.
 */
void intel_region_copy( struct intel_context *intel,
			struct intel_region *dst,
			GLuint dst_offset,
			GLuint dstx, GLuint dsty,
			struct intel_region *src,
			GLuint src_offset,
			GLuint srcx, GLuint srcy,
			GLuint width, GLuint height )
{
   DBG("%s\n", __FUNCTION__);

   assert(src->cpp == dst->cpp);

   intelEmitCopyBlit(intel,
		     dst->cpp,
		     src->pitch, src->buffer, src_offset,
		     dst->pitch, dst->buffer, dst_offset, 
		     srcx, srcy,
		     dstx, dsty,
		     width, height);
}

/* Fill a rectangular sub-region.  Need better logic about when to
 * push buffers into AGP - will currently do so whenever possible.
 */
void intel_region_fill( struct intel_context *intel,
			struct intel_region *dst,
			GLuint dst_offset,
			GLuint dstx, GLuint dsty,
			GLuint width, GLuint height,
			GLuint color )
{
   DBG("%s\n", __FUNCTION__);
   
   intelEmitFillBlit(intel,
		     dst->cpp,
		     dst->pitch, dst->buffer, dst_offset, 
		     dstx, dsty,
		     width, height,
		     color );
}

