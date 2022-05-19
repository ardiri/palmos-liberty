/*
 * @(#)game.c
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

// emulation state structure (used for saving state)
typedef struct
{
  struct  
  {
    UInt16     regBC;              // BC register
    UInt16     regDE;              // DE register
    UInt16     regHL;              // HL register
    UInt8      regA;               // accumulator
    UInt8      regF;               // flags
    UInt16     regSP;              // stack pointer
    UInt16     regPC;              // program counter
    UInt8      regInt;             // interrupt flag
    UInt8      regTimer;           // z80 timer
    UInt8      regSerialTimer;     // z80 serial timer

    UInt8      regA_old;           // old accumulator
  } z80;

  UInt8        xBitOff;          
  UInt8        xByteOff;
  UInt8        wxBitOff;
  UInt8        wxByteOff;
  UInt8        xOffset;            // screen/window adjustment variables
  UInt8        oldBgPallete;       // temp palette variable

  UInt8        keyState;           // the key state (button press)
  UInt8        OldKeyState;        // previous key state value (for Joypad int)

  UInt8        cfgSoundMute;       // the sound is *NOT active*
  UInt8        cfgSoundVolume;     // the sound volume
  UInt8        cfgSoundChan1;      // channel 1 is active
  UInt8        cfgSoundChan2;      // channel 2 is active

  UInt16       currRomIndex;       // the current rom page index
} EmuStateType;

// global variable structure
typedef struct
{
  UInt32       keyMask;
  DmOpenRef    dbGameRomRef;

  EmuStateType emuState;           // stored state information

  struct 
  {
    UInt8      *ptrCurrentLine;    // screen buffer pointer

    UInt8      *ptrHLConv;         // the HLConversion table
    UInt8      *ptrBGPalette;      // the background palette table
    UInt8      *ptrSpritePalette0; // the sprite 0 palette table
    UInt8      *ptrSpritePalette1; // the sprite 1 palette table

    UInt8      *ptrLCDScreen;      // pointer to the REAL LCD screen
    UInt8      frameBlitConst;     // the frame blit constant
    UInt8      frameBlit;          // the frame blit value (tmp)

    UInt8      *ptrScreen;         // offscreen buffer resource
    UInt16     screenOffset;       // the LCD blitting offset
    UInt16     screenAdjustment;   // how many bytes to add to wrap to new line

    UInt8      *ptr32KRam;         // the 32K ram chunk
    UInt8      *ptrTileTable;      // a 4K tile reference table 
    UInt8      *ptrBGTileRam;      // the background tile ram (6K)

    UInt16     pageCount;
    UInt8      **ptrPages;         // references to the ROM pages
    UInt8      *ptrCurrRom;        // the current "rom" reference

    UInt8      sndChannel1[16];    // misc. sound storage channel 1
    UInt8      sndChannel2[16];    // misc. sound storage channel 2

    Boolean    skipCPUCycles;      // should we skip cpu cycles? (speed up on some games)
    Boolean    _padding;           

#ifdef RUMBLEPAK
    UInt8      *ptrVibrateOn;      // rumble pak: vibrate on
    UInt8      *ptrVibrateOff;     // rumble pak: vibrate off
#endif

#ifdef PALM_HIRES
    UInt8      m68k_device;        // 68k device?
    UInt8      hires320x320;       // 320x320 unit?
    UInt8      use_api;            // require api_blitting?
    UInt8      _padding2;
    WinHandle  winOffscreen;       // offscreen window stuff for API work
    WinHandle  winDisplay;
    UInt8      *ptrXlat;           // 2bpp -> pixel doubling table
#endif    

    // 
    // not "used" in game emulation state (ignore all below)
    //

#ifdef PALM_HIRES
    BitmapType *bmpOffscreen;    
    MemHandle  hanXlat;            // 2bpp -> pixel double table handle
#endif    
    UInt16     bmpBitsSize;
    
    MemHandle  hanHLConv;          // the HLConversion table handle

    UInt8      *stdBGPalette;      // the default background palette table
    UInt8      *stdSpritePalette0; // the default sprite 0 palette table
    UInt8      *stdSpritePalette1; // the default sprite 1 palette table

    MemHandle  han32KBank;         // the first 32K of a ROM (copy)
  } emu;

  struct 
  {
#ifdef GAMEPAD
    Boolean    gamePadPresent;     // is the gamepad driver present
    UInt16     gamePadLibRef;      // library reference for gamepad
#endif
  } hardware;

} GameGlobals;

#ifdef USE_GLOBALS
// globals object
static GameGlobals gbls;
static GameGlobals *globals;
#endif

static void   GameEncodingInflate(UInt8 *, UInt16, UInt8 *, UInt16)  __GAME__;
static void   GameEncodingDeflate(UInt8 *, UInt16, UInt8 *, UInt16)  __GAME__;
static UInt16 GameEncodingInflateLength(UInt8 *, UInt16)             __GAME__;
static UInt16 GameEncodingDeflateLength(UInt8 *, UInt16)             __GAME__;

/**
 * Initialize the Game.
 *
 * @param prefs the global preference data.
 * @return 0 if successful, an error code otherwise
 */  
