/*
 * @(#)device.c
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
 */

#include "palm.h"

// globals variable structure
typedef struct
{
  UInt32  romVersion;                      // the rom version of the device
  UInt32  depthState;                      // the screen depth state (old)

#ifdef HANDERA
  struct
  {
    Boolean device;
    VgaScreenModeType scrMode;             // current mode (scale, 1:1 etc)
    VgaRotateModeType scrRotate;           // current display rotation
  } handera;
#endif

#ifdef SONY
  struct
  {
    Boolean device;
    UInt16  libRef;
  } sony;
#endif

} DeviceGlobals;

#ifdef USE_GLOBALS
// globals object
static DeviceGlobals gbls;
static DeviceGlobals *globals;
#endif

static Err DeviceBeamWriteProc(const void *, UInt32 *, void *)      __DEVICE__;

/**
 * Initialize the device.
 */
void
DeviceInitialize()
{
#ifdef SONY
  Err           err;
#endif
#ifdef HANDERA
  UInt32        trgVersion;
#endif
#ifndef USE_GLOBALS
  DeviceGlobals *globals;
 
  // create the globals objects, and register it
  globals = (DeviceGlobals *)MemPtrNew(sizeof(DeviceGlobals));
  FtrSet(appCreator, ftrDeviceGlobals, (UInt32)globals);
#else
  globals = &gbls;
#endif

  // get the rom version for this device
  FtrGet(sysFtrCreator, sysFtrNumROMVersion, &globals->romVersion);

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3)) 
  {
    UInt32 depth = 2;
    UInt32 *depthPtr;

    depthPtr = &depth;

    // save the current display state
    WinScreenMode(winScreenModeGet,NULL,NULL,&globals->depthState,NULL);

    // change into 2bpp display mode
    WinScreenMode(winScreenModeSet,NULL,NULL,depthPtr,NULL);

#ifdef HANDERA
    globals->handera.device =
      (FtrGet(TRGSysFtrID, TRGVgaFtrNum, &trgVersion) != ftrErrNoSuchFeature);
    if (globals->handera.device)
    {
      VgaGetScreenMode(&globals->handera.scrMode,
                       &globals->handera.scrRotate);
      VgaSetScreenMode(screenModeScaleToFit, rotateModeNone);
    }
#endif

#ifdef SONY
    err = SysLibFind(sonySysLibNameHR, &globals->sony.libRef);
    if (err != errNone)
      err = SysLibLoad('libr', sonySysFileCHRLib, &globals->sony.libRef);
    globals->sony.device = (err == errNone);

    // FORCE SONY INTO 160x160 scale
    if (globals->sony.device)
    {
      UInt32 width, height;

      width  = 160;
      height = 160;

      // try to open the hires library
      err = HROpen(globals->sony.libRef);
      if (!err)

        // change into 160x160!
        HRWinScreenMode(globals->sony.libRef, winScreenModeSet,
                        &width, &height, depthPtr, NULL);
    } 
#endif
  }
}

/**
 * Check the compatability status of the device we are working with.
 *
 * @return true if the device is supported, false otherwise.
 */
Boolean 
DeviceCheckCompatability()
{
  Boolean result = true;

  // the device is only compatable if rom 3.0 or higher is available
  result = (
            DeviceSupportsVersion(romVersion3) &&
            ((DeviceGetSupportedDepths() & 0x02) != 0)
           );

  // not compatable :(
  if (!result) 
  { 
    // display a incompatability dialog
    FormPtr form = FrmInitForm(deviForm);
    FrmDoDialog(form);
    FrmDeleteForm(form);

    // lets exit the application "gracefully" :>
    if (!DeviceSupportsVersion(romVersion2)) 
      AppLaunchWithCommand(sysFileCDefaultApp,sysAppLaunchCmdNormalLaunch,NULL);
  }

  return result;
}

/**
 * Determine if the "DeviceGrayscale" routine is supported on the device.
 *
 * @return true if supported, false otherwise.
 */
Boolean
DeviceSupportsGrayscale() 
{
  Boolean result = false;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3)) 
  {
    UInt32 cpuID;

    // get the processor ID
    FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
    cpuID = cpuID & sysFtrNumProcessorMask;

    // the "rules" for grayscale support
    result = (
              (cpuID == sysFtrNumProcessor328) ||
              (cpuID == sysFtrNumProcessorEZ)  ||
	      (cpuID == sysFtrNumProcessorVZ)  ||
	      (cpuID == sysFtrNumProcessorSuperVZ)
             ) &&
             (DeviceGetSupportedDepths() < 0x80); // if 8bpp, color device :)
  }

  return result;
}

