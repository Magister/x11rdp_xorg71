# 
#  $Id: Makefile.am,v 1.3 2005/07/14 14:56:13 ajax Exp $
# 
#  Copyright © 2003 Keith Packard, Noah Levitt
# 
#  Permission to use, copy, modify, distribute, and sell this software and its
#  documentation for any purpose is hereby granted without fee, provided that
#  the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting
#  documentation, and that the name of Keith Packard not be used in
#  advertising or publicity pertaining to distribution of the software without
#  specific, written prior permission.  Keith Packard makes no
#  representations about the suitability of this software for any purpose.  It
#  is provided "as is" without express or implied warranty.
# 
#  KEITH PACKARD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
#  INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
#  EVENT SHALL KEITH PACKARD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
#  CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
#  DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
#  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
#  PERFORMANCE OF THIS SOFTWARE.

lib_LTLIBRARIES = libXfont.la

if XFONT_FONTCACHE
FONTCACHE_DIR = fontcache
FONTCACHE_LIB = fontcache/libfontcache.la
endif

if XFONT_FONTFILE
FONTFILE_DIR = fontfile
FONTFILE_LIB = fontfile/libfontfile.la
endif

if XFONT_FREETYPE
FREETYPE_DIR = FreeType
FREETYPE_LIB = FreeType/libft.la
endif

if XFONT_BITMAP
BITMAP_DIR = bitmap
BITMAP_LIB = bitmap/libbitmap.la
endif

if XFONT_BUILTINS
BUILTINS_DIR = builtins
BUILTINS_LIB = builtins/libbuiltins.la
endif

if XFONT_FC
FC_DIR = fc
FC_LIB = fc/libfc.la
endif

if XFONT_TYPE1
TYPE1_DIR = Type1
TYPE1_LIB = Type1/libtype1.la
endif

if XFONT_SPEEDO
SPEEDO_DIR = Speedo
SPEEDO_LIB = Speedo/libspeedo.la
endif

UTIL_DIR = util
UTIL_LIB = util/libutil.la

STUBS_LIB = stubs/libstubs.la
STUBS_DIR = stubs

SUBDIRS=\
	$(FONTFILE_DIR) $(FREETYPE_DIR) $(BITMAP_DIR) \
	$(BUILTINS_DIR) $(FC_DIR) $(UTIL_DIR) $(STUBS_DIR) $(FONTCACHE_DIR) \
	$(TYPE1_DIR) $(SPEEDO_DIR)

libXfont_la_LIBADD = \
	$(FONTFILE_LIB) $(FREETYPE_LIB) $(BITMAP_LIB) \
	$(BUILTINS_LIB) $(FC_LIB) $(UTIL_LIB) $(STUBS_LIB) $(FONTCACHE_LIB) \
	$(TYPE1_LIB) $(SPEEDO_LIB) \
	$(FREETYPE_LIBS) $(Z_LIBS) $(MATH_LIBS) 	

libXfont_la_SOURCES = dummy.c

libXfont_la_LDFLAGS = -version-number 1:4:1 -no-undefined
