/*
 * @(#)palm.c
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

typedef struct
{
  PreferencesType *prefs;

  Int32           evtTimeOut;
  Int16           timerDiff;
  Int16           ticksPerFrame;
  UInt32          timerPointA;
  UInt32          timerPointB;

  Char            **dataList1;
  Char            **dataList2;
  Char            **dataList3;
  UInt16          dataListCount;        
  UInt16          dataIndex;          

  UInt8           lgray;
  UInt8           dgray;

#ifdef PALM_5WAY
  UInt32          keyStateDPAD;
#endif  

  ConfigType      *config;
} Globals;

#ifdef USE_GLOBALS
// globals object
static Globals gbls;
static Globals *globals;
#endif

// interface
static Boolean mainFormEventHandler(EventType *);
static Boolean gameFormEventHandler(EventType *);
static Boolean infoFormEventHandler(EventType *);
static Boolean dvlpFormEventHandler(EventType *);
static Boolean cfigFormEventHandler(EventType *);
static Boolean grayFormEventHandler(EventType *);
static Boolean gcolFormEventHandler(EventType *);
#ifdef PROTECTION_ON
static Boolean regiFormEventHandler(EventType *);
static Boolean rbugFormEventHandler(EventType *);
#endif
static Boolean helpFormEventHandler(EventType *);
static Boolean xmemFormEventHandler(EventType *);
static Boolean warnFormEventHandler(EventType *);
static Boolean saveFormEventHandler(EventType *);
static Boolean xromFormEventHandler(EventType *);

static Boolean rom32KSizeCheck(Char *)                            __REGISTER__;

/**
 * The Form:mainForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
mainFormEventHandler(EventType *event)
{
  Boolean processed = false;
  Char    ch;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:

         // initialize the "display"
         {
           FormType *frm = FrmGetActiveForm();
           UInt16   i, menuID;

           // dynamically adjust the menu :)
           menuID = mainMenu_nogray;
                if (DeviceSupportsGrayscale())   menuID = mainMenu_gray;
           else if (DeviceSupportsGrayPalette()) menuID = mainMenu_graycolor;
           FrmSetMenu(frm, menuID);
           FrmDrawForm(frm);

           // adjust the "configurations" stuff
           globals->config = &(globals->prefs->config);

           // configure the grayscale settings - as appropriate
           if (DeviceSupportsGrayscale()) 
           {
             DeviceGrayscale(graySet,
                             &globals->config->lgray,
                             &globals->config->dgray);
           }
           else
           if (DeviceSupportsGrayPalette()) 
           {
             DeviceGrayPalette(palSet,
                               globals->config->lRGB, 
                               globals->config->dRGB);
           }

           globals->dataList1 = 
             (Char **)MemPtrNew(MAX_LISTITEMS * sizeof(Char *));
           globals->dataList2 = 
             (Char **)MemPtrNew(MAX_LISTITEMS * sizeof(Char *));
           for (i=0; i<MAX_LISTITEMS; i++) {
             globals->dataList1[i] = (Char *)MemPtrNew(32 * sizeof(Char));
             globals->dataList2[i] = (Char *)MemPtrNew(16 * sizeof(Char));
           }
           globals->dataListCount = 0;

           // generate the ROM list
           {
             EventType event;
             
             MemSet(&event, sizeof(EventType), 0);
             event.eType = appGenerateROMList;
             EvtAddEventToQueue(&event);
           }
         }
         processed = true;
         break;

    case appGenerateROMList:
         {
           FormType      *frm;
           ScrollBarType *sclBar;
           UInt16        i, count;
           UInt16        val, min, max, pge;
           Char          *romName;

           // this is the last played game (keep track of it)
           romName = &globals->prefs->game.strGBRomName[5];

           // generate the "rom list"
           globals->dataIndex = noListSelection;
           count              = 0;
           {
             DmSearchStateType stateInfo;
             Err               error;
             UInt16            card, pos;
             LocalID           dbID;
             UInt32            dbSize;
             Char              strDatabaseName[32];
             Char              strDatabaseSize[16];

             error =
               DmGetNextDatabaseByTypeCreator(true, &stateInfo,
                                              romType, appCreator, false,
                                              &card, &dbID);

             while ((error == 0) && (count < MAX_LISTITEMS)) 
             {
               // extract the database information
               DmDatabaseInfo(card, dbID, strDatabaseName, 
                              NULL, NULL, NULL, NULL, NULL,
                              NULL, NULL, NULL, NULL, NULL);
               DmDatabaseSize(card, dbID, NULL, &dbSize, NULL);
  
               // strip the "Gmbt_" from the name
               i = 0;
               while ((strDatabaseName[i+5] != '\0')) 
               {
                 strDatabaseName[i] = strDatabaseName[i+5]; i++;
               }
               strDatabaseName[i] = '\0';

               // determine the size (round to 8K chunks)
               dbSize = (dbSize >> 14) << 4;
               StrPrintF(strDatabaseSize, "%d Kb", (Int16)dbSize);
  
               // find out where it should go
               pos = 0;
               while ((pos < count) &&
                      (StrCompare(strDatabaseName, 
                                  globals->dataList1[pos]) > 0)) 
               {
                 pos++;
               }

               // do we need to shift a few things?
               if (pos < count) 
               {
                 // move em down
                 for (i=count; i>pos; i--) 
                 {
                   StrCopy(globals->dataList1[i], globals->dataList1[i-1]);
                   StrCopy(globals->dataList2[i], globals->dataList2[i-1]);
                 }
               }

               // copy it to the list
               StrCopy(globals->dataList1[pos], strDatabaseName);
               StrCopy(globals->dataList2[pos], strDatabaseSize);

               // next sequence
               error =
                 DmGetNextDatabaseByTypeCreator(false, &stateInfo,
                                                romType, appCreator, false,
                                                &card, &dbID);
               count++;
             }
           }
           globals->dataListCount = count;

           // select the "last played" rom, or, the first if none available
           for (i=0; i<globals->dataListCount; i++) 
           {
             if (StrCompare(globals->dataList1[i], romName) == 0) 
               globals->dataIndex = i; 
           }
           if ((globals->dataIndex == noListSelection) && 
               (globals->dataListCount != 0)) 
             globals->dataIndex = 0;

           frm = FrmGetActiveForm();

           // adjust the "scrollbar"
           sclBar =  
             (ScrollBarType *)FrmGetObjectPtr(frm, 
               FrmGetObjectIndex(frm, mainFormScrollBar)); 

           SclGetScrollBar(sclBar, &val, &min, &max, &pge);
           max = (count > pge) ? count - pge : 0;
           val = globals->dataIndex;
           if (val > max) val = max;
           SclSetScrollBar(sclBar, val, min, max, pge);

           // make sure the "freespaces" are empty! :)
           for (; count<MAX_LISTITEMS; count++) 
           {
             MemSet(globals->dataList1[count], 32 * sizeof(Char), 0);
             MemSet(globals->dataList2[count], 16 * sizeof(Char), 0);
           }

           // redraw the display
           {
             EventType event;
             
             MemSet(&event, sizeof(EventType), 0);
             event.eType = appRedrawEvent;
             EvtAddEventToQueue(&event);
           }
         }
         processed = true;
         break;

    case frmUpdateEvent:
         FrmDrawForm(FrmGetActiveForm());

    case appRedrawEvent:
         {
           const CustomPatternType erase = {0,0,0,0,0,0,0,0};
           FormType      *frm;
           ScrollBarType *sclBar;
           RectangleType rctBounds, rctClip;
           UInt16        i, x, y;
           UInt16        val, min, max, pge;

           frm = FrmGetActiveForm();
           WinGetClip(&rctClip);

           // draw seperators
           WinDrawLine(   0,  44, 159,  44);
           WinDrawLine(   0,  45, 159,  45);

           WinDrawLine(   0, 145, 159, 145);
           WinDrawLine(   0, 146, 159, 146);

           // get the "scrollbar" values
           sclBar =  
             (ScrollBarType *)FrmGetObjectPtr(frm, 
               FrmGetObjectIndex(frm, mainFormScrollBar)); 
           SclGetScrollBar(sclBar, &val, &min, &max, &pge);

           // draw the "list"
           FrmGetObjectBounds(frm,
             FrmGetObjectIndex(frm, mainFormGameGadget), &rctBounds);
           WinSetClip(&rctBounds);
           WinSetPattern(&erase);
           WinFillRectangle(&rctBounds, 0);

           x = rctBounds.topLeft.x; 
           y = rctBounds.topLeft.y; 
           for (i=val; i<globals->dataListCount; i++) 
           {
             x = rctBounds.topLeft.x + 4; 
             WinDrawChars(globals->dataList1[i], 
                          StrLen(globals->dataList1[i]), x, y);

             x = rctBounds.topLeft.x +
                 rctBounds.extent.x - 4 -
                 FntCharsWidth(globals->dataList2[i], 
                               StrLen(globals->dataList2[i])); 
             WinDrawChars(globals->dataList2[i], 
                          StrLen(globals->dataList2[i]), x, y);

             y += FntLineHeight();
           }

           // invert the "selected" item
           rctBounds.topLeft.y += FntLineHeight() * 
                                  (globals->dataIndex - val);
           rctBounds.extent.y   = FntLineHeight();
           WinInvertRectangle(&rctBounds, 0); 

           WinSetClip(&rctClip);

           // show the registration status
           RegisterShowMessage(globals->prefs);
         }
         processed = true;
         break;

    case menuEvent:

         // what menu?
         switch (event->data.menu.itemID)
         {
           case mainMenuItemPlay:

                // lets make sure there is a selection :)
                if (globals->dataIndex != noListSelection) 
                {
                  UInt16  index;

                  index = globals->dataIndex;
                  // are we allowed to play!
                  if (
                       rom32KSizeCheck(globals->dataList1[index]) ||
                       CHECK_SIGNATURE(globals->prefs)
                     ) 
                  {
                    StrPrintF(globals->prefs->game.strGBRomName, "Gmbt_%s", 
                              globals->dataList1[index]);
                    FrmGotoForm(gameForm);
                  }

#ifdef PROTECTION_ON
                  // "insufficient" privelledges
                  else
                    ApplicationDisplayDialog(rbugForm);
#endif
                }
                else  
                  SndPlaySystemSound(sndError);

                processed = true;
                break;

           case mainMenuItemBeam:

                // lets make sure there is a selection :)
                if (globals->dataIndex != noListSelection) 
                {
                  // beam the database [errors handled by Palm]
		  DeviceBeamDatabase(globals->dataList1[globals->dataIndex]);
		}
		else
                  SndPlaySystemSound(sndError);

                processed = true;
                break;

           case mainMenuItemDelete:

                // lets make sure there is a selection :)
                if (globals->dataIndex != noListSelection) 
                {
                  // lets make sure!!!
                  if (FrmCustomAlert(removeROMAlert, 
                                     globals->dataList1[globals->dataIndex],
                                     NULL, NULL) == 0) 
                  {
                    Char    dbName[32];
                    UInt16  card;
                    LocalID dbID;
                    Boolean deleteOk;

                    deleteOk = true;

                    //
                    // remove the gameboy ROM
                    //

                    card = 0;
                    StrPrintF(dbName, "Gmbt_%s", 
                              globals->dataList1[globals->dataIndex]);
                    dbID = DmFindDatabase(card, dbName);
                    if (dbID != NULL)
                      deleteOk &= (DmDeleteDatabase(card, dbID) == errNone);
                    else
                      deleteOk &= false;

                    //
                    // remove the saved game state for this ROM (if available)
                    //

                    StrPrintF(dbName, "Gmbt_!SAVE!_%s", 
                              globals->dataList1[globals->dataIndex]);
                    dbID = DmFindDatabase(card, dbName);
                    if ((dbID != NULL) && (deleteOk))
                      deleteOk &= (DmDeleteDatabase(card, dbID) == errNone);

                    //
                    // remove the config state for this ROM (if available)
                    //

                    StrPrintF(dbName, "Gmbt_CONFIG_%s", 
                              globals->dataList1[globals->dataIndex]);
                    dbID = DmFindDatabase(card, dbName);
                    if ((dbID != NULL) && (deleteOk))
                      deleteOk &= (DmDeleteDatabase(card, dbID) == errNone);

                    // something bad go wrong?
                    if (!deleteOk)
                      FrmAlert(deleteErrorAlert);

                    // generate the ROM list
                    {
                      EventType event;
                    
                      MemSet(&event, sizeof(EventType), 0);
                      event.eType = appGenerateROMList;
                      EvtAddEventToQueue(&event);
                    }
                  }
                }
                else 
                  SndPlaySystemSound(sndError);

                processed = true;
                break;

           case mainMenuItemSavedGames:
                ApplicationDisplayDialog(saveForm);

                processed = true;
                break;

           default:
                break;
         }
         break;

    case penDownEvent:
         {
           FormType      *frm;
           ScrollBarType *sclBar;
           RectangleType rctBounds, rctClip;
           UInt16        val, min, max, pge;

           frm = FrmGetActiveForm();
           WinGetClip(&rctClip);

           // get the "scrollbar" values
           sclBar =  
             (ScrollBarType *)FrmGetObjectPtr(frm, 
               FrmGetObjectIndex(frm, mainFormScrollBar)); 
           SclGetScrollBar(sclBar, &val, &min, &max, &pge);

           // get the "bounding" box of the gadget
           FrmGetObjectBounds(frm,
             FrmGetObjectIndex(frm, mainFormGameGadget), &rctBounds);
           WinSetClip(&rctBounds);

           // is the "pen" within the list?
           if (RctPtInRectangle(event->screenX, event->screenY, &rctBounds)) 
           {
             UInt16 newValue;

             newValue = val +
                        (event->screenY - rctBounds.topLeft.y) / 
                        FntLineHeight();
             if (
                 (newValue < globals->dataListCount) &&
                 (newValue != globals->dataIndex)
                ) 
             {
               // turn off the old value the "selected" item
               rctBounds.topLeft.y += FntLineHeight() * 
                                      (globals->dataIndex - val);
               rctBounds.extent.y   = FntLineHeight();
               WinInvertRectangle(&rctBounds, 0); 

               // invert the "selected" item
               rctBounds.topLeft.y += FntLineHeight() * 
                                      (newValue - globals->dataIndex);
               rctBounds.extent.y   = FntLineHeight();
               WinInvertRectangle(&rctBounds, 0); 

               globals->dataIndex = newValue;
             }
           }

           WinSetClip(&rctClip);
         }
         break;

    case keyDownEvent:

         switch (event->data.keyDown.chr)
         {
           case chrLineFeed:
#ifdef SONY
           case vchrJogPress:                  // jog dial press: sony
#endif
#ifdef HANDERA
           case chrCarriageReturn:             // jog dial press: handera
#endif

                // regenerate menu event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType            = menuEvent;
                  event.data.menu.itemID = mainMenuItemPlay;
                  EvtAddEventToQueue(&event);
                }           
                processed = true;
                break;

           case pageUpChr:
#ifdef SONY
           case vchrJogUp:                    // jog dial up: sony
#endif
#ifdef HANDERA
           case vchrPrevField:                // jog dial up: handera
#endif
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  RectangleType rctBounds, rctClip;
                  UInt16        val, min, max, pge;

                  frm = FrmGetActiveForm();
                  WinGetClip(&rctClip);

                  sclBar = 
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, mainFormScrollBar));
                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);

                  // are we @ top? if so, move scroll bar up one
                  if (globals->dataIndex == val) 
                  {
                    // lets make sure we can do this
                    if (val > 0) 
                    {
                      // move up!
                      globals->dataIndex = --val;
                      SclSetScrollBar(sclBar, val, min, max, pge);

                      // redraw the display
                      {
                        EventType event;
             
                        MemSet(&event, sizeof(EventType), 0);
                        event.eType = appRedrawEvent;
                        EvtAddEventToQueue(&event);
                      }
                    }
                  }
 
                  // ok, just move "current" selection
                  else 
                  {
                    // get the "bounding" box of the gadget
                    FrmGetObjectBounds(frm,
                      FrmGetObjectIndex(frm, mainFormGameGadget), &rctBounds);
                    WinSetClip(&rctBounds);

                    // turn off the old value the "selected" item
                    rctBounds.topLeft.y += FntLineHeight() * 
                                           (globals->dataIndex - val);
                    rctBounds.extent.y   = FntLineHeight();
                    WinInvertRectangle(&rctBounds, 0); 

                    globals->dataIndex--;

                    // invert the "selected" item
                    rctBounds.topLeft.y -= FntLineHeight();
                    WinInvertRectangle(&rctBounds, 0); 
                  }

                  WinSetClip(&rctClip);
                }
                processed = true;
                break;

           case pageDownChr:
#ifdef SONY
           case vchrJogDown:                  // jog dial down: sony
#endif
#ifdef HANDERA
           case vchrNextField:                // jog dial down: handera
#endif
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  RectangleType rctBounds, rctClip;
                  UInt16        val, min, max, pge;

                  frm = FrmGetActiveForm();
                  WinGetClip(&rctClip);

                  sclBar = 
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, mainFormScrollBar));
                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);

                  // are we @ bottom? if so, move scroll bar down one
                  if (globals->dataIndex == (val + (pge-1))) 
                  {
                    // lets make sure we can do this
                    if (val < max) 
                    {
                      // move down
                      val++; globals->dataIndex++; 
                      SclSetScrollBar(sclBar, val, min, max, pge);

                      // redraw the display
                      {
                        EventType event;
             
                        MemSet(&event, sizeof(EventType), 0);
                        event.eType = appRedrawEvent;
                        EvtAddEventToQueue(&event);
                      }
                    }
                  }
 
                  // ok, just move "current" selection
                  else 
                  if (globals->dataIndex < (globals->dataListCount-1)) 
                  {
                    // get the "bounding" box of the gadget
                    FrmGetObjectBounds(frm,
                      FrmGetObjectIndex(frm, mainFormGameGadget), &rctBounds);
                    WinSetClip(&rctBounds);

                    // turn off the old value the "selected" item
                    rctBounds.topLeft.y += FntLineHeight() * 
                                           (globals->dataIndex - val);
                    rctBounds.extent.y   = FntLineHeight();
                    WinInvertRectangle(&rctBounds, 0); 

                    globals->dataIndex++;

                    // invert the "selected" item
                    rctBounds.topLeft.y += FntLineHeight();
                    WinInvertRectangle(&rctBounds, 0); 
                  }

                  WinSetClip(&rctClip);
                }
                processed = true;
                break;

           default:

                ch = (event->data.keyDown.chr & ~0x20); // turn on the shift
                if ((ch >= 'A') && (ch <= 'Z')) 
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  UInt16        val, min, max, pge;
                  RectangleType rctBounds, rctClip;
                  UInt16        jumpIndex, i;

                  frm = FrmGetActiveForm();
                  WinGetClip(&rctClip);

                  sclBar = 
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, mainFormScrollBar));
                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);

                  // lets try and find a match :)
                  jumpIndex = noListSelection;
                  for (i=0; i<globals->dataListCount; i++) 
                  {
                    if (StrNCaselessCompare(globals->dataList1[i], &ch, 1) == 0)
                    {
                      jumpIndex = i; break; 
                    }
                  }

                  // we found a match, lets adjust :)
                  if ((jumpIndex != noListSelection) &&
                      (jumpIndex != globals->dataIndex))
                  {
                    // need to adjust the scroll view point
                    if ((jumpIndex <  val) ||
                        (jumpIndex >= (val+pge)))
                    {
                      globals->dataIndex = jumpIndex;
                      val                = jumpIndex;
                      if (val > max)
                        val = max;

                      // adjust
                      SclSetScrollBar(sclBar, val, min, max, pge);

                      // redraw the display
                      {
                        EventType event;
           
                        MemSet(&event, sizeof(EventType), 0);
                        event.eType = appRedrawEvent;
                        EvtAddEventToQueue(&event);
                      }
                    }

                    // erase old, and highlight new
                    else
                    {
                      // get the "bounding" box of the gadget
                      FrmGetObjectBounds(frm,
                        FrmGetObjectIndex(frm, mainFormGameGadget), &rctBounds);
                      WinSetClip(&rctBounds);

                      // turn off the old value the "selected" item
                      rctBounds.topLeft.y += FntLineHeight() * 
                                             (globals->dataIndex - val);
                      rctBounds.extent.y   = FntLineHeight();
                      WinInvertRectangle(&rctBounds, 0); 

                      // invert the "selected" item
                      rctBounds.topLeft.y += FntLineHeight() * 
                                             (jumpIndex - globals->dataIndex);
                      WinInvertRectangle(&rctBounds, 0); 

                      // this is our new index :) save :P
                      globals->dataIndex = jumpIndex;
                    }
                  }
                  else
                    SndPlaySystemSound(sndError);

                  WinSetClip(&rctClip);

                  processed = true;
                }
                break;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case mainFormHelpButton:

                // regenerate menu event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType            = menuEvent;
                  event.data.menu.itemID = menuItemHelp;
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           case mainFormAboutButton:

                // regenerate menu event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType            = menuEvent;
                  event.data.menu.itemID = menuItemAbout;
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           case mainFormPlayButton:

                // we should "turn" it off :)) it is not a pushbutton here
                CtlSetValue(event->data.ctlSelect.pControl, 0);
                CtlDrawControl(event->data.ctlSelect.pControl);

                // regenerate menu event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType            = menuEvent;
                  event.data.menu.itemID = mainMenuItemPlay;
                  EvtAddEventToQueue(&event);
                }           
                processed = true;
                break;

           default:
                break;
         }
         break;

    case sclRepeatEvent:
         {
           FormType      *frm;
           ScrollBarType *sclBar;
           UInt16        val, min, max, pge;

           frm = FrmGetActiveForm();
           sclBar = 
             (ScrollBarType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, mainFormScrollBar));

           SclGetScrollBar(sclBar, &val, &min, &max, &pge);

           // redraw the display
           {
             EventType event;
             
             MemSet(&event, sizeof(EventType), 0);
             event.eType = appRedrawEvent;
             EvtAddEventToQueue(&event);
           }
         }
         break;

    case frmCloseEvent:

         // clean up memory
         {
           UInt8 i;

           // free memory allocated
           for (i=0; i<MAX_LISTITEMS; i++) {
             MemPtrFree(globals->dataList1[i]);
             MemPtrFree(globals->dataList2[i]);
           }
           MemPtrFree(globals->dataList1);
           MemPtrFree(globals->dataList2);
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:gameForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
gameFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:

         // adjust the "configurations" stuff
         {
	   UInt16  card;
	   LocalID dbID;
	   Char    strGBRomName[17];
	   Char    strConfigName[32];

           MemSet(strGBRomName, 17, 0);
           StrNCopy(strGBRomName, globals->prefs->game.strGBRomName+5, 16);
	   StrPrintF(strConfigName, "Gmbt_CONFIG_%s", strGBRomName);

           globals->config = (ConfigType *)MemPtrNew(sizeof(ConfigType));

	   card = 0;
	   dbID = DmFindDatabase(card, strConfigName);
	   if (dbID == NULL) 
           {
	     MemMove(globals->config, 
                     &globals->prefs->config, sizeof(ConfigType));
           }
	   else 
           {
             DmOpenRef dbRef;
	     MemHandle binHandle;
	     UInt16    index;
	     Boolean   loadOk;

             // open it for reading
	     dbRef = DmOpenDatabase(card, dbID, dmModeReadOnly);

             loadOk = true;

             // load it
	     index = 0;
	     binHandle = DmGetRecord(dbRef, index);

	     if ((binHandle != NULL) && 
                 (MemHandleSize(binHandle) == sizeof(ConfigType))) 
             {
               MemMove((MemPtr)globals->config,
                       (MemPtr)MemHandleLock(binHandle), sizeof(ConfigType));
               MemHandleUnlock(binHandle);
	       DmReleaseRecord(dbRef, index, false);
             }
	     else loadOk &= false;

	     // close it
	     DmCloseDatabase(dbRef);

             // did something go wrong? if so, default :)
	     if (!loadOk)
	       MemMove(globals->config, 
                       &globals->prefs->config, sizeof(ConfigType));
	   }
         }

         {
           FormType *frm = FrmGetActiveForm();
           UInt16   menuID;
           UInt16   errCode;

           // dynamically adjust the menu :)
           menuID = gameMenu_nogray;
                if (DeviceSupportsGrayscale())   menuID = gameMenu_gray;
           else if (DeviceSupportsGrayPalette()) menuID = gameMenu_graycolor;
           FrmSetMenu(frm, menuID);

           // lets try and initialize the game environemnt
           errCode = GameInitialize(globals->prefs);

           // nothing went wrong?
           if (errCode == errNone) 
           {
             globals->prefs->game.keyMaskSpecial = 0;
             globals->prefs->game.gamePaused     = false;
             globals->prefs->game.gamePlaying    = true;

             // are we supposed to restore this game?
             if (globals->prefs->game.restoring)
               GameRestore(globals->prefs);
             globals->prefs->game.restoring = false;

             FrmDrawForm(FrmGetActiveForm());

             // adjust the "configurations" stuff - as appropriate
             if (DeviceSupportsGrayscale()) 
             {
               DeviceGrayscale(graySet,
                               &globals->config->lgray,
                               &globals->config->dgray);
             }
             else
             if (DeviceSupportsGrayPalette()) 
             {
               DeviceGrayPalette(palSet,
                                 globals->config->lRGB, 
                                 globals->config->dRGB);
             }
           }

           // oops.. act appropriately
           else 
           {
             // what went wrong?
             switch (errCode) 
             {
               case 1: ApplicationDisplayDialog(xmemForm);  // no memory
                    break;

               case 2: ApplicationDisplayDialog(xromForm);  // bad rom
                    break;
                  
               default:
                    break;
             }

             // get out :)
             {
               EventType event;

               MemSet(&event, sizeof(EventType), 0);
               event.eType = menuEvent;
               event.data.frmClose.formID = gameMenuItemExit;
               EvtAddEventToQueue(&event);
             }
           }
         }
         processed = true;
         break;

    case menuEvent:

         // what menu?
         switch (event->data.menu.itemID)
         {
           case gameMenuItemPause:
                globals->prefs->game.gamePaused = 
                  !globals->prefs->game.gamePaused;

                processed = true;
                break;

           case gameMenuItemPressKeys:

                globals->prefs->game.keyMaskSpecial |= keyBitHard1;
                globals->prefs->game.keyMaskSpecial |= keyBitHard2;
                globals->prefs->game.keyMaskSpecial |= keyBitHard3;
                globals->prefs->game.keyMaskSpecial |= keyBitHard4;
                globals->prefs->game.keyMaskSpecial |= keyBitPageUp;
                globals->prefs->game.keyMaskSpecial |= keyBitPageDown;
                globals->prefs->game.keyMaskSpecial |= keyBitCradle;
                globals->prefs->game.keyMaskSpecial |= keyBitAntenna;

                processed = true;
                break;

           case gameMenuItemSaveGame:
                GameSave(globals->prefs);

                processed = true;
                break;

           case gameMenuItemReset:
                GameReset();
                globals->prefs->game.keyMaskSpecial = 0;
                globals->prefs->game.gamePaused     = false;

                processed = true;
                break;

           case gameMenuItemExit:
                globals->prefs->game.gamePlaying    = false;
                FrmGotoForm(mainForm);

                processed = true;
                break;

           default:
                break;
         }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case gameFormStartButton:
                {
                  // since we MASK these, it is a good "fake"
                  globals->prefs->game.keyMaskSpecial |= keyBitCradle;
                }
                processed = true;
                break;

           case gameFormSelectButton:
                {
                  // since we MASK these, it is a good "fake"
                  globals->prefs->game.keyMaskSpecial |= keyBitAntenna;
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case keyDownEvent:

         // ignore hard keys [sometimes they get through]
         if ((event->data.keyDown.modifiers & commandKeyMask) &&
             (TxtCharIsHardKey(event->data.keyDown.modifiers,
                               event->data.keyDown.chr)))
         {
           switch (event->data.keyDown.chr)
           {
             case vchrHard1:
             case vchrHard2:
             case vchrHard3:
             case vchrHard4:
                  processed = true;
                  break;

             default:
                  break;
           }

           goto KEYDOWN_ABORT;
         }

#ifdef PALM_5WAY
         // palmos 5.0+ [5-way dpad]
         if (EvtKeydownIsVirtual(event) && IsFiveWayNavEvent(event))
         {
           if (event->data.keyDown.keyCode & (navBitUp | navChangeUp))
           {
             globals->keyStateDPAD |= globals->prefs->config.ctlKeyUp;
           }
           if (event->data.keyDown.keyCode & (navBitDown | navChangeDown))
           {
             globals->keyStateDPAD |= globals->prefs->config.ctlKeyDown;
           }
           if (event->data.keyDown.keyCode & (navBitLeft | navChangeLeft))
           {
             globals->keyStateDPAD |= globals->prefs->config.ctlKeyLeft;
           }
           if (event->data.keyDown.keyCode & (navBitRight | navChangeRight))
           {
             globals->keyStateDPAD |= globals->prefs->config.ctlKeyRight;
           }
           if (event->data.keyDown.keyCode & (navBitSelect | navChangeSelect))
           {
             // ignore it :)
           }
           processed = true;
           
           goto KEYDOWN_ABORT;
         }
#endif

         switch (event->data.keyDown.chr)
         {
           case findChr:

                // are we supposed to use them in the app?
                if (globals->config->cfgUseFindCalc) 
                {
                  SndPlaySystemSound(sndClick);

                  // redirect the event
                  {
                    EventType event;

                    MemSet(&event, sizeof(EventType), 0);
                    event.eType = ctlSelectEvent;
                    event.data.ctlSelect.controlID = gameFormStartButton;
                    EvtAddEventToQueue(&event);
                  }
                  processed = true;
                }
                break;

           case calcChr:

                // are we supposed to use them in the app?
                if (globals->config->cfgUseFindCalc) 
                {
                  SndPlaySystemSound(sndClick);

                  // redirect the event
                  {
                    EventType event;

                    MemSet(&event, sizeof(EventType), 0);
                    event.eType = ctlSelectEvent;
                    event.data.ctlSelect.controlID = gameFormSelectButton;
                    EvtAddEventToQueue(&event);
                  }
                  processed = true;
                }
                break;

           default:
                break;
         }

KEYDOWN_ABORT:
         
         break;

    case penDownEvent:
    case penMoveEvent:

         // NOTE: the following code is "generic", however, may not always
         //       be the "perfect" definition of the  areas that should be
	 //       for the "stylus" area. give and take, for speed issues.
         //
         //                                               Aaron Ardiri, 2000

         {
	   RectangleType penRegion = { {32,170}, {96,48} };

           // did the user tap in the "control" area?
	   if (RctPtInRectangle(event->screenX, event->screenY, &penRegion)) 
           {
             Coord         x, y;
             RectangleType rctUp        = { { 51,170}, { 19, 16} };
             RectangleType rctDown      = { { 51,202}, { 19, 16} };
             RectangleType rctLeft      = { { 32,186}, { 19, 16} };
             RectangleType rctRight     = { { 70,186}, { 19, 16} };
             RectangleType rctUpLeft    = { { 32,170}, { 19, 16} }; 
             RectangleType rctUpRight   = { { 70,170}, { 19, 16} };
             RectangleType rctDownLeft  = { { 32,202}, { 19, 16} };
             RectangleType rctDownRight = { { 70,202}, { 19, 16} };
             RectangleType rctButtonA   = { { 89,170}, { 40, 24} };
             RectangleType rctButtonB   = { { 89,194}, { 40, 24} };

             x = event->screenX;
             y = event->screenY;

	     if (RctPtInRectangle(x, y, &rctUp))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyUp;
             else
	     if (RctPtInRectangle(x, y, &rctDown))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyDown;
             else
	     if (RctPtInRectangle(x, y, &rctLeft))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyLeft;
             else
	     if (RctPtInRectangle(x, y, &rctRight))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyRight;
             else
	     if (RctPtInRectangle(x, y, &rctUpRight))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyUp | 
		 globals->config->ctlKeyRight;
             else
	     if (RctPtInRectangle(x, y, &rctUpLeft))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyUp | 
		 globals->config->ctlKeyLeft;
             else
	     if (RctPtInRectangle(x, y, &rctDownRight))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyDown | 
		 globals->config->ctlKeyRight;
             else
	     if (RctPtInRectangle(x, y, &rctDownLeft))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyDown | 
		 globals->config->ctlKeyLeft;
             else
	     if (RctPtInRectangle(x, y, &rctButtonA))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyButtonA;
             else
	     if (RctPtInRectangle(x, y, &rctButtonB))
               globals->prefs->game.keyMaskSpecial |= 
	         globals->config->ctlKeyButtonB;

             // we have handled this event, lets continue
             processed = true;
           }
         }
	 break;

    case nilEvent:

         // make sure the active window is the form 
         if (WinGetActiveWindow() == (WinHandle)FrmGetActiveForm()) 
         {
           UInt32 keyState = KeyCurrentState();

#ifdef PALM_5WAY
           keyState = keyState | globals->keyStateDPAD;
           if (keyState & keyBitNavLRS)
           {
             if (keyState & keyBitNavLeft)   keyState |= globals->prefs->config.ctlKeyLeft;
             if (keyState & keyBitNavRight)  keyState |= globals->prefs->config.ctlKeyRight;
           }
           globals->keyStateDPAD = 0;           
#endif

           // mask out keys we dont want (from system)
           keyState = (keyState & 0x0007e) | 
                      globals->prefs->game.keyMaskSpecial;
           globals->prefs->game.keyMaskSpecial = 0; // reset it

           // play the game!
           GameEmulation(globals->prefs, globals->config, keyState);
         }
         processed = true;
         break;

    case frmCloseEvent:

         // adjust the "configurations" stuff
         {
	   UInt16  card;
	   LocalID dbID;
	   Char    strGBRomName[17];
	   Char    strConfigName[32];

           MemSet(strGBRomName, 17, 0);
           StrNCopy(strGBRomName, globals->prefs->game.strGBRomName+5, 16);
	   StrPrintF(strConfigName, "Gmbt_CONFIG_%s", strGBRomName);

           // remove if available
	   card = 0;
	   dbID = DmFindDatabase(card, strConfigName);
	   if (dbID != NULL) DmDeleteDatabase(card, dbID);

	   // save the configuration 
	   card = 0;
	   if (DmCreateDatabase(card, strConfigName,
                                appCreator, cfgType, false) == errNone) 
           {
             DmOpenRef dbRef;
	     MemHandle binHandle;
	     UInt16    index;
	     Boolean   saveOk;
#ifdef SET_BACKUPBIT
             UInt16    dbAttributes;
#endif

	     dbID  = DmFindDatabase(card, strConfigName);
	     dbRef = DmOpenDatabase(card, dbID, dmModeReadWrite);

             saveOk = true;

             // save it
	     index = 0;
	     binHandle = DmNewRecord(dbRef, &index, sizeof(ConfigType));
	     if (binHandle != NULL) 
             {
	       DmWrite((MemPtr)MemHandleLock(binHandle), 0,
                       (MemPtr)globals->config, sizeof(ConfigType));
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
	     if (!saveOk) DmDeleteDatabase(card, dbID);
           }

           // clean up
           MemPtrFree(globals->config);
           
           // adjust the "configurations" stuff [default config]
           globals->config = &(globals->prefs->config);
	 }

         // terminate the game environemnt
         GameTerminate(globals->prefs);

         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:cfigForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
cfigFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         // which configuration are we dealing with here?
	 { 
	   Char   str[32];
           Coord  x;
	   FontID currFont = FntGetFont();
	     
	   StrCopy(str, "- DEFAULT CONFIG -");
           if (globals->prefs->game.gamePlaying)
	     StrPrintF(str, "- %s -", globals->prefs->game.strGBRomName + 5);

	   FntSetFont(boldFont);
           x = (160 - FntCharsWidth(str, StrLen(str))) >> 1;
	   WinDrawChars(str, StrLen(str), x, 12);
	   FntSetFont(currFont);
         }

         // adjust "controls" on the form
         {
           FormType    *frm;
           UInt16      index;
           ListType    *lstHard1, *lstHard2, *lstHard3, *lstHard4;
           ListType    *lstUp, *lstDown;
           ControlType *ctlHard1, *ctlHard2, *ctlHard3, *ctlHard4;
           ControlType *ctlUp, *ctlDown;
           ListType    *lstFrameBlit;
           ControlType *ctlCalcFind, *ctlCPUCycle, *ctlFrameBlit;
           ControlType *ctlSoundVolume, *ctlSoundMute;
           ControlType *ctlSoundChannel1, *ctlSoundChannel2;
           UInt16      *choices[] = { 
                                      &(globals->config->ctlKeyLeft), 
                                      &(globals->config->ctlKeyRight), 
                                      &(globals->config->ctlKeyUp), 
                                      &(globals->config->ctlKeyDown), 
                                      &(globals->config->ctlKeyButtonA),
                                      &(globals->config->ctlKeyButtonB) 
                                    };

           frm = FrmGetActiveForm();
           ctlHard1 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey1Trigger));
           ctlHard2 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey2Trigger));
           ctlHard3 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey3Trigger));
           ctlHard4 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey4Trigger));
           ctlUp =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageUpTrigger));
           ctlDown =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageDownTrigger));
         
           lstHard1 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey1List));
           lstHard2 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey2List));
           lstHard3 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey3List));
           lstHard4 =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormHardKey4List));
           lstUp =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageUpList));
           lstDown =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormPageDownList));

           LstSetSelection(lstHard1, 0);
           CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, 0));
           LstSetSelection(lstHard2, 0);
           CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, 0));
           LstSetSelection(lstHard3, 0);
           CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, 0));
           LstSetSelection(lstHard4, 0);
           CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, 0));
           LstSetSelection(lstDown, 0);
           CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, 0));
           LstSetSelection(lstUp, 0);
           CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, 0));

           // show the "current" settings
           for (index=0; index<6; index++) 
           {
             if ((*(choices[index]) & keyBitHard1) != 0) 
             { 
               LstSetSelection(lstHard1, index);
               CtlSetLabel(ctlHard1, LstGetSelectionText(lstHard1, index));
             }  

             if ((*(choices[index]) & keyBitHard2) != 0) 
             { 
               LstSetSelection(lstHard2, index);
               CtlSetLabel(ctlHard2, LstGetSelectionText(lstHard2, index));
             }  

             if ((*(choices[index]) & keyBitHard3) != 0) 
             { 
               LstSetSelection(lstHard3, index);
               CtlSetLabel(ctlHard3, LstGetSelectionText(lstHard3, index));
             }  

             if ((*(choices[index]) & keyBitHard4) != 0) 
             { 
               LstSetSelection(lstHard4, index);
               CtlSetLabel(ctlHard4, LstGetSelectionText(lstHard4, index));
             }  

             if ((*(choices[index]) & keyBitPageUp) != 0) 
             { 
               LstSetSelection(lstUp, index);
               CtlSetLabel(ctlUp, LstGetSelectionText(lstUp, index));
             }  

             if ((*(choices[index]) & keyBitPageDown) != 0) 
             { 
               LstSetSelection(lstDown, index);
               CtlSetLabel(ctlDown, LstGetSelectionText(lstDown, index));
             }  
           } 

           // button redirection state
           ctlCalcFind =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormButtonCheckbox));
           CtlSetValue(ctlCalcFind, 
                       (globals->config->cfgUseFindCalc) ? 1 : 0);

           // cpu cycle skip
           ctlCPUCycle =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormCPUCycleCheckbox));
           CtlSetValue(ctlCPUCycle, 
                       (globals->config->cfgSkipCPUCycles) ? 1 : 0);

           // frame skipping
           ctlFrameBlit =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormBlitTrigger));
           lstFrameBlit =
             (ListType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormBlitList));

           LstSetSelection(lstFrameBlit, globals->config->cfgFrameBlit);
           CtlSetLabel(ctlFrameBlit, 
             LstGetSelectionText(lstFrameBlit, 
               globals->config->cfgFrameBlit));

           // sound configuration
           ctlSoundVolume =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 cfigFormSound0Button + globals->config->cfgSoundVolume));
           CtlSetValue(ctlSoundVolume, 1);

           ctlSoundMute =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormMuteCheckbox));
           CtlSetValue(ctlSoundMute, 
                       (globals->config->cfgSoundMute) ? 1 : 0);

           ctlSoundChannel1 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormChannel1Checkbox));
           CtlSetValue(ctlSoundChannel1, 
                       (globals->config->cfgSoundChannel1) ? 1 : 0);
           ctlSoundChannel2 =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, cfigFormChannel2Checkbox));
           CtlSetValue(ctlSoundChannel2, 
                       (globals->config->cfgSoundChannel2) ? 1 : 0);
         }
         processed = true;
         break;

    case ctlEnterEvent:

         switch (event->data.ctlEnter.controlID) 
         {
	   case cfigFormSound0Button:
	   case cfigFormSound1Button:
	   case cfigFormSound2Button:
	   case cfigFormSound3Button:
                {
                  ControlType *newCtl, *oldCtl, *muteCtl;
                  UInt16      objIndex;
                  FormType    *frm = FrmGetActiveForm();

                  newCtl = event->data.ctlEnter.pControl;
                  
                  // we dont want an audible beep from the system
                  objIndex = FrmGetControlGroupSelection(frm, newCtl->group);

                  if (objIndex != frmNoSelectedControl) 
                  {
                    oldCtl = (ControlType *)FrmGetObjectPtr(frm, objIndex);
                    CtlSetValue(oldCtl, 0);
                  }
                  CtlSetValue(newCtl, 1);

                  // act as we needed
                  muteCtl =
                    (ControlType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormMuteCheckbox));

                  // system NOT muted, play sound
                  if (CtlGetValue(muteCtl) == 0) 
                  {
                    SndCommandType testBeep =
		      {sndCmdFreqDurationAmp,0,512,32,sndMaxAmp >>
		       (cfigFormSound3Button-event->data.ctlEnter.controlID)};
                    SndDoCmd(0,&testBeep,0);
		  }
		}

                processed = true;
                break;

	   default:
                break;
	 }
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case cfigFormOkButton:
                {
                  ListType    *lstHard1, *lstHard2, *lstHard3, *lstHard4;
                  ListType    *lstUp, *lstDown;
                  ListType    *lstFrameBlit;
                  ControlType *ctlCalcFind, *ctlCPUCycle;
                  ControlType *ctlSoundVolume, *ctlSoundMute;
                  ControlType *ctlSoundChannel1, *ctlSoundChannel2;
                  UInt16      index;
                  FormType    *frm;
                  UInt16      keySignature;
                  UInt16      *choices[] = { 
                                       &(globals->config->ctlKeyLeft), 
                                       &(globals->config->ctlKeyRight), 
                                       &(globals->config->ctlKeyUp), 
                                       &(globals->config->ctlKeyDown), 
                                       &(globals->config->ctlKeyButtonA),
                                       &(globals->config->ctlKeyButtonB) 
                                           };

                  frm = FrmGetActiveForm();
                  lstHard1 = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey1List));
                  lstHard2 = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey2List));
                  lstHard3 = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey3List));
                  lstHard4 = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormHardKey4List));
                  lstUp = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormPageUpList));
                  lstDown = 
                    (ListType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, cfigFormPageDownList));

                  keySignature = 
                    (
                      (0x01 << LstGetSelection(lstHard1)) |
                      (0x01 << LstGetSelection(lstHard2)) |
                      (0x01 << LstGetSelection(lstHard3)) |
                      (0x01 << LstGetSelection(lstHard4)) |
                      (0x01 << LstGetSelection(lstUp))    |
                      (0x01 << LstGetSelection(lstDown))
                    );

                  // only process if good setting is selected.
                  if (keySignature == 0x3F) 
                  {
                    // key preferences
                    for (index=0; index<6; index++) 
                    {
                      *(choices[index]) = 0;
                    }
                    if (LstGetSelection(lstHard1) != noListSelection) 
                      *(choices[LstGetSelection(lstHard1)]) |= keyBitHard1;
                    if (LstGetSelection(lstHard2) != noListSelection) 
                      *(choices[LstGetSelection(lstHard2)]) |= keyBitHard2;
                    if (LstGetSelection(lstHard3) != noListSelection) 
                      *(choices[LstGetSelection(lstHard3)]) |= keyBitHard3;
                    if (LstGetSelection(lstHard4) != noListSelection) 
                      *(choices[LstGetSelection(lstHard4)]) |= keyBitHard4;
                    if (LstGetSelection(lstUp) != noListSelection) 
                      *(choices[LstGetSelection(lstUp)])    |= keyBitPageUp;
                    if (LstGetSelection(lstDown) != noListSelection) 
                      *(choices[LstGetSelection(lstDown)])  |= keyBitPageDown;

                    // button redir
                    ctlCalcFind =
                      (ControlType *)FrmGetObjectPtr(frm,
                         FrmGetObjectIndex(frm, cfigFormButtonCheckbox));
                    globals->config->cfgUseFindCalc = 
                      (CtlGetValue(ctlCalcFind) == 1);

                    // cpu cycle skip
                    ctlCPUCycle =
                      (ControlType *)FrmGetObjectPtr(frm,
                         FrmGetObjectIndex(frm, cfigFormCPUCycleCheckbox));
                    globals->config->cfgSkipCPUCycles = 
                      (CtlGetValue(ctlCPUCycle) == 1);

                    // frame skipping
                    lstFrameBlit =
                      (ListType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, cfigFormBlitList));
                    globals->config->cfgFrameBlit = 
                      LstGetSelection(lstFrameBlit);

                    // sound configuration
                    for (index=0; index<4; index++) 
                    {
                      ctlSoundVolume =
                        (ControlType *)FrmGetObjectPtr(frm,
                          FrmGetObjectIndex(frm, 
                            cfigFormSound0Button + index));

                      if (CtlGetValue(ctlSoundVolume) == 1) 
                        globals->config->cfgSoundVolume = index; 
                    }

                    ctlSoundMute =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, cfigFormMuteCheckbox));
                    globals->config->cfgSoundMute = 
                      (CtlGetValue(ctlSoundMute) == 1);

                    ctlSoundChannel1 =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, cfigFormChannel1Checkbox));
                    globals->config->cfgSoundChannel1 = 
                      (CtlGetValue(ctlSoundChannel1) == 1);

                    ctlSoundChannel2 =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, cfigFormChannel2Checkbox));
                    globals->config->cfgSoundChannel2 = 
                      (CtlGetValue(ctlSoundChannel2) == 1);

                    // send a close event
                    {
                      EventType event;

                      MemSet(&event, sizeof(EventType), 0);
                      event.eType = frmCloseEvent;
                      event.data.frmClose.formID = FrmGetActiveFormID();
                      EvtAddEventToQueue(&event);
                    }
                  }
                  else 
                    SndPlaySystemSound(sndError);
                }
                processed = true;
                break;

           case cfigFormCancelButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:infoForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
infoFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case infoFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:dvlpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
dvlpFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case dvlpFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:grayForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
grayFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         // which configuration are we dealing with here?
	 { 
	   Char   str[32];
           Coord  x;
	   FontID currFont = FntGetFont();
	     
	   StrCopy(str, "- DEFAULT CONFIG -");
           if (globals->prefs->game.gamePlaying)
	     StrPrintF(str, "- %s -", globals->prefs->game.strGBRomName + 5);

	   FntSetFont(boldFont);
           x = (160 - FntCharsWidth(str, StrLen(str))) >> 1;
	   WinDrawChars(str, StrLen(str), x, 14);
	   FntSetFont(currFont);
         }

         // adjust "controls" on the form
         {
           FormType    *frm;
           ControlType *ctlWhite, *ctlLGray, *ctlDGray, *ctlBlack;

           frm = FrmGetActiveForm();

           ctlWhite =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, grayWhite1Button));
           ctlLGray =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm,
                 grayLightGray1Button + globals->config->lgray));
           ctlDGray =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm,
                 grayDarkGray1Button + globals->config->dgray));
           ctlBlack =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, grayBlack7Button));

           CtlSetValue(ctlWhite, 1); CtlDrawControl(ctlWhite);
           CtlSetValue(ctlLGray, 1); CtlDrawControl(ctlLGray);
           CtlSetValue(ctlDGray, 1); CtlDrawControl(ctlDGray);
           CtlSetValue(ctlBlack, 1); CtlDrawControl(ctlBlack);
         }

         // pre 3.5 - we must 'redraw' form to actually display PUSHBUTTONS
         if (!DeviceSupportsVersion(romVersion3_5)) 
           FrmDrawForm(FrmGetActiveForm());

         processed = true;
         break;

    case ctlEnterEvent:

         switch (event->data.ctlEnter.controlID) 
         {
           case grayLightGray1Button:
           case grayLightGray7Button:
           case grayDarkGray1Button:
           case grayDarkGray7Button:

           // stupid user, they must select one of the other options
           SndPlaySystemSound(sndError);
           processed = true;
         }
         break;

    case ctlSelectEvent:
         
         switch (event->data.ctlSelect.controlID) 
         {
           case grayLightGray2Button:
           case grayLightGray3Button:
           case grayLightGray4Button:
           case grayLightGray5Button:
           case grayLightGray6Button:

                globals->config->lgray = event->data.ctlEnter.controlID -
                                           grayLightGray1Button;
                DeviceGrayscale(graySet, 
                                &globals->config->lgray, 
                                &globals->config->dgray);
                processed = true;
                break;

           case grayDarkGray2Button:
           case grayDarkGray3Button:
           case grayDarkGray4Button:
           case grayDarkGray5Button:
           case grayDarkGray6Button:

                globals->config->dgray = event->data.ctlEnter.controlID -
                                           grayDarkGray1Button;
                DeviceGrayscale(graySet, 
                                &globals->config->lgray, 
                                &globals->config->dgray);
                processed = true;
                break;

           case grayFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;
               
           default:
                break;
         } 
         break;

    default:
         break;
  }

  return processed;
} 

/**
 * The Form:gcolForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean
gcolFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType)
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         // which configuration are we dealing with here?
	 { 
	   Char   str[32];
           Coord  x;
	   FontID currFont = FntGetFont();
	     
	   StrCopy(str, "- DEFAULT CONFIG -");
           if (globals->prefs->game.gamePlaying)
	     StrPrintF(str, "- %s -", globals->prefs->game.strGBRomName + 5);

	   FntSetFont(boldFont);
           x = (160 - FntCharsWidth(str, StrLen(str))) >> 1;
	   WinDrawChars(str, StrLen(str), x, 14);
	   FntSetFont(currFont);
         }

         // adjust "controls" on the form
         {
           FormType    *frm;
           ControlType *ctlRed[2], *ctlGreen[2], *ctlBlue[2];

           frm = FrmGetActiveForm();

           ctlRed[0] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormLRButton0 + globals->config->lRGB[0]));
           ctlGreen[0] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormLGButton0 + globals->config->lRGB[1]));
           ctlBlue[0] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormLBButton0 + globals->config->lRGB[2]));
           ctlRed[1] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormDRButton0 + globals->config->dRGB[0]));
           ctlGreen[1] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormDGButton0 + globals->config->dRGB[1]));
           ctlBlue[1] =
             (ControlType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, 
                 gcolFormDBButton0 + globals->config->dRGB[2]));

           CtlSetValue(ctlRed[0], 1);   CtlDrawControl(ctlRed[0]);
           CtlSetValue(ctlGreen[0], 1); CtlDrawControl(ctlGreen[0]);
           CtlSetValue(ctlBlue[0], 1);  CtlDrawControl(ctlBlue[0]);
           CtlSetValue(ctlRed[1], 1);   CtlDrawControl(ctlRed[1]);
           CtlSetValue(ctlGreen[1], 1); CtlDrawControl(ctlGreen[1]);
           CtlSetValue(ctlBlue[1], 1);  CtlDrawControl(ctlBlue[1]);
         }

         // pre 3.5 - we must 'redraw' form to actually display PUSHBUTTONS
         if (!DeviceSupportsVersion(romVersion3_5)) 
           FrmDrawForm(FrmGetActiveForm());

         processed = true;
         break;

    case ctlSelectEvent:
         
         switch (event->data.ctlSelect.controlID) 
         {
           case gcolFormLRButton0: case gcolFormLRButton1: 
           case gcolFormLRButton2: case gcolFormLRButton3: 
           case gcolFormLRButton4: case gcolFormLRButton5:
           case gcolFormLRButton6: case gcolFormLRButton7: 
           case gcolFormLRButton8: case gcolFormLGButton0: 
           case gcolFormLGButton1: case gcolFormLGButton2: 
           case gcolFormLGButton3: case gcolFormLGButton4: 
           case gcolFormLGButton5: case gcolFormLGButton6: 
           case gcolFormLGButton7: case gcolFormLGButton8:
           case gcolFormLBButton0: case gcolFormLBButton1: 
           case gcolFormLBButton2: case gcolFormLBButton3: 
           case gcolFormLBButton4: case gcolFormLBButton5:
           case gcolFormLBButton6: case gcolFormLBButton7: 
           case gcolFormLBButton8: case gcolFormDRButton0: 
           case gcolFormDRButton1: case gcolFormDRButton2: 
           case gcolFormDRButton3: case gcolFormDRButton4: 
           case gcolFormDRButton5: case gcolFormDRButton6: 
           case gcolFormDRButton7: case gcolFormDRButton8:
           case gcolFormDGButton0: case gcolFormDGButton1: 
           case gcolFormDGButton2: case gcolFormDGButton3: 
           case gcolFormDGButton4: case gcolFormDGButton5:
           case gcolFormDGButton6: case gcolFormDGButton7: 
           case gcolFormDGButton8: case gcolFormDBButton0: 
           case gcolFormDBButton1: case gcolFormDBButton2: 
           case gcolFormDBButton3: case gcolFormDBButton4: 
           case gcolFormDBButton5: case gcolFormDBButton6: 
           case gcolFormDBButton7: case gcolFormDBButton8:

                // determine the changes 
                {
                  FormType    *frm;
                  ControlType *ctl;
                  Int16       index = 0;

                  frm = FrmGetActiveForm();

                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormLRButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->lRGB[0] = index - 1; index = 0;
                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormLGButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->lRGB[1] = index - 1; index = 0;
                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormLBButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->lRGB[2] = index - 1; index = 0;
                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormDRButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->dRGB[0] = index - 1; index = 0;
                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormDGButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->dRGB[1] = index - 1; index = 0;
                  do 
                  {
                    ctl =
                      (ControlType *)FrmGetObjectPtr(frm,
                        FrmGetObjectIndex(frm, gcolFormDBButton0 + index++));
                  } while (CtlGetValue(ctl) != 1);
                  globals->config->dRGB[2] = index - 1; index = 0;
                }

                // apply the changes 
                DeviceGrayPalette(palSet,
                                  globals->config->lRGB, 
                                  globals->config->dRGB);

                processed = true;
                break;

           case gcolFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;
               
           default:
                break;
         } 
         break;

    default:
         break;
  }

  return processed;
} 

#ifdef PROTECTION_ON
/**
 * The Form:regiForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
regiFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         // display the HotSync username on the screen (in HEX)
         {
           Char  ID[40];
           Char  tmp[10], num[3];
           Coord x;
           UInt8 i, checksum;

           FontID font = FntGetFont();

           // initialize
           StrCopy(ID, "");

           // generate strings
           checksum = 0;
           for (i=0; i<MAX_IDLENGTH; i++) 
           {
             checksum ^= (UInt8)globals->prefs->system.hotSyncUsername[i];
             StrIToH(tmp, (UInt8)globals->prefs->system.hotSyncUsername[i]);
             StrNCopy(num, &tmp[StrLen(tmp)-2], 2); num[2] = '\0';
             StrCat(ID, num); StrCat(ID, ":");
           }
           StrIToH(tmp, checksum);
           StrNCopy(num, &tmp[StrLen(tmp)-2], 2); num[2] = '\0';
           StrCat(ID, num);

           FntSetFont(boldFont);
           x = (160 - FntCharsWidth(ID, StrLen(ID))) >> 1;
           WinDrawChars(ID, StrLen(ID), x, 68);
           FntSetFont(font);
         }
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case regiFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }

                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:rbugForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
rbugFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case rbugFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}
#endif

/**
 * The Form:helpForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
helpFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         {
           UInt16 helpHeight;

           helpHeight = InitInstructions();

           // help exists?
           if (helpHeight != 0)
           {
             FormType      *frm;
             ScrollBarType *sclBar;
             UInt16        val, min, max, pge;

             frm    = FrmGetActiveForm();
             FrmDrawForm(frm);

             sclBar =
               (ScrollBarType *)FrmGetObjectPtr(frm,
                 FrmGetObjectIndex(frm, helpFormScrollBar));

             SclGetScrollBar(sclBar, &val, &min, &max, &pge);
             val = helpHeight;
             max = (val > pge) ? (val-(pge+16)) : 0;
             SclSetScrollBar(sclBar, 0, min, max, pge);

             DrawInstructions(0);
           }

           // no help, close form
           else
           {
             EventType newEvent;

             MemSet(&newEvent, sizeof(EventType), 0);
             newEvent.eType = frmCloseEvent;
             newEvent.data.frmClose.formID = FrmGetActiveFormID();
             EvtAddEventToQueue(&newEvent);
           }
         }
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case helpFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case sclRepeatEvent:
         {
           FormType      *frm;
           ScrollBarType *sclBar;
           UInt16        val, min, max, pge;

           frm = FrmGetActiveForm();
           sclBar = 
             (ScrollBarType *)FrmGetObjectPtr(frm,
               FrmGetObjectIndex(frm, helpFormScrollBar));

           SclGetScrollBar(sclBar, &val, &min, &max, &pge);
           DrawInstructions(val);
         }
         break;

    case keyDownEvent:

         switch (event->data.keyDown.chr)
         {
           case pageUpChr:
#ifdef SONY
           case vchrJogUp:                    // jog dial up: sony
#endif
#ifdef HANDERA
           case vchrPrevField:                // jog dial up: handera
#endif
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  UInt16        val, min, max, pge;

                  frm = FrmGetActiveForm();
                  sclBar = 
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, helpFormScrollBar));

                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);
                  val = (pge > val) ? 0 : (val-pge); 
                  SclSetScrollBar(sclBar, val, min, max, pge);
                  DrawInstructions(val);
                }
                processed = true;
                break;

           case pageDownChr:
#ifdef SONY
           case vchrJogDown:                  // jog dial down: sony
#endif
#ifdef HANDERA
           case vchrNextField:                // jog dial down: handera
#endif
                {
                  FormType      *frm;
                  ScrollBarType *sclBar;
                  UInt16        val, min, max, pge;

                  frm = FrmGetActiveForm();
                  sclBar = 
                    (ScrollBarType *)FrmGetObjectPtr(frm,
                      FrmGetObjectIndex(frm, helpFormScrollBar));

                  SclGetScrollBar(sclBar, &val, &min, &max, &pge);
                  val = (max < (val+pge)) ? max : (val+pge); 
                  SclSetScrollBar(sclBar, val, min, max, pge);
                  DrawInstructions(val);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case frmCloseEvent:

         // clean up
         QuitInstructions();
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:xmemForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
xmemFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case xmemFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:warnForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
warnFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         {
           FormType      *frm;
           ControlType   *ctlShowWarn;

           frm = FrmGetActiveForm();
           ctlShowWarn =
             (ControlType *)FrmGetObjectPtr(frm,
                FrmGetObjectIndex(frm, warnFormShowWarnCheckbox));

           CtlSetValue(ctlShowWarn, globals->prefs->system.showWarning ? 1 : 0);
         }
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case warnFormShowWarnCheckbox:
                globals->prefs->system.showWarning = 
                  (CtlGetValue(event->data.ctlEnter.pControl) == 1);

                processed = true;
                break;

           case warnFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:saveForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
saveFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());

         {
           UInt16 i;
           
           globals->dataList3 = 
             (Char **)MemPtrNew(MAX_LISTITEMS * sizeof(Char *));
           for (i=0; i<MAX_LISTITEMS; i++) 
           {
             globals->dataList3[i] = (Char *)MemPtrNew(32 * sizeof(Char));
           }

           // generate the SAVED list
           {
             EventType event;
             
             MemSet(&event, sizeof(EventType), 0);
             event.eType = appGenerateSAVEDList;
             EvtAddEventToQueue(&event);
           }
         }

         processed = true;
         break;
   
    case appGenerateSAVEDList:
         {
           FormType *frm;
           ListType *lstGame;
           UInt16   i, count;

           count = 0;
           {
             DmSearchStateType stateInfo;
             Err               error;
             UInt16            card, pos;
             LocalID           dbID;
             Char              strDatabaseName[32];

             error =
               DmGetNextDatabaseByTypeCreator(true, &stateInfo,
                                              saveType, appCreator, false,
                                              &card, &dbID);

             while ((error == 0) && (count < MAX_LISTITEMS)) 
             {
               // extract the database information
               DmDatabaseInfo(card, dbID, strDatabaseName, 
                              NULL, NULL, NULL, NULL, NULL,
                              NULL, NULL, NULL, NULL, NULL);
  
               // strip the "Gmbt_!SAVE!_" from the name
               i = 0;
               while ((strDatabaseName[i+12] != '\0')) 
               {
                 strDatabaseName[i] = strDatabaseName[i+12]; i++;
               }
               strDatabaseName[i] = '\0';

               // find out where it should go
               pos = 0;
               while ((pos < count) &&
                      (StrCompare(strDatabaseName, 
                                  globals->dataList3[pos]) > 0)) 
               {
                 pos++;
               }

               // do we need to shift a few things?
               if (pos < count) 
               {
                 // move em down
                 for (i=count; i>pos; i--) 
                   StrCopy(globals->dataList3[i], globals->dataList3[i-1]);
               }

               // copy it to the list
               StrCopy(globals->dataList3[pos], strDatabaseName);

               // next sequence
               error =
                 DmGetNextDatabaseByTypeCreator(false, &stateInfo,
                                                saveType, appCreator, false,
                                                &card, &dbID);
               count++;
             }
           }

           // adjust the lists
           frm = FrmGetActiveForm();

           lstGame =
             (ListType *)FrmGetObjectPtr(frm,
                FrmGetObjectIndex(frm, saveFormSavedList));

           LstSetListChoices(lstGame, globals->dataList3, count);
           LstSetSelection(lstGame, (count != 0) ? 0 : noListSelection);
           LstDrawList(lstGame);

           // make sure the "freespaces" are empty! :)
           for (; count<MAX_LISTITEMS; count++) 
             MemSet(globals->dataList3[count], 32 * sizeof(Char), 0);
         }
         processed = true;
         break;

    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case saveFormRestoreButton:
                {
                  FormType *frm;
                  ListType *lstGame;

                  // initialize
                  frm = FrmGetActiveForm();
                  lstGame =
                    (ListType *)FrmGetObjectPtr(frm,
                       FrmGetObjectIndex(frm, saveFormSavedList));

                  // was a selection made?
                  if (LstGetSelection(lstGame) != noListSelection) 
                  {
                    Char dbName[32];

                    StrPrintF(dbName, "Gmbt_%s", 
                              LstGetSelectionText(lstGame, 
                                LstGetSelection(lstGame)));

                    // if the game exists, play!
                    if ((DmFindDatabase(0, dbName) != 0) ||
                        (DmFindDatabase(1, dbName) != 0)) 
                    {
                      // lets do a little "32K rom size check"
                      if (
                           rom32KSizeCheck(dbName+5) ||
                           CHECK_SIGNATURE(globals->prefs)
                         ) 
                      {
                        StrCopy(globals->prefs->game.strGBRomName, dbName);

                        // send a close event
                        {
                          EventType event;

                          MemSet(&event, sizeof(EventType), 0);
                          event.eType = frmCloseEvent;
                          event.data.frmClose.formID = FrmGetActiveFormID();
                          EvtAddEventToQueue(&event);
                        }

                        // go to the game form and continue game
                        FrmGotoForm(gameForm);
                        globals->prefs->game.restoring = true;
                      }

                      // "insufficient" privelledges
                      else
                        ApplicationDisplayDialog(rbugForm);
                    }
                    else
                      SndPlaySystemSound(sndError);

                  }
                  else
                    SndPlaySystemSound(sndError);
                }

                processed = true;
                break;

           case saveFormDeleteButton:
                {
                  FormType *frm;
                  ListType *lstGame;

                  // initialize
                  frm = FrmGetActiveForm();
                  lstGame =
                    (ListType *)FrmGetObjectPtr(frm,
                       FrmGetObjectIndex(frm, saveFormSavedList));

                  // was a selection made?
                  if (LstGetSelection(lstGame) != noListSelection) 
                  {
                    Char *gameName;

                    gameName = LstGetSelectionText(lstGame,
                                 LstGetSelection(lstGame));

                    // lets make sure!!!
                    if (FrmCustomAlert(removeSAVEDAlert, 
                                       gameName, NULL, NULL) == 0) 
                    {
                      Char     dbName[32];
                      UInt16   card;
                      LocalID  dbID;
                      Boolean  deleteOk;

                      StrPrintF(dbName, "Gmbt_!SAVE!_%s", gameName); 

                      deleteOk = true;

                      // remove the gameboy saved state
                      card = 0;
                      dbID = DmFindDatabase(card, dbName);
                      if (dbID != NULL)
                        deleteOk &= (DmDeleteDatabase(card, dbID) == errNone);
                      else
                        deleteOk &= false;

                      // something bad go wrong?
                      if (!deleteOk)
                        FrmAlert(deleteErrorAlert);

                      // generate the saved state list
                      {
                        EventType event;
                      
                        MemSet(&event, sizeof(EventType), 0);
                        event.eType = appGenerateSAVEDList;
                        EvtAddEventToQueue(&event);
                      }
                    }
                  }
                  else
                    SndPlaySystemSound(sndError);
                }

                processed = true;
                break;

           case saveFormDoneButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    case frmCloseEvent:

         // clean up memory
         {
           UInt8 i;

           // free memory allocated
           for (i=0; i<MAX_LISTITEMS; i++) 
             MemPtrFree(globals->dataList3[i]);
           MemPtrFree(globals->dataList3);
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Form:xromForm event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
static Boolean 
xromFormEventHandler(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType) 
  {
    case frmOpenEvent:
         FrmDrawForm(FrmGetActiveForm());
         processed = true;
         break;
   
    case ctlSelectEvent:

         switch (event->data.ctlSelect.controlID)
         {
           case xromFormOkButton:

                // send a close event
                {
                  EventType event;

                  MemSet(&event, sizeof(EventType), 0);
                  event.eType = frmCloseEvent;
                  event.data.frmClose.formID = FrmGetActiveFormID();
                  EvtAddEventToQueue(&event);
                }
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * The Palm Computing Platform initialization routine.
 */
