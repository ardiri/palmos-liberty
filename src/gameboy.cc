/*
 * @(#)gameboy.cc
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

#include "resource.h"

	.file	"gameboy.cc"
.section	game, "x"
	.even

// constant labels
#define regBC    	  0
#define regB     	  0
#define regC     	  1
#define regDE    	  2
#define regD     	  2
#define regE     	  3
#define regHL    	  4
#define regH     	  4
#define regL     	  5
#define regA     	  6
#define regF     	  7
#define regSP    	  8
#define regPC    	  10
#define regInt   	  12
#define regTimer 	  13
#define regSerTimer	  14  // z80 CPU references
#define regA_old     	  15
 
#define xBitOff		  16
#define xByteOff	  17
#define wxBitOff	  18
#define wxByteOff	  19
#define xOffset		  20
#define oldBgPal	  21

#define SelKey		  22
#define OldSelKey         23 // (used for Joypad interrupt)

#define cfgSoundMute      24
#define cfgSoundVolume    25
#define cfgSoundChan1     26
#define cfgSoundChan2     27

#define curRomIndex	  28

#define ptrCurLine	  30

#define ptrHLConv	  34
#define ptrBgPal	  38
#define ptrObjPal0	  42
#define ptrObjPal1	  46

#define ptrLCDScreen	  50
#define frameBlitConst	  54
#define frameBlit	  55

#define ptrScreen	  56
#define screenOffset      60
#define screenAdjustment  62

#define ptr32KRam	  64
#define ptrTileTable      68
#define ptrBGTileRam	  72

#define pageCount	  76
#define	ptrPageTbl	  78
#define ptrCurRom	  82

#define snd1Vol           86
#define snd1Env           87
#define snd1ECtr          88
#define snd1Div           89
#define snd1FCtr          90
#define snd1FTime         91
#define snd1Freq          92 
#define snd1Length        94
#define snd1Duty          95

#define cTimerFrame	  96
#define	cTimerSec	  97
#define cTimerMin	  98
#define	cTimerHour	  99
#define	cTimerDayL	  100
#define	cTimerDayH	  101

#define snd2Vol           102
#define snd2Env           103
#define snd2ECtr          104
#define snd2Freq          106
#define snd2Length        108
#define snd2Duty          109

#define DynaNext          110
#define	cCycles	          114
#define	CurRAMBank        116
#define skipCPUCycles     118

#ifdef RUMBLEPAK
#define ptrVibrateOn      120
#define ptrVibrateOff     124
#endif

#ifdef PALM_HIRES
#define m68k_device       128
#define hires320x320      129
#define use_api           130
#define winOffscreen      132
#define winDisplay        136
#define ptrXlat           140
#endif

#define	StartDLine	  0x7F4c
#define	ScreenWinY	  0x7f4d

// independant resources
#include "sound.inc"
#include "z80mem.inc"
#include "z80.inc"
#include "screen.inc"

/*
 * NOTES:
 *
 *   The GameBoy emulation code (one very huge assembler routine) should 
 *   be placed within this file. The following assumptions can be made:
 *
 *     - register "a1" = pointer to gameboy specific "globals" 
 *
 * // Aaron Ardiri
 * aaron@ardiri.com
 */

.globl	EmulateFrame


/*  2/13/02 - MJE - ISSUE? 
              Some interrupt routines are setting the
              interrupt flag before checking if that
              particular interrupt is enabled.
              Others are only setting the flag if the
              selected interrupt IS enabled
*/

EmulateFrame:
|	move.l	#0, %a5
|	move.w	#15000, %d1
|xTest:
|	subq	#1, %d1
|	bne	xTest
|	rts

|	ori	#0x0700, %SR

	move.l	%a1, %a0		| m68k-palmos-gcc makes us do this :>