UInt16   
GameInitialize(PreferencesType *prefs)
{
  UInt16      result = 0;
  Boolean     memOk  = true;
  Boolean     romOk  = true;
#ifdef PALM_HIRES
  UInt32      winVersion;
  UInt32      cpuID;
#endif  
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // create the "globals" reference, and register it
  globals = (GameGlobals *)MemPtrNew(sizeof(GameGlobals));
  FtrSet(appCreator, ftrGameGlobals, (UInt32)globals);
#else
  globals = &gbls;
#endif
  MemSet(globals, sizeof(GameGlobals), 0);

  // load the gamepad driver if available
#ifdef GAMEPAD
  {
    Err err;

    // attempt to load the library
    err = SysLibFind(GPD_LIB_NAME,&globals->hardware.gamePadLibRef);
    if (err == sysErrLibNotFound)
      err = SysLibLoad('libr',GPD_LIB_CREATOR,&globals->hardware.gamePadLibRef);

    // lets determine if it is available
    globals->hardware.gamePadPresent = (err == errNone);

    // open the library if available
    if (globals->hardware.gamePadPresent)
      GPDOpen(globals->hardware.gamePadLibRef);
  }
#endif

  // configure the "screen blitting" constants
#ifdef HANDERA
  {
    UInt32 version;

    // lets check for the handera device (VGA extensions must exist)
    if (FtrGet(TRGSysFtrID,TRGVgaFtrNum,&version) != ftrErrNoSuchFeature) 
    {
      globals->emu.screenOffset     = ((56*240) + 40) >> 2;  // @ 40,56
      globals->emu.screenAdjustment = 20;
    }

    // normal device :)
    else {
      globals->emu.screenOffset     = ((16*160) +  0) >> 2;  // @  0,16
      globals->emu.screenAdjustment = 0;
    }
  }
#else
  globals->emu.screenOffset       = ((16*160) +  0) >> 2;    // @  0,16
  globals->emu.screenAdjustment   = 0;
#endif

  // setup the valid keys available at this point in time
  globals->keyMask = KeySetMask(~(keyBitsAll ^
                                (keyBitPower   | keyBitCradle | 
				 keyBitAntenna | keyBitContrast)));

  // screen buffer
#ifdef PALM_HIRES
  // lets check if we are running on a "m68k chip" 
  FtrGet(sysFtrCreator, sysFtrNumProcessorID, &cpuID);
  globals->emu.m68k_device = ((cpuID == sysFtrNumProcessor328) ||
                              (cpuID == sysFtrNumProcessorEZ)  ||
                              (cpuID == sysFtrNumProcessorVZ)  ||
                              (cpuID == sysFtrNumProcessorSuperVZ));
    
  // palmHDD?
  FtrGet(sysFtrCreator, sysFtrNumWinVersion, &winVersion);
  if (winVersion >= 4)
  {
    UInt32 width, height;
    
    // get the current display information
    WinScreenGetAttribute(winScreenWidth,  &width);
    WinScreenGetAttribute(winScreenHeight, &height);
  	
    // make note of hires320x320 if needed
    globals->emu.hires320x320 = (width == 320);
    if (globals->emu.hires320x320)
    {
      globals->emu.screenOffset     = ((32*320) + 0) >> 2;  // @ 0,32
      globals->emu.screenAdjustment = 40;
    }
  }
    
  if ((winVersion >= 4) && (!globals->emu.m68k_device))
  {
    Err err;    

    globals->emu.use_api       = true;
    globals->emu.winDisplay    = WinGetDisplayWindow();
    globals->emu.bmpOffscreen  = BmpCreate(176, 176, 2, NULL, &err);
    if (err == errNone)
      globals->emu.winOffscreen = WinCreateBitmapWindow(globals->emu.bmpOffscreen, &err);  	
    if (err == errNone)
      globals->emu.ptrScreen = BmpGetBits(globals->emu.bmpOffscreen);
    memOk &= (globals->emu.ptrScreen         != NULL);
  }
  else  
#endif  
  {  	
    globals->emu.ptrScreen         = (UInt8 *)MemPtrNew(7744  * sizeof(UInt8));
    memOk &= (globals->emu.ptrScreen         != NULL);
  }
  globals->emu.bmpBitsSize       = 7744;  // <-- force this
  globals->emu.ptr32KRam         = (UInt8 *)MemPtrNew(32768 * sizeof(UInt8));
  memOk &= (globals->emu.ptr32KRam         != NULL);
  globals->emu.ptrTileTable      = (UInt8 *)MemPtrNew(4096 * sizeof(UInt8));
  memOk &= (globals->emu.ptrTileTable      != NULL);
  globals->emu.ptrBGTileRam      = (UInt8 *)MemPtrNew(6144 * sizeof(UInt8));
  memOk &= (globals->emu.ptrBGTileRam      != NULL);
  globals->emu.ptrBGPalette      = (UInt8 *)MemPtrNew(256   * sizeof(UInt8));
  memOk &= (globals->emu.ptrBGPalette      != NULL);
  globals->emu.ptrSpritePalette0 = (UInt8 *)MemPtrNew(256   * sizeof(UInt8));
  memOk &= (globals->emu.ptrSpritePalette0 != NULL);
  globals->emu.ptrSpritePalette1 = (UInt8 *)MemPtrNew(256   * sizeof(UInt8));
  memOk &= (globals->emu.ptrSpritePalette1 != NULL);

  // load and lock the any binary resources into memory
  globals->emu.hanHLConv = DmGet1Resource(binType, binaryHLConversion);
  globals->emu.ptrHLConv = (UInt8 *)MemHandleLock(globals->emu.hanHLConv);
  globals->emu.hanXlat   = DmGet1Resource(binType, binaryXlat);
  globals->emu.ptrXlat   = (UInt8 *)MemHandleLock(globals->emu.hanXlat);

  // load the "default" palettes into dynamic memory
  {
    MemHandle binHandle;
    UInt16    binSize;

    binHandle = DmGet1Resource(binType, binaryBackgroundPalette);
    binSize   = MemHandleSize(binHandle);
    globals->emu.stdBGPalette      = (UInt8 *)MemPtrNew(binSize*sizeof(UInt8));
    if (globals->emu.stdBGPalette != NULL) 
      MemMove(globals->emu.stdBGPalette, 
              (UInt8 *)MemHandleLock(binHandle), binSize);
    else memOk &= false;
    MemHandleUnlock(binHandle);
    DmReleaseResource(binHandle);
 
    binHandle = DmGet1Resource(binType, binarySpritePalette0);
    binSize   = MemHandleSize(binHandle);
    globals->emu.stdSpritePalette0 = (UInt8 *)MemPtrNew(binSize*sizeof(UInt8));
    if (globals->emu.stdSpritePalette0 != NULL) 
      MemMove(globals->emu.stdSpritePalette0, 
              (UInt8 *)MemHandleLock(binHandle), binSize);
    else memOk &= false;
    MemHandleUnlock(binHandle);
    DmReleaseResource(binHandle);

    binHandle = DmGet1Resource(binType, binarySpritePalette1);
    binSize   = MemHandleSize(binHandle);
    globals->emu.stdSpritePalette1 = (UInt8 *)MemPtrNew(binSize*sizeof(UInt8));
    if (globals->emu.stdSpritePalette1 != NULL) 
      MemMove(globals->emu.stdSpritePalette1, 
              (UInt8 *)MemHandleLock(binHandle), binSize);
    else memOk &= false;
    MemHandleUnlock(binHandle);
    DmReleaseResource(binHandle);
  }

  // load and lock the rom resources into memory
  {
    UInt16  card;
    LocalID dbID;
      
    // open the database
    card = 0;
    dbID = DmFindDatabase(card, prefs->game.strGBRomName);

    // what? not there.. must be on card 1
    if (dbID == NULL) 
    {
      card = 1;
      dbID = DmFindDatabase(card, prefs->game.strGBRomName);
    }
    globals->dbGameRomRef = DmOpenDatabase(card, dbID, dmModeReadOnly);

    // the 16K rom chunks for this rom
    globals->emu.pageCount = DmNumResources(globals->dbGameRomRef);
    globals->emu.ptrPages = 
      (UInt8 **)MemPtrNew(globals->emu.pageCount * sizeof(UInt8 *));
    if (globals->emu.ptrPages != NULL) 
    {
      // double check the "rom" image
      romOk = ((globals->emu.pageCount > 1) &&
               (MemHandleSize(DmGetResource(datType,0)) == 16384));

      // it was not ok? bail out
      if (!romOk) 
      {
        // clean up
	MemPtrFree(globals->emu.ptrPages);
	globals->emu.ptrPages = NULL;
      }

      // alright!!! valid rom
      else 
      {
        globals->emu.ptrPages[0] = 
          (UInt8 *)MemHandleLock(DmGetResource(datType,0));
        globals->emu.ptrPages[1] = 
          (UInt8 *)MemHandleLock(DmGetResource(datType,1));

        // load and lock the "remainding" rom chunks :))
        {
          GameAdjustmentType adjustType;

          // define the "adjustment"
          adjustType.adjustMode = gameLoadROM;
          adjustType.data.loadROM.pageCount = globals->emu.pageCount;
          adjustType.data.loadROM.ptrPages  = globals->emu.ptrPages;

          // do it! :)) - this will help our "hackers"
          RegisterAdjustGame(prefs, &adjustType);
        }
      }

      globals->emu.han32KBank = DmNewHandle(globals->dbGameRomRef, 32768);
      if (globals->emu.han32KBank != NULL) 
      {
        UInt8 *ptr = MemHandleLock(globals->emu.han32KBank);

        DmWrite(ptr,     0, globals->emu.ptrPages[0], 16384);
        DmWrite(ptr, 16384, globals->emu.ptrPages[1], 16384);

        MemPtrUnlock(globals->emu.ptrPages[0]);
        MemPtrUnlock(globals->emu.ptrPages[1]);  // unlock

	globals->emu.ptrPages[0] = ptr;
	globals->emu.ptrPages[1] = ptr + 16384;  // re-define
      }
    }
    else
      memOk &= false;
  }

  // lets flag our progress...
  result += (memOk ? 0 : 1);
  result += (romOk ? 0 : 2);

  // configure the rumble pak vibration pointers
#ifdef RUMBLEPAK
  if ((result == 0) &&                                // no problems
      ((*(globals->emu.ptrPages[0]+0x0147) == 0x1C) ||
       (*(globals->emu.ptrPages[0]+0x0147) == 0x1D) ||
       (*(globals->emu.ptrPages[0]+0x0147) == 0x1E))) // rumble pack available?
  {
    UInt32 version;
    UInt16 card;

    // lets assume there is no rumble pack module
    globals->emu.ptrVibrateOn  = NULL;
    globals->emu.ptrVibrateOff = NULL;

    // are we dealing with a Handspring device?
    if (FtrGet(hsFtrCreator,hsFtrIDVersion,&version) != ftrErrNoSuchFeature) 
    {
      // is there a spring board loaded?
      if (MemNumCards() > 1) 
      {
        Boolean supported;
        UInt8   cardName[32] = { };
        UInt8   *romID;
	UInt16  romIDlength;

        // lets assume, no support
        supported = false;

        //
        // MEMORY ACCESS MODULE
        //

	card = 1;
        if (SysGetROMToken(card,'RSid',&romID,&romIDlength) == errNone) 
        {
          // memory access module?
          supported |= (StrNCompare("MemA", romID, 4) == 0);
        }

        //
        // INNOPAK/2V
        //

	card = 1;
        if (MemCardInfo(card,cardName,NULL,NULL,NULL,NULL,NULL,NULL) == errNone)
        {
          // innopak/2V?
          supported |= (StrCompare(cardName, "IG_Barbie_1") == 0);
        }

        // we found a compatible device :) yay :P
        if (supported) 
        {
          UInt32 csBase, csSize;
          Err    err, e;
            
	  // lets get a reference to the hardware address
          err = HsCardAttrGet(card,hsCardAttrCsBase,&csBase); e  = err;
          err = HsCardAttrGet(card,hsCardAttrCsSize,&csSize); e |= err;

          // lets rock and roll :))
	  if (e == errNone) 
          {
            // get a reference to the hardware
	    UInt8 *ptrHardware = (UInt8 *)(csBase + csSize);

            // define our pointers :)
            globals->emu.ptrVibrateOn  = ptrHardware + 0x06;
            globals->emu.ptrVibrateOff = ptrHardware + 0x04;
	  }
	}
      }
    }
  }
#else
  globals->emu.ptrVibrateOn  = NULL;
  globals->emu.ptrVibrateOff = NULL;
#endif

  // has everything turned out as planned?
  if (result == 0) 
  {
    // new game, or no saved game?
    if (
        (!prefs->game.gamePlaying) ||
        (DmFindDatabase(0, "Gmbt_!SAVED_GAME!") == 0)
       ) 
      GameReset();

    // restored game - load everything back :)) 
    else 
    {
      LocalID   dbID;
      UInt16    card;
      DmOpenRef dbRef;
      MemHandle binHandle;
      UInt16    index, binSize;
      Boolean   loadOk;

      // open it for reading
      card  = 0;
      dbID  = DmFindDatabase(card, "Gmbt_!SAVED_GAME!");
      dbRef = DmOpenDatabase(card, dbID, dmModeReadOnly);

      // lets make sure the "save state is ok"
      loadOk = (DmNumRecords(dbRef) == 8);                // we save 8 things
      if (!loadOk) goto LOADFAIL;

      // load the "emulation state"
      index = 0;
      binSize = sizeof(EmuStateType);
      binHandle = DmGetRecord(dbRef, index);
      if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
      {
        MemMove((MemPtr)&(globals->emuState),
                (MemPtr)MemHandleLock(binHandle), binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

      // load the screen memory
      index++;
      binSize   = globals->emu.bmpBitsSize;
      binHandle = DmGetRecord(dbRef, index);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
        if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
        {
          // extract it
          GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrScreen, binSize);
        }
        else loadOk &= false;
  
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;
  
      // load the "32K" ram
      index++;
      binSize = MemPtrSize(globals->emu.ptr32KRam);
      binHandle = DmGetRecord(dbRef, index);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
        if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
        {
          // extract it
          GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptr32KRam, binSize);
        }
        else loadOk &= false;
  
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

      // load the Tile Table 
      index++;
      binSize = MemPtrSize(globals->emu.ptrTileTable);
      binHandle = DmGetRecord(dbRef, index);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
        if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
        {
          // extract it
          GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrTileTable, binSize);
        }
        else loadOk &= false;
  
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

      // load the BG Tile ram
      index++;
      binSize = MemPtrSize(globals->emu.ptrBGTileRam);
      binHandle = DmGetRecord(dbRef, index);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
        if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
        {
          // extract it
          GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrBGTileRam, binSize);
        }
        else loadOk &= false;
  
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

      // load the background palette
      index++;
      binSize = MemPtrSize(globals->emu.ptrBGPalette);
      binHandle = DmGetRecord(dbRef, index);
      if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
      {
        MemMove((MemPtr)globals->emu.ptrBGPalette,
                (MemPtr)MemHandleLock(binHandle), binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

      // load sprite palette #0
      index++;
      binSize = MemPtrSize(globals->emu.ptrSpritePalette0);
      binHandle = DmGetRecord(dbRef, index);
      if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
      {
        MemMove((MemPtr)globals->emu.ptrSpritePalette0,
                (MemPtr)MemHandleLock(binHandle), binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;
  
      // load sprite palette #1
      index++;
      binSize = MemPtrSize(globals->emu.ptrSpritePalette1);
      binHandle = DmGetRecord(dbRef, index);
      if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
      {
        MemMove((MemPtr)globals->emu.ptrSpritePalette1,
                (MemPtr)MemHandleLock(binHandle), binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, false);
      }
      else loadOk &= false;

LOADFAIL:

      // close database
      DmCloseDatabase(dbRef);
      
      // did something go wrong? if so, reset
      if (!loadOk) 
      {
        FrmAlert(loadErrorAlert);

        DmDeleteDatabase(card, dbID);              // remove saved game
        GameReset();
      }
    }

    // adjust the "current" rom pointer
    globals->emu.ptrCurrRom = 
      globals->emu.ptrPages[globals->emuState.currRomIndex];
  }

  return result;
}

/**
 * Reset the emulator.
 */  
void
GameReset()
{
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *)&globals);
#endif

  // reset the z80 cpu registers and bits :)
  globals->emuState.z80.regBC          = 0x0013; // VGB sets to 0
  globals->emuState.z80.regDE          = 0x00d8; // VGB sets to 0
  globals->emuState.z80.regHL          = 0x014d; // VGB sets to 0
  globals->emuState.z80.regA           = 0x01;   // GBC sets to 0x11
                                                 // SGB sets to 0x01
                                                 // GBP sets to 0xff
  globals->emuState.z80.regF           = 0xb0;
  globals->emuState.z80.regSP          = 0xfffe;
  globals->emuState.z80.regPC          = 0x0100;
  globals->emuState.z80.regInt         = 0xff;
  globals->emuState.z80.regTimer       = 0x00;
  globals->emuState.z80.regSerialTimer = 0x00;

  globals->emuState.xBitOff            = 0x10;        
  globals->emuState.xByteOff           = 0x00;
  globals->emuState.wxBitOff           = 0x00;
  globals->emuState.wxByteOff          = 0x00;
  globals->emuState.xOffset            = 0x00;
  globals->emuState.oldBgPallete       = 0x00;

  globals->emu.frameBlit               = 0x00;

  // reset back to the "first" rom page
  globals->emuState.currRomIndex       = 0x01;
  globals->emu.ptrCurrRom = 
    globals->emu.ptrPages[globals->emuState.currRomIndex];

  // reset the "palettes" - copy from the default resources
  MemMove(globals->emu.ptrBGPalette,
          globals->emu.stdBGPalette,
          MemPtrSize(globals->emu.stdBGPalette));

  MemMove(globals->emu.ptrSpritePalette0,
          globals->emu.stdSpritePalette0,
          MemPtrSize(globals->emu.stdSpritePalette0));

  MemMove(globals->emu.ptrSpritePalette1,
          globals->emu.stdSpritePalette1,
          MemPtrSize(globals->emu.stdSpritePalette1));

  // reset the screen memory 
  MemSet(globals->emu.ptrScreen, globals->emu.bmpBitsSize, 0);

  // reset the ram 
  MemSet(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam), 0);
  globals->emu.ptr32KRam[0x7f00] = 0xcf;
  globals->emu.ptr32KRam[0x7f10] = 0x80;
  globals->emu.ptr32KRam[0x7f11] = 0xbf;
  globals->emu.ptr32KRam[0x7f12] = 0xf3;
  globals->emu.ptr32KRam[0x7f14] = 0xbf;
  globals->emu.ptr32KRam[0x7f16] = 0x3f;
  globals->emu.ptr32KRam[0x7f19] = 0xbf;
  globals->emu.ptr32KRam[0x7f1a] = 0x7f;
  globals->emu.ptr32KRam[0x7f1b] = 0xff;
  globals->emu.ptr32KRam[0x7f1c] = 0x9f;
  globals->emu.ptr32KRam[0x7f1e] = 0xbf;
  globals->emu.ptr32KRam[0x7f20] = 0xff;
  globals->emu.ptr32KRam[0x7f23] = 0xbf;
  globals->emu.ptr32KRam[0x7f24] = 0x77;
  globals->emu.ptr32KRam[0x7f25] = 0xf3;
  globals->emu.ptr32KRam[0x7f26] = 0xf1;
  globals->emu.ptr32KRam[0x7f40] = 0x91;
  globals->emu.ptr32KRam[0x7f41] = 0x80;
  globals->emu.ptr32KRam[0x7f47] = 0xfc;
  globals->emu.ptr32KRam[0x7f48] = 0xff;
  globals->emu.ptr32KRam[0x7f49] = 0xff;

  MemSet(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable), 0);
  MemSet(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam), 0);
}