void  
InitApplication()
{
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  // load preferences
  {
    Boolean reset; 
    UInt16  prefSize;
    Int16   flag;

    // allocate memory for preferences
    globals->prefs = (PreferencesType *)MemPtrNew(sizeof(PreferencesType));

    // lets see how large the preference is (if it is there)
    reset    = true;
    prefSize = 0;
    flag     = PrefGetAppPreferences(appCreator, 0, NULL, &prefSize, true);

    // ensure the prefs are loaded correctly
    if ((flag != noPreferenceFound) && (prefSize == sizeof(PreferencesType))) 
    {
      // extract all the bytes
      PrefGetAppPreferences(appCreator, 0, globals->prefs, &prefSize, true);

      // lets check if we need to reset
      reset = !CHECK_SIGNATURE(globals->prefs) ||
              (globals->prefs->system.signatureVersion != VERSION);
    }

    // we need to reset the preferences?
    if (reset) 
    {
      // set default values
      prefSize = sizeof(PreferencesType);
      MemSet(globals->prefs, prefSize, 0);

      globals->prefs->system.signatureVersion = VERSION;
      StrCopy(globals->prefs->system.signature, "|HaCkMe|");

      globals->prefs->config.ctlKeyButtonA    = keyBitHard4;
      globals->prefs->config.ctlKeyLeft       = keyBitHard2;
      globals->prefs->config.ctlKeyUp         = keyBitPageUp;
      globals->prefs->config.ctlKeyDown       = keyBitPageDown;
      globals->prefs->config.ctlKeyRight      = keyBitHard3;
      globals->prefs->config.ctlKeyButtonB    = keyBitHard1;

      globals->prefs->config.cfgUseFindCalc   = true;
      globals->prefs->config.cfgSkipCPUCycles = false;
      globals->prefs->config.cfgFrameBlit     = 1;
      globals->prefs->config.cfgSoundVolume   = 3;
      globals->prefs->config.cfgSoundMute     = true;
      globals->prefs->config.cfgSoundChannel1 = true;
      globals->prefs->config.cfgSoundChannel2 = true;

      if (DeviceSupportsGrayscale()) 
      {
        DeviceGrayscale(grayGet,
                        &globals->prefs->config.lgray, 
                        &globals->prefs->config.dgray);
      }
      else
      if (DeviceSupportsGrayPalette()) 
      {
        globals->prefs->config.lRGB[0] = 8;
        globals->prefs->config.lRGB[1] = 7;
        globals->prefs->config.lRGB[2] = 6; // musky yellow

        globals->prefs->config.dRGB[0] = 3;
        globals->prefs->config.dRGB[1] = 4;
        globals->prefs->config.dRGB[2] = 5; // musky blue
      }

      globals->prefs->game.gamePlaying        = false;
      globals->prefs->game.restoring          = false;
      globals->prefs->system.showWarning      = true;
    }
  }

  // get the current grayscale settings - as appropriate
  if (DeviceSupportsGrayscale()) 
    DeviceGrayscale(grayGet, &globals->lgray, &globals->dgray);

  // get the HotSync user name
  globals->prefs->system.hotSyncUsername = 
    (Char *)MemPtrNew(dlkUserNameBufSize * sizeof(Char));
  MemSet(globals->prefs->system.hotSyncUsername, dlkUserNameBufSize, 0);
  DlkGetSyncInfo(NULL, NULL, NULL,
                 globals->prefs->system.hotSyncUsername, NULL, NULL);
  {
    Char *ptrStr;

    ptrStr = StrChr(globals->prefs->system.hotSyncUsername, spaceChr);
    if (ptrStr != NULL) 
    { 
      // erase everything after the FIRST space
      UInt8 index = ((UInt32)ptrStr - 
                     (UInt32)globals->prefs->system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }

    ptrStr = StrChr(globals->prefs->system.hotSyncUsername, '\0');
    if (ptrStr != NULL) 
    { 
      // erase everything after the FIRST null char
      UInt8 index = ((UInt32)ptrStr - 
                     (UInt32)globals->prefs->system.hotSyncUsername);
      MemSet(ptrStr, dlkUserNameBufSize - index, 0);
    }
  }
  
  // setup the valid keys available while the app is running
  KeySetMask(~(keyBitsAll ^ 
             (keyBitPower   | keyBitCradle   | 
              keyBitPageUp  | keyBitPageDown |
#ifdef HANDERA
              keyBitJogUp   | keyBitJogDown  |
#endif
#ifdef PALM_5WAY
              keyBitNavLRS  |
#endif              
              keyBitAntenna | keyBitContrast)));

  // initialize the registration system
  RegisterInitialize(globals->prefs);

  globals->evtTimeOut    = evtWaitForever;
  globals->ticksPerFrame = SysTicksPerSecond() / GAME_FPS;

  // do we new to display the warning statement?
  if (globals->prefs->system.showWarning) {
    ApplicationDisplayDialog(warnForm);
  }

  // goto the appropriate form
  if ((globals->prefs->game.gamePlaying) &&  // ROM still there?
      ((DmFindDatabase(0, globals->prefs->game.strGBRomName) != 0) ||
       (DmFindDatabase(1, globals->prefs->game.strGBRomName) != 0))) 
  {
    // lets do a little "32K rom size check"
    if (
         rom32KSizeCheck(globals->prefs->game.strGBRomName+5) ||
         CHECK_SIGNATURE(globals->prefs)
       ) 
      FrmGotoForm(gameForm);

    // "insufficient" privelledges
    else 
    {
      ApplicationDisplayDialog(rbugForm);

      // we gotta delete the saved game - if it exists :)) 
      {
        UInt16  card;
	LocalID dbID;

	card = 0;
	dbID = DmFindDatabase(card, "Gmbt_!SAVED_GAME!");
	if (dbID != NULL) DmDeleteDatabase(card, dbID);
      }

      // go to the main form 
      FrmGotoForm(mainForm);
      globals->prefs->game.gamePlaying = false;
    }
  }
  else 
  {
    FrmGotoForm(mainForm);
    globals->prefs->game.gamePlaying = false;
  }

  // adjust the "configurations" stuff [default config]
  globals->config = &(globals->prefs->config);
}

/**
 * The Palm Computing Platform entry routine (mainline).
 *
 * @param cmd         a word value specifying the launch code.
 * @param cmdPBP      pointer to a structure associated with the launch code.
 * @param launchFlags additional launch flags.
 * @return zero if launch successful, non zero otherwise.
 */
UInt32  
PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
  UInt32 result = 0;

  // what type of launch was this?
  switch (cmd) 
  {
    case sysAppLaunchCmdNormalLaunch:
         {
           DeviceInitialize();

           // is this device compatable?
           if (DeviceCheckCompatability()) 
           {
#ifndef USE_GLOBALS
             Globals *globals;

             // create the globals object, and register it
             globals = (Globals *)MemPtrNew(sizeof(Globals));
             FtrSet(appCreator, ftrGlobals, (UInt32)globals);
#else
             globals = &gbls;
#endif

             InitApplication();
             EventLoop();
             EndApplication();

#ifndef USE_GLOBALS
             // clean up
             MemPtrFree(globals);

             // unregister the feature
             FtrUnregister(appCreator, ftrGlobals);
#endif
           }

           DeviceTerminate();
         }
         break;

    default:
         break;
  }

  return result;
}

