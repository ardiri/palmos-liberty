/***************************************************************
*
*  Project:
*	Handspring Common Includes
*
*  Copyright info:
*	Copyright 1998 Handspring, Inc. All Rights Reserved.   
*
*
*  FileName:
*	HsExt.h
* 
*  Description:
*	Public header file for the Handspring extensions to PalmOS
*
*  ToDo:
* 
*  History:
*	13-Jan-1999 RM - Created by Ron Marianetti
*	06-Apr-2000 BP - Modified to support new gcc compiler
*			 with multisegment support
*	13-Jul-2000 BP - Modified to support compiling apps with 
*			 the 3.1 sdk 
****************************************************************/

#ifndef	  __HSEXT_H__
#define	  __HSEXT_H__

#include <PalmTypes.h>

// Handspring Database creator IDs 
// ------------------------------------------------------------------
// Apps						  // Reg : Comment
// ------------------------------------------------------------------
#define hsFileCAdvCalculator		'HsPr'	  // Yes : Advanced Calculator application
#define hsFileCCardSetup		'HsCd'	  // Yes : ALL card setup utilities have
						  //	  this creator
#define	hsFileCSampleCardWelcome	'CWel'	  // Yes : Sample card welcome app
#define	hsFileCHandTerminal		'HTrm'	  // Yes : HandTerminal application sample code

#define hsFileCFileMover		'FlMv'	  // Yes : FileMover application (for flash cards)

#define hsFileCFileInstaller		'FlIn'	  // Yes : FileInstaller application (for flash cards)

#define hsFileCDateBook3H		'HsDB'	  // Yes : Handspring's version of Datebk3.

#define hsFileCBackup			'HsBk'	  // Yes : Serial Flash Backup application
#define hsFileCBackupWelcome		'HsBw'	  // Yes : Not used

#define hsFileCPhone			'HsPh'	  // Yes : Phone application
#define hsFileCSMS			'SMSa'	  // No:  Phone SMS app (Short Message Service)

#define hsFileCImagine			'HsIm'    // Yes : Imagine (imaging app)

#define hsFileCAddrBookExtensions       'HsAX'    // Yes : Handspring address book extensions
#define hsFileCDialingPanel             'HsDi'    // Yes : Dialing Panel...
#define hsFileCAddrBookOneFingerMode	'HsAl'	  // Yes : Address book modifications - one finger mode

#define	hsFileCPinballGame		'HsPg'	  // Yes: Pinball color game

// Testing apps...
#define hsFileCLangSpoofer		'HsLS'    // Yes : Language spoofer
#define hsFileCAddrTester		'HsAT'    // Yes : Address Tester
#define hsFileCMailWrapper              'HsMW'    // Yes : Mail Wrapper...
#define hsFileCUsbTester                'HsU2'    // No  : Usb tester...
#define hsFileCPhoneWrapper		'HsPW'	  // Yes : Phone Wrapper

#define hsFileCUseSerial		'HSUS'	  // Yes : App to signal FlashPrep for serial use

// ------- Types
#define	hsFileTCardSetup		'HsSU'	  // N/A : ALL card setup utilities have this
						  //		type
#define	hsFileTCardSetupShell		'HApp'	  // N/A : Card Setup Shell application type

// Creators for things other than apps...
#define hsFileCAutomaticHelper          'Auto'    // Yes : Apps that handle the "automatic"
                                                  //       choice for dialing.
// ------------------------------------------------------------------
// Panels					  // Reg : Comment
// ------------------------------------------------------------------

#define hsFileCPhoneServicesPanel	'hsPS'	  // No: Phone preference panel

// ------------------------------------------------------------------
// Patches
// ------------------------------------------------------------------
//#define hsFileC....

// ------- Types
#define	hsFileTHandspringHal		'HwAl'	  // N/A : File type of HAL.prc
#define	hsFileTHandspringPatch		'HsPt'	  // N/A  : File type of HsExtensions.prc

#define	hsFileTHandspringUIObj		'HsUi'	  // N/A  : File type of HsExtUIRsc.prc

// ------------------------------------------------------------------
// Extensions
// ------------------------------------------------------------------
#define hsFileCHandspringExt		'HsEx'	  // Yes : Handspring extensions

// ------------------------------------------------------------------
// Libraries
// ------------------------------------------------------------------
#define hsFileCSampleSerLib		'HsCs'	  // Yes : Sample Serial 16650 library
#define hsFileCTaskLib                  'HsTS'    // Yes: Task library
#define hsFileCFfsCSegLib		'FfMg'	  // Yes : Flash File System code segment
						  //		library
#define hsFileCLdbMgr			'HsLm'	  // Yes : Launcher Database Manager library
#define hsFileCModemSerLib		'HsMs'	  // Yes : Modem Module Serial library

