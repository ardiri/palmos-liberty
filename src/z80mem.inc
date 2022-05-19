/*
 * @(#)z80mem.inc
 *
 * Copyright 2000-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                      Michael Ethetton (mailto:methetton@gambitstudios.com)
 * All rights reserved
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

/*  This code is used for memory writing */

| Write 0000-1FFF  RAM Bank Enable
| Write 2000-3FFF  ROM Bank Select
| Write 4000-5FFF  RAM Bank Select
| Write 6000-7FFF  MCB1 ROM/RAM Bank Select
| Write 8000-87FF  RAM tile data
| Write 8800-8FFF  RAM tile data
| Write 9000-97FF  RAM tile data
| Write 9800-9BFF  RAM screen data
| Write 9C00-9FFF  RAM screen data
| Write A000-BFFF  RAM external (switchable)
| Write C000-DFFF  RAM internal
| Write E000-FDFF  RAM internal Echo
| Write FE00-FEFF  RAM OAM sprite table
| Write FF00-FFFF  RAM I/O ports & internal RAM

| d0 = high byte of address
| d1 = byte to write
| d4 = address to write


WriteMem:
	add.w	%d0, %d0			| get jump address
	move.w	Write(%pc, %d0.w), %d0		| get address of code for routine
	jmp	Write(%pc, %d0.w)


Write:
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write

	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write

	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write

	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write
	.word	WROM0_lo-Write

	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write

	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write

	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write

	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write
	.word	WROM0_hi-Write

	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write

	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write

	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write

	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write
	.word	WROM1_lo-Write

	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write

	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write

	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write

	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write
	.word	WROM1_hi-Write

	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write
	.word	WRAM_tile0-Write

	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write
	.word	WRAM_tile1-Write

	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write
	.word	WRAM_tile2-Write

	.word	WRAM_screen0-Write
	.word	WRAM_screen0-Write
	.word	WRAM_screen0-Write
	.word	WRAM_screen0-Write

	.word	WRAM_screen1-Write
	.word	WRAM_screen1-Write
	.word	WRAM_screen1-Write
	.word	WRAM_screen1-Write

	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write

	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write

	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write

	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write
	.word	WRAM_ext-Write

	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write

	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write

	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write

	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write
	.word	WRAM_int-Write

	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write

	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write

	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write

	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write
	.word	WRAM_echo-Write

	.word	WRAM_sprite-Write

	.word	WRAM_io-Write




| Write 0000-1FFF  RAM Bank Enable
WROM0_lo:
	rts				| Bank enable not needed



| Write 2000-3FFF  ROM Bank Select
WROM0_hi:
	andi.w	#0x00FF, %d1
	cmp.w	pageCount(%a0), %d1	| see if ROM Page out of range
        blo.s   WROMPageOK
	move.w  pageCount(%a0), %d0
	subq.w  #1, %d0
	and.w   %d0, %d1
WROMPageOK:
	tst.b	%d1
	bne.s	WROMPage0		| If 0, change to ROM 1
	moveq	#1, %d1			| Page # is ROM 1
WROMPage0:

	move.l	%a6, %d0		| see if SP in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	WROM_SPOk
	move.l	%a6, %d0		| See if SP in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcc.s	WROM_SPOk
	move.l	%d0, %a6		| In high ROM
WROM_SPOk:

	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	WROM_PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcc.s	WROM_PCOk
	move.l	%d0, %a3		| In high ROM - take out offset
WROM_PCOk:

	move.w	%d1, curRomIndex(%a0)	| save index (for save state)

	lsl.w	#2, %d1			| Get Bank*4
	move.l	%a1, %d0		| Save RAM Pointer
	move.l	ptrPageTbl(%a0), %a1
	move.l	(%a1, %d1.w), %a5	| Get new ROM Ptr
	move.l	%a5,ptrCurRom(%a0)	| Save ROM memory ptr
	lea.l	-0x4000(%a5), %a5

	move.l	%d0, %a1		| Restore RAM Ptr.

	cmpa.l	#0x08000, %a3		| PC in high ROM?
	bcc.s	WROM_pcCont
	adda.l	%a5, %a3		| Add back offset if PC in upper ROM
WROM_pcCont:

	cmpa.l	#0x8000, %a6		| In high ROM?
	bcc.s	WROM_spCont
	adda.l	%a5, %a6		| Add offset if SP in upper ROM
WROM_spCont:
	rts



| Write 4000-5FFF  RAM Bank Select
WROM1_lo:

| Following code for future SRAM support:
|	move.b	%d1, CurRAMBank(%a0)	| store RAM Bank	
|	movem.l	%d0-%d7/%a0-%a6, -(%sp)
|	move.l	switchBank(%a0), %a1
|
|	jsr	(%a1)
|	movem.l	(%sp)+, %d0-%d7/%a0-%a6

#ifdef RUMBLEPAK
	andi.b	#0x08, %d1		| get the "rumble" bit
	beq.s	RumbleOFF
	move.l	ptrVibrateOn(%a0), %d1
	beq.s	RumbleRet
	exg	%d1, %a6
	move.b	#1, (%a6)
	exg	%d1, %a6
	rts
RumbleOFF:
	move.l	ptrVibrateOff(%a0), %d1
	beq.s	RumbleRet
	exg	%d1, %a6
	move.b	#0, (%a6)
	exg	%d1, %a6
RumbleRet:
#endif
	rts


| Write 6000-7FFF  MCB1 ROM/RAM Bank Select
WROM1_hi:
	rts			| not implemented



| Write 8000-87FF  RAM tile data
WRAM_tile0:
	move.b  %d1, (%a1, %d4.l)
	andi.w	#0xFFFE, %d4		| get even word address

 	move.w	(%a1, %d4.l), %d1	| get tile value

	swap	%d2			| store RegA temp.
	move.w	%d1,%d2			| copy value
	andi.w	#0x7FFE, %d2		| make +, drop low bit
	eor.w	%d2, %d1		| save sign, low bit only (H7L0)
	move.l	%a6, %d0		| store a6 temp
	movea.l	ptrHLConv(%a0), %a6	| location of pixel table
	or.w	(%a6,%d2.w), %d1	| convert HHLL to hlhlhlhlhlhlhlhl

	movea.l	ptrBGTileRam(%a0), %a6	| save it to converted tile table
	lea.l	-0x8000(%a6), %a6
	move.w	%d1, (%a6,%d4.l)

	movea.l	ptrBgPal(%a0), %a6	| location of BG palette
	addi.w	#0x6000, %d4		| move address to echo RAM for converted values
	adda.l	%d4, %a1		| get location into %a1
	clr.w	%d2

	move.w	%d1, -(%sp)		| convert to current palette
	move.b	(%sp)+, %d2
	move.b	(%a6,%d2.w), (%a1)	| Write Pattern high
	move.b	%d1, %d2
	move.b	(%a6,%d2.w), 1(%a1)	| Write Pattern low

	move.l	%d0, %a6		| restore a6
	clr.w	%d2
	swap	%d2
	suba.l	%d4, %a1		| restore a1
	rts


| Write 8800-8FFF  RAM tile data
WRAM_tile1:
	move.b  %d1, (%a1, %d4.l)
	andi.w	#0xFFFE, %d4		| get even word address

 	move.w	(%a1, %d4.l), %d1	| get tile value

	swap	%d2			| store RegA temp.
	move.w	%d1,%d2			| copy value
	andi.w	#0x7FFE, %d2		| make +, drop low bit
	eor.w	%d2, %d1		| save sign, low bit only (H7L0)
	move.l	%a6, %d0		| store a6 temp
	movea.l	ptrHLConv(%a0), %a6	| location of pixel table
	or.w	(%a6,%d2.w), %d1	| convert HHLL to hlhlhlhlhlhlhlhl

	movea.l	ptrBGTileRam(%a0), %a6	| save it to converted tile table
	lea.l	-0x8000(%a6), %a6
	move.w	%d1, (%a6,%d4.l)

	movea.l	ptrBgPal(%a0), %a6	| location of BG palette
	addi.w	#0x6000, %d4		| move address to echo RAM for converted values
	adda.l	%d4, %a1		| get location into %a1
	clr.w	%d2

	move.w	%d1, -(%sp)		| convert to current palette
	move.b	(%sp)+, %d2
	move.b	(%a6,%d2.w), (%a1)	| Write Pattern high
	move.b	%d1, %d2
	move.b	(%a6,%d2.w), 1(%a1)	| Write Pattern low

	move.l	%d0, %a6		| restore a6
	clr.w	%d2
	swap	%d2
	suba.l	%d4, %a1		| restore a1
	rts