/**
 * The application event handling routine.
 *
 * @param event the event to process.
 * @return true if the event was handled, false otherwise.
 */
Boolean 
ApplicationHandleEvent(EventType *event)
{
  Boolean processed = false;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  switch (event->eType)
  {
    case frmLoadEvent:
         {
           UInt16   formID;
           FormType *frm;

           // get the appropriate form details
           formID = event->data.frmLoad.formID;
           frm    = FrmInitForm(formID);

           FrmSetActiveForm(frm);
           switch (formID) 
           {
             case mainForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)mainFormEventHandler);

                  processed = true;
                  break;

             case gameForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)gameFormEventHandler);

                  processed = true;
                  break;

             case infoForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)infoFormEventHandler);

                  processed = true;
                  break;

             case dvlpForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)dvlpFormEventHandler);

                  processed = true;
                  break;

             case cfigForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)cfigFormEventHandler);

                  processed = true;
                  break;

             case grayForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)grayFormEventHandler);

                  processed = true;
                  break;

             case gcolForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)gcolFormEventHandler);

                  processed = true;
                  break;

#ifdef PROTECTION_ON
             case regiForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)regiFormEventHandler);

                  processed = true;
                  break;

             case rbugForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)rbugFormEventHandler);

                  processed = true;
                  break;