/**
 * Save the state of the current game.
 *
 * @param prefs the global preference data.
 */  
void
GameSave(PreferencesType *prefs)
{
  UInt16      card;
  LocalID     dbID;
  Char        strSaveName[32];
  Char        strGBRomName[17];
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *)&globals);
#endif

  MemSet(strGBRomName, 17, 0);
  StrNCopy(strGBRomName, prefs->game.strGBRomName+5, 16);
  StrPrintF(strSaveName, "Gmbt_!SAVE!_%s", strGBRomName);

  // remove if available
  card = 0;
  dbID = DmFindDatabase(card, strSaveName);
  if (dbID != NULL) DmDeleteDatabase(card, dbID);

  // save the state of this game (do this regardless)
  card = 0;
  if (DmCreateDatabase(card, strSaveName,
                       appCreator, saveType, false) == errNone) 
  {
    DmOpenRef dbRef;
    MemHandle binHandle;
    UInt16    index, binSize;
    Boolean   saveOk;
#ifdef SET_BACKUPBIT
    UInt16    dbAttributes;
#endif
 
    // open it for writing
    dbID  = DmFindDatabase(card, strSaveName);
    dbRef = DmOpenDatabase(card, dbID, dmModeReadWrite);
  
    saveOk = true;

    // save the "emulation state"
    index = 0;
    binSize = sizeof(EmuStateType);
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
              (MemPtr)&(globals->emuState), binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save the screen memory
    index++;
    binSize   = GameEncodingDeflateLength(globals->emu.ptrScreen, globals->emu.bmpBitsSize);
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {      
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
      if (DeviceSupportsVersion(romVersion5))
      {
      	UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
        GameEncodingDeflate(globals->emu.ptrScreen, globals->emu.bmpBitsSize, tmpBuffer, binSize);
        DmWrite(buffer, 0, tmpBuffer, binSize);
        MemPtrFree(tmpBuffer);      	
      }
      else
#endif
      {
        // compress it (directly to database)
        MemSemaphoreReserve(true);
        GameEncodingDeflate(globals->emu.ptrScreen, globals->emu.bmpBitsSize, buffer, binSize);
        MemSemaphoreRelease(true);
      }

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save the "32K" ram
    index++;
    binSize   = GameEncodingDeflateLength(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam));
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
      if (DeviceSupportsVersion(romVersion5))
      {
      	UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
        GameEncodingDeflate(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam), tmpBuffer, binSize);
        DmWrite(buffer, 0, tmpBuffer, binSize);
        MemPtrFree(tmpBuffer);      	
      }
      else