| Write 9000-97FF  RAM tile data
WRAM_tile2:
	move.b  %d1, (%a1, %d4.l)
	andi.w	#0xFFFE, %d4		| get even word address

 	move.w	(%a1, %d4.l), %d1	| get tile value

	swap	%d2			| store RegA temp.
	move.w	%d1,%d2			| copy value
	andi.w	#0x7FFE, %d2		| make +, drop low bit
	eor.w	%d2, %d1		| save sign, low bit only (H7L0)
	move.l	%a6, %d0		| store a6 temp
	movea.l	ptrHLConv(%a0), %a6	| location of pixel table
	or.w	(%a6,%d2.w), %d1	| convert HHLL to hlhlhlhlhlhlhlhl

	movea.l	ptrBGTileRam(%a0), %a6	| save it to converted tile table
	lea.l	-0x8000(%a6), %a6
	move.w	%d1, (%a6,%d4.l)

	movea.l	ptrBgPal(%a0), %a6	| location of BG palette
	addi.w	#0x6000, %d4		| move address to echo RAM for converted values
	adda.l	%d4, %a1		| get location into %a1
	clr.w	%d2

	move.w	%d1, -(%sp)		| convert to current palette
	move.b	(%sp)+, %d2
	move.b	(%a6,%d2.w), (%a1)	| Write Pattern high
	move.b	%d1, %d2
	move.b	(%a6,%d2.w), 1(%a1)	| Write Pattern low

	move.l	%d0, %a6		| restore a6
	clr.w	%d2
	swap	%d2
	suba.l	%d4, %a1		| restore a1
	rts


| Write 9800-9BFF  RAM screen data
WRAM_screen0:
	andi.w	#0x00FF, %d1
	move.b  %d1, (%a1, %d4.l)
	lsl.w	#4, %d1
	sub.w	#0x9800, %d4
	add.w	%d4, %d4
	add.l	ptrTileTable(%a0), %d4		| save calculated tile data pointer to table
	exg	%d4, %a6
	move.w	%d1, (%a6)
	exg	%d4, %a6
	moveq	#0, %d4
	rts


| Write 9C00-9FFF  RAM screen data
WRAM_screen1:
	andi.w	#0x00FF, %d1
	move.b  %d1, (%a1, %d4.l)
	lsl.w	#4, %d1
	sub.w	#0x9800, %d4
	add.w	%d4, %d4
	add.l	ptrTileTable(%a0), %d4		| save calculated tile data pointer to table
	exg	%d4, %a6
	move.w	%d1, (%a6)
	exg	%d4, %a6
	moveq	#0, %d4
	rts


| Write A000-BFFF  RAM external (switchable)
WRAM_ext:
	move.b  %d1, (%a1, %d4.l)
	rts


| Write C000-DFFF  RAM internal
WRAM_int:
	move.b  %d1, (%a1, %d4.l)
	rts


| Write E000-FDFF  RAM internal Echo
WRAM_echo:
	andi.w	#0xDFFF, %d4		| mask bit
	move.b  %d1, (%a1, %d4.l)
	rts


| Write FE00-FEFF  RAM OAM sprite table
WRAM_sprite:
	move.b  %d1, (%a1, %d4.l)
	rts


| Write FF00-FFFF  RAM I/O ports & internal RAM
WRAM_io:
	cmpi.b	#0x4d, %d4
	bhi.s	WRAM_ioWrite
	moveq	#0, %d0
	move.b	%d4, %d0
	add.w	%d0, %d0
	move.w	zHigh2(%pc, %d0.w), %d0	| get address of code for instruction
 	jmp	zHigh2(%pc, %d0.w)	| do instruction

WRAM_ioWrite:
	move.b	%d1, (%a1, %d4.l)	| Write byte
	cmpi.b	#0xFF, %d4		| See if writing to IE
	bne.s	WRAM_ioExit
	tst.b	regInt(%a0)		| If interrupts aren't enabled, don't do anything
	beq.s	WRAM_ioExit
	move.w	%d5, -(%sp)		| save counter
	clr.w	%d5
	bsr	zFB			| Do EI to check for interrupts
	move.w	(%sp)+, %d5		| restore inst.  counter
WRAM_ioExit:
	rts

	


