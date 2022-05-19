/*
 * @(#)register.h
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

#ifndef _REGISTER_H
#define _REGISTER_H

#include "palm.h"

#define MAX_IDLENGTH 8

enum gameAdjustMode
{
  gameLoadROM = 0,
  gameFreeROM
};

typedef struct
{
  UInt8 adjustMode;                        // what type of adjustment?
  union {

    // generic
    struct {
      UInt8  unused[16];
    } generic; 

    // load ROM
    struct {
      UInt16 pageCount;
      UInt8  **ptrPages;
    } loadROM;

    // free ROM
    struct {
      UInt16 pageCount;
      UInt8  **ptrPages;
    } freeROM;

  } data;
} GameAdjustmentType;

extern void    RegisterInitialize(PreferencesType *)              __REGISTER__;
extern void    RegisterShowMessage(PreferencesType *)             __REGISTER__; 
extern Boolean RegisterAdjustGame(PreferencesType *, 
                                  GameAdjustmentType *)           __REGISTER__;
extern UInt8   RegisterChecksum(UInt8*, UInt16);
extern void    RegisterDecryptChunk(UInt8*, Int16, Int16, UInt8);
extern void    RegisterTerminate()                                __REGISTER__;

extern void    _regiLoader(PreferencesType *)                     __SAFE0001__;
extern Boolean _adjustGame(PreferencesType *, 
                           GameAdjustmentType *)                  __SAFE0002__;

#endif