/**
 * Grayscale routine/settings for the device.
 *
 * @param mode the desired mode of operation.
 * @param lgray the lGray index (0..6) in intensity
 * @param dgray the dGray index (0..6) in intensity
 */
void
DeviceGrayscale(UInt16 mode, 
                UInt8  *lgray, 
                UInt8  *dgray)
{
  UInt32 cpuID;

#define LGPMR1 ((unsigned char *)0xFFFFFA32)
#define LGPMR2 ((unsigned char *)0xFFFFFA33)

  // get the processor ID
  FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
  switch (cpuID & sysFtrNumProcessorMask)
  {
    case sysFtrNumProcessor328:
         {
           UInt8 gray[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07 };

           switch (mode)
           {
             case grayGet:
                  {
                    UInt8 data;

                    // light gray
                    data = (UInt8)(((*LGPMR1) & 0xF0) >> 4);
                    *lgray = 0;
                    while (gray[*lgray] < data) 
                      (*lgray)++;

                    // dark gray
                    data = (UInt8)((*LGPMR2) & 0x0F);
                    *dgray = 0;
                    while (gray[*dgray] < data) 
                      (*dgray)++;
                  }
                  break;

             case graySet:
                  *LGPMR1 = (gray[*lgray] << 4);
                  *LGPMR2 = (0x70 | gray[*dgray]);
                  break;

             default:
                  break;
           }
         }
         break;

    case sysFtrNumProcessorEZ:
    case sysFtrNumProcessorVZ:
    case sysFtrNumProcessorSuperVZ:
         {
           UInt8 gray[] = { 0x00, 0x03, 0x04, 0x07, 0x0A, 0x0C, 0x0F };

           switch (mode)
           {
             case grayGet:
                  {
                    UInt8 data;

                    // light gray
                    data = (UInt8)((*LGPMR2) & 0x0F);
                    *lgray = 0;
                    while (gray[*lgray] < data)
                      (*lgray)++;

                    // dark gray
                    data = (UInt8)(((*LGPMR2) & 0xF0) >> 4);
                    *dgray = 0;
                    while (gray[*dgray] < data)
                      (*dgray)++;
                  }
                  break;

             case graySet:
                  *LGPMR2 = ((gray[*dgray] << 4) | gray[*lgray]);
                  break;

             default:
                  break;
           }
         }
         break;

    default:
         break;
  }
}

/**
 * Determine if the "DeviceGrayPalette" routine is supported on the device.
 *
 * @return true if supported, false otherwise.
 */
Boolean
DeviceSupportsGrayPalette() 
{
  return (DeviceSupportsVersion(romVersion3_5));
}

/**
 * Gray color palette routine/settings for the device.
 *
 * @param mode the desired mode of operation.
 * @param lgray the lGray RGB index (0..8) in intensity
 * @param dgray the dGray RGB index (0..8) in intensity
 */
void
DeviceGrayPalette(UInt16 mode, 
                  UInt8  *lgray, 
                  UInt8  *dgray)
{
  RGBColorType palette[]      = {
                                  { 0, 0xff, 0xff, 0xff },
                                  { 1, 0xc0, 0xc0, 0xc0 },
                                  { 2, 0x80, 0x80, 0x80 },
                                  { 3, 0x00, 0x00, 0x00 }
                                };
  UInt8        colorMapping[] = {
                                  0x00, 0x20, 0x40, 
                                  0x60, 0x80, 0xA0,
                                  0xC0, 0xE0, 0xff 
                                };

  switch (mode) 
  {
    case palGet:
         WinPalette(winPaletteGet, 0, 4, palette);
	 {
           // light gray
           lgray[0] = lgray[1] = lgray[2] = 0;
	   while (colorMapping[lgray[0]] < palette[1].r) { lgray[0]++; }
	   while (colorMapping[lgray[1]] < palette[1].g) { lgray[1]++; }
	   while (colorMapping[lgray[2]] < palette[1].b) { lgray[2]++; }

           // dark gray
           dgray[0] = dgray[1] = dgray[2] = 0;
	   while (colorMapping[dgray[0]] < palette[2].r) { dgray[0]++; }
	   while (colorMapping[dgray[1]] < palette[2].g) { dgray[1]++; }
	   while (colorMapping[dgray[2]] < palette[2].b) { dgray[2]++; }
         }
         break;

    case palSet:
	 {
           // light gray
	   palette[1].r = colorMapping[lgray[0]];
	   palette[1].g = colorMapping[lgray[1]];
	   palette[1].b = colorMapping[lgray[2]];

           // dark gray
	   palette[2].r = colorMapping[dgray[0]];
	   palette[2].g = colorMapping[dgray[1]];
	   palette[2].b = colorMapping[dgray[2]];
	 }
         WinPalette(winPaletteSet, 0, 4, palette);
         break;

    case palReset:
         WinPalette(winPaletteSetToDefault, 0, 4, palette);
         break;

    default:
         break;
  }
}

