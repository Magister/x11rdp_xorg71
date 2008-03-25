/*
Copyright 2005-2008 Jay Sorg

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This is the main file called from main.c
Sets up the  functions

*/

#include "rdp.h"

rdpScreenInfoRec g_rdpScreen; /* the one screen */
ScreenPtr g_pScreen = 0;
int g_rdpGCIndex = -1;
/* set all these at once, use function set_bpp */
/* only allow 8 and 16 bpp for not, adding 32 later */
int g_bpp = 16;
int g_Bpp = 2;
int g_Bpp_mask = 0xffff;
static int g_firstTime = 1;
static int g_redBits = 5;
static int g_greenBits = 6;
static int g_blueBits = 5;
static int g_initOutputCalled = 0;
/* Common pixmap formats */
static PixmapFormatRec g_formats[MAXFORMATS] =
{
  { 1, 1, BITMAP_SCANLINE_PAD },
  { 4, 8, BITMAP_SCANLINE_PAD },
  { 8, 8, BITMAP_SCANLINE_PAD },
  { 15, 16, BITMAP_SCANLINE_PAD },
  { 16, 16, BITMAP_SCANLINE_PAD },
  { 24, 32, BITMAP_SCANLINE_PAD },
  { 32, 32, BITMAP_SCANLINE_PAD },
};
static int g_numFormats = 7;
static miPointerSpriteFuncRec g_rdpSpritePointerFuncs =
{
  /* these are in rdpinput.c */
  rdpSpriteRealizeCursor,
  rdpSpriteUnrealizeCursor,
  rdpSpriteSetCursor,
  rdpSpriteMoveCursor,
};
static miPointerScreenFuncRec g_rdpPointerCursorFuncs =
{
  /* these are in rdpinput.c */
  rdpCursorOffScreen,
  rdpCrossScreen,
  miPointerWarpCursor /* don't need to set last 2 funcs
                         EnqueueEvent and NewEventScreen */
};

/******************************************************************************/
/* returns error, zero is good */
static int
set_bpp(int bpp)
{
  int rv;

  rv = 0;
  g_bpp = bpp;
  if (g_bpp == 8)
  {
    g_Bpp = 1;
    g_Bpp_mask = 0xff;
    g_redBits = 3;
    g_greenBits = 3;
    g_blueBits = 2;
  }
  else if (g_bpp == 16)
  {
    g_Bpp = 2;
    g_Bpp_mask = 0xffff;
    g_redBits = 5;
    g_greenBits = 6;
    g_blueBits = 5;
  }
  else if (g_bpp == 32)
  {
    g_Bpp = 4;
    g_Bpp_mask = 0xffffff;
    g_redBits = 8;
    g_greenBits = 8;
    g_blueBits = 8;
  }
  else
  {
    rv = 1;
  }
  return rv;
}

/******************************************************************************/
static void
rdpWakeupHandler(int i, pointer blockData, unsigned long err,
                 pointer pReadmask)
{
  g_pScreen->WakeupHandler = g_rdpScreen.WakeupHandler;
  g_pScreen->WakeupHandler(i, blockData, err, pReadmask);
  g_pScreen->WakeupHandler = rdpWakeupHandler;
}

/******************************************************************************/
static void
rdpBlockHandler1(pointer blockData, OSTimePtr pTimeout, pointer pReadmask)
{
}

/******************************************************************************/
static void
rdpWakeupHandler1(pointer blockData, int result, pointer pReadmask)
{
  rdpup_check();
}