zHigh2:
	.word	zFF00-zHigh2
	.word	zFF01-zHigh2
	.word	zFF02-zHigh2
	.word	zFF03-zHigh2
	.word	zFF04-zHigh2
	.word	zFF05-zHigh2
	.word	zFF06-zHigh2
	.word	zFF07-zHigh2
	.word	zFF08-zHigh2
	.word	zFF09-zHigh2
	.word	zFF0A-zHigh2
	.word	zFF0B-zHigh2
	.word	zFF0C-zHigh2
	.word	zFF0D-zHigh2
	.word	zFF0E-zHigh2
	.word	zFF0F-zHigh2
	.word 	zFF10-zHigh2
	.word	zFF11-zHigh2
	.word	zFF12-zHigh2
	.word	zFF13-zHigh2
	.word	zFF14-zHigh2
	.word	zFF15-zHigh2
	.word	zFF16-zHigh2
	.word	zFF17-zHigh2
	.word	zFF18-zHigh2
	.word	zFF19-zHigh2
	.word	zFF1A-zHigh2
	.word	zFF1B-zHigh2
	.word	zFF1C-zHigh2
	.word	zFF1D-zHigh2
	.word	zFF1E-zHigh2
	.word	zFF1F-zHigh2
	.word	zFF20-zHigh2
	.word	zFF21-zHigh2
	.word	zFF22-zHigh2
	.word	zFF23-zHigh2
	.word	zFF24-zHigh2
	.word	zFF25-zHigh2
	.word	zFF26-zHigh2
	.word	zFF27-zHigh2
	.word	zFF28-zHigh2
	.word	zFF29-zHigh2
	.word	zFF2A-zHigh2
	.word	zFF2B-zHigh2
	.word	zFF2C-zHigh2
	.word	zFF2D-zHigh2
	.word	zFF2E-zHigh2
	.word	zFF2F-zHigh2
	.word	zFF30-zHigh2
	.word	zFF31-zHigh2
	.word	zFF32-zHigh2
	.word	zFF33-zHigh2
	.word	zFF34-zHigh2
	.word	zFF35-zHigh2
	.word	zFF36-zHigh2
	.word	zFF37-zHigh2
	.word	zFF38-zHigh2
	.word	zFF39-zHigh2
	.word	zFF3A-zHigh2
	.word	zFF3B-zHigh2
	.word	zFF3C-zHigh2
	.word	zFF3D-zHigh2
	.word	zFF3E-zHigh2
	.word	zFF3F-zHigh2
	.word	zFF40-zHigh2
	.word	zFF41-zHigh2
	.word	zFF42-zHigh2
	.word	zFF43-zHigh2
	.word	zFF44-zHigh2
	.word	zFF45-zHigh2
	.word	zFF46-zHigh2
	.word	zFF47-zHigh2
	.word	zFF48-zHigh2
	.word	zFF49-zHigh2
	.word	zFF4A-zHigh2
	.word	zFF4B-zHigh2
	.word	zFF4C-zHigh2
	.word	zFF4D-zHigh2





| *************************************
| * Loads joypad byte

					| P1
zFF00:
	btst	#4, %d1			| See if Scanning U/D/L/R
	beq.s	zKeybd2
	btst	#5, %d1			| scanning Start/Sel/B/A
	beq.s	zKeybd1
	st	(%a1, %d4.l)
	rts

zKeybd1:
	move.b	SelKey(%a0), %d1	| St/Sel/B/A
	not	%d1
	andi.b	#0x0F, %d1
	ori.b	#0xD0, %d1
	move.b	%d1, (%a1, %d4.l)
	rts

zKeybd2:
	move.b	SelKey(%a0), %d1	| U/D/L/R
	not	%d1
	lsr.b	#4, %d1
	ori.b	#0xE0, %d1
	move.b	%d1, (%a1, %d4.l)
	rts


zFF01:
	move.b	%d1, (%a1, %d4.l)	| SB Serial transfer
	rts

					| SC
zFF02:
	tst.b	%d1			| See if Start Timer
	bpl.s	zFF02W
	move.b	#9, regSerTimer(%a0)	| Load the Serial Timer Countdown
zFF02W:
	/* VGB - %d1|=0x7E */
	move.b	%d1, (%a1, %d4.l)
	rts


zFF03:
	move.b	%d1, (%a1, %d4.l)		| * not used *
	rts

zFF04:
	clr.b	(%a1, %d4.l)			| DIV
	rts

zFF05:
	move.b	%d1, (%a1, %d4.l)		| TIMA timer counter
	rts

zFF06:
	move.b	%d1, (%a1, %d4.l)		| TMA timer modulo
	rts
zFF07:
	/* VGB - %d1|=0xF8 */
	move.b	%d1, (%a1, %d4.l)		| TAC timer control
	rts

zFF08:
	move.b	%d1, (%a1, %d4.l)		| 0x08-0x0E **not used**
	rts

zFF09:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF0A:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF0B:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF0C:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF0D:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF0E:
	move.b	%d1, (%a1, %d4.l)	
	rts

zFF0F:
	/* VGB - sets %d1&=0x1F instead */
	ori.b	#0xE0, %d1		| IF
	move.b	%d1, (%a1, %d4.l)
	rts

					| Sound Registers...
