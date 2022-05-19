/*
 * @(#)register.c
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

// global variable structure
typedef struct
{
  MemHandle adjustGame;
  UInt16    adjustGameSize;
  Boolean   demoMode;
} RegisterGlobals;

#ifdef USE_GLOBALS
// globals object
static RegisterGlobals gbls;
static RegisterGlobals *globals;
#endif

typedef struct
{
  UInt32    keyData;
  UInt8     keyValue;
} KeyPreferencesType;

/**
 * Initialize the registration routines.
 *
 * @param the global preferences structure.
 */
void
RegisterInitialize(PreferencesType *prefs)
{
#ifndef USE_GLOBALS
  RegisterGlobals *globals;

  // create the globals object, and register it
  globals = (RegisterGlobals *)MemPtrNew(sizeof(RegisterGlobals));
  FtrSet(appCreator, ftrRegisterGlobals, (UInt32)globals);
#else
  globals = &gbls;
#endif
  
#ifdef PROTECTION_ON
  // 
  // perform the registration check :))
  //

  StrCopy(prefs->system.signature, "-HaCkMe-");
  {
    MemHandle dataHandle;
    UInt8     *dataChunk;
    Int16     dataChunkSize;
    UInt8     *ptrCodeChunk;
    void      (*regiLoader)(PreferencesType *);

    // extract the "encrypted" code
    dataHandle    = DmGet1Resource('code', 0x0006);  // code0006.bin
    dataChunk     = (UInt8 *)MemHandleLock(dataHandle);
    dataChunkSize = MemHandleSize(dataHandle);
    ptrCodeChunk  = (UInt8 *)MemPtrNew(dataChunkSize);
    MemMove(ptrCodeChunk, dataChunk, dataChunkSize);
    MemHandleUnlock(dataHandle);
    DmReleaseResource(dataHandle);

    // decrypt it
    RegisterDecryptChunk(ptrCodeChunk, dataChunkSize, 0x0002, 0x00);

    // execute the code chunk
    regiLoader = (void *)ptrCodeChunk;
    regiLoader(prefs);

    // we dont need the memory anymore, dispose of it
    MemPtrFree(ptrCodeChunk);
  }
#else
  {
    StrCopy(prefs->system.signature, "|HaCkMe|");
    _regiLoader(prefs);
  }
#endif
}

/**
 * Display the "*UNREGISTERED*" message on the screen if needed.
 *
 * @param the global preferences structure.
 */
void
RegisterShowMessage(PreferencesType *prefs)
{
  const RectangleType rect      = {{0,148},{160,14}};
  const CustomPatternType erase = {0,0,0,0,0,0,0,0};
#ifndef USE_GLOBALS
  RegisterGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrRegisterGlobals, (UInt32 *)&globals);
#endif

  // we MUST be on the main form to do this
  if (FrmGetActiveFormID() == mainForm) 
  {
    Coord x;
    Char  str[32];

    // erase the are behind
    WinSetPattern(&erase);
    WinFillRectangle(&rect, 0);

    // draw our status
    StrCopy(str, (CHECK_SIGNATURE(prefs))
#ifdef LANG_de
            ? (globals->demoMode 
               ? "- DEMO VERSION -" : "- REGISTRIERTE VERSION -")
            : "* NICHT REGISTRIERT *"                             // german
#else
 #ifdef LANG_fr
            ? (globals->demoMode 
               ? "- VERSION DE DEMO -" : "- VERSION ENREGISTRE -")
            : "* NON ENREGISTRE *"                                  // generic

 #else
  #ifdef LANG_es
            ? (globals->demoMode 
               ? "- VERSION DE DEMO -" : "- VERSION REGISTRADA -")
            : "* SIN REGISTRAR *"                                  // generic

  #else
            ? (globals->demoMode 
               ? "- DEMO VERSION -" : "- REGISTERED VERSION -")
            : "* UNREGISTERED *"                                  // generic
#endif
 #endif
#endif
            );
    x = (rect.extent.x - FntCharsWidth(str, StrLen(str))) >> 1;

    WinDrawChars(str, StrLen(str), x, rect.topLeft.y);
  }
}

/**
 * Adjust the game status that is "sensitive" to cracking
 * 
 * @param prefs the global preferences type.
 * @param adjustType the adjustment definition.
 * @return true if the adjustment was performed, false otherwise.
 */
Boolean
RegisterAdjustGame(PreferencesType    *prefs,
                   GameAdjustmentType *adjustType)
{
  Boolean result = false;
#ifndef USE_GLOBALS
  RegisterGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrRegisterGlobals, (UInt32 *)&globals);