#endif

             case helpForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)helpFormEventHandler);

                  processed = true;
                  break;

             case xmemForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)xmemFormEventHandler);

                  processed = true;
                  break;

             case warnForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)warnFormEventHandler);

                  processed = true;
                  break;

             case saveForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)saveFormEventHandler);

                  processed = true;
                  break;

             case xromForm:
                  FrmSetEventHandler(frm, 
                    (FormEventHandlerPtr)xromFormEventHandler);

                  processed = true;
                  break;

             default:
                  break;
           }
         }
         break;

    case penUpEvent:
    case penDownEvent:
    case penMoveEvent:
         {
           // we have to handle this case specially for the gameForm :))
           if (WinGetActiveWindow() == (WinHandle)FrmGetFormPtr(gameForm)) 
             processed = gameFormEventHandler(event);
         }
         break;

    case keyDownEvent:
         {
           // we have to handle this case specially for the gameForm :))
           if (WinGetActiveWindow() == (WinHandle)FrmGetFormPtr(gameForm))
             processed = gameFormEventHandler(event);

           // we have to handle this case specially for the mainForm :))
           if (WinGetActiveWindow() == (WinHandle)FrmGetFormPtr(mainForm))
             processed = mainFormEventHandler(event);
         }
         break;

    case winEnterEvent:
         {
           if (event->data.winEnter.enterWindow ==
                (WinHandle)FrmGetFormPtr(gameForm)) 
           {

             // when game screen is active, animate
             globals->evtTimeOut = 1;
             processed           = true;
           }
         }
         break;

    case winExitEvent:
         {
           if (event->data.winExit.exitWindow ==
                (WinHandle)FrmGetFormPtr(gameForm)) 
           {
             // when game screen is not active, stop animation
             globals->evtTimeOut = evtWaitForever;
             processed           = true;
           }
         }
         break;
         
    case menuEvent:

         switch (event->data.menu.itemID) 
         {
           case menuItemGrayscale:
                ApplicationDisplayDialog(grayForm);
                processed = true;
                break;

           case menuItemGrayPalette:
                ApplicationDisplayDialog(gcolForm);
                processed = true;
                break;

           case menuItemConfig:
                ApplicationDisplayDialog(cfigForm);
                processed = true;
                break;

#ifdef PROTECTION_ON
           case menuItemRegister:
                ApplicationDisplayDialog(regiForm);
                processed = true;
                break;
#endif

           case menuItemDeveloper:
                ApplicationDisplayDialog(dvlpForm);
                processed = true;
                break;

           case menuItemWarning:
                ApplicationDisplayDialog(warnForm);
                processed = true;
                break;

           case menuItemHelp:
                ApplicationDisplayDialog(helpForm);
                processed = true;
                break;

           case menuItemAbout:
                ApplicationDisplayDialog(infoForm);
                processed = true;
                break;

           default:
                break;
         }
         break;

    default:
         break;
  }

  return processed;
}