/**
 * Get the supported depths the device can handle. 
 *
 * @return the depths supported (1011b = 2^3 | 2^1 | 2^0 = 4,2,1 bpp).
 */
UInt32  
DeviceGetSupportedDepths()
{
  UInt32 result = 0x00000001;

  // only OS 3.0 and above have > 1bpp display via API's
  if (DeviceSupportsVersion(romVersion3)) 
    WinScreenMode(winScreenModeGetSupportedDepths,NULL,NULL,&result,NULL);

  return result;
}

/**
 * Check if the device is compatable with a particular ROM version.
 *
 * @param version the ROM version to compare against.
 * @return true if it is compatable, false otherwise.
 */
Boolean 
DeviceSupportsVersion(UInt32 version)
{
#ifndef USE_GLOBALS
  DeviceGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *)&globals);
#endif

  return (globals->romVersion >= version);
}

/**
 * Determine the pointer to the bitmap data chunk for a specific window.
 *
 * @param win the window.
 * @return a pointer to the bitmap data chunk.
 */
void *
DeviceWindowGetPointer(WinHandle win)
{
  void *result = NULL;

  // palmos 3.5        - use BmpGetBits()
  if (DeviceSupportsVersion(romVersion3_5)) 
    result = BmpGetBits(WinGetBitmap(win));

  // palmos pre 3.5    - use standard technique
  else
    result = (void *)win->displayAddrV20;
    
  return result;
}

/**
 * Reset the device to its original state.
 */
void
DeviceTerminate()
{
#ifndef USE_GLOBALS
  DeviceGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrDeviceGlobals, (UInt32 *)&globals);
#endif

  // restore the current display state
  if (DeviceSupportsVersion(romVersion3)) 
    WinScreenMode(winScreenModeSet,NULL,NULL,&globals->depthState,NULL);

#ifdef HANDERA
  if (globals->handera.device)
  {
    VgaSetScreenMode(globals->handera.scrMode,
                     globals->handera.scrRotate);
  }
#endif

#ifdef SONY
  if (globals->sony.device)
  {
    HRWinScreenMode(globals->sony.libRef,
                    winScreenModeSetToDefaults, NULL, NULL, NULL, NULL);

    // close the hires library
    HRClose(globals->sony.libRef);
  }
#endif

#ifndef USE_GLOBALS
  // clean up
  MemPtrFree(globals);

  // unregister global data
  FtrUnregister(appCreator, ftrDeviceGlobals);
#endif
}

/**
 * Send a database using the Exchange API (Beam).
 *
 * @param romName the rom file to beam.
 * @return true if successful, false otherwise.
 */
Boolean
DeviceBeamDatabase(Char *romName)
{
  Char    dbName[32];
  UInt16  card;
  LocalID dbID;
  Boolean result = true;

  // initialize
  StrPrintF(dbName, "Gmbt_%s", romName);
  card = 0;
  dbID = DmFindDatabase(card, dbName);
  if (dbID == NULL) 
  {
    card = 1;
    dbID = DmFindDatabase(card, dbName);
  }

  // send over the database
  if (dbID != NULL) 
  {
    ExgSocketType socket;
    Err           err, e;
    Char          strName[32];
    UInt16        i;

    // we need to add the ".pdb" extension (so receiver knows what to do)
    StrPrintF(strName, "%s.pdb", dbName);
    for (i=0;i<StrLen(strName)-4;i++) 
      if (strName[i] == '.') strName[i] = '_';  // lets not confuse ExgMgr!

    // configure
    MemSet(&socket, sizeof(ExgSocketType), 0);
    socket.target      = sysFileCLauncher;
    socket.description = romName;
    socket.name        = strName;

    err = errNone;
    e = ExgPut(&socket); err |= e;
    if (err == errNone)
    {
      e = ExgDBWrite(DeviceBeamWriteProc, &socket, NULL, dbID, card); err |= e;
      e = ExgDisconnect(&socket, err); err |= e;
    }

    // did everything go ok?
    result = (err == errNone);
  }
  else 
    result = false;

  return result;
}

/**
 * ExgDBWrite callback function.
 *
 * @param data the data to send.
 * @param size the size of the packet.
 * @param userData the protocol.
 * @return errNone if successful, other otherwise.
 */
static Err
DeviceBeamWriteProc(const void *data,
                    UInt32     *size,
                    void       *userData)
{
  Err err = errNone;

  // send it over!
  *size = ExgSend((ExgSocketType *)userData, (MemPtr)data, *size, &err);

  return err;
}
