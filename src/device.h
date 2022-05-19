/*
 * @(#)device.h
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

#ifndef _DEVICE_H
#define _DEVICE_H

#include "palm.h"

#define romVersion1   sysMakeROMVersion(1,0,0,sysROMStageDevelopment,0)
#define romVersion2   sysMakeROMVersion(2,0,0,sysROMStageDevelopment,0)
#define romVersion3   sysMakeROMVersion(3,0,0,sysROMStageDevelopment,0)
#define romVersion3_1 sysMakeROMVersion(3,1,0,sysROMStageDevelopment,0)
#define romVersion3_2 sysMakeROMVersion(3,2,0,sysROMStageDevelopment,0)
#define romVersion3_5 sysMakeROMVersion(3,5,0,sysROMStageDevelopment,0)
#define romVersion4   sysMakeROMVersion(4,0,0,sysROMStageDevelopment,0)
#define romVersion5   sysMakeROMVersion(5,0,0,sysROMStageDevelopment,0)

#define sysFtrNumProcessorVZ      0x00030000
#define sysFtrNumProcessorSuperVZ 0x00040000

enum
{
  palReset = 0,
  palGet,
  palSet
};

enum
{
  graySet = 0,
  grayGet
};

extern Boolean DeviceCheckCompatability()                           __DEVICE__;
extern void    DeviceInitialize()                                   __DEVICE__;
extern Boolean DeviceSupportsGrayscale()                            __DEVICE__;
extern void    DeviceGrayscale(UInt16, UInt8 *, UInt8 *)            __DEVICE__;
extern Boolean DeviceSupportsGrayPalette()                          __DEVICE__;
extern void    DeviceGrayPalette(UInt16, UInt8 *, UInt8 *)          __DEVICE__;
extern UInt32  DeviceGetSupportedDepths()                           __DEVICE__;
extern Boolean DeviceSupportsVersion(UInt32)                        __DEVICE__;
extern void    *DeviceWindowGetPointer(WinHandle)                   __DEVICE__;
extern Boolean DeviceBeamDatabase(Char *)                           __DEVICE__;
extern void    DeviceTerminate()                                    __DEVICE__;

#endif