/**
 * Display a MODAL dialog to the user (this is a modified FrmDoDialog)
 *
 * @param formID the ID of the form to display.
 */
void
ApplicationDisplayDialog(UInt16 formID)
{
  FormActiveStateType frmCurrState;
  FormType            *frmActive      = NULL;
  WinHandle           winDrawWindow   = NULL;
  WinHandle           winActiveWindow = NULL;
#ifndef USE_GLOBALS
  Globals             *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  // save the active form/window
  if (DeviceSupportsVersion(romVersion3)) 
    FrmSaveActiveState(&frmCurrState);

  else 
  {
    frmActive       = FrmGetActiveForm();
    winDrawWindow   = WinGetDrawWindow();
    winActiveWindow = WinGetActiveWindow();  // < palmos3.0, manual work
  }

  {
    EventType event;
    UInt16    err;
    Boolean   keepFormOpen;

    MemSet(&event, sizeof(EventType), 0);

    // send a load form event
    event.eType = frmLoadEvent;
    event.data.frmLoad.formID = formID;
    EvtAddEventToQueue(&event);

    // send a open form event
    event.eType = frmOpenEvent;
    event.data.frmLoad.formID = formID;
    EvtAddEventToQueue(&event);

    // handle all events here (trap them before the OS does) :)
    keepFormOpen = true;
    while (keepFormOpen) 
    {
      EvtGetEvent(&event, globals->evtTimeOut);

      // this is our exit condition! :)
      keepFormOpen = (event.eType != frmCloseEvent);

      if (!ApplicationHandleEvent(&event))
        if (!SysHandleEvent(&event)) 
          if (!MenuHandleEvent(0, &event, &err)) 
            FrmDispatchEvent(&event);

      if (event.eType == appStopEvent) 
      {
        keepFormOpen = false;
        EvtAddEventToQueue(&event);  // tap "applications", need to exit
      }
    }
  }

  // restore the active form/window
  if (DeviceSupportsVersion(romVersion3)) 
    FrmRestoreActiveState(&frmCurrState);

  else 
  {
    FrmSetActiveForm(frmActive);
    WinSetDrawWindow(winDrawWindow);
    WinSetActiveWindow(winActiveWindow);     // < palmos3.0, manual work
  }

  // redraw the display
  {
    EventType event;
             
    MemSet(&event, sizeof(EventType), 0);
    event.eType = appRedrawEvent;
    EvtAddEventToQueue(&event);
  }
}