#endif
      {
        // compress it (directly to database)
        MemSemaphoreReserve(true);
        GameEncodingDeflate(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam), buffer, binSize);
        MemSemaphoreRelease(true);
      }

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save the Tile Table 
    index++;
    binSize   = GameEncodingDeflateLength(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable));
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
      if (DeviceSupportsVersion(romVersion5))
      {
      	UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
        GameEncodingDeflate(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable), tmpBuffer, binSize);
        DmWrite(buffer, 0, tmpBuffer, binSize);
        MemPtrFree(tmpBuffer);      	
      }
      else
#endif
      {
        // compress it (directly to database)
        MemSemaphoreReserve(true);
        GameEncodingDeflate(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable), buffer, binSize);
        MemSemaphoreRelease(true);
      }

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save the BG Tile ram
    index++;
    binSize   = GameEncodingDeflateLength(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam));
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL)
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
      if (DeviceSupportsVersion(romVersion5))
      {
      	UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
        GameEncodingDeflate(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam), tmpBuffer, binSize);
        DmWrite(buffer, 0, tmpBuffer, binSize);
        MemPtrFree(tmpBuffer);      	
      }
      else
#endif
      {
        // compress it (directly to database)
        MemSemaphoreReserve(true);
        GameEncodingDeflate(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam), buffer, binSize);
        MemSemaphoreRelease(true);
      }

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save the background palette
    index++;
    binSize = MemPtrSize(globals->emu.ptrBGPalette);
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
              (MemPtr)globals->emu.ptrBGPalette, binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save sprite palette #0
    index++;
    binSize = MemPtrSize(globals->emu.ptrSpritePalette0);
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
              (MemPtr)globals->emu.ptrSpritePalette0, binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // save sprite palette #1
    index++;
    binSize = MemPtrSize(globals->emu.ptrSpritePalette1);
    binHandle = DmNewRecord(dbRef, &index, binSize);
    if (binHandle != NULL) 
    {
      DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
              (MemPtr)globals->emu.ptrSpritePalette1, binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, true);
    }
    else saveOk &= false;

    // close database
    DmCloseDatabase(dbRef);

