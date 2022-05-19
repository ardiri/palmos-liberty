/*
 * @(#)palm.h
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

#ifndef _PALM_H
#define _PALM_H

// system includes
#include <PalmOS.h>
#include <System/DLServer.h>

// resource "include" :P
#include "resource.h"

// special includes (additional hardware etc)
#ifdef GAMEPAD
#include "hardware/GPDLib.h"
#endif
#ifdef RUMBLEPAK
#include "hardware/Handspring.h"
#endif
#ifdef SONY
#include "hardware/SonyHR.h"
#include "hardware/SonyChars.h"
#endif
#ifdef HANDERA
#include "hardware/HanderaVGA.h"
#include "hardware/HanderaChars.h"
#endif
#ifdef PALMOS5
#include "hardware/PalmHDD.h"
#endif
#ifdef PALM_5WAY
#include "hardware/PalmChars.h"
#endif

// application constants and structures
#define appCreator        'Gmbt'
#define keyCreator        '_GBK'
#define romType           'gROM'
#define datType           '_dat'
#define binType           '_bin'  
#define saveType          'save'
#define cfgType           '_cfg'
#define sramType          'sram'
#define __REGISTER__      __attribute__ ((section ("register")))// code0002.bin
#define __DEVICE__        __attribute__ ((section ("device")))  // code0003.bin
#define __GAME__          __attribute__ ((section ("game")))    // code0004.bin
#define __HELP__          __attribute__ ((section ("help")))    // code0005.bin
#define __SAFE0001__      __attribute__ ((section ("safe0001")))// code0006.bin
#define __SAFE0002__      __attribute__ ((section ("safe0002")))// code0007.bin

#define ftrGlobals         1000
#define ftrDeviceGlobals   1001
#define ftrGameGlobals     1002
#define ftrHelpGlobals     1003
#define ftrRegisterGlobals 1004

#define MAX_LISTITEMS      32   // 32 items MAX in the lists
#define GAME_FPS           15   // call "emulateframe" 15 times per second
#define VERSION            0

enum appEvents
{
  appRedrawEvent = firstUserEvent,
  appGenerateROMList,
  appGenerateSAVEDList
};

typedef struct 
{
  UInt16   ctlKeyButtonA;               // key definition for button A
  UInt16   ctlKeyButtonB;               // key definition for button B
  UInt16   ctlKeyUp;                    // key definition for move up
  UInt16   ctlKeyDown;                  // key definition for move down
  UInt16   ctlKeyLeft;                  // key definition for move left
  UInt16   ctlKeyRight;                 // key definition for move right

  Boolean  cfgUseFindCalc;              // use the "find" and "calc" buttons
  Boolean  cfgSkipCPUCycles;            // skip CPU cycles (better emu on some)
  UInt16   cfgFrameBlit;                // frame blit configuration 

  UInt16   cfgSoundVolume;              // sound volume
  Boolean  cfgSoundMute;                // mute sound?
  Boolean  cfgSoundChannel1;            // channel 1 active?
  Boolean  cfgSoundChannel2;            // channel 2 active?

  UInt8    lgray;                       // the light gray configuration setting
  UInt8    dgray;                       // the dark gray configuration setting
  UInt8    lRGB[3];                     // the light gray RGB indexes
  UInt8    dRGB[3];                     // the dark gray RGB indexes
} ConfigType;

typedef struct
{
  struct 
  {
    UInt8    signatureVersion;          // a preference version number
    Char     signature[16];             // a "signature" for decryption
    Char     *hotSyncUsername;          // the HotSync user name of the user
    Boolean  showWarning;               // do we show the ROM warning?
  } system;

  ConfigType config;                    // default configuration resource

  struct 
  {
    Char     strGBRomName[32];          // the currently active ROM!
    Boolean  restoring;                 // is this game being restored?

    Boolean  gamePlaying;               // is there a game in play?
    Boolean  gamePaused;                // is the game currently paused?

    UInt32   keyMaskSpecial;            // a little "hack" for start/select 
  } game;
  
} PreferencesType;

// this is our 'double check' for decryption - make sure it worked :P
#define CHECK_SIGNATURE(x) (StrCompare(x->system.signature, "|HaCkMe|") == 0)

// local includes
#include "device.h"
#include "help.h"
#include "game.h"
#include "register.h"

#ifdef PRCTOOLS_FLASHBUG
#include "gccfix.h"         // prc-tools 2.0 code resuorce unlock bug :)
#endif

// functions
extern UInt32  PilotMain(UInt16, MemPtr, UInt16);
extern void    InitApplication(void);
extern Boolean ApplicationHandleEvent(EventType *);
extern void    ApplicationDisplayDialog(UInt16);
extern void    EventLoop(void);
extern void    EndApplication(void);

#endif 
