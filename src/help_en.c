/*
 * @(#)help_en.c
 *
 * Copyright 2000-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Michael Ethetton (mailto:methetton@gambitstudios.com)
 * All rights reserved.
 * 
 * This file was generated as part of the "liberty" program developed for 
 * the Palm Computing Platform designed by Palm: http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). Redistribution or modification without 
 * prior consent of the original author(s) is prohibited.
 *
 * NOTE:
 * The following portions of code found within this source code file are
 * owned exclusively by Michael Ethetton, and shall not be used in or
 * sold to other projects (internal or external) without the written
 * permission of Michael Ethetton.
 *
 * - z-80 CPU Emulation
 * - Nintendo Gameboy Emulation System
 *
 * The following portions of code found within this source code file are
 * owned exclusively by Aaron Ardiri, and shall not be used in or 
 * sold to other projects (internal or external) without the written 
 * permission of Aaron Ardiri.
 *
 * - GNU prc-tools 2.0 application framework
 * - Help System 
 * - Device Configuration Module
 * - Multiple Code Segments Solution
 *
 * It shall be noted that Aaron Ardiri has licensed the above source code
 * and framework to Michael Ethetton for use only in the "liberty" project.
 *
 * ------------------------------------------------------------------------
 *                THIS FILE CONTAINS THE ENGLISH LANGUAGE TEXT
 * ------------------------------------------------------------------------
 */

#include "palm.h"

typedef struct 
{
  UInt32    keyMask;
  WinHandle helpWindow;
} HelpGlobals;

#ifdef USE_GLOBALS
// globals object
static HelpGlobals gbls;
static HelpGlobals *globals;
#endif

/**
 * Initialize the instructions screen.
 * 
 * @return the height in pixels of the instructions data area.
 */
