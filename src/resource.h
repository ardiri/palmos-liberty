/*
 * @(#)resource.h
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

#ifdef PROTECTION_ON

/*
#ifndef LANG_en
#error "unsupported translation for protected build."
#endif
*/

#endif

// COMPILING:
// ----------
//
// The following are used to experiment with BETA quality components
// of the Liberty application. To enable a particular portion, uncomment
// the #define, and it will compile in.
//
// #define DEBUG              1
   #define SONY               1 // support for Sony (320x320) devices
   #define HANDERA            1 // support for Handera QVGA (240x320) device
   #define PALM_HIRES         1 // support for Palm HiRes (320x320) device

   #define USE_GLOBALS        1 // use real globals, not features
   #define SET_BACKUPBIT      1 // the config+saved games should be backed up?
   #define PALM_5WAY          1 // 5-way dpad (palm)
   #define PALMOS5            1 // palmos 5 compliant if needed
   
// 
// The following are in the *public* version, we should clean these up
// one day :)) [if this list gets too big, lets code review hey? clean up]
//
   #define GAMEPAD            1 // support for the gamepad device
   #define RUMBLEPAK          1 // support for rumble pak (vibration)

// bitmaps

#define bitmapIcon                     1000
#define bitmapPalm                     1001
#define bitmapLogo                     1002
#define bitmapPaw                      1003
#define bitmapGambit                   1004
#define bitmapAbout                    1005
#define bitmapHelp                     1006
#define bitmapSButtons                 1007
#define bitmapGrayscaleTest1           1010
#define bitmapGrayscaleTest2           1020
#define bitmapKeyConfig                1030
#define bitmapHelpSelectList           2000
#define bitmapHelpStylusControl        2001
  
// alerts

#define removeROMAlert                 1000
#define removeSAVEDAlert               1001
#define deleteErrorAlert               1002
#define extractErrorAlert              1003
#define saveErrorAlert                 1004
#define loadErrorAlert                 1005

// dialogs
  