#endif

  // this stuff can only be done if the user is registered
  if (CHECK_SIGNATURE(prefs)) 
  {
    Boolean (*adjustGame)(PreferencesType *, GameAdjustmentType *);

    // perform the 'adjustment' :)
    adjustGame = (void *)MemHandleLock(globals->adjustGame);
    result     = adjustGame(prefs, adjustType);
    MemHandleUnlock(globals->adjustGame);
  }

  return result;
}

/**
 * Generate a checksum of a data chunk.
 *
 * @param dataChunk a pointer to the chunk of data to decrypt.
 * @param dataChunkSize the size of the chunk of data to decrypt.
 */
UInt8
RegisterChecksum(UInt8  *dataChunk, 
                 UInt16 dataChunkSize) 
{
  UInt8  key;
  UInt16 i;

  // calculate a checksum of the data chunk
  key = 0;
  for (i=0; i<dataChunkSize; i++) 
    key ^= dataChunk[i];

  return key;
}

/**
 * Decrypt a data chunk using an RC4 style algorithm.
 *
 * @param dataChunk a pointer to the chunk of data to decrypt.
 * @param dataChunkSize the size of the chunk of data to decrypt.
 * @param keyResource the resource ID of the key to be used in decryption. 
 * @param key the starting key value, zero if autogenerated.
 */
void
RegisterDecryptChunk(UInt8 *dataChunk, 
                     Int16 dataChunkSize,
                     Int16 keyResource,
                     UInt8 key) 
{
  MemHandle       keyChunkHandle;
  UInt8           *keyChunk;
  Int16           i, index, keyChunkSize;

  // 
  // NOTE: the decryption/encryption is similar to the RC4 (USA military)
  //       method for securing data. RC4 can be applied twice to get the
  //       original data (as it is an XOR based algorithm). The 'key' used
  //       by this algorithm is the "register" code segment. After each 
  //       byte is adjusted, the key is dynamically updated using a simple
  //       "windowing" effect.
  //
  //       *all* code used for registration purposes is placed in the 
  //       "register" code segment. patching this segment will create an
  //       invalid 'key' data set and decryption will result in the Palm
  //       device giving FATAL EXCEPTIONS upon execution of the data chunks.
  //
  //                                                      Aaron Ardiri, 2000

  keyChunkHandle = DmGet1Resource('code', keyResource);
  keyChunkSize   = MemHandleSize(keyChunkHandle);
  keyChunk       = (UInt8 *)MemPtrNew(keyChunkSize);
  MemMove(keyChunk, (UInt8 *)MemHandleLock(keyChunkHandle), keyChunkSize);
  MemHandleUnlock(keyChunkHandle);
  DmReleaseResource(keyChunkHandle);

  // what is our starting value?
  key = (key == 0) ? RegisterChecksum(keyChunk, keyChunkSize) : key;

  // decrypt our data chunk, using an RC4 level algorithm
  index = key;
  for (i=0; i<dataChunkSize; i++) 
  {
    // adjust the byte
    dataChunk[i] ^= key;

    // dynamically update the key
    do 
    {
      index = (index + key + 1) % keyChunkSize;
      key   = keyChunk[index];
    } while (key == 0);

    keyChunk[index] = (UInt8)((key + index) & 0xff);
  }

  // clean up
  MemPtrFree(keyChunk);
}

/**
 * Terminate the registration routines.
 */
void
RegisterTerminate()
{
#ifndef USE_GLOBALS
  RegisterGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrRegisterGlobals, (UInt32 *)&globals);
#endif

  // clean up memory
  if (globals->adjustGame)
    MemHandleFree(globals->adjustGame);

#ifndef USE_GLOBALS
  MemPtrFree(globals);

  // unregister global data
  FtrUnregister(appCreator, ftrRegisterGlobals);
#endif
}

/**
 * Perform the necessary data adjustments and verify the existance
 * of a registered "keygen.pdb" file and set the registration flag
 * appropriately.
 *
 * @param prefs the global preferences type.
 */