// Driver database creator and driver creator for the built-in Serial driver
// IMPORTANT: There is a "feature" in the Connection Panel that makes it
//  always pick the first serial driver as the default method when creating
//  a new connection and assuming the first method supports modems. 
// So... we make sure the creator of the built-in serial comes first in
//  the directory listing by giving it a creator ID that sorts before
//  all the other serial driver creator IDs (hsFileCUsbLib, hsFileCSerDrvrAuto, 
//	sysFileCVirtIrComm). 
#define	hsFileCSerLib			'A8xZ'	  // Yes  : SerialHWMgr driver ID

// Driver database creator for the USB drivers
#define	hsFileCUsbLib			'HsUs'	  // Yes : USB Library database creator
// These driver ID's are installed by the USB database
#define	hsFileCSerDrvrUSBConsole	'HsUc'	  // Yes : SerialHWMgr driver ID
#define	hsFileCSerDrvrUSBDesktop	'HsUd'	  // Yes : SerialHWMgr driver ID
#define	hsFileCSerDrvrUSBRemFS		'HsUr'	  // Yes : SerialHWMgr driver ID
#define	hsFileCSerDrvrUSBGeneric	'HsUg'	  // Yes : SerialHWMgr driver ID
#define	hsFileCSerDrvrUSBGeneric2	'HsUh'	  // Yes : SerialHWMgr driver ID

// Driver database creator and driver creator for the auto-detect virtual driver
#define	hsFileCSerDrvrAuto		'HsSA'	  // Yes  : SerialHWMgr driver ID

// ------- Types
#define hsFileTCodeSeg			'CSeg'	  // N/A  : Code segment library

// ------------------------------------------------------------------
// ROM tokens
// ------------------------------------------------------------------
#define hsFileCFFSInfoTok1		'Ffs1'	  // Yes : Flash file system info
						  //	    ROM Token string; see
						  //	    FfsMgrPrv.h for format
						  //	    details (FOR HANDSPRING, INC.
						  //	    USE ONLY!)

#define hsFileCFfsCrType		'Ffs2'	  // Yes : ascii string of colon-separated
// creator and type of the module's Flash File System Manager. This value
// MUST be unique for each Flash File System Manager implementation. This
// token provides a level of indirction that allows applications to locate
// the provider of FfsMgr services on any module that supports this
// architecture and is fully compatible with the Handspring, Inc.'s FfsMgr
// API. The format is "CCCC:TTTT", where CCCC is the unique 4-character
// creator ID of your FfsMgr registred with Palm Computing, and TTTT is the
// 4-character type id of your FfsMgr (must be mixed case ascii). (for example,
// the reserved value used by Handspring, Inc. implementation is "FfMg:CSeg")

#define	hsFileCCardAccessTime		'HsAT'	  // Yes : ascii decimal string of
						  //	    access time in nano-seconds

#define	hsFileCCardWelcomeDuringReset	'HsWR'	  // Yes : actual value ignored. Presence
						  //		means launch welcome app during reset

#define	hsFileCCardWelcomeAppTypeCreator 'HsWT'	  // Yes : type & creator of card welcome app
						  // ONLY AVAILABLE IN VERSION 3.5 AND LATER
						  //       Ex: 	-tokStr "HsWT" "applMYAP"  

#define hsFileCSmartSmallromToken	'HsSR'	  // Yes: defines that the given smallrom is
						  //      smart about running from RAM--it will
						  //      auto-drop into the appropriate debugger.

//................................................................
// Miscellaneous database types
//................................................................

#define hsFileTModemSetupPatch		'HsMp'	  // N/A  : The Patch for the Modem Module's
						  //	     CardSetup app has this type and
						  //	     the same creator as the Modem
						  //	     Module's serial library (hsFileCModemSerLib)

//................................................................
// System Resource types and IDs
//................................................................
//#define	hsResT....

//=============================================================================
// Handspring API Equates
//=============================================================================

// This is the Handspring feature id. Apps can tell if they're on
//  a handspring device if they get 0 err back from: 
//		err = FtrGet (hsFtrCreator, hsFtrIDVersion, &value)
#define	hsFtrCreator				 'hsEx'

// 0xMMmfsbbb, where MM is major version, m is minor version
// f is bug fix, s is stage: 3-release,2-beta,1-alpha,0-development,
// bbb is build number for non-releases 
// V1.12b3   would be: 0x01122003
// V2.00a2   would be: 0x02001002
// V1.01     would be: 0x01013000
#define	hsFtrIDVersion				  0		 

// Modification date of Handspring extensions
#define	hsFtrIDModDate				  1		 

// Feature number indicating that the Launcher Database Mgr library is loaded.
//  The value of the feature is the refNum of the loaded library.
//  Call FtrGet (hsFtrCreator, hsFtrIDLdbMgrLibRefNum, ...) to get this feature.
#define	hsFtrIDLdbMgrLibRefNum		  2

// If this feature is present, then we won't use the optimization to grab the
// memory semaphore before doing multiple data manager calls.
#define hsFtrIDNoMemSemaphore         3