#define infoForm                       2000
#define infoFormOkButton               2001
#define deviForm                       2100
#define dvlpForm                       2200
#define dvlpFormOkButton               2201
#define cfigForm                       2300
#define cfigFormHardKey1Trigger        2301
#define cfigFormHardKey1List           2302
#define cfigFormHardKey2Trigger        2303
#define cfigFormHardKey2List           2304
#define cfigFormHardKey3Trigger        2305
#define cfigFormHardKey3List           2306
#define cfigFormHardKey4Trigger        2307
#define cfigFormHardKey4List           2308
#define cfigFormPageUpTrigger          2309
#define cfigFormPageUpList             2310
#define cfigFormPageDownTrigger        2311
#define cfigFormPageDownList           2312
#define cfigFormButtonCheckbox         2313
#define cfigFormCPUCycleCheckbox       2314
#define cfigFormBlitTrigger            2315
#define cfigFormBlitList               2316
#define cfigFormSound0Button           2317
#define cfigFormSound1Button           2318
#define cfigFormSound2Button           2319
#define cfigFormSound3Button           2320
#define cfigFormMuteCheckbox           2321
#define cfigFormChannel1Checkbox       2322
#define cfigFormChannel2Checkbox       2323
#define cfigFormOkButton               2324
#define cfigFormCancelButton           2325
#define grayForm                       2400
#define grayWhite1Button               2401
#define grayWhite2Button               2402
#define grayWhite3Button               2403
#define grayWhite4Button               2404
#define grayWhite5Button               2405
#define grayWhite6Button               2406
#define grayWhite7Button               2407
#define grayLightGray1Button           2408
#define grayLightGray2Button           2409
#define grayLightGray3Button           2410
#define grayLightGray4Button           2411
#define grayLightGray5Button           2412
#define grayLightGray6Button           2413
#define grayLightGray7Button           2414
#define grayDarkGray1Button            2415
#define grayDarkGray2Button            2416
#define grayDarkGray3Button            2417
#define grayDarkGray4Button            2418
#define grayDarkGray5Button            2419
#define grayDarkGray6Button            2420
#define grayDarkGray7Button            2421
#define grayBlack1Button               2422
#define grayBlack2Button               2423
#define grayBlack3Button               2424
#define grayBlack4Button               2425
#define grayBlack5Button               2426
#define grayBlack6Button               2427
#define grayBlack7Button               2428
#define grayFormOkButton               2429
#define gcolForm                       2500
#define gcolFormLRButton0              2501
#define gcolFormLRButton1              2502
#define gcolFormLRButton2              2503
#define gcolFormLRButton3              2504
#define gcolFormLRButton4              2505
#define gcolFormLRButton5              2506
#define gcolFormLRButton6              2507
#define gcolFormLRButton7              2508
#define gcolFormLRButton8              2509
#define gcolFormLGButton0              2510
#define gcolFormLGButton1              2511
#define gcolFormLGButton2              2512
#define gcolFormLGButton3              2513
#define gcolFormLGButton4              2514
#define gcolFormLGButton5              2515
#define gcolFormLGButton6              2516
#define gcolFormLGButton7              2517
#define gcolFormLGButton8              2518
#define gcolFormLBButton0              2519
#define gcolFormLBButton1              2520
#define gcolFormLBButton2              2521
#define gcolFormLBButton3              2522
#define gcolFormLBButton4              2523
#define gcolFormLBButton5              2524
#define gcolFormLBButton6              2525
#define gcolFormLBButton7              2526
#define gcolFormLBButton8              2527
#define gcolFormDRButton0              2528
#define gcolFormDRButton1              2529
#define gcolFormDRButton2              2530
#define gcolFormDRButton3              2531
#define gcolFormDRButton4              2532
#define gcolFormDRButton5              2533
#define gcolFormDRButton6              2534
#define gcolFormDRButton7              2535
#define gcolFormDRButton8              2536
#define gcolFormDGButton0              2537
#define gcolFormDGButton1              2538
#define gcolFormDGButton2              2539
#define gcolFormDGButton3              2540
#define gcolFormDGButton4              2541
#define gcolFormDGButton5              2542
#define gcolFormDGButton6              2543
#define gcolFormDGButton7              2544
#define gcolFormDGButton8              2545
#define gcolFormDBButton0              2546
#define gcolFormDBButton1              2547
#define gcolFormDBButton2              2548
#define gcolFormDBButton3              2549
#define gcolFormDBButton4              2550
#define gcolFormDBButton5              2551
#define gcolFormDBButton6              2552
#define gcolFormDBButton7              2553
#define gcolFormDBButton8              2554
#define gcolFormOkButton               2555
#define regiForm                       2600
#define regiFormOkButton               2601
#define rbugForm                       2700
#define rbugFormOkButton               2701
#define helpForm                       2800
#define helpFormScrollBar              2801
#define helpFormOkButton               2802
#define xmemForm                       2900
#define xmemFormOkButton               2901
#define warnForm                       3000
#define warnFormShowWarnCheckbox       3001
#define warnFormOkButton               3002
#define saveForm                       3100
#define saveFormSavedList              3102
#define saveFormRestoreButton          3103
#define saveFormDeleteButton           3104
#define saveFormDoneButton             3105
#define xromForm                       3200
#define xromFormOkButton               3201

// forms

#define mainForm                       4000
#define mainFormAboutButton            4001
#define mainFormHelpButton             4002
#define mainFormGameGadget             4003
#define mainFormScrollBar              4004
#define mainFormPlayButton             4005
#define gameForm                       4100
#define gameFormStartButton            4101
#define gameFormSelectButton           4102

// menus

#define menuItemGrayscale              5001
#define menuItemGrayPalette            5002
#define menuItemOverclock              5003
#define menuItemConfig                 5004
#define menuItemRegister               5005
#define menuItemDeveloper              5006
#define menuItemWarning                5007
#define menuItemHelp                   5008
#define menuItemAbout                  5009
#define mainMenu_nogray                5100
#define mainMenu_gray                  5101
#define mainMenu_graycolor             5102
#define mainMenuItemPlay               5110
#define mainMenuItemBeam               5111
#define mainMenuItemDelete             5112
#define mainMenuItemSavedGames         5113
#define gameMenu_nogray                5200
#define gameMenu_gray                  5201
#define gameMenu_graycolor             5202
#define gameMenuItemPause              5211
#define gameMenuItemPressKeys          5212
#define gameMenuItemSaveGame           5213
#define gameMenuItemReset              5214
#define gameMenuItemExit               5215

// binary resources
#define binaryHLConversion             6000
#define binaryBackgroundPalette        6001
#define binarySpritePalette0           6002
#define binarySpritePalette1           6003
#define binaryXlat                     6004

// saved game resources
#define savedScreen                    7000
#define saved32KRam                    7001
#define savedGlobalsState              7002
#define savedBackgroundPalette         7003
#define savedSpritePalette0            7004
#define savedSpritePalette1            7005