zFF10:
	move.b	%d1, (%a1, %d4.l)		| Snd 1 Freq. Sweep Register
	move.b	%d1, %d0		| Sweep Time in bits 4-6
	lsr.b	#4, %d0			| move to bits 0-2
	andi.b	#0x07, %d0
	move.b	%d0, snd1FTime(%a0)	| Save it to counter
	move.b	%d0, snd1FCtr(%a0)
	andi.b	#0x07, %d1		| Get freq. change divisor
	move.b	%d1, snd1Div(%a0)

	move.w	4(%a1, %d4.l), %d0 		| Save Freq to Current Frequency
	move.b	3(%a1, %d4.l), %d0
	andi.w	#0x07FF, %d0
	move.w	%d0, snd1Freq(%a0)
	rts

zFF11:
	move.b	%d1, (%a1, %d4.l)
	rts

zFF12:
	move.b	%d1, (%a1, %d4.l)		| Snd 1 Envelope
	move.b	%d1, %d0		| Envelope sweep & counter
	andi.b	#0x03, %d0
	move.b	%d0, snd1Env(%a0)
	move.b	%d0, snd1ECtr(%a0)
	andi.b	#0xF0, %d1		| set initial volume
	lsr.b	#4, %d1
	move.b	%d1, snd1Vol(%a0)
	rts

zFF13:
	move.b	%d1, (%a1, %d4.l)		| Snd 1 Freq (Low)
	move.w	1(%a1, %d4.l), %d0 		| Save to Current Frequency
	move.b	%d1, %d0
	andi.w	#0x07FF, %d0
	move.w	%d0, snd1Freq(%a0)
	rts

zFF14:
	move.b	%d1, (%a1, %d4.l)		| Snd 1 Freq (High)
	move.w	(%a1, %d4.l), %d0 		| Save to Current Frequency
	move.b	-1(%a1, %d4.l), %d0
	andi.w	#0x07FF, %d0
	move.w	%d0, snd1Freq(%a0)
	rts

zFF15:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF16:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF17:
	move.b	%d1, (%a1, %d4.l)		| Snd 2 Envelope
	move.b	%d1, %d0		| Envelope sweep & counter
	andi.b	#0x03, %d0
	move.b	%d0, snd2Env(%a0)
	move.b	%d0, snd2ECtr(%a0)
	andi.b	#0xF0, %d1		| set initial volume
	lsr.b	#4, %d1
	move.b	%d1, snd2Vol(%a0)
	rts

zFF18:
	move.b	%d1, (%a1, %d4.l)		| Snd 2 Freq (Low)
	move.b	1(%a1, %d4.l), %d0 		| Save to Current Frequency
	andi.b	#0x07, %d0
	move.b	%d0, snd2Freq(%a0)
	move.b	%d1, snd2Freq+1(%a0)
	rts

zFF19:
	move.b	%d1, (%a1, %d4.l)		| Snd 2 Freq (High)
	move.b	-1(%a1, %d4.l), snd2Freq+1(%a0)	| Save to Current Frequency
	move.b	%d1, %d0
	andi.b	#0x07, %d0
	move.b	%d0, snd2Freq(%a0)
	rts

zFF1A:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF1B:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF1C:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF1D:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF1E:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF1F:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF20:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF21:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF22:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF23:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF24:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF25:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF26:
	andi.b	#0x7F, (%a1, %d4.l)	| Turn on/off Sound (bit 7)
	andi.b	#0x80, %d1		| Bits 0-3 are current sound channel status
	or.b	%d1, (%a1, %d4.l)
	rts


zFF27:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF28:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF29:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2A:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2B:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2C:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2D:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2E:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF2F:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF30:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF31:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF32:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF33:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF34:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF35:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF36:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF37:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF38:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF39:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3A:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3B:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3C:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3D:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3E:
	move.b	%d1, (%a1, %d4.l)
	rts
zFF3F:
	move.b	%d1, (%a1, %d4.l)
	rts



zFF40:
	cmp.b	(%a1, %d4.l), %d1		| LCDC
	beq.s	zFF40Exit		| If no change, do nothing
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	zFF40DontDraw
	swap	%d2
	move.b	%d1, %d2
	bsr	DrawLines		| If so, draw up to current line
	move.b	%d2, %d1
	clr.b	%d2
	swap	%d2
zFF40DontDraw:
	move.b	%d1, (%a1, %d4.l)		| LCDC
	/* VGB - Sets table address pointers here.  (ChrGen, BgdTab, WndTab) */