/******************************************************************************/
/* returns boolean, true if everything is ok */
static Bool
rdpScreenInit(int index, ScreenPtr pScreen, int argc, char** argv)
{
  int dpix;
  int dpiy;
  int ret;
  Bool vis_found;
  VisualPtr vis;
  PictureScreenPtr ps;

  g_pScreen = pScreen;

  /*dpix = 75;
  dpiy = 75;*/
  dpix = 100;
  dpiy = 100;
  if (monitorResolution != 0)
  {
    dpix = monitorResolution;
    dpiy = monitorResolution;
  }
  g_rdpScreen.paddedWidthInBytes = PixmapBytePad(g_rdpScreen.width,
                                                 g_rdpScreen.depth);
  g_rdpScreen.bitsPerPixel = rdpBitsPerPixel(g_rdpScreen.depth);
  ErrorF("\n");
  ErrorF("X11rdp, an X server for xrdp\n");
  ErrorF("Version %s\n", X11RDPVER);
  ErrorF("Copyright (C) 2005-2008 Jay Sorg\n");
  ErrorF("See http://xrdp.sf.net for information on xrdp.\n");
#if defined(XORG_VERSION_CURRENT) && defined (XVENDORNAME)
  ErrorF("Underlying X server release %d, %s\n",
         XORG_VERSION_CURRENT, XVENDORNAME);
#endif
#if defined(XORG_RELEASE)
  ErrorF("Xorg %s\n", XORG_RELEASE);
#endif
  ErrorF("Screen width %d height %d depth %d bpp %d\n", g_rdpScreen.width,
         g_rdpScreen.height, g_rdpScreen.depth, g_rdpScreen.bitsPerPixel);
  ErrorF("dpix %d dpiy %d\n", dpix, dpiy);
  if (g_rdpScreen.pfbMemory == 0)
  {
    g_rdpScreen.sizeInBytes =
           (g_rdpScreen.paddedWidthInBytes * g_rdpScreen.height);
    ErrorF("buffer size %d\n", g_rdpScreen.sizeInBytes);
    g_rdpScreen.pfbMemory = (char*)g_malloc(g_rdpScreen.sizeInBytes, 1);
  }
  if (g_rdpScreen.pfbMemory == 0)
  {
    rdpLog("rdpScreenInit g_malloc failed\n");
    return 0;
  }
  miClearVisualTypes();
  if (defaultColorVisualClass == -1)
  {
    defaultColorVisualClass = TrueColor;
  }
  if (!miSetVisualTypes(g_rdpScreen.depth,
                         miGetDefaultVisualMask(g_rdpScreen.depth),
                         8, defaultColorVisualClass))
  {
    rdpLog("rdpScreenInit miSetVisualTypes failed\n");
    return 0;
  }
  miSetPixmapDepths();
  switch (g_rdpScreen.bitsPerPixel)
  {
    case 8:
      ret = fbScreenInit(pScreen, g_rdpScreen.pfbMemory,
                          g_rdpScreen.width, g_rdpScreen.height,
                          dpix, dpiy, g_rdpScreen.paddedWidthInBytes);
      break;
    case 16:
      ret = fbScreenInit(pScreen, g_rdpScreen.pfbMemory,
                          g_rdpScreen.width, g_rdpScreen.height,
                          dpix, dpiy, g_rdpScreen.paddedWidthInBytes / 2);
      break;
    case 32:
      ret = fbScreenInit(pScreen, g_rdpScreen.pfbMemory,
                          g_rdpScreen.width, g_rdpScreen.height,
                          dpix, dpiy, g_rdpScreen.paddedWidthInBytes / 4);
      break;
    default:
      return 0;
  }
  if (!ret)
  {
    return 0;
  }

  miInitializeBackingStore(pScreen);

  /* this is for rgb, not bgr, just doing rgb for now */
  vis = g_pScreen->visuals + (g_pScreen->numVisuals - 1);
  while (vis >= pScreen->visuals)
  {
    if ((vis->class | DynamicClass) == DirectColor)
    {
      vis->offsetBlue = 0;
      vis->blueMask = (1 << g_blueBits) - 1;
      vis->offsetGreen = g_blueBits;
      vis->greenMask = ((1 << g_greenBits) - 1) << vis->offsetGreen;
      vis->offsetRed = g_blueBits + g_greenBits;
      vis->redMask = ((1 << g_redBits) - 1) << vis->offsetRed;
    }
    vis--;
  }

  if (g_rdpScreen.bitsPerPixel > 4)
  {
    fbPictureInit(pScreen, 0, 0);
  }

  if (!AllocateGCPrivate(pScreen, g_rdpGCIndex, sizeof(rdpGCRec)))
  {
    FatalError("rdpScreenInit: AllocateGCPrivate failed\n");
  }
  /* Random screen procedures */
  g_rdpScreen.CloseScreen = pScreen->CloseScreen;
  /* GC procedures */
  g_rdpScreen.CreateGC = pScreen->CreateGC;
  /* Window Procedures */
  g_rdpScreen.PaintWindowBackground = pScreen->PaintWindowBackground;
  g_rdpScreen.PaintWindowBorder = pScreen->PaintWindowBorder;
  g_rdpScreen.CopyWindow = pScreen->CopyWindow;
  g_rdpScreen.ClearToBackground = pScreen->ClearToBackground;
  /* Backing store procedures */
  g_rdpScreen.RestoreAreas = pScreen->RestoreAreas;
  g_rdpScreen.WakeupHandler = pScreen->WakeupHandler;
  ps = GetPictureScreenIfSet(pScreen);
  if (ps)
  {
    g_rdpScreen.Composite = ps->Composite;
  }
  pScreen->blackPixel = g_rdpScreen.blackPixel;
  pScreen->whitePixel = g_rdpScreen.whitePixel;
  /* Random screen procedures */
  pScreen->CloseScreen = rdpCloseScreen;
  pScreen->WakeupHandler = rdpWakeupHandler;
  if (ps)
  {
    ps->Composite = rdpComposite;
  }
  pScreen->SaveScreen = rdpSaveScreen;
  /* GC procedures */
  pScreen->CreateGC = rdpCreateGC;
  /* Window Procedures */
  pScreen->PaintWindowBackground = rdpPaintWindowBackground;
  pScreen->PaintWindowBorder = rdpPaintWindowBorder;
  pScreen->CopyWindow = rdpCopyWindow;
  pScreen->ClearToBackground = rdpClearToBackground;
  /* Backing store procedures */
  pScreen->RestoreAreas = rdpRestoreAreas;
  miPointerInitialize(pScreen, &g_rdpSpritePointerFuncs,
                      &g_rdpPointerCursorFuncs, 1);
  vis_found = 0;
  vis = g_pScreen->visuals + (g_pScreen->numVisuals - 1);
  while (vis >= pScreen->visuals)
  {
    if (vis->vid == pScreen->rootVisual)
    {
      vis_found = 1;
    }
    vis--;
  }
  if (!vis_found)
  {
    rdpLog("rdpScreenInit: couldn't find root visual\n");
    exit(1);
  }
  if (g_rdpScreen.bitsPerPixel == 1)
  {
    ret = mfbCreateDefColormap(pScreen);
  }
  else
  {
    ret = fbCreateDefColormap(pScreen);
  }
  if (ret)
  {
    ret = rdpup_init();
  }
  if (ret)
  {
    RegisterBlockAndWakeupHandlers(rdpBlockHandler1, rdpWakeupHandler1, NULL);
  }
  return ret;
}