// ------------------------------------------------------------------------------
// Error codes
// ------------------------------------------------------------------------------
#define	hsErrorClass				  (appErrorClass+0x0100)
#define	hsErrNotSupported			  (hsErrorClass | 1)
#define	hsErrInvalidCardNum			  (hsErrorClass | 2)
#define	hsErrReadOnly				  (hsErrorClass | 3)
#define	hsErrInvalidParam			  (hsErrorClass | 4)
#define	hsErrBufferTooSmall			  (hsErrorClass | 5)
#define	hsErrInvalidCardHdr			  (hsErrorClass | 6)
#define	hsErrCardPatchAlreadyInstalled  	  (hsErrorClass | 7)
#define	hsErrCardPatchNotInstalled	  	  (hsErrorClass | 8)
#define	hsErrNotEnoughPower			  (hsErrorClass | 9)
#define	hsErrCardNotInstalled		  	  (hsErrorClass | 10)

// ------------------------------------------------------------------------------
//  Key codes and events
// ------------------------------------------------------------------------------

// Max card user event number that can be passed to HsCardEventPost()
#define	hsMaxCardEvent				  0x07

// Max app user event number that can be passed to HsAppEventPost()
#define	hsMaxAppEvent				  0x03

// Keycode range assigned to us from 3Com: vchrSlinkyMin to vchrSlinkyMax
#define	hsChrRangeMin				  0x1600
#define	hsChrRangeMax				  0x16FF

// New key codes we generate for the "dot" soft icons
#define hsChrMidLeftDot				  hsChrRangeMin
#define hsChrMidRightDot			  (hsChrRangeMin+1)
#define hsChrBotLeftDot				  (hsChrRangeMin+2)
#define hsChrBotRightDot			  (hsChrRangeMin+3)

// The virtual cradle 2 character i
#define	hsChrCradle2OnChr			  (hsChrRangeMin+4)	  // dock input level asserted
#define	hsChrCradle2OffChr			  (hsChrRangeMin+5)	  // dock input level de-asserted

// card removed or inserted
#define	hsChrCardStatusChg			  (hsChrRangeMin+6)	  

// Range of key events available to the HsCardEventPost() call
#define	hsChrCardUserFirst			  (hsChrRangeMin+0x80)
#define	hsChrCardUserLast			  (hsChrCardUserFirst+hsMaxCardEvent)

// Range of key events available to the HsAppEventPost() call
#define	hsChrAppUserFirst			  (hsChrCardUserLast+1)
#define	hsChrAppUserLast			  (hsChrAppUserFirst+hsMaxAppEvent)

// ------------------------------------------------------------------------------
// Special Databases that can be present on a card
// ------------------------------------------------------------------------------

// Handspring defined selector codes for the "CardSetup" application on
//  a card
#define	hsSysAppLaunchCmdInstall	  (sysAppLaunchCmdCustomBase+0)
#define	hsSysAppLaunchCmdRemove		  (sysAppLaunchCmdCustomBase+1)

// The cmdPBP parmeter to the setup app's PilotMain() will point to
//  this structure when the hsSysAppLaunchCmdInstall action code is sent
typedef struct 
  {
	UInt16		cardNo;			// card # of removable card
	UInt8		isReset;		// true if being called due to a soft or
						//  hard reset. The setup app must be "tolerant"
						//  of being sent an install action code
						//  during reset even if it was already
						//  installed before the reset. 
	UInt8		reserved;

	// Database info of the launched setup app, sent for convenience
	UInt16		setupCardNo;		// card # of setup app 
	LocalID		setupDbID;		// database ID of setup app

  } HsSysAppLaunchCmdInstallType;

// The cmdPBP parmeter to the setup app's PilotMain() will point to
//  this structure when the hsSysAppLaunchCmdRemove action code is sent
typedef struct 
  {
	UInt16		cardNo;			// card # of removable card
  } HsSysAppLaunchCmdRemoveType;

// If a card has a database with this type and creator, it will be automatically
//  copied to card 0 when inserted and sent an action code of 
// hsSysAppLaunchCodeInstall. Likewise, when the card is removed,
//  this app will be sent an action code of hsSysAppLaunchCodeRemove and
//  then it will be deleted.
// 
// hsFileCCardSetup			<= defined in HsCreators.h
// hsFileTCardSetup			<= defined in HsCreators.h

// If a card has an application database with this name, it will be
//  automatically launched when the card is inserted
#define	hsWelcomeAppName			  "CardWelcome"

// ------------------------------------------------------------------------------
// Library alias names that SysLibFind maps to a real library name
//  according to the appropriate hsPrefSerialLibXXX setting.
// 
// By Convention, library alias names start with a '*'. The exception
//  is the "Serial Library" name which is mapped in order to 
//  be compatible with pre-existing applications that already use it. 
// ------------------------------------------------------------------------------
#define	hsLibAliasDefault		  "Serial Library" //hsPrefSerialLibDef
#define	hsLibAliasHotSyncLocal		  "*HsLoc  SerLib" //hsPrefSerialLibHotSyncLocal
#define	hsLibAliasHotSyncModem		  "*HsMdm  SerLib" //hsPrefSerialLibHotSyncModem
#define	hsLibAliasIrda			  "*Irda   SerLib" //hsPrefSerialLibIrda
#define	hsLibAliasConsole		  "*Cons   SerLib" //hsPrefSerialLibConsole