zFF40Exit:
	rts

zFF41:
	andi.b	#0xf8, %d1		| STAT (LCDC Status)
	/* VGB - Does not Set high bit */
|	ori.b	#0x80, %d1
|	move.b	%d1, (%a1, %d4.l)  <----BUG????
	andi.b	#0x07, (%a1, %d4.l)	| We were not saving the low bits of STAT
	or.b	%d1, (%a1, %d4.l)
	rts

zFF42:
	cmp.b	(%a1, %d4.l), %d1		| SCY
	beq.s	zFF42Exit		| If no change, do nothing
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	zFF42DontDraw
	swap	%d2
	move.b	%d1, %d2
	bsr	DrawLines		| If so, draw up to current line
	move.b	%d2, %d1
	clr.b	%d2
	swap	%d2
zFF42DontDraw:
	move.b	%d1, (%a1, %d4.l)		| SCY
zFF42Exit:
	rts


zFF43:
	cmp.b	(%a1, %d4.l), %d1		| SCX
	beq.s	zFF43Exit		| If no change, do nothing
	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	zFF43DontDraw
	swap	%d2
	move.b	%d1, %d2
	bsr	DrawLines		| If so, draw up to current line
	move.b	%d2, %d1
	clr.b	%d2
	swap	%d2
zFF43DontDraw:

	move.b	%d1, (%a1, %d4.l)		| Save SCX
	move.b	%d1, %d4		| temp store
	and.b	#07, %d1		| Get remainder (x scroll offset)
|	bne.s	zFF43Not0
|	move.b	#0, xBitOff(%a0)	| No offset
|	move.b	%d4, %d1
|	lsr.b	#3, %d1			| /8 pixels per tile
|	move.b	%d1, xByteOff(%a0)	| Save Byte Offset
|	rts

zFF43Not0:
	move.b	#8, %d0
	sub.b	%d1, %d0		| Invert remainder
	add.b	%d0, %d0		| (offset * 2)
	move.b	%d0, xBitOff(%a0)	| Save Pixel Offset

	move.b	%d4, %d1
	lsr.b	#3, %d1			| /8 pixels per tile
	move.b	%d1, xByteOff(%a0)	| Save Byte Offset
zFF43Exit:
	rts


zFF44:
	clr.b	(%a1, %d4.l)		| LY
	move.l	ptrScreen(%a0), ptrCurLine(%a0) | Reset Screen Pointer
	add.l	#704,ptrCurLine(%a0)
	rts

zFF45:
	move.b	%d1, (%a1, %d4.l)		| LYC
	rts



| *************************************
| *  Performs the DMA transfer function
zFF46:
					| DMA
	moveq	#0, %d0
	movem.l	%d2-%d7/%a1-%a4, -(%sp)	| Store registers we will trash
	move.b	%d2, -(%sp)		| Get xfer FROM address
	move.w	(%sp)+, %d0

	bmi.s	zDMARAM
	clr.b	%d0
	move.l	%d0,%a4			| Get from address to a4
	adda.l	%a2,%a4			| Copying from ROM
	bra.s	zDMANext
zDMARAM:
	clr.b	%d0
	move.l	%d0, %a4		| Get from address to a4
	adda.l	%a1, %a4		| copying from RAM
zDMANext:
	adda.l	#0xFE00, %a1		| Go to Sprite Ram

					| Copy 160 Bytes
	movem.l	(%a4)+, %d0-%d7/%a2-%a3	| get 40 bytes
	movem.l	%d0-%d7/%a2-%a3, (%a1)	| copy

	movem.l	(%a4)+, %d0-%d7/%a2-%a3	| get next 40 bytes
	movem.l	%d0-%d7/%a2-%a3, 40(%a1)	| copy

	movem.l	(%a4)+, %d0-%d7/%a2-%a3	| get next 40 bytes
	movem.l	%d0-%d7/%a2-%a3, 80(%a1)	| copy

	movem.l	(%a4)+, %d0-%d7/%a2-%a3	| get next 40 bytes
	movem.l	%d0-%d7/%a2-%a3, 120(%a1)	| copy

	movem.l	(%sp)+, %d2-%d7/%a1-%a4	| Restore trashed regs.
	moveq	#0, %d1
	moveq	#0, %d0

	cmpi.b	#0x3E, (%a3)		| See if next inst. is LD a
	bne.s	DontSkip
	addq.l	#2, %a3			| skip load
	move.b	#1, %d2			| Move 1 to A
	