/******************************************************************************/
/* this is the first function called, it can be called many times
   returns the number or parameters processed
   if it dosen't apply to the rdp part, return 0 */
int
ddxProcessArgument(int argc, char** argv, int i)
{
  if (g_firstTime)
  {
    memset(&g_rdpScreen, 0, sizeof(g_rdpScreen));
    g_rdpScreen.width  = 800;
    g_rdpScreen.height = 600;
    g_rdpScreen.depth = 8;
    set_bpp(8);
    g_rdpScreen.blackPixel = 1;
    g_firstTime = 0;
  }
  if (strcmp(argv[i], "-geometry") == 0)
  {
    if (i + 1 >= argc)
    {
      UseMsg();
    }
    if (sscanf(argv[i + 1], "%dx%d", &g_rdpScreen.width,
               &g_rdpScreen.height) != 2)
    {
      ErrorF("Invalid geometry %s\n", argv[i + 1]);
      UseMsg();
    }
    return 2;
  }
  if (strcmp (argv[i], "-depth") == 0)
  {
    if (i + 1 >= argc)
    {
      UseMsg();
    }
    g_rdpScreen.depth = atoi(argv[i + 1]);
    if (set_bpp(g_rdpScreen.depth) != 0)
    {
      UseMsg();
    }
    return 2;
  }
  return 0;
}