// Actual library name of the Dragonball's built-in serial library.
// This is the default value of the hsPrefDefSerialLib pref setting which
//  SysLibFind uses to map an incoming library name to an actual library
//  name. 
#define	hsLibNameBuiltInSerial		  "BuiltIn SerLib"

// ...........................................................
// Fake Srm Manager port's that we patch into HotSync and
//  other places. These port's let us distinguish at the
//  SrmOpen() level, what application is opening the port and
//  what they are doing. 
// This allows us to have finer control over routing various
//  applications to substitute serial drivers. 
// For example, we allow modules to replace the default serial
//  port for local HotSync vs. modem HotSync vs. IRDA
//   vs. console
// ...........................................................

// This is the port parameter passed in to SrmOpen()
#define	hsSerPortHotSyncLocal	0xF000
#define	hsSerPortHotSyncModem	0xF001
// NOTE: serPortIrPort is already defined for IR
// NOTE: serPortConsolePort is already defined for console
// NOTE: serPortCradlePort is already defined for all others

// ------------------------------------------------------------------------------
// Flags for the HsDatabaseCopy routine
// ------------------------------------------------------------------------------
#define	hsDbCopyFlagPreserveCrDate	  0x0001	  // preserve creation date
#define	hsDbCopyFlagPreserveModDate	  0x0002	  // preserve modification date
#define	hsDbCopyFlagPreserveModNum	  0x0004	  // preserve modification number
#define	hsDbCopyFlagPreserveBckUpDate 	  0x0008	  // preserve backup  date
#define	hsDbCopyFlagOKToOverwrite	  0x0010	  // if true, it's OK to overwrite
							  //  existing database. 
#define	hsDbCopyFlagDeleteFirst		  0x0020	  // delete dest DB first, if it
							  //  exists. Use this if space
							  //  is limited on dest card.
							  //  Implies hsDbCopyFlagOKToOverwrite
#define	hsDbCopyFlagPreserveUniqueIDSeed  0x0040	  // preserve database unique ID seed

// ------------------------------------------------------------------------------
// Flags for the HsDmGetNextDBByTypeCreator() call
// ------------------------------------------------------------------------------
#define	hsDmGetNextDBFlagOneCard	  0x00001	  // Only search 1 card

// ------------------------------------------------------------------------------
// Reason codes for the card power handler
// ------------------------------------------------------------------------------
typedef enum
  {
	hsCardPwrDownNormal = 0,			  // normal power down
	hsCardPwrDownLowVoltage = 1			  // low voltage
  } HsCardPwrDownEnum;


// ------------------------------------------------------------------------------
// Equates for the HsCardAttrGet/Set calls
// ------------------------------------------------------------------------------

// Prototypes for the various handlers that can be installed for a card
typedef void	(*HsCardIntHandlerPtr) (UInt32 cardParam, Boolean* sysAwakeP);
typedef void	(*HsCardPwrHandlerPtr) (UInt32 cardParam, Boolean sleep,
			  		UInt16 /*HsCardPwrDownEnum*/ reason);
typedef Boolean (*HsCardEvtHandlerPtr) (UInt32 cardParam, UInt16 evtNum,
					UInt16 evtParam);

// The attributes
typedef enum
  {					// Type     : RW : Description
					// ---------------------------------
	hsCardAttrRemovable,		// UInt8	: R  : true if card is removable
	hsCardAttrHwInstalled,		// UInt8	: R  : true if card hardware is installed
	hsCardAttrSwInstalled,		// UInt8	: R  : true if OS knows about card

	hsCardAttrCsBase,		// UInt32	: R  : address of first chip select
	hsCardAttrCsSize,		// UInt32	: RW : size of chip selects
	hsCardAttrAccessTime,		// UInt32	: RW : required access time

	hsCardAttrReset,		// UInt8	: RW : if true, assert reset to card
	hsCardAttrIntEnable,		// UInt8	: RW : if true, enable card interrupt

	hsCardAttrCardParam,		// UInt32	: RW : parameter passed to int handler,
					//		       power handler, and event handler

	hsCardAttrIntHandler,		// HsCardIntHandlerPtr  
					//		: RW : card interrupt handler

	hsCardAttrPwrHandler,		// HsCardPwrHandlerPtr
					//		: RW : card power handler

	hsCardAttrEvtHandler,		// HsCardEvtHandlerPtr
					//		: RW : card event handler

	hsCardAttrLogicalBase,		// UInt32	: R  : logical base address of card
	hsCardAttrLogicalSize,		// UInt32	: R  : total reserved address space for card
	hsCardAttrHdrOffset,		// UInt32	: R  : offset from halCardAttrLogicalBase to
					//		       card header

	hsCardAttrSetupInstalled,	// UInt8	: R  : true if card setup app is installed

	// Leave this one at end!!!
	hsCardAttrLast

  } HsCardAttrEnum;