#ifdef SET_BACKUPBIT
    // set the backup bit
    DmDatabaseInfo(card, dbID,
                   NULL, &dbAttributes, NULL, NULL, NULL,
                   NULL, NULL, NULL, NULL, NULL, NULL);
    dbAttributes |= dmHdrAttrBackup;
    DmSetDatabaseInfo(card, dbID,
                      NULL, &dbAttributes, NULL, NULL, NULL,
                      NULL, NULL, NULL, NULL, NULL, NULL);
#endif

    // did something go wrong during the save?
    if (!saveOk) 
    {
      FrmAlert(saveErrorAlert);

      DmDeleteDatabase(card, dbID);
    }
  }
}

/**
 * Restore the state of the current game.
 *
 * @param prefs the global preference data.
 */  
void
GameRestore(PreferencesType *prefs)
{
  Char        strSaveName[32];
  Char        strGBRomName[17];
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *)&globals);
#endif

  MemSet(strGBRomName, 17, 0);
  StrNCopy(strGBRomName, prefs->game.strGBRomName+5, 16);
  StrPrintF(strSaveName, "Gmbt_!SAVE!_%s", strGBRomName);

  // no saved game?
  if (DmFindDatabase(0, strSaveName) == 0)  
    GameReset();

  // restored game - load everything back :)) 
  else 
  {
    LocalID   dbID;
    UInt16    card;
    DmOpenRef dbRef;
    MemHandle binHandle;
    UInt16    index, binSize;
    Boolean   loadOk;

    // open it for reading
    card  = 0;
    dbID  = DmFindDatabase(card, strSaveName);
    dbRef = DmOpenDatabase(card, dbID, dmModeReadOnly);

    // lets make sure the "save state is ok"
    loadOk = (DmNumRecords(dbRef) == 8);                  // we save 8 things
    if (!loadOk) goto LOADFAIL;

    // load the "emulation state"
    index = 0;
    binSize = sizeof(EmuStateType);
    binHandle = DmGetRecord(dbRef, index);
    if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize))  
    {
      MemMove((MemPtr)&(globals->emuState),
              (MemPtr)MemHandleLock(binHandle), binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the screen memory
    index++;
    binSize   = globals->emu.bmpBitsSize;      
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL)  
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
      {
        // extract it
        GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrScreen, binSize);
      }
      else loadOk &= false;

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the "32K" ram
    index++;
    binSize = MemPtrSize(globals->emu.ptr32KRam);
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL) 
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
      {
        // extract it
        GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptr32KRam, binSize);
      }
      else loadOk &= false;

      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the Tile Table 
    index++;
    binSize = MemPtrSize(globals->emu.ptrTileTable);
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL) 
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
      {
        // extract it
        GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrTileTable, binSize);
      }
      else loadOk &= false;
 
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the BG Tile ram
    index++;
    binSize = MemPtrSize(globals->emu.ptrBGTileRam);
    binHandle = DmGetRecord(dbRef, index);
    if (binHandle != NULL)
    {
      UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle);
      if (GameEncodingInflateLength(buffer, MemHandleSize(binHandle)) == binSize) 
      {
        // extract it
        GameEncodingInflate(buffer, MemHandleSize(binHandle), globals->emu.ptrBGTileRam, binSize);
      }
      else loadOk &= false;
  
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load the background palette
    index++;
    binSize = MemPtrSize(globals->emu.ptrBGPalette);
    binHandle = DmGetRecord(dbRef, index);
    if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
    {
      MemMove((MemPtr)globals->emu.ptrBGPalette,
              (MemPtr)MemHandleLock(binHandle), binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load sprite palette #0
    index++;
    binSize = MemPtrSize(globals->emu.ptrSpritePalette0);
    binHandle = DmGetRecord(dbRef, index);
    if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
    {
      MemMove((MemPtr)globals->emu.ptrSpritePalette0,
              (MemPtr)MemHandleLock(binHandle), binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

    // load sprite palette #1
    index++;
    binSize = MemPtrSize(globals->emu.ptrSpritePalette1);
    binHandle = DmGetRecord(dbRef, index);
    if ((binHandle != NULL) && (MemHandleSize(binHandle) == binSize)) 
    {
      MemMove((MemPtr)globals->emu.ptrSpritePalette1,
              (MemPtr)MemHandleLock(binHandle), binSize);
      MemHandleUnlock(binHandle);
      DmReleaseRecord(dbRef, index, false);
    }
    else loadOk &= false;

LOADFAIL:

    // close database
    DmCloseDatabase(dbRef);
    
    // did something go wrong? if so, reset
    if (!loadOk) 
    {
      FrmAlert(loadErrorAlert);

      DmDeleteDatabase(card, dbID);                // remove saved game
      GameReset();
    }
  }

  // adjust the "current" rom pointer
  globals->emu.ptrCurrRom = 
    globals->emu.ptrPages[globals->emuState.currRomIndex];
}

/**
 * Perform the emulation.
 * 
 * @param prefs the global preference data.
 * @param config the current configuration chunk.
 * @param keyStatus the current key state.
 */  
void   
GameEmulation(PreferencesType *prefs, ConfigType *config, UInt32 keyStatus)
{
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *)&globals);
#endif

  // the game is paused?
  if (prefs->game.gamePaused) 
  {
    Char   str[32];
    FontID currFont = FntGetFont();

    // lets write "PAUSED" over the middle of the screen :))
    StrCopy(str, "  *  PAUSED  *  ");
    FntSetFont(boldFont);
    WinDrawChars(str, StrLen(str),
                 80 - (FntCharsWidth(str, StrLen(str)) >> 1), 80);
    FntSetFont(currFont);

    // if they hit a key, lets stop being paused :)
    if (keyStatus != 0) prefs->game.gamePaused = false;
  }

  // the game is active
  else 
  {
    MemPtr ptrEmu = &(globals->emuState);

    // do the emulation
    globals->emuState.keyState       =
      (((keyStatus & config->ctlKeyDown)    != 0) ? 0x80 : 0x00) |
      (((keyStatus & config->ctlKeyUp)      != 0) ? 0x40 : 0x00) |
      (((keyStatus & config->ctlKeyLeft)    != 0) ? 0x20 : 0x00) |
      (((keyStatus & config->ctlKeyRight)   != 0) ? 0x10 : 0x00) |
      (((keyStatus & keyBitCradle)          != 0) ? 0x08 : 0x00) |
      (((keyStatus & keyBitAntenna)         != 0) ? 0x04 : 0x00) |
      (((keyStatus & config->ctlKeyButtonB) != 0) ? 0x02 : 0x00) |
      (((keyStatus & config->ctlKeyButtonA) != 0) ? 0x01 : 0x00);

    // additional checks here :)) 
#ifdef GAMEPAD
    if (globals->hardware.gamePadPresent) 
    {
      UInt8 gamePadKeyStatus;
      Err   err;

      // read the state of the gamepad
      err = GPDReadInstant(globals->hardware.gamePadLibRef, &gamePadKeyStatus);
      if (err == errNone) 
      {
        // process
        globals->emuState.keyState       |=
          (((gamePadKeyStatus & GAMEPAD_DOWN)      != 0) ? 0x80 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_UP)        != 0) ? 0x40 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_LEFT)      != 0) ? 0x20 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_RIGHT)     != 0) ? 0x10 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_START)     != 0) ? 0x08 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_SELECT)    != 0) ? 0x04 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_LEFTFIRE)  != 0) ? 0x02 : 0x00) |
          (((gamePadKeyStatus & GAMEPAD_RIGHTFIRE) != 0) ? 0x01 : 0x00);
      }
    }