|*****  Update Timer variables
	add.b	#1, cTimerFrame(%a0)
	cmp.b	#15, cTimerFrame(%a0)	| 15 fps
	bne.s	cTimerCont
	add.b	#1, cTimerSec(%a0)
	cmp.b	#60, cTimerSec(%a0)	| 60 sec/min
	bne.s	cTimerCont
	move.b	#0, cTimerSec(%a0)
	add.b	#1, cTimerMin(%a0)
	cmp.b	#60, cTimerMin(%a0)	| 60 min/hr
	bne.s	cTimerCont
	move.b	#0, cTimerMin(%a0)
	add.b	#1, cTimerHour(%a0)
	cmp.b	#24, cTimerHour(%a0)	| 24 hr/day
	bne.s	cTimerCont
	move.b	#0, cTimerHour(%a0)
	add.w	#1, cTimerDayL(%a0)	
	cmp.w	#365, cTimerDayL(%a0)	| 365 days/yr
	bne.s	cTimerCont
	move.w	#0, cTimerDayL(%a0)
cTimerCont:

	move.l	ptr32KRam(%a0), %a1	| RAM ptr
	
	lea.l	0x7F00(%a1), %a4	| a4 = ptr to FF00
	lea.l	-0x08000(%a1), %a1	| Get offset to RAM

	move.l	ptrPageTbl(%a0), %a2	| Get ROM page 0 ptr
	move.l	(%a2), %a2

	move.l  ptrCurRom(%a0), %a5	| get current ROM page ptr
	lea.l	-0x4000(%a5), %a5


	moveq	#0, %d0
	movea.l	%a1, %a3

	move.w	regPC(%a0), %d0		| Get current PC
	bmi.s	PCInRAM			| If reading from RAM

	movea.l	%a2, %a3
	cmpi.w	#0x4000, %d0		| If reading from Bank 0
	blt.s	PCInRAM
	movea.l	%a5, %a3		| current Bank

	
PCInRAM:
	adda.l	%d0, %a3

	moveq	#0, %d1
	moveq	#0, %d2
	moveq	#0, %d3
	moveq	#0, %d4
	move.b	regA(%a0), %d2		| Get Registers
	move.w	regHL(%a0), %d3

	moveq	#0, %d0			| Get SP
	movea.l	%a1, %a6
	move.w	regSP(%a0), %d0
	bmi.s	SPInRAM
	movea.l	%a2, %a6
 	cmpi.w	#0x4000, %d0		| If reading from Bank 0
	blt.s	SPInRAM
	movea.l	%a5, %a6		| current Bank

SPInRAM:
	adda.l	%d0, %a6

	btst.b	#4, regF(%a0)		| C flag
	sne	%d6
	btst.b	#7, regF(%a0)		| Z flag
	sne	%d7

	move.w	#0x0201, regA(%a0)	| Set up 2 frames (15 fps) per loop
					| Stored in regA, regF as temp values

|	tst.l	0x50(%a4)
|	bne.s	DispFrameLoop
|	trap	#8

/* Set the # of CPU Cycles per emulation loop */
	move.w	#20, cCycles(%a0)
	tst.w	skipCPUCycles(%a0)
	beq.s	DispFrameLoop
	move.w	#10, cCycles(%a0)

DispFrameLoop:
	moveq	#0, %d4
	move.w	#0xFF00, %d4
	clr.w	0x4c(%a1, %d4.l)		| reset location of last screen line drawn
	move.l	ptrScreen(%a0), ptrCurLine(%a0)
	add.l	#704,ptrCurLine(%a0)

Do144Lines:
	moveq	#0, %d4
	move.w	#0xFF00, %d4
	andi.b	#0xFC, 0x41(%a1, %d4.l)	| OAM mode
	ori.b	#2, 0x41(%a1, %d4.l)


|******	Execute 21 CPU cycles
|	move.w	#20, %d5
	move.w	cCycles(%a0), %d5
	bsr	Z80Loop			| Do Instructions
	move.w	#0xFF00, %d4
	ori.b	#3, 0x41(%a1, %d4.l)	| Transfer mode


|******	Execute 21 CPU cycles
|	move.w	#20, %d5
	move.w	cCycles(%a0), %d5
	bsr	Z80Loop			| Do Instructions