// ------------------------------------------------------------------------------
// Equates for the HsPrefGet/Set calls
// ------------------------------------------------------------------------------

// The prefs
typedef enum
  {	// Type : Description
	// ---------------------------------

	// The following are used by SysLibFind() to resolve a virtual library
	//  name to an actual one. 
	hsPrefSerialLibDef,			// Char[] : Name of serial library
						//    to substitute for hsLibAliasDefault
	hsPrefSerialLibHotSyncLocal,  		// Char[] : Name of serial library
						//    to substitute for hsLibAliasHotSyncLocal
	hsPrefSerialLibHotSyncModem,  		// Char[] : Name of serial library
						//    to substitute for hsLibAliasHotSyncModem
	hsPrefSerialLibIrda,		  	// Char[] : Name of serial library
						//    to substitute for hsLibAliasIrda
	hsPrefSerialLibConsole,		  	// Char[] : Name of serial library
						//    to substitute for hsLibAliasConsole

	// Leave this one at end!!!
	hsPrefLast

  } HsPrefEnum;


// ------------------------------------------------------------------------------
// Equates for the HsCardPower call
// ------------------------------------------------------------------------------

// The commands
typedef enum
  {	// IN/OUT : Description
	// ---------------------------------
									  
	hsCardPowerCmdBatteryCapacity,	  // OUT : Capacity of battery in milliwatts
	hsCardPowerCmdBatteryPercent,	  // OUT : Current percent level of battery 
					  //		(0 -> 100)
	hsCardPowerCmdBatteryVoltage,	  // OUT : Battery voltage in millivolts
	hsCardPowerCmdTotalLoad,	  // OUT : Total load on batteries in milliwatts
	hsCardPowerCmdCardLoad,		  // OUT : load on batteries from Springboard
	hsCardPowerCmdAvailablePower,	  // OUT : Additional available milliwatts for 
					  //		Springboard
	hsCardPowerCmdAddLoad,		  // IN  : Add *valueP milliwatts of load to 
					  //		Springboard
	hsCardPowerCmdRemoveLoad,	  // IN  : Remove *valueP milliwatts of load 
					  //		from Springboard
	hsCardPowerCmdCurrentMaxVcc,	  // OUT : max sustained current from Vcc 
					  //		in milliamps
	hsCardPowerCmdCurrentMaxVBat,	  // OUT : max sustained current from VBat 
					  //		in milliamps
	hsCardPowerCmdVccMin,		  // OUT : min Vcc voltage in millivolts
	hsCardPowerCmdVccMax,		  // OUT : max Vcc voltage in millivolts
	hsCardPowerCmdVccNominal,	  // OUT : nominal Vcc voltage in millivolts
	hsCardPowerCmdVBatMin,		  // OUT : min VBat voltage in millivolts
	hsCardPowerCmdVBatMax,		  // OUT : max VBat voltage in millivolts
	hsCardPowerCmdVBatNominal,	  // OUT : nominal VBat voltage in millivolts

	// Leave this one at end!!!
	hsCardPowerCmdLast

  } HsCardPowerCmdEnum;

// ------------------------------------------------------------------------------
// Prototype of the App Event Handler that can be setup using
//  HsAppEventHandlerSet() and triggered using HsAppEventPost(). The
//  evtRefCon is a copy of evtRefCon passed to HsAppEventHandlerSet()
//
// This routine should not rely on globals since it may be called
//  while in the context of another app's action code. 
// ------------------------------------------------------------------------------
typedef Boolean (*HsAppEvtHandlerPtr) (UInt32 evtRefcon, UInt16 evtNum, 
					UInt16 evtParam);

// ------------------------------------------------------------------------------
// Notification Manager equates and structures. 
// ------------------------------------------------------------------------------

// This Notification is sent out using SysNotifyBroadcast() whenever a 
//  card is inserted or removed. The notifyDetailsP field of the event
//  contains a pointer to a HsNotifyCardChangeType that indicates
//  if the card was inserted or removed. 
// The inserted event is sent AFTER the card setup app runs but before we
//  launch the welcome app
// The removed event is sent out BEFORE the card setup app runs
#define	  hsNotifyCardChangeEvent	hsFileCHandspringExt

typedef struct
  {
	UInt8	inserted;				// true if card  inserted, false if card removed
	UInt8	reserved1;				// reserved, set to 0
	UInt16	cardNo;					// card number of card that was inserted/removed
	UInt32	reserved2;				// reserved, set to 0
  }
HsNotifyCardChangeType;
	