UInt16
InitInstructions()
{
  const RectangleType     rect  = {{0,0},{142,427}};
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
  UInt16      err;
  DmOpenRef   resourceRef;
  UInt16      result = 0;
#ifndef USE_GLOBALS
  HelpGlobals *globals;

  // create the globals object, and register it
  globals = (HelpGlobals *)MemPtrNew(sizeof(HelpGlobals));
  FtrSet(appCreator, ftrHelpGlobals, (UInt32)globals);
#else
  globals = &gbls;
#endif

  // setup the valid keys available at this point in time
  globals->keyMask = KeySetMask(~(keyBitsAll ^ 
                                 (keyBitPower   | keyBitCradle   |
                                  keyBitPageUp  | keyBitPageDown |
#ifdef HANDERA
                                  keyBitJogUp   | keyBitJogDown  |
#endif
                                  keyBitAntenna | keyBitContrast)));

  // initialize windows
  globals->helpWindow = 
    WinCreateOffscreenWindow(rect.extent.x,rect.extent.y,screenFormat,&err);
  err |= (globals->helpWindow == NULL);

  // open the application's resource database
  {
    UInt16  card;
    LocalID dbID;

    SysCurAppDatabase(&card, &dbID);
    resourceRef = DmOpenDatabase(card, dbID, dmModeReadOnly);
  }

  // draw the help
  if (err == errNone) 
  {
    FontID    font;
    WinHandle currWindow;

    currWindow = WinGetDrawWindow();
    font       = FntGetFont();

    // draw to help window
    WinSetDrawWindow(globals->helpWindow);
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);

    {
      Char  *str, *ptrStr;
      Coord x, y;

      // initialize
      y   = 2;
      str = (Char *)MemPtrNew(256 * sizeof(Char));

      x = 4;
      StrCopy(str,
"Liberty supports games designed for the original GameBoy\231.");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      //
      // "playing" games
      //

      StrCopy(str, "GameBoy\231 Games");
      FntSetFont(boldFont);
      WinSetUnderlineMode(grayUnderline);
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y); y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);
      FntSetFont(font);

      // add space (little)
      y += FntLineHeight() >> 1;

      x = 4;
      StrCopy(str,
"Select the GameBoy\231 game you wish to play from the list shown.");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      // show the list
      x = 5;
      {
        MemHandle bitmapHandle = DmGet1Resource('Tbmp', bitmapHelpSelectList);
	WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), x, y);
	MemHandleUnlock(bitmapHandle);
	DmReleaseResource(bitmapHandle);
      }

      // add space (little)
      y += 44 + (FntLineHeight() >> 1);

      x = 4;
      StrCopy(str,
"Using the menu commands you can start emulation or remove any game from \
your device.");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      x = 4;
      StrCopy(str,
"Each game can be restored or saved at any time and has its own \
configuration options (see below).");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      //
      // "configuration"
      //

      StrCopy(str, "Configuration");
      FntSetFont(boldFont);
      WinSetUnderlineMode(grayUnderline);
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y); y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);
      FntSetFont(font);

      // add space (little)
      y += FntLineHeight() >> 1;

      x = 4;
      StrCopy(str,
"Define the key assignments as appropriate or use the grafitti drawing \
area as follows:");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      // show the grafitti stuff
      x = 17;
      {
        MemHandle bitmapHandle = DmGet1Resource('Tbmp', bitmapHelpStylusControl);
	WinDrawBitmap((BitmapType *)MemHandleLock(bitmapHandle), x, y);
	MemHandleUnlock(bitmapHandle);
	DmReleaseResource(bitmapHandle);
      }

      // add space (little)
      y += 57 + (FntLineHeight() >> 1);

      x = 4;
      StrCopy(str,
"The frame blit option controls how often a GameBoy\231 game frame is \
copied to the screen.");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      //
      // "display setup"
      //

      StrCopy(str, "Display Setup");
      FntSetFont(boldFont);
      WinSetUnderlineMode(grayUnderline);
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y); y += FntLineHeight();
      WinSetUnderlineMode(noUnderline);
      FntSetFont(font);

      // add space (little)
      y += FntLineHeight() >> 1;

      x = 4;
      StrCopy(str,
"Adjust the display settings to your liking. If you own a color device \
you can even colorize the display! What an eye opener!");
      ptrStr = str;
      while (StrLen(ptrStr) != 0) 
      {
        UInt8 count = FntWordWrap(ptrStr, rect.extent.x-x);

	x = (rect.extent.x - FntCharsWidth(ptrStr, count)) >> 1;
	WinDrawChars(ptrStr, count, x, y); y += FntLineHeight(); x = 4;
	ptrStr += count;
      }

      // add space (little)
      y += FntLineHeight() >> 1;

      StrCopy(str, "ENJOY!");
      FntSetFont(boldFont);
      x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;
      WinDrawChars(str, StrLen(str), x, y); y += FntLineHeight();

      // clean up
      MemPtrFree(str);
    }

    FntSetFont(font);
    WinSetDrawWindow(currWindow);

    result = rect.extent.y - 1;
  }
  else 
  {
    result = 0;
    ApplicationDisplayDialog(xmemForm);
  }

  // close the database we just opened
  DmCloseDatabase(resourceRef);

  return result;
}

/**
 * Draw the instructions on the screen.
 * 
 * @param offset the offset height of the window to start copying from.
 */
void 
DrawInstructions(UInt16 offset)
{
  const RectangleType helpArea = {{0,offset},{142,116}};
#ifndef USE_GLOBALS
  HelpGlobals *globals;

  // get globals reference
  FtrGet(appCreator, ftrHelpGlobals, (UInt32 *)&globals);
#endif

  // blit the required area
  if (globals->helpWindow != NULL) 
    WinCopyRectangle(globals->helpWindow, 
                     WinGetDrawWindow(), &helpArea, 3, 16, winPaint);
}

/**
 * Terminate the instructions screen.
 */
void
QuitInstructions()
{
#ifndef USE_GLOBALS
  HelpGlobals *globals;

  // get globals reference
  FtrGet(appCreator, ftrHelpGlobals, (UInt32 *)&globals);
#endif

  // return the state of the key processing
  KeySetMask(globals->keyMask);

  // clean up memory
  if (globals->helpWindow != NULL) 
    WinDeleteWindow(globals->helpWindow, false);

#ifndef USE_GLOBALS
  MemPtrFree(globals);

  // unregister global data
  FtrUnregister(appCreator, ftrHelpGlobals);
#endif
}