/**
 * The Palm Computing Platform event processing loop.
 */
void  
EventLoop()
{
  EventType event;
  UInt16    err;
  FormType  *frm;
#ifndef USE_GLOBALS
  Globals   *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  // reset the timer (just in case)
  globals->timerPointA = TimGetTicks();

  do 
  {
    EvtGetEvent(&event, globals->evtTimeOut);
    frm = FrmGetActiveForm();

    //
    // ANIMATION HANDLING:
    //

    // nil event occured, register time state
    if (
        (globals->config->cfgFrameBlit == 0) &&
        (WinGetActiveWindow() == (WinHandle)frm) &&
        (frm == FrmGetFormPtr(gameForm)) && (event.eType == nilEvent) 
       )
    {
      globals->timerPointA = TimGetTicks();
    }

    //
    // EVENT HANDLING:
    //

    if (!ApplicationHandleEvent(&event)) 
      if (!SysHandleEvent(&event)) 
        if (!MenuHandleEvent(0, &event, &err)) 
          FrmDispatchEvent(&event);

    //
    // ANIMATION HANDLING:
    //

    // on a form that requires animations, calc time since last nilEvent
    if (
        (globals->config->cfgFrameBlit == 0) &&
        (WinGetActiveWindow() == (WinHandle)frm) &&
        (frm == FrmGetFormPtr(gameForm)) 
       )
    {
      globals->timerPointB = TimGetTicks();

      // calculate the delay required
      globals->timerDiff  = (globals->timerPointB - globals->timerPointA);
      globals->evtTimeOut = (globals->timerDiff > globals->ticksPerFrame) ?
        1 : (globals->ticksPerFrame - globals->timerDiff);

      // manually add nilEvent if needed (only when pen held down)
      if ((globals->evtTimeOut <= 1) && (event.eType == penMoveEvent))
      {
        EventType event;

        // lets flush the pen events from the queue (jebus, holding it down)
        EvtFlushPenQueue();

        // insert a nilEvent for renderering :)
        MemSet(&event, sizeof(EventType), 0);
        event.eType = nilEvent;
        EvtAddEventToQueue(&event);
      }
    }
    else
      globals->evtTimeOut  = 1;

  } while (event.eType != appStopEvent);
}