//=============================================================================
// Handspring selectors for the Handspring system trap 
// 
// NOTE: If you add traps here, you must:
//
//	  1.) Add a prototype to the Prototypes section of this header or
//		  to the prototype section of HsExtPrv.h if it's  private call
// 
//	  2.) Modify the _PrvHsSelector() routine in HsExtensions.c to recognize
//		  and dispatch to the new call
//
// IMPORTANT: If you change any of these trap numbers, be sure to
//  update any *Patches.txt patch files that use the trap number to patch
//  the object code. 
// 
//=============================================================================
// This is the trap number we use for the Hs trap calls
// IMPORTANT: If this changes, you must manually update the any object code
//  patches files. PalmVSystemboot10001Patches.txt is at least one that
//  patches this trap in .
#define	sysTrapHsSelector			sysTrapOEMDispatch

#define	hsSelectorBase	  			0

#define hsSelInfo 				0x0

#define hsSelPrvInit 				0x1
#define hsSelPrvCallSafely 			0x2
#define hsSelPrvCallSafelyNewStack 		0x3
#define hsSelDatabaseCopy 			0x4
#define hsSelExtKeyboardEnable 			0x5
#define hsSelCardAttrGet 			0x6
#define hsSelCardAttrSet 			0x7
#define hsSelCardEventPost 			0x8
#define hsSelPrvErrCatchListP 			0x9
#define hsSelPrefGet 				0xA
#define hsSelPrefSet 				0xB
#define hsSelDmGetNextDBByTypeCreator		0xC
#define hsSelDmGetNextDBInit 			0xD
#define hsSelCardHdrUpdate 			0xE
#define hsSelAppEventHandlerSet 		0xF
#define hsSelAppEventPost 			0x10
#define hsSelUsbCommStatePtr 			0x11
#define hsSelCardPatchInstall 			0x12
#define hsSelCardPatchRemove 			0x13
#define hsSelEvtResetAutoOffTimer 		0x14
#define hsSelDmDatabaseUniqueIDSeed		0x15
#define hsSelAboutHandspringApp 		0x16
#define hsSelDmDatabaseIsOpen 			0x17
#define hsSelDmDatabaseIsProtected 		0x18
#define hsSelDlkForceSlowSync 			0x19
#define hsSelPrvHandleCardChangeEvent		0x1A
#define hsSelCardPower 				0x1B
#define	hsSelDmDatabaseDeleted			0x1C
#define	hsSelDmLockFileSystem			0x1D
#define	hsSelPrvLaunchCompareFunc		0x1E

// WARNING!  Leave this one at the end! 
// When adding new traps, renumber it
// to one greater than the last trap.
#define hsSelLast  				0x1F

#define	hsNumSels	 (hsSelLast - hsSelBase)

// <chg 06-Apr-2000 BP> added support for gcc 2.95 and new callseq
#if (defined __GNUC__) && (EMULATION_LEVEL == EMULATION_NONE)
  #if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)

	#ifndef _Str
	#define _Str(X)  #X
	#endif

	#define _OS_CALL_WITH_UNPOPPED_16BIT_SELECTOR(table, vector, selector)	\
		__attribute__ ((__callseq__ (										\
			"move.w #" _Str(selector) ",-(%%sp); "							\
			"trap #" _Str(table) "; dc.w " _Str(vector))))
	
	#define SYS_SEL_TRAP(trapNum, selector) \
		_OS_CALL_WITH_UNPOPPED_16BIT_SELECTOR(sysDispatchTrapNum, trapNum, selector)	
  #else // GNUC < 2.95
	#define SYS_SEL_TRAP(trapNum, selector) \
	    __attribute__ ((inline (0x3f3c, selector, m68kTrapInstr+sysDispatchTrapNum,trapNum)))
  #endif // GNUC < 2.95
#elif defined (__MWERKS__)
	#define SYS_SEL_TRAP(trapNum, selector) \
		= {0x3f3c, selector, m68kTrapInstr+sysDispatchTrapNum,trapNum}
#else
  	#define SYS_SEL_TRAP(trapNum, selector) 
#endif	

//=============================================================================
// HsCardErrTry / Catch / support
//
// ---------------------------------------------------------------------
// 
//  Typical Use:
//		volatile UInt16	  x;
//      x = 0;
//		HsCardErrTry 
//        {
//          // access card in some manner that may fail
//          value = cardBaseP[0]
//  
//          // do other stuff too
//			x = 1;		// Set local variable in Try
// 
//          // access card again
//          value = cardBaseP[1]  
//
//		  }
//
//		HsCardErrCatch 
//        {
//          // Recover or cleanup after a failure in above Try block
//          // The code in this block does NOT execute if the above
//          //  try block completes without a card removal
//			if (x > 1) 
//			  SysBeep(1);
// 
//		  } HsCardErrEnd
//   
//
// ***IMPORTANT***
//
//	DO NOT, under any circumstances, "return" or "goto" out of
//  the HsCardErrTry or HsCardErrCatch blocks.  If your code leaves
//  the Try block prematurely, it will corrupt the try/catch
//  exception list, and very bad things will happen (the problem
//  will typically manifest itself at a later time, making it
//  very difficult to debug).
//
//  See also comments about volatile variables next to the definition
//  of HsCardErrCatch.
//
// ***IMPORTANT***
//   
//=============================================================================
	