|****	Serial interrupt
	move.w	#0xFF00, %d4
	tst.b	0x02(%a1, %d4.l)		| Is Serial started?
	bpl.s	NoSerialInt		| no, branch

	btst	#0, 0x02(%a1, %d4.l)	| Is timer external?
	beq.s	NoSerialInt		| Yes, and we are not receiving, so continue

	subq.b	#1, regSerTimer(%a0)	| decrement Serial Timer
	bne.s	NoSerialInt		| If no overflow, continue

	bset	#3, 0x0F(%a1, %d4.l)	| raise Serial Interrupt Flag

	bclr	#7, 0x02(%a1, %d4.l)	| Stop serial
	move.b	#0xFF, 0x01(%a1, %d4.l)	| Set recieved data to FF

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NoSerialInt
	move.b	#0xFF, %d4
	btst	#3, 0(%a1, %d4.l)		| Is Serial Int. Enabled?
	beq.s	NoSerialInt
	clr.b	%d4
	bclr	#3, 0x0F(%a1, %d4.l)	| Clear Serial IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	SerialNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
SerialNotHalt:

	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zSerPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zSerPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zSerPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x58(%a2), %a3		| jump to interrupt handler

NoSerialInt:


|******	Execute 21 CPU cycles
|	move.w	#20, %d5
	move.w	cCycles(%a0), %d5
	bsr	Z80Loop			| Do Instructions
	move.w	#0xFF00, %d4
	subq.b	#1, 4(%a1, %d4.l)		| Decrement DivReg

	btst	#3, 0x41(%a1, %d4.l)	| Horizontal Interrupt
	beq.s	NoHBlankInt		| If bit 3 set of LCDSTAT


|****	Do HBlank Interrupt
	tst.b	0x40(%a1, %d4.l)		| Is screen on?
	bpl.s	NoHBlankInt

	// MJE - Added 1.31 (was not checking this before!)
	btst	#3, 0x41(%a1, %d4.l)		| Is HBlank Int. enabled?
	beq.s	NoHBlankInt

	move.b	#0xFF, %d4
	btst	#1, 0(%a1, %d4.l)		| Is LCDC Int. Enabled?
	beq.s	NoHBlankInt
	clr.b	%d4
	bset	#1, 0x0F(%a1, %d4.l)	| raise LCDC Interrupt

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NoHBlankInt

	bclr	#1, 0x0F(%a1, %d4.l)	| Clear LCDC IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	LCDCNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
LCDCNotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zLCDCNHPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zLCDCNHPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zLCDCNHPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x48(%a2), %a3		| jump to interrupt handler


       *  DRAW FRAME UP TO CURRENT LINE *
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	DrawCLineRet
	bsr	DrawLines		| If so, draw current line
DrawCLineRet:


NoHBlankInt:


|****	Update Timer, Do any interrupt
	move.w	#0xFF00, %d4
	btst	#2, 0x07(%a1, %d4.l)	| Is Timer On?
	beq.s	NoTimerInt		| no, branch
	btst	#1, 0x07(%a1, %d4.l)	| Is Timer 16.384 KHz?
	bne.s	DoTimerInc		| yes, do increment
	not.b	regTimer(%a0)		| toggle Timer
	bne.s	NoTimerInt		| if no increment this pass, branch

DoTimerInc:
	addq.b	#1, 0x05(%a1, %d4.l)	| increment TIMA (Timer Counter)
	bne.s	NoTimerInt		| If no overflow, continue

	move.b	0x06(%a1, %d4.l), 0x05(%a1, %d4.l)	| Load TIMA with TMA (Timer Modulo)

	bset	#2, 0x0F(%a1, %d4.l)	| raise Timer Interrupt Flag

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NoTimerInt
	move.b	#0xFF, %d4
	btst	#2, 0(%a1, %d4.l)		| Is Timer Int. Enabled?
	beq.s	NoTimerInt
	clr.b	%d4
	bclr	#2, 0x0F(%a1, %d4.l)	| Clear Timer IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	TimerNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
TimerNotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	TimerPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	TimerPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
TimerPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x50(%a2), %a3		| jump to interrupt handler

NoTimerInt:
	move.w	#0xFF00, %d4
	andi.b  #0xFC, 0x41(%a1, %d4.l)	| HBlank mode

|******	Execute 51 CPU cycles
|	move.w	#50, %d5
	move.w	cCycles(%a0), %d5
	add.w	%d5, %d5
	add.w	#10, %d5
	bsr	Z80Loop			| Do Instructions

	move.w	#0xFF00, %d4
	subq.b	#1, 0x04(%a1, %d4.l)	| Decrement DivReg

	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line #
	addq.b  #1, %d0			| Increment Line
	move.b	%d0, 0x44(%a1, %d4.l)	| Save line

	andi.b  #0xFB, 0x41(%a1, %d4.l)	| Clear the coincidence flag

	cmp.b   0x45(%a1, %d4.l), %d0	| See if coincidence
	bne.s	NoCoincidence		| no

|****	Do LCDC Interrupt
	tst.b	0x40(%a1, %d4.l)		| Is screen on?
	bpl.s	NoCoincidence

	ori.b   #4, 0x41(%a1, %d4.l)	| Set the coincidence flag
	btst	#6, 0x41(%a1, %d4.l)	| Is= LCDC coincidence Int. enabled?
	beq.s	NoCoincidence
	move.b	#0xFF, %d4
	btst	#1, 0(%a1, %d4.l)		| Is LCDC Int. Enabled?
	beq.s	NoCoincidence
	clr.b	%d4
	bset	#1, 0x0F(%a1, %d4.l)	| raise LCDC Interrupt

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NoCoincidence

	bclr	#1, 0x0F(%a1, %d4.l)	| Clear LCDC IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	LCDC2NotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
LCDC2NotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	LCDC2PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	LCDC2PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
LCDC2PCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x48(%a2), %a3		| jump to interrupt handler
	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line # back to d0


|       *  DRAW FRAME UP TO CURRENT LINE *
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	DrawCLineRet2
	bsr	DrawLines		| If so, draw current line
DrawCLineRet2:

NoCoincidence:


|****	Update Timer, Do any interrupt
	move.w	#0xFF00, %d4
	btst	#2, 0x07(%a1, %d4.l)	| Is Timer On?
	beq.s	NoTimerCont		| no, branch
	btst	#1, 0x07(%a1, %d4.l)	| Is Timer 16.384 KHz?
	beq.s	NoTimerCont		| no, branch
	addq.b	#1, 0x05(%a1, %d4.l)	| increment TIMA (Timer Counter)
	bne.s	NoTimerCont		| If no overflow, continue

	move.b	0x06(%a1, %d4.l), 0x05(%a1, %d4.l)	| Load TIMA with TMA (Timer Modulo)

	bset	#2, 0x0F(%a1, %d4.l)	| raise Timer Interrupt Flag

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NoTimerCont

	move.b	#0xFF, %d4
	btst	#2, 0(%a1, %d4.l)		| Is Timer Int. Enabled?
	beq.s	NoTimerCont

	clr.b	%d4
	bclr	#2, 0x0F(%a1, %d4.l)	| Clear Timer IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	TimerNotHalt2
	addq.l	#1, %a3			| If so, skip to next instruction
TimerNotHalt2:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	Timer2PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	Timer2PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
Timer2PCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x50(%a2), %a3		| jump to interrupt handler
	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line # back in d0


NoTimerCont:
	cmpi.b  #144, %d0		| Loop for 144 lines
	bne	Do144Lines		| (until VBlank)

	



|       *  DRAW FRAME *
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	DrawLineRet
	move.w	#0xFF00, %d4
	move.b	#143, 0x44(%a1, %d4.l)	| Subtract 1 from line # for drawing
	bsr	DrawLines		| draw current line

|	*  DRAW ANY REMAINING SPRITES *
DrawSLoop:
	move.b	#158, 0x44(%a1, %d4.l)	| move line # to end
	bsr	DrawJustSprites		| draw any sprites for line
	move.w	#0xFF00, %d4
	move.b	#144, 0x44(%a1, %d4.l)	| Restore Line #