#endif

    globals->emuState.cfgSoundMute   = config->cfgSoundMute;
    globals->emuState.cfgSoundVolume = config->cfgSoundVolume;
    globals->emuState.cfgSoundChan1  = config->cfgSoundChannel1; 
    globals->emuState.cfgSoundChan2  = config->cfgSoundChannel2;

    // setup the screen writing information
    globals->emu.ptrLCDScreen        = 
      DeviceWindowGetPointer(WinGetDrawWindow());
    globals->emu.frameBlitConst      = (UInt8)config->cfgFrameBlit; 
    globals->emu.skipCPUCycles       = config->cfgSkipCPUCycles; 

    // push all registers (except a7) on stack
    asm("movem.l %%d0-%%d7/%%a0-%%a6, -(%%sp)" : : );
    asm("move.l  %0, %%a1" : : "g" (ptrEmu));   // a1 = pointer to "data"

    // *do* the emulation :P
    EmulateFrame();

    // pop all registers (except a7) off stack
    asm("movem.l (%%sp)+, %%d0-%%d7/%%a0-%%a6" : : );
  }     
}
  
/**
 * Terminate the game.
 *
 * @param prefs the global preference data.
 */
void   
GameTerminate(PreferencesType *prefs)
{
#ifndef USE_GLOBALS
  GameGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrGameGlobals, (UInt32 *)&globals);
#endif

  // return the state of key processing
  KeySetMask(globals->keyMask);

  // unload the gamepad driver (if available)