// Try & Catch macros
#define	HsCardErrTry											\
	{															\
	  ErrExceptionType	_tryObject;								\
	  MemPtr*	volatile _listP;								\
	  _listP = HsPrvErrCatchListP();			\
	  _tryObject.err = 0;					\
	  _tryObject.nextP = (ErrExceptionPtr)*_listP;		\
	  *_listP = (MemPtr)&_tryObject;			\
	  if (ErrSetJump(_tryObject.state) == 0)                \
	    {

		
// NOTE: All variables referenced in and after the ErrCatch must 
// be declared volatile.  Here's how for variables and pointers:
//	volatile UInt16			  oldMode;
//	ShlDBHdrTablePtr volatile hdrTabP = nil;
// 
// If you have many local variables after the ErrCatch you may
// opt to put the ErrTry and ErrCatch in a separate enclosing function.
#define	HsCardErrCatch											\
		  *_listP = (MemPtr)_tryObject.nextP;		\
		} 														\
	  else														\
	    {														\
		  *_listP = (MemPtr)_tryObject.nextP;
			
#define	HsCardErrEnd											\
		}														\
	}

//=============================================================================
// Prototypes
//=============================================================================
UInt32	HsInfo (UInt16 item, UInt16 paramSize,  void* paramP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelInfo);


Err	HsDatabaseCopy (UInt16 srcCardNo, LocalID srcDbID, UInt16 dstCardNo,
			char* dstNameP, UInt32 hsDbCopyFlags, char* tmpNameP,
			LocalID* dstDbIDP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDatabaseCopy);

Err	HsExtKeyboardEnable (Boolean enable)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelExtKeyboardEnable);


Err	HsCardAttrGet (UInt16 cardNo, UInt16 /*HsCardAttrEnum*/ attr, void* valueP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardAttrGet);

Err	HsCardAttrSet (UInt16 cardNo, UInt16 /*HsCardAttrEnum*/ attr, void* valueP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardAttrSet);

Err	HsCardEventPost (UInt16 cardNo, UInt16 evtNum, UInt16 evtParam)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardEventPost);

MemPtr*	HsPrvErrCatchListP (void)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrvErrCatchListP);

Err	HsPrefGet (UInt16 /*HsPrefEnum*/ pref, void* bufP, UInt32* prefSizeP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrefGet);

Err	HsPrefSet (UInt16 /*HsPrefEnum*/ pref, void* bufP, UInt32 prefSize)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelPrefSet);

// This call behaves the same as the PalmOS DmGetNextDatabaseByTypeCreator
//   but can also be used in conjunction with the HsGetNextDBInit()
//   call to start the search at a particular card number or limit
//   the search to one card. 
Err	HsDmGetNextDBByTypeCreator (Boolean newSearch, 
			DmSearchStatePtr stateInfoP, UInt32	type, UInt32 creator, 
			Boolean onlyLatestVers, UInt16* cardNoP, LocalID* dbIDP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmGetNextDBByTypeCreator);

// Can be used to init the stateInfo for HsDmGetNextDBByTypeCreator()
//  so that it starts at a particular card number or limits the
//  search to one card. The flags are 1 or more of hsDmGetNextDBFlagXXX
Err	HsDmGetNextDBInit (DmSearchStatePtr stateInfoP, UInt32 flags, UInt16 cardNo)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmGetNextDBInit);

// This is an advanced call for use by card flash utilities that
// change which card header to use AFTER the card is installed.
Err	HsCardHdrUpdate (UInt16 cardNo, void* newCardHdrP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardHdrUpdate);

// Register an app event handler that can be triggered using
//  HsAppEventPost(). The evtRefCon will be passed to the
//  event handler when it is called. 
// The event Handler should not rely on global variables since it may 
//  be called while in the context of another app's action code. 
//  Instead, pass in a pointer in evtRefCon to globals. 
Err	HsAppEventHandlerSet (HsAppEvtHandlerPtr procP, UInt32 evtRefCon)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelAppEventHandlerSet);

// Post an event to be processed by the AppEventHandler procedure
//  registered with HsAppEventHandlerSet(). The evtNum param
//  can be from 0 to hsMaxAppEvent. 
Err	HsAppEventPost (UInt16 evtNum, UInt16 evtParam)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelAppEventPost);

// Returns a pointer to a 4 byte area of global
//	  memory that can be shared between the debugger stub's 
//	  communication support and run-time communication support.
// This can be used by USB for example to store the enumeration state.
// This is an exported stub into the HAL layer routine which actually
//    does the real work. 
UInt32*	HsUsbCommStatePtr (void)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelUsbCommStatePtr);