DrawLineRet:


|       *  PLAY SOUNDS  *
	bsr	DoSound	


******	In VBLANK ******
	moveq	#0, %d4
	move.w	#0xFF00, %d4
	andi.b  #0xFC, 0x41(%a1, %d4.l)
	ori.b   #1, 0x41(%a1, %d4.l)

	tst.b	0x40(%a1, %d4.l)		| Is screen on?
	bpl.s	NotEnteringVBlank

| Dont raise for Bubble Bobble
	cmp.l	#0x42554242, 0x134(%a2)	| See if cart name= 'BUBB'
	beq.s	NoRaise
	bset	#0, 0x0F(%a1, %d4.l)	| raise VBlank Interrupt Flag
NoRaise:
	move.b	#0xFF, %d4
	btst	#0, 0(%a1, %d4.l)		| Is VBlank Int. Enabled?
	beq.s	NotEnteringVBlank	| no

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	NotEnteringVBlank

	clr.b	%d4
	bclr	#0, 0x0F(%a1, %d4.l)	| Clear VBlank IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	VBNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
VBNotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	VBPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	VBPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
VBPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x40(%a2), %a3		| jump to interrupt handler

NotEnteringVBlank:




|*****  Do JoyPad Interrupt flag
	// See if joypad button has been depressed.
	// If so, set Joypad Interrupt Flag, do Interrupt if enabled
	move.b	OldSelKey(%a0), %d0
	move.b	SelKey(%a0), %d1
	eor.b	%d1, %d0		| d0 will be non-zero if any change of state
	and.b	%d1, %d0		| d0 will be non-zero if state went from 0 to 1 (depressed)
	beq.s	BtnNotPushed

	move.w	#0xFF00, %d4
	bset	#4, 0x0F(%a1, %d4.l)	| raise JoyPad Interrupt Flag
BtnNotPushed:
	move.b	SelKey(%a0), OldSelKey(%a0)


DoVBLine:

|******	Execute 14 CPU cycles
	move.w	#13, %d5
	bsr	Z80Loop			| Do Instructions
VBCPURet1:

	move.w	#0xFF00, %d4
	cmpi.b  #153, 0x44(%a1, %d4.l)	| See if last line
	bne.s	VBNotTopFrame
	clr.b   0x44(%a1, %d4.l)		| yes, loop to 1st line (last line is shorter)
	andi.b  #0xFB, 0x41(%a1, %d4.l)	| Clear the coincidence flag

VBNotTopFrame:

|******	Execute 49 CPU cycles
	move.w	#48, %d5
	bsr	Z80Loop			| Do Instructions

	move.w	#0xFF00, %d4
	subq.b	#1, 0x04(%a1, %d4.l)	| Decrement DivReg

|****	Serial interrupt
	tst.b	0x02(%a1, %d4.l)		| Is Serial started?
	bpl.s	VBNoSerialInt		| no, branch

	btst	#0, 0x02(%a1, %d4.l)	| Is timer external?
	beq.s	VBNoSerialInt		| Yes, and we are not receiving, so continue

	subq.b	#1, regSerTimer(%a0)	| decrement Serial Timer
	bne.s	VBNoSerialInt		| If no overflow, continue

	bset	#3, 0x0F(%a1, %d4.l)	| raise Serial Interrupt Flag

	bclr	#7, 0x02(%a1, %d4.l)	| Stop serial
	move.b	#0xFF, 0x01(%a1, %d4.l)	| Set recieved data to FF

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	VBNoSerialInt

	move.b	#0xFF, %d4
	btst	#3, 0(%a1, %d4.l)		| Is Serial Int. Enabled?
	beq.s	VBNoSerialInt

	clr.b	%d4
	bclr	#3, 0x0F(%a1, %d4.l)	| Clear Serial IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	VBSerialNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
VBSerialNotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	VBSerPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	VBSerPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
VBSerPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x58(%a2), %a3		| jump to interrupt handler