DontSkip:
	moveq	#0, %d1
	move.w	#0xFF04, %d1
	subq.b	#2, (%a1, %d1.l)	| Whole thing takes >640 cycles
	moveq	#0, %d5
	rts


zFF47:
	cmp.b	(%a1, %d4.l), %d1		| BGP
	beq.s	zFF47Exit

	tst.b	frameBlit(%a0)		| Is drawing frame?
	bne.s	zFF47DontDraw
	swap	%d2
	move.b	%d1, %d2
	bsr	DrawLines		| If so, draw up to current line
	move.b	%d2, %d1
	clr.b	%d2
	swap	%d2
zFF47DontDraw:

	move.b	%d1, (%a1, %d4.l)
	bne.s	zFF47Not0
zFF47Exit:
	rts

zFF47Not0:

	cmp.b	oldBgPal(%a0), %d1	| See if new palette=old palette
	beq	zFF47Cont		| If so, dont need to convert
	move.b	%d1, oldBgPal(%a0)	| Save new palette setting
	movem.l	%d2/%a2/%a3, -(%sp)	| Convert palette table
	move.b	%d1, %d2
	move.b	%d1, %d0
	movea.l	ptrBgPal(%a0), %a2

	moveq	#3, %d1
					| First, separate palette to d0.L
					| (each 2 bits=1 byte)
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
	ror.b	#4, %d2

	move.b	%d0, -(%sp)		| Do 0M
	move.w	(%sp)+, %d0
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
	rol.b	#2, %d2

	swap	%d0			| Do 0D
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
	ror.b	#4, %d2

	move.b	%d0, -(%sp)		| Do 0H
	move.w	(%sp)+, %d0
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
					| d0= 0L 0M 0D 0H
	lea.l	-18(%sp), %sp		| hold 16 bytes for a palette conv. table
	movea.l	%sp, %a3		| hold 16 bytes for a palette conv. table
	move.l	%d0, (%a3)+		| save low halves of nibbles
	move.l	%d0, (%a3)+
	move.l	%d0, (%a3)+
	move.l	%d0, (%a3)+

	move.l	%d0, %d1		| save high halves of nibbles
	lsl.l	#2, %d1

	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)

	move.w	%d1, -(%sp)
	move.b	(%sp)+, %d1
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	
	swap	%d1
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)

	move.w	%d1, -(%sp)
	move.b	(%sp)+, %d1
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)
	or.b	%d1, -(%a3)

	moveq	#0, %d2			| loop 16x
BldLoop1:
	movea.l	%sp, %a3
	move.b	(%a3, %d2), %d0
	lsl.b	#4, %d0
	move.b	%d0, -(%sp)
	move.w	(%sp)+, %d1
	move.b	%d0, %d1
	move.w	%d1, %d0
	swap	%d0
	move.w	%d1, %d0
	
	move.l	(%a3)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a3)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a3)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a3)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table

	addq	#1, %d2
	cmpi	#16, %d2
	bne.s	BldLoop1

	lea.l	18(%sp), %sp

| *** Update Background/window tile table

	movea.l	ptrBGTileRam(%a0), %a3	| Converted Tile ptr
	lea.l	0x7000(%a1), %a2	| ptr to $E000
	lea.l	0x7000(%a2), %a2
	move.w	#0x17ff, %d4
	move.l	%a1, %d1
	movea.l	ptrBgPal(%a0), %a1	| location of BG palette
	moveq	#0, %d0
BGPalTileLoop:
	move.b	(%a3)+, %d0		| Get tile value
	move.b	(%a1,%d0.w), (%a2)+	| Write Pattern low

	dbra	%d4, BGPalTileLoop
	movea.l	%d1, %a1		| restore pointer
	movem.l	(%sp)+, %d2/%a2/%a3

zFF47Cont:
	moveq	#0, %d1
	rts



zFF48:
	cmp.b	(%a1, %d4.l), %d1		| OBP0
	beq.s	zFF48Cont

	move.b	%d1, (%a1, %d4.l)

	movem.l	%d2/%a1-%a2, -(%sp)	| Build Palette table
	movea.l	ptrObjPal0(%a0), %a2
	bra.s	BuildPal	
zFF48Cont:
	moveq	#0, %d1
	rts

zFF49:					| OBP1
	cmp.b	(%a1, %d4.l), %d1
	beq.s	zFF48Cont

	move.b	%d1, (%a1, %d4.l)

	movem.l	%d2/%a1-%a2, -(%sp)	| Build Palette table
	movea.l	ptrObjPal1(%a0), %a2