#ifdef GAMEPAD
  if (globals->hardware.gamePadPresent) 
  {
    Err    err;
    UInt32 gamePadUserCount;

    err = GPDClose(globals->hardware.gamePadLibRef, &gamePadUserCount);
    if (gamePadUserCount == 0) 
      SysLibRemove(globals->hardware.gamePadLibRef);
  }
#endif

  // disable the rumble pak module vibration pointers
#ifdef RUMBLEPAK
  if (globals->emu.ptrVibrateOff != NULL) 
    *globals->emu.ptrVibrateOff = 0x00;         // turn it off :)
#endif

  // unlock the rom resources from memory and close database
  if (globals->emu.ptrPages != NULL) 
  {
    if (globals->emu.han32KBank != NULL) 
    {
      MemHandleUnlock(globals->emu.han32KBank);
      MemHandleFree(globals->emu.han32KBank);
    }
    else 
    {
      MemPtrUnlock(globals->emu.ptrPages[0]);
      MemPtrUnlock(globals->emu.ptrPages[1]);
    }

    // unlock the "remainding" rom chunks :))
    {
      GameAdjustmentType adjustType;

      // define the "adjustment"
      adjustType.adjustMode = gameFreeROM;
      adjustType.data.freeROM.pageCount = globals->emu.pageCount;
      adjustType.data.freeROM.ptrPages  = globals->emu.ptrPages;

      // do it! :)) - this will help our "hackers"
      RegisterAdjustGame(prefs, &adjustType);
    }

    // clean up :)
    MemPtrFree(globals->emu.ptrPages);
  }
  DmCloseDatabase(globals->dbGameRomRef);

  // remove the "saved game" database
  {
    UInt16  card;
    LocalID dbID;

    // remove if available
    card = 0;
    dbID = DmFindDatabase(card, "Gmbt_!SAVED_GAME!");
    if (dbID != NULL) DmDeleteDatabase(card, dbID);
  }

  // do we need to save the state of this game?
  if (prefs->game.gamePlaying) 
  { 
    UInt16  card;
    LocalID dbID;

    // create a new saved game
    card = 0;
    if (DmCreateDatabase(card, "Gmbt_!SAVED_GAME!", 
                         appCreator, saveType, false) == errNone) 
    {
      DmOpenRef dbRef;
      MemHandle binHandle;
      UInt16    index, binSize;
      Boolean   saveOk;
 
      // open it for writing
      dbID  = DmFindDatabase(card, "Gmbt_!SAVED_GAME!");
      dbRef = DmOpenDatabase(card, dbID, dmModeReadWrite);
  
      saveOk = true;
  
      // save the "emulation state"
      index = 0;
      binSize = sizeof(EmuStateType);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
                (MemPtr)&(globals->emuState), binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save the screen memory
      index++;
      binSize   = GameEncodingDeflateLength(globals->emu.ptrScreen, globals->emu.bmpBitsSize);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
        if (DeviceSupportsVersion(romVersion5))
        {
          UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
          GameEncodingDeflate(globals->emu.ptrScreen, globals->emu.bmpBitsSize, tmpBuffer, binSize);
          DmWrite(buffer, 0, tmpBuffer, binSize);
          MemPtrFree(tmpBuffer);      	
        }
        else
#endif
        {
          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals->emu.ptrScreen, globals->emu.bmpBitsSize, buffer, binSize);
          MemSemaphoreRelease(true);
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save the "32K" ram
      index++;
      binSize   = GameEncodingDeflateLength(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam));
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
        if (DeviceSupportsVersion(romVersion5))
        {
          UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
          GameEncodingDeflate(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam), tmpBuffer, binSize);
          DmWrite(buffer, 0, tmpBuffer, binSize);
          MemPtrFree(tmpBuffer);      	
        }
        else
#endif
        {
          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals->emu.ptr32KRam, MemPtrSize(globals->emu.ptr32KRam), buffer, binSize);
          MemSemaphoreRelease(true);
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save the Tile Table 
      index++;
      binSize   = GameEncodingDeflateLength(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable));
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
        if (DeviceSupportsVersion(romVersion5))
        {
          UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
          GameEncodingDeflate(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable), tmpBuffer, binSize);
          DmWrite(buffer, 0, tmpBuffer, binSize);
          MemPtrFree(tmpBuffer);      	
        }
        else
#endif
        {
          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals->emu.ptrTileTable, MemPtrSize(globals->emu.ptrTileTable), buffer, binSize);
          MemSemaphoreRelease(true);
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save the BG Tile ram
      index++;
      binSize   = GameEncodingDeflateLength(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam));
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        UInt8 *buffer = (UInt8 *)MemHandleLock(binHandle); 

#ifdef PALMOS5
        if (DeviceSupportsVersion(romVersion5))
        {
          UInt8 *tmpBuffer = (UInt8 *)MemPtrNew(binSize);
          GameEncodingDeflate(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam), tmpBuffer, binSize);
          DmWrite(buffer, 0, tmpBuffer, binSize);
          MemPtrFree(tmpBuffer);      	
        }
        else
#endif
        {
          // compress it (directly to database)
          MemSemaphoreReserve(true);
          GameEncodingDeflate(globals->emu.ptrBGTileRam, MemPtrSize(globals->emu.ptrBGTileRam), buffer, binSize);
          MemSemaphoreRelease(true);
        }

        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save the background palette
      index++;
      binSize = MemPtrSize(globals->emu.ptrBGPalette);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
                (MemPtr)globals->emu.ptrBGPalette, binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save sprite palette #0
      index++;
      binSize = MemPtrSize(globals->emu.ptrSpritePalette0);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
                (MemPtr)globals->emu.ptrSpritePalette0, binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // save sprite palette #1
      index++;
      binSize = MemPtrSize(globals->emu.ptrSpritePalette1);
      binHandle = DmNewRecord(dbRef, &index, binSize);
      if (binHandle != NULL) 
      {
        DmWrite((MemPtr)MemHandleLock(binHandle), 0, 
                (MemPtr)globals->emu.ptrSpritePalette1, binSize);
        MemHandleUnlock(binHandle);
        DmReleaseRecord(dbRef, index, true);
      }
      else saveOk &= false;

      // close database
      DmCloseDatabase(dbRef);

      // did something go wrong during the save?
      if (!saveOk) 
      {
        FrmAlert(saveErrorAlert);

        DmDeleteDatabase(card, dbID);
      }
    }
  }

  // free memory used by emulator resources