VBNoSerialInt:


****	Update Timer, do any interrupt
	move.w	#0xFF00, %d4
	btst	#2, 0x07(%a1, %d4.l)	| Is Timer On?
	beq.s	VBNoTimerInt		| no, branch
	btst	#1, 0x07(%a1, %d4.l)	| Is Timer 16.384 KHz?
	bne.s	VBDoTimerInc		| yes, do increment
	not.b	regTimer(%a0)		| toggle Timer
	bne.s	VBNoTimerInt		| if no increment this pass, branch

VBDoTimerInc:
	addq.b	#1, 0x05(%a1, %d4.l)	| increment TIMA (Timer Counter)
	bne.s	VBNoTimerInt		| If no overflow, continue

	move.b	0x06(%a1, %d4.l), 0x05(%a1, %d4.l)	| Load TIMA with TMA (Timer Modulo)

	bset	#2, 0x0F(%a1, %d4.l)		| raise Timer Interrupt Flag

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	VBNoTimerInt

	move.b	#0xFF, %d4
	btst	#2, 0(%a1, %d4.l)		| Is Timer Int. Enabled?
	beq.s	VBNoTimerInt

	clr.b	%d4
	bclr	#2, 0x0F(%a1, %d4.l)	| Clear Timer IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	VBTimerNotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
VBTimerNotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	VBTimPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	VBTimPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
VBTimPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x50(%a2), %a3		| jump to interrupt handler

VBNoTimerInt:


|******	Execute 51 CPU cycles
	move.w	#50, %d5
	bsr	Z80Loop			| Do Instructions

	move.w	#0xFF00, %d4
	subq.b	#1, 0x04(%a1, %d4.l)	| Decrement DivReg

	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line #
	beq.s	VBLine0			| If Line 0, skip increment
	addq.b  #1, %d0			| Increment Line

|	cmpi.b  #154, %d0		| See if below bottom
|	bcs.s	VBNotTopFrame
|	clr.b   %d0			| yes, loop to 1st line

|VBNotTopFrame:
	move.b	%d0, 0x44(%a1, %d4.l)	| Save line

	andi.b  #0xFB, 0x41(%a1, %d4.l)	| Clear the coincidence flag
VBLine0:
	cmp.b   0x45(%a1, %d4.l), %d0	| See if coincidence
	bne.s	VBNoCoincidence		| no

|****	Do LCDC Interrupt
	tst.b	0x40(%a1, %d4.l)		| Is screen on?
	bpl.s	VBNoCoincidence

	ori.b   #4, 0x41(%a1, %d4.l)	| Set the coincidence flag
	btst	#6, 0x41(%a1, %d4.l)	| Is= LCDC coincidence Int. enabled?
	beq.s	VBNoCoincidence

	move.b	#0xFF, %d4
	btst	#1, 0(%a1, %d4.l)		| Is LCDC Int. Enabled?
	beq.s	VBNoCoincidence

	clr.b	%d4
	bset	#1, 0x0F(%a1, %d4.l)	| raise LCDC Interrupt

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	VBNoCoincidence

	bclr	#1, 0x0F(%a1, %d4.l)	| Clear LCDC IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	VBLCDC2NotHalt
	addq.l	#1, %a3			| If so, skip to next instruction
VBLCDC2NotHalt:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	VBLC2PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	VBLC2PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
VBLC2PCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x48(%a2), %a3		| jump to interrupt handler
	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line # back to d0

VBNoCoincidence:


|****	Update Timer, do any interrupt
	move.w	#0xFF00, %d4
	btst	#2, 0x07(%a1, %d4.l)	| Is Timer On?
	beq.s	VBNoTimer		| no, branch
	btst	#1, 0x07(%a1, %d4.l)	| Is Timer 16.384 KHz?
	beq.s	VBNoTimer		| no, branch
	addq.b	#1, 0x05(%a1, %d4.l)	| increment TIMA (Timer Counter)
	bne.s	VBNoTimer		| If no overflow, continue

	move.b	0x06(%a1, %d4.l), 0x05(%a1, %d4.l)	| Load TIMA with TMA (Timer Modulo)

	bset	#2, 0x0F(%a1, %d4.l)	| raise Timer Interrupt Flag

	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	VBNoTimer

	move.b	#0xFF, %d4
	btst	#2, 0(%a1, %d4.l)		| Is Timer Int. Enabled?
	beq.s	VBNoTimer

	clr.b	%d4
	bclr	#2, 0x0F(%a1, %d4.l)	| Clear Timer IF
	clr.b	regInt(%a0)		| disable Interrupts
	cmpi.b	#0x76, (%a3)		| See if currently at HALT
	bne.s	VBTimerNotHalt2
	addq.l	#1, %a3			| If so, skip to next instruction
VBTimerNotHalt2:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	VBTim2PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	VBTim2PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
VBTim2PCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte
	lea.l	0x50(%a2), %a3		| jump to interrupt handler
	move.b	0x44(%a1, %d4.l), %d0	| Get Cur Line # back in d0

VBNoTimer:

	tst.b	%d0			| See if completed VBlank (and 1 frame)
	bne	DoVBLine		| no, loop


	subq.b	#1, regF(%a0)
	bpl	DispFrameLoop		| Do one full frame

	subq.b	#1, frameBlit(%a0)	| Decrement frameBlit
	bpl.s	BlitCont		| If at end of skipping, reset counter
	move.b	frameBlitConst(%a0), frameBlit(%a0)
	
	bsr     CopyScreen              | show the screen

BlitCont:
	subq.b	#1, regA(%a0)
	beq.s	Done2Frames		| If so, continue
	move.b	#1, regF(%a0)
	bra	DispFrameLoop		| No, do another
Done2Frames:


|****	Save Registers
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	SavePCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	SavePCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
SavePCOk:
	move.w	%d0, regPC(%a0)		| Save current PC


	move.b	%d2, regA(%a0)		| Save Registers
	move.w	%d3, regHL(%a0)


	move.l	%a6, %d0		| see if SP in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	SaveSPOk
	move.l	%a6, %d0		| See if SP in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	SaveSPOk
	move.l	%a6, %d0		| In RAM
	sub.l	%a1, %d0
SaveSPOk:
	move.w	%d0, regSP(%a0)		| Save current SP


	andi.b	#0x10, %d6		| Get C flag
	andi.b	#0x80, %d7		| Get Z flag
	or.b	%d7, %d6		| Move (x) to d6
	move.b	%d6, regF(%a0)

	rts


| Move Screenbuffer to Physical Screen
CopyScreen:

#ifdef PALM_HIRES
	tst.b	use_api(%a0)	 	| do we need api blitting?
	bne	CopyScreen_api		| yes

	tst.b	hires320x320(%a0)	| we on 68k unit then?
	bne	CopyScreen_asmHI	| no, asm 320x320	
#endif

CopyScreen_asm:

	movem.l	%d0-%d7/%a0-%a5, -(%sp)	
	move.l	ptrLCDScreen(%a0), %a1

	add.w	screenOffset(%a0), %a1
	move.w	screenAdjustment(%a0), %d1	| screen blitting stuff

	move.l	ptrScreen(%a0), %a2
	lea.l	706(%a2), %a0			| add 706, move to a0

	move.w	#35, %d0
ScrMoveLoop:
	movem.l	(%a0)+, %d2-%d7/%a2-%a5		| Line #1
	movem.l	%d2-%d7/%a2-%a5, (%a1)
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	movem.l	(%a0)+, %d2-%d7/%a2-%a5		| Line #2
	movem.l	%d2-%d7/%a2-%a5, 40(%a1)
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	movem.l	(%a0)+, %d2-%d7/%a2-%a5		| Line #3
	movem.l	%d2-%d7/%a2-%a5, 80(%a1)
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	movem.l	(%a0)+, %d2-%d7/%a2-%a5		| Line #4
	movem.l	%d2-%d7/%a2-%a5, 120(%a1)
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	lea.l	160(%a1), %a1

	dbra	%d0, ScrMoveLoop

	movem.l	(%sp)+,%d0-%d7/%a0-%a5
	rts