/**
 * The Palm Computing Platform termination routine.
 */
void  
EndApplication()
{
  UInt16  prefSize;
#ifndef USE_GLOBALS
  Globals *globals;

  // get globals reference
  FtrGet(appCreator, ftrGlobals, (UInt32 *)&globals);
#endif

  // ensure all forms are closed
  FrmCloseAllForms();

  // restore the key state
  KeySetMask(keyBitsAll);

  // reset the grayscale settings - as appropriate
  if (DeviceSupportsGrayscale()) 
    DeviceGrayscale(graySet, &globals->lgray, &globals->dgray);

  else
  if (DeviceSupportsGrayPalette()) 
  {
    DeviceGrayPalette(palReset, NULL, NULL);
  }

  // terminate the registration system
  RegisterTerminate();

  // save preferences
  MemPtrFree(globals->prefs->system.hotSyncUsername);
  globals->prefs->system.hotSyncUsername = NULL;

  // lets add our 'check' data chunk
  prefSize = sizeof(PreferencesType);
  StrCopy(globals->prefs->system.signature, "|HaCkMe|");
  PrefSetAppPreferences(appCreator, 0, 1, globals->prefs, prefSize, true);
  MemPtrFree(globals->prefs);
}

/**
 * Check the size of the gameboy rom image.
 *
 * @param romName the name of the rom image.
 * @return true if 32K in size, false otherwise.
 */
static Boolean 
rom32KSizeCheck(Char *romName)
{
  Boolean result = false;
  Char    dbName[32];
  UInt16  card;
  LocalID dbID;

  // generate the database name
  StrPrintF(dbName, "Gmbt_%s", romName);

  // locate it (if it is there)
  card = 0;
  dbID = DmFindDatabase(card, dbName);
  if (dbID == NULL) 
  {
    card = 1;
    dbID = DmFindDatabase(card, dbName);
  }

  // does it exist?
  if (dbID != NULL) 
  {
    UInt32 dbSize;

    // extract the database size
    DmDatabaseSize(card, dbID, NULL, &dbSize, NULL);
  
    // determine the size (round to 8K chunks)
    result = ((dbSize >> 14) << 4) == 32;
  }

  return result;
}