#ifdef PALM_HIRES
  if (globals->emu.use_api)
  {
    if (globals->emu.bmpOffscreen      != NULL)         
      BmpDelete(globals->emu.bmpOffscreen);
    if (globals->emu.winOffscreen      != NULL)         
      WinDeleteWindow(globals->emu.winOffscreen, false);
  }
  else  
#endif
  {
    if (globals->emu.ptrScreen         != NULL)         
      MemPtrFree(globals->emu.ptrScreen);
  }
  if (globals->emu.ptr32KRam         != NULL)
    MemPtrFree(globals->emu.ptr32KRam);
  if (globals->emu.ptrTileTable      != NULL)
    MemPtrFree(globals->emu.ptrTileTable);
  if (globals->emu.ptrBGPalette      != NULL)
    MemPtrFree(globals->emu.ptrBGTileRam);
  if (globals->emu.ptrBGPalette      != NULL)
    MemPtrFree(globals->emu.ptrBGPalette);
  if (globals->emu.ptrSpritePalette1 != NULL) 
    MemPtrFree(globals->emu.ptrSpritePalette0);
  if (globals->emu.ptrSpritePalette1 != NULL) 
    MemPtrFree(globals->emu.ptrSpritePalette1);

  // free and unlock read-only resource
  if (globals->emu.ptrHLConv         != NULL) 
    MemPtrUnlock(globals->emu.ptrHLConv);
  DmReleaseResource(globals->emu.hanHLConv);
  if (globals->emu.ptrXlat           != NULL) 
    MemPtrUnlock(globals->emu.ptrXlat);
  DmReleaseResource(globals->emu.hanXlat);
  if (globals->emu.stdBGPalette      != NULL) 
    MemPtrFree(globals->emu.stdBGPalette);
  if (globals->emu.stdSpritePalette0 != NULL) 
    MemPtrFree(globals->emu.stdSpritePalette0);
  if (globals->emu.stdSpritePalette1 != NULL) 
    MemPtrFree(globals->emu.stdSpritePalette1);

#ifndef USE_GLOBALS
  // clean up windows/memory
  MemPtrFree(globals);

  // unregister global data
  FtrUnregister(appCreator, ftrGameGlobals);
#endif
}

/**
 * Inflate a RLE compressed data chunk into an uncompressed chunk.
 *
 * @param ptrData the source data buffer.
 * @param ptrOutput the output data buffer.
 */
static void
GameEncodingInflate(UInt8 *ptrData,   UInt16 datSize,
		    UInt8 *ptrOutput, UInt16 outSize)
{
  // lets make sure our "pointers" are valid
  if ((ptrData != NULL) && (ptrOutput != NULL) && 
      (datSize != 0) &&
      (outSize >= GameEncodingInflateLength(ptrData, datSize))) 
  {
    UInt16 i, j, length, outlength, index;
    UInt8  data, freq;

    // setup time :)
    length    = datSize;
    outlength = outSize;
    index     = 0;

    // do all RLE chunks
    for (i=0; i<length; i+=2) 
    {
      // extract the block info
      data = ptrData[i];
      freq = ptrData[i+1];

      // extract XXXX bytes
      for (j=0; (j<freq) && (index < outlength); j++) 
        ptrOutput[index++] = data;
    }
  }
}

/**
 * Deflate an uncompressed data chunk into a RLE compressed chunk.
 *
 * @param ptrData the source data buffer.
 * @param ptrOutput the output data buffer.
 */
static void
GameEncodingDeflate(UInt8 *ptrData,   UInt16 datSize,
		    UInt8 *ptrOutput, UInt16 outSize) 
{  
  // lets make sure our "pointers" are valid
  if ((ptrData != NULL) && (ptrOutput != NULL) && 
      (datSize != 0) &&
      (outSize >= GameEncodingDeflateLength(ptrData, datSize))) 
  {
    UInt8  data, freq;
    UInt16 length, index, b_size;

    // setup time :)
    length = datSize;
    index  = 0;
    b_size = 2;

    // encoding!!
    data  = ptrData[index++];
    freq  = 1;
    while (index < length) 
    {
      // same data, increase RLE entry
      if (data == ptrData[index]) 
      {
        // max RLE = 255
	if (freq == 255) 
        {
          // save the state
          ptrOutput[b_size-2] = data;
          ptrOutput[b_size-1] = freq;

	  freq    = 1;
          b_size += 2; // two more bytes
	}

	// ok, keep counting
	else freq++;
      }

      // different data, new RLE entry
      else 
      {
        // save the state
        ptrOutput[b_size-2] = data;
        ptrOutput[b_size-1] = freq;

        data    = ptrData[index];
	freq    = 1;
        b_size += 2; // two more bytes
      }

      index++;
    }

    ptrOutput[b_size-2] = data;
    ptrOutput[b_size-1] = freq;
  }
}

/**
 * Calculate the size of the output buffer required when making a call to 
 * the function GameEncodingInflate() with a specific pointer of memory.
 *
 * @param ptrData the source data buffer.
 * @return the size of the inflated buffer
 */
static UInt16
GameEncodingInflateLength(UInt8 *ptrData, UInt16 datSize)
{
  UInt16 result = 0;

  // lets make sure our "pointer" is valid
  if ((ptrData != NULL) && (datSize != 0)) 
  {
    UInt16 length, i;

    // setup time :)
    length = datSize;

    // count the bytes
    result = 0;
    for (i=1; i<length; i+=2) 
      result += ptrData[i];    // add up all the RLE counts
  }

  return result;
}

/**
 * Calculate the size of the output buffer required when making a call to 
 * the function GameEncodingDeflate() with a specific pointer of memory.
 *
 * @param ptrData the source data buffer.
 * @return the size of the deflated buffer
 */
static UInt16
GameEncodingDeflateLength(UInt8 *ptrData, UInt16 datSize)
{
  UInt16 result = 0;

  // lets make sure our "pointer" is valid
  if ((ptrData != NULL) && (datSize > 0)) 
  {
    UInt8  data, freq;
    UInt16 length, index;

    // setup time :)
    length = datSize;
    index  = 0;
    result = 2;

    // encoding!!
    data  = ptrData[index++];
    freq  = 1;
    while (index < length) 
    {
      // same data, increase RLE entry
      if (data == ptrData[index]) 
      {
        // max RLE = 255
	if (freq == 255) 
        {
	  freq    = 1;
          result += 2; // two more bytes
	}

	// ok, keep counting
	else freq++;
      }

      // different data, new RLE entry
      else 
      {
        data    = ptrData[index];
	freq    = 1;
        result += 2; // two more bytes
      }

      index++;
    }
  }

  return result;
}