#ifdef PALM_HIRES

CopyScreen_asmHI:

	movem.l	%d0-%d7/%a0-%a5, -(%sp)	
	move.l	ptrLCDScreen(%a0), %a1

	add.w	screenOffset(%a0), %a1
	move.l	#80, %d1			| screen blitting stuff

	move.l	ptrXlat(%a0), %a2
	clr.l   %d3
	
	move.l	ptrScreen(%a0), %a0
	lea.l	706(%a0), %a0			| add 706, move to a0
	lea.l   40(%a0), %a3

	move.w	#35, %d0
ScrMoveLoop2:

	move.l  #39, %d2
ScrMoveLoop2a:					| Line #1
        move.b  (%a0)+, %d3
        move.l  %d3, %d4
        add.w   %d4, %d4            
        move.w  (%a2, %d4), %d4    		| x = table2bpp[v]
        move.w  %d4, 80(%a1)         		| *ptrLCDScreen(y+1) = x;        
        move.w  %d4, (%a1)+         		| *ptrLCDScreen(y)   = x;
	dbra	%d2, ScrMoveLoop2a
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	move.l  #39, %d2
ScrMoveLoop2b:					| Line #2
        move.b  (%a0)+, %d3
        move.l  %d3, %d4
        add.w   %d4, %d4            
        move.w  (%a2, %d4), %d4    		| x = table2bpp[v]
        move.w  %d4, 80(%a1)         		| *ptrLCDScreen(y+1) = x;        
        move.w  %d4, (%a1)+         		| *ptrLCDScreen(y)   = x;
	dbra	%d2, ScrMoveLoop2b
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	move.l  #39, %d2
ScrMoveLoop2c:					| Line #3
        move.b  (%a0)+, %d3
        move.l  %d3, %d4
        add.w   %d4, %d4            
        move.w  (%a2, %d4), %d4    		| x = table2bpp[v]
        move.w  %d4, 80(%a1)         		| *ptrLCDScreen(y+1) = x;        
        move.w  %d4, (%a1)+         		| *ptrLCDScreen(y)   = x;
	dbra	%d2, ScrMoveLoop2c
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap
	
	move.l  #39, %d2
ScrMoveLoop2d:					| Line #4
        move.b  (%a0)+, %d3
        move.l  %d3, %d4
        add.w   %d4, %d4            
        move.w  (%a2, %d4), %d4    		| x = table2bpp[v]
        move.w  %d4, 80(%a1)         		| *ptrLCDScreen(y+1) = x;        
        move.w  %d4, (%a1)+         		| *ptrLCDScreen(y)   = x;
	dbra	%d2, ScrMoveLoop2d
	addq.l	#4, %a0
	add.w   %d1, %a1			| adjust for wrap

	dbra	%d0, ScrMoveLoop2

	movem.l	(%sp)+,%d0-%d7/%a0-%a5
	rts

CopyScreen_api:

	movem.l	%d0-%d7/%a0-%a5, -(%sp)	

	lea	-8(%sp), %sp			| make space for RectangleType struct
	move.l  %sp, %a1
	
        move.w  #8,   (%a1)
        move.w  #16, 2(%a1)
        move.w  #160,4(%a1)
        move.w  #144,6(%a1)			| rect = { { 8, 16 }, { 160, 144 }};

        clr.w   -(%sp)				| winPaint
        move.w  #16,-(%sp)			| y = 16
        clr.w   -(%sp)				| x = 0
        move.l  %a1,-(%sp)			| &rect
        move.l  winDisplay(%a0),-(%sp)		| WinGetDisplayWindow();
        move.l  winOffscreen(%a0),-(%sp)	| globals->emu.winOffscreen;

        trap #15
        .word   0xA209				| WinCopyRectangle()
        lea     26(%sp), %sp
  	movem.l	(%sp)+,%d0-%d7/%a0-%a5
        
	rts       
#endif        