BuildPal:
	tst.b	%d1			| See if Palette is 00
	beq	BuildPal0

	move.b	%d1, %d2
	move.b	%d1, %d0

	moveq	#3, %d1
					| First, separate palette to d0.L
					| (each 2 bits=1 byte)
	moveq	#0, %d0			| (Palette 00 is transparent)
	ror.b	#4, %d2

	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
	roL.b	#2, %d2

	swap	%d0			| Do 0D
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
	ror.b	#4, %d2

	move.b	%d0, -(%sp)		| Do 0H
	move.w	(%sp)+, %d0
	move.b	%d1, %d0		| lower 2 bits
	and.b	%d2, %d0
					| d0= 0L 0M 0D 0H
	lea.l	-18(%sp), %sp		| hold 16 bytes for a palette conv. table
	movea.l	%sp, %a1		| hold 16 bytes for a palette conv. table
	move.l	%d0, (%a1)+		| save low halves of nibbles
	move.l	%d0, (%a1)+
	move.l	%d0, (%a1)+
	move.l	%d0, (%a1)+

	move.l	%d0, %d1		| save high halves of nibbles
	lsl.l	#2, %d1

	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)

	move.w	%d1, -(%sp)
	move.b	(%sp)+, %d1
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	
	swap	%d1
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)

	move.w	%d1, -(%sp)
	move.b	(%sp)+, %d1
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)
	or.b	%d1, -(%a1)

	moveq	#0, %d2			| loop 16x
BldLoop2:
	movea.l	%sp, %a1
	move.b	(%a1, %d2), %d0
	lsl.b	#4, %d0
	move.b	%d0, -(%sp)
	move.w	(%sp)+, %d1
	move.b	%d0, %d1
	move.w	%d1, %d0
	swap	%d0
	move.w	%d1, %d0
	
	move.l	(%a1)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a1)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a1)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table
	move.l	(%a1)+, %d1
	or.l	%d0, %d1
	move.l	%d1,(%a2)+		| Save in table

	addq	#1, %d2
	cmpi	#16, %d2
	bne.s	BldLoop2
	lea.l	18(%sp), %sp
	movem.l	(%sp)+, %d2/%a1-%a2

	moveq	#0, %d1
	rts

BuildPal0:
	movem.l	%d3-%d7, -(%sp)
	lea.l	256(%a2), %a2		| Start at end of table
	moveq	#0, %d0			| load 0s
	moveq	#0, %d1
	moveq	#0, %d2
	moveq	#0, %d3
	moveq	#0, %d4
	moveq	#0, %d5
	moveq	#0, %d6
	moveq	#0, %d7

	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 32
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 64
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 96
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 128

	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 160
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 192
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 224
	movem.l	%d0-%d7, -(%a2)		| Save all 0s to table 256

	movem.l	(%sp)+, %d3-%d7		| restore registers
	movem.l	(%sp)+, %d2/%a1-%a2

	rts




zFF4A:
	move.b	%d1, (%a1, %d4.l)		| WY
	rts


zFF4B:
	move.b	%d1, (%a1, %d4.l)		| WX

	move.b	%d1, %d0
	addq	#1, %d1
	lsr.b	#3, %d1			| / 8
	move.b	%d1, wxByteOff(%a0)	| Save it

	move.b	%d0, %d1
	addq	#1, %d1
	and.b	#07, %d1		| Get remainder
	add.b	%d1, %d1		| (offset * 2)
	move.b	%d1, wxBitOff(%a0)	| Save pixel offset
	rts

zFF4C:
	rts

zFF4D:
					| ColorGB - High-speed mode switch
	rts
zFF4F:
					| ColorGB - VRAM Bank
	rts

zFF51:	
					| ColorGB - DMA Src (hi,lo)
zFF52:

zFF53:
					| ColorGB - DMA Dest (hi,lo)
zFF54:

zFF55:
					| ColorGB - DMA Xfer Start (len/type)
	rts

zFF56:
					| ColorGB - IR Port

zFF57:
					| ColorGB - ??

zFF68:
					| ColorGB - BPALDAT (bgp Index)
	rts

zFF69:
					| ColorGB - BPALREG (bgp Data)
	rts

zFF6A:
					| ColorGB - SPALDAT (obp index)
	rts

zFF6B:
					| ColorGB - SPALREG (obp data)
	rts

zFF6C:
					| ColorGB - ??

zFF6D:
					| ColorGB - ??

zFF5E:
					| ColorGB - ??

zFF6F:
					| ColorGB - ??

zFF70:
					| ColorGB - Internal WRAM Bank
	rts