void
_regiLoader(PreferencesType *prefs)
{
#ifndef USE_GLOBALS
  RegisterGlobals *globals;

  // get a globals reference
  FtrGet(appCreator, ftrRegisterGlobals, (UInt32 *)&globals);
#endif

  // lets assume there is no keygen.pdb file
  globals->adjustGame = NULL;
  globals->demoMode   = false;

#ifdef PROTECTION_ON
  {
    DmOpenRef keygenDB;

    // try and open the database
    keygenDB = DmOpenDatabaseByTypeCreator('_key',appCreator,dmModeReadOnly);
    if (keygenDB != NULL) 
    {
      MemHandle recordH;
      UInt8     *recordChunk, key;
      UInt16    i, recordChunkSize;

      // get a reference to the first record
      recordH = DmGetRecord(keygenDB, 0);

      // process the record
      recordChunkSize = MemHandleSize(recordH);
      recordChunk     = (UInt8 *)MemPtrNew(recordChunkSize);
      MemMove(recordChunk, MemHandleLock(recordH), recordChunkSize);
      MemHandleUnlock(recordH);

      // decrypt it (based on the @message file)
      key = 0;
      for (i=0; i<MAX_IDLENGTH; i++) 
        key += prefs->system.hotSyncUsername[i];
      key = (key ^ 0xff) & 0xff;
      if (key == 0) key = 0x20; // key *cannot* be zero

      RegisterDecryptChunk(recordChunk, recordChunkSize, 0x0007, key);

      // extract the 'decryption' signature
      StrNCopy(prefs->system.signature, (Char *)recordChunk, 16);
      prefs->system.signature[15] = '\0';

      // signature bad? lets try loading "@DEMO@" (key = 0x5A) keygen
      if (!CHECK_SIGNATURE(prefs)) 
      {
        KeyPreferencesType *keyPrefs;
        UInt16             prefSize;
        Int16              flag;

        // allocate memory for preferences
        keyPrefs = (KeyPreferencesType *)MemPtrNew(sizeof(KeyPreferencesType));
   
        // lets see how large the prefernces is (if it is there)
        prefSize = 0;
        flag     = PrefGetAppPreferences(keyCreator, 0, NULL, &prefSize, true);

        // prefs available?
        if ((flag != noPreferenceFound) && 
            (prefSize == sizeof(KeyPreferencesType))) 
        {
          PrefGetAppPreferences(keyCreator, 0, keyPrefs, &prefSize, true);

          // validation check
          if ((keyPrefs->keyData == 0) && (keyPrefs->keyValue > 0))
            keyPrefs->keyValue--;
          else
            keyPrefs->keyValue = 0;
        }

        // no prefs, initialize them
        else  
        {
          keyPrefs->keyData  = 0;
          keyPrefs->keyValue = 30;
        }

        // we "can" do it (not expired)
        if (keyPrefs->keyValue != 0) 
        {
          MemMove(recordChunk, MemHandleLock(recordH), recordChunkSize);
          MemHandleUnlock(recordH);
          RegisterDecryptChunk(recordChunk, recordChunkSize, 0x0007, 0x5A);

          // extract the 'decryption' signature
          StrNCopy(prefs->system.signature, (Char *)recordChunk, 16);
          prefs->system.signature[15] = '\0';
      
          // are we running in "DEMO" mode?
          globals->demoMode = (CHECK_SIGNATURE(prefs));
        }

        // save the current settings
        prefSize = sizeof(KeyPreferencesType);
        PrefSetAppPreferences(keyCreator, 0, 1, keyPrefs, prefSize, true);
        MemPtrFree(keyPrefs);
      }

      // if the signature is good, lets configure everything we need
      if (CHECK_SIGNATURE(prefs)) 
      {
        globals->adjustGameSize = recordChunkSize - 16;
        globals->adjustGame     = MemHandleNew(globals->adjustGameSize);
        MemMove(MemHandleLock(globals->adjustGame), 
                recordChunk+16, globals->adjustGameSize);
        MemHandleUnlock(globals->adjustGame);
      }

      // release the resource
      MemPtrFree(recordChunk);
      DmReleaseRecord(keygenDB, 0, false);

      DmCloseDatabase(keygenDB);
    }
  }
#else
  {
    MemHandle codeH = DmGet1Resource('code', 0x0007);
    globals->adjustGameSize = MemHandleSize(codeH);
    globals->adjustGame     = MemHandleNew(globals->adjustGameSize);
    MemMove(MemHandleLock(globals->adjustGame),
            MemHandleLock(codeH), globals->adjustGameSize);
    MemHandleUnlock(globals->adjustGame);
    MemHandleUnlock(codeH);
  }
#endif
}

/**
 * Adjust the game status that is "sensitive" to cracking
 *
 * @param prefs the global preferences type.
 * @param adjustType the adjustment definition.
 * @return true if the adjustment was performed, false otherwise.
 */
Boolean
_adjustGame(PreferencesType    *prefs,
            GameAdjustmentType *adjustType) 
{
  Boolean result = true;

  // what task do we need to do?
  switch (adjustType->adjustMode) 
  {
    case gameLoadROM:
         {
           UInt16 i;

           // load and lock X ROM chunks
           for (i=2; i<adjustType->data.loadROM.pageCount; i++) 
             adjustType->data.loadROM.ptrPages[i] = 
               (UInt8 *)MemHandleLock(DmGetResource(datType, i));
         }
         break;

    case gameFreeROM:
         {
           UInt16 i;

           // unlock X ROM chunks
           for (i=2; i<adjustType->data.freeROM.pageCount; i++) 
             MemPtrUnlock(adjustType->data.freeROM.ptrPages[i]);
         }
         break;

    default:
         result = false;
         break;
  }

  return result;
}