/******************************************************************************/
void
OsVendorInit(void)
{
}

/******************************************************************************/
/* ddxInitGlobals - called by |InitGlobals| from os/util.c */
void
ddxInitGlobals(void)
{
}

/******************************************************************************/
int
XkbDDXSwitchScreen(DeviceIntPtr dev, KeyCode key, XkbAction* act)
{
  return 1;
}

/******************************************************************************/
int
XkbDDXPrivate(DeviceIntPtr dev, KeyCode key, XkbAction* act)
{
  return 0;
}

/******************************************************************************/
int
XkbDDXTerminateServer(DeviceIntPtr dev, KeyCode key, XkbAction* act)
{
  GiveUp(1);
  return 0;
}

/******************************************************************************/
/* InitOutput is called every time the server resets.  It should call
   AddScreen for each screen (but we only ever have one), and in turn this
   will call rdpScreenInit. */
void
InitOutput(ScreenInfo* screenInfo, int argc, char** argv)
{
  int i;

  g_initOutputCalled = 1;
  /* initialize pixmap formats */
  screenInfo->imageByteOrder = IMAGE_BYTE_ORDER;
  screenInfo->bitmapScanlineUnit = BITMAP_SCANLINE_UNIT;
  screenInfo->bitmapScanlinePad = BITMAP_SCANLINE_PAD;
  screenInfo->bitmapBitOrder = BITMAP_BIT_ORDER;
  screenInfo->numPixmapFormats = g_numFormats;
  for (i = 0; i < g_numFormats; i++)
  {
    screenInfo->formats[i] = g_formats[i];
  }
  g_rdpGCIndex = AllocateGCPrivateIndex();
  if (g_rdpGCIndex < 0)
  {
    FatalError("InitOutput: AllocateGCPrivateIndex failed\n");
  }
  if (!AddCallback(&ClientStateCallback, rdpClientStateChange, NULL))
  {
    rdpLog("InitOutput: AddCallback failed\n");
    return;
  }
  /* initialize screen */
  if (AddScreen(rdpScreenInit, argc, argv) == -1)
  {
    FatalError("Couldn't add screen\n");
  }
}

/******************************************************************************/
void
InitInput(int argc, char** argv)
{
  DeviceIntPtr p;
  DeviceIntPtr k;

  k = AddInputDevice(rdpKeybdProc, 1);
  p = AddInputDevice(rdpMouseProc, 1);
  RegisterKeyboardDevice(k);
  RegisterPointerDevice(p);
  /* screenInfo must be globally defined */
  miRegisterPointerDevice(screenInfo.screens[0], p);
  mieqInit(k, p);
}

/******************************************************************************/
void
ddxGiveUp(void)
{
  char unixSocketName[32];

  g_free(g_rdpScreen.pfbMemory);
  if (g_initOutputCalled)
  {
    sprintf(unixSocketName, "/tmp/.X11-unix/X%s", display);
    unlink(unixSocketName);
  }
}

/******************************************************************************/
Bool
LegalModifier(unsigned int key, DevicePtr pDev)
{
  return 1; /* true */
}

/******************************************************************************/
void
ProcessInputEvents(void)
{
  mieqProcessInputEvents();
  miPointerUpdate();
}

/******************************************************************************/
/* needed for some reason? todo
   needs to be rfb */
void
rfbRootPropertyChange(PropertyPtr pProp)
{
}

/******************************************************************************/
void
AbortDDX(void)
{
  ddxGiveUp();
}

/******************************************************************************/
void
OsVendorFatalError(void)
{
}

/******************************************************************************/
/* print the command list parameters and exit the program */
void
ddxUseMsg(void)
{
  ErrorF("\n");
  ErrorF("X11rdp specific options\n");
  ErrorF("-geometry WxH          set framebuffer width & height\n");
  ErrorF("-depth D               set framebuffer depth\n");
  ErrorF("\n");
  exit(1);
}

/******************************************************************************/
void
OsVendorPreInit(void)
{
}