// Patch a system trap for a card. This call should be used instead of
//  SysSetTrapAddress() by card setup utilities because it will 
//  insure compatibility with HackMaster and any HackMaster 
//  hacks that are installed. 
// The implementation of the patch should use HsCardPatchPrevProc() 
//   to get the address of the old routine to chain to:
//		HsCardPatchPrevProc (&oldProcP);
//		(*oldProcP)(); 
// IMPORTANT: Setup utilities are only allowed to install *ONE*
//  patch for each trapNum!
Err	HsCardPatchInstall (UInt16 trapNum, void* procP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPatchInstall);

// Remove a patch of a system trap installed using HsSysPatchInstall().
// The 'creator' and 'id' must the same as passed to HsSysPatchInstall().
Err	HsCardPatchRemove (UInt16 trapNum)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPatchRemove);

// Macro to get the old routine address of a trap patched using
//  HsCardPatchInstall()
#define	HsCardPatchPrevProc(trapNum,oldProcPP)					  \
		FtrGet (hsFileCCardSetup, trapNum, (UInt32*)oldProcPP)

// Called from interrupt routines to reset the auto-off timer
// and tell the system whether or not the user hardware (i.e. LCD)
// needs to be woken up or not. Note: The granularity of the
// stayAwakeTicks is only about 5*sysTicksPerSecond. 
Err	HsEvtResetAutoOffTimer (Int32 stayAwakeTicks, Boolean userOn)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelEvtResetAutoOffTimer);

// Call to get or set the Unique ID seed field of a database
Err	HsDmDatabaseUniqueIDSeed (UInt16 cardNo, LocalID dbID, 
			Boolean set, UInt32* uniqueIDSeed)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseUniqueIDSeed);

// <chg 29-Jun-99 dia> Added extra credits string parameter.
void	HsAboutHandspringApp (UInt16 appCardNo, LocalID appDbId,
			char* copyrightYearStrP, Char* extraCreditsStrP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelAboutHandspringApp);

// <chg 30-Jun-99 dia> Defined macros to make about box easier to call.
#define HsAboutHandspringAppWithYearId(yearId)								\
	do																		\
	  {																		\
		UInt16	appCardNo;													\
		LocalID	appDbId;													\
		MemHandle	yearStrH;												\
		Char* 	yearStrP;													\
																			\
		yearStrH = DmGetResource (strRsc, yearId);							\
		yearStrP = MemHandleLock (yearStrH);								\
		SysCurAppDatabase(&appCardNo, &appDbId);							\
		HsAboutHandspringApp (appCardNo, appDbId, yearStrP, NULL);			\
		MemPtrUnlock (yearStrP);											\
		DmReleaseResource (yearStrH);										\
	  }																		\
	while (0)
		
#define HsAboutHandspringAppWithYearCredId(yearId, creditsId)				\
	do																		\
	  {																		\
		UInt16	appCardNo;													\
		LocalID	appDbId;													\
		MemHandle	yearStrH, extraStrH;									\
		Char* 	yearStrP;													\
		Char*	extraStrP;													\
																			\
		yearStrH = DmGetResource (strRsc, yearId);							\
		yearStrP = MemHandleLock (yearStrH);								\
		extraStrH = DmGetResource (strRsc, creditsId);						\
		extraStrP = MemHandleLock (extraStrH);								\
		SysCurAppDatabase(&appCardNo, &appDbId);							\
		HsAboutHandspringApp (appCardNo, appDbId, yearStrP, extraStrP);		\
		MemPtrUnlock (extraStrP);											\
		DmReleaseResource (extraStrH);										\
		MemPtrUnlock (yearStrP);											\
		DmReleaseResource (yearStrH);										\
	  }																		\
	while (0)


UInt8		HsDmDatabaseIsOpen (UInt16 cardNo, LocalID dbID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseIsOpen);

UInt8		HsDmDatabaseIsProtected (UInt16 cardNo, LocalID dbID)
				SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseIsProtected);

// =====================================================================
// The following API calls are only available for versions 0x02000000
//   and greater
// =====================================================================
Err	HsDlkForceSlowSync (Boolean clearLastPCIID, Boolean clearAllSyncPrefs, 
			Boolean* didAnythingP, UInt32* lastSyncDateP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDlkForceSlowSync);

// =====================================================================
// The following API calls are only available for versions 0x03500000
//   and greater
// =====================================================================

// Power Management information and Springboard load registration
Err	HsCardPower (UInt16 cardNo, UInt16 /*HsCardPowerCmdEnum*/ cmd, UInt32* valueP)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelCardPower);

// Notify OS that a database on a read-only (i.e. Flash memory) card has been deleted
// This will clean up all Alarms, Notifies, etc. associated with the database
Err	HsDmDatabaseDeleted (UInt16 cardNo, LocalID dbID)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmDatabaseDeleted);

// Lock/Unlock the file system for reading or writing
Err	HsDmLockFileSystem (Boolean lock, Boolean writeAccess)
	SYS_SEL_TRAP (sysTrapHsSelector, hsSelDmLockFileSystem);

#endif
