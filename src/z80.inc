/*
 * @(#)z80.inc
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

/*
 * PRE-REQUIREMENTS:
 * =================
 *
 *  d0 - scatch pad
 *  d1 - temp, data to write
 *  d2 - A
 *  d3 - HL
 *  d4 - temp, address to write
 *  d5 - instruction counter
 *  d6 - C flag
 *  d7 - Z flag
 *
 *  a0 - pointer -> register chunk (BC @ offset 0)
 *  a1 - ramoffset ptr (-0x8000 to allow for address values)
 *  a2 - romoffset 0 ptr
 *  a3 - current read address  (current PC + ram/rom offset)
 *  a4 - unused
 *  a5 - current ROM page ptr (-0x4000 to allow for address values)
 *  a6 - Current SP address (SP + ram/rom offset)
 */

/*
 * z-80 instruction loop
 */

Z80Loop:
DoNext:
	moveq	#0, %d0
	move.b	(%a3)+, %d0		| Get next instruction
DoNext2:
	add.w	%d0, %d0
	move.w	zOP(%pc, %d0.w), %d0	| get address of code for instruction
	jmp	zOP(%pc, %d0.w)		| do instruction

/*
 * z-80 instruction lookup table
 */
zOP:
	.word	z00-zOP
	.word	z01-zOP
	.word	z02-zOP
	.word	z03-zOP
	.word	z04-zOP
	.word	z05-zOP
	.word	z06-zOP
	.word	z07-zOP
	.word	z08-zOP
	.word	z09-zOP
	.word	z0A-zOP
	.word	z0B-zOP
	.word	z0C-zOP
	.word	z0D-zOP
	.word	z0E-zOP
	.word	z0F-zOP
	.word	z10-zOP
	.word	z11-zOP
	.word	z12-zOP
	.word	z13-zOP
	.word	z14-zOP
	.word	z15-zOP
	.word	z16-zOP
	.word	z17-zOP
	.word	z18-zOP
	.word	z19-zOP
	.word	z1A-zOP
	.word	z1B-zOP
	.word	z1C-zOP
	.word	z1D-zOP
	.word	z1E-zOP
	.word	z1F-zOP
	.word	z20-zOP
	.word	z21-zOP
	.word	z22-zOP
	.word	z23-zOP
	.word	z24-zOP
	.word	z25-zOP
	.word	z26-zOP
	.word	z27-zOP
	.word	z28-zOP
	.word	z29-zOP
	.word	z2A-zOP
	.word	z2B-zOP
	.word	z2C-zOP
	.word	z2D-zOP
	.word	z2E-zOP
	.word	z2F-zOP
	.word	z30-zOP
	.word	z31-zOP
	.word	z32-zOP
	.word	z33-zOP
	.word	z34-zOP
	.word	z35-zOP
	.word	z36-zOP
	.word	z37-zOP
	.word	z38-zOP
	.word	z39-zOP
	.word	z3A-zOP
	.word	z3B-zOP
	.word	z3C-zOP
	.word	z3D-zOP
	.word	z3E-zOP
	.word	z3F-zOP
	.word	z40-zOP
	.word	z41-zOP
	.word	z42-zOP
	.word	z43-zOP
	.word	z44-zOP
	.word	z45-zOP
	.word	z46-zOP
	.word	z47-zOP
	.word	z48-zOP
	.word	z49-zOP
	.word	z4A-zOP
	.word	z4B-zOP
	.word	z4C-zOP
	.word	z4D-zOP
	.word	z4E-zOP
	.word	z4F-zOP
	.word	z50-zOP
	.word	z51-zOP
	.word	z52-zOP
	.word	z53-zOP
	.word	z54-zOP
	.word	z55-zOP
	.word	z56-zOP
	.word	z57-zOP
	.word	z58-zOP
	.word	z59-zOP
	.word	z5A-zOP
	.word	z5B-zOP
	.word	z5C-zOP
	.word	z5D-zOP
	.word	z5E-zOP
	.word	z5F-zOP
	.word	z60-zOP
	.word	z61-zOP
	.word	z62-zOP
	.word	z63-zOP
	.word	z64-zOP
	.word	z65-zOP
	.word	z66-zOP
	.word	z67-zOP
	.word	z68-zOP
	.word	z69-zOP
	.word	z6A-zOP
	.word	z6B-zOP
	.word	z6C-zOP
	.word	z6D-zOP
	.word	z6E-zOP
	.word	z6F-zOP
	.word	z70-zOP
	.word	z71-zOP
	.word	z72-zOP
	.word	z73-zOP
	.word	z74-zOP
	.word	z75-zOP
	.word	z76-zOP
	.word	z77-zOP
	.word	z78-zOP
	.word	z79-zOP
	.word	z7A-zOP
	.word	z7B-zOP
	.word	z7C-zOP
	.word	z7D-zOP
	.word	z7E-zOP
	.word	z7F-zOP
	.word	z80-zOP
	.word	z81-zOP
	.word	z82-zOP
	.word	z83-zOP
	.word	z84-zOP
	.word	z85-zOP
	.word	z86-zOP
	.word	z87-zOP
	.word	z88-zOP
	.word	z89-zOP
	.word	z8A-zOP
	.word	z8B-zOP
	.word	z8C-zOP
	.word	z8D-zOP
	.word	z8E-zOP
	.word	z8F-zOP
	.word	z90-zOP
	.word	z91-zOP
	.word	z92-zOP
	.word	z93-zOP
	.word	z94-zOP
	.word	z95-zOP
	.word	z96-zOP
	.word	z97-zOP
	.word	z98-zOP
	.word	z99-zOP
	.word	z9A-zOP
	.word	z9B-zOP
	.word	z9C-zOP
	.word	z9D-zOP
	.word	z9E-zOP
	.word	z9F-zOP
	.word	zA0-zOP
	.word	zA1-zOP
	.word	zA2-zOP
	.word	zA3-zOP
	.word	zA4-zOP
	.word	zA5-zOP
	.word	zA6-zOP
	.word	zA7-zOP
	.word	zA8-zOP
	.word	zA9-zOP
	.word	zAA-zOP
	.word	zAB-zOP
	.word	zAC-zOP
	.word	zAD-zOP
	.word	zAE-zOP
	.word	zAF-zOP
	.word	zB0-zOP
	.word	zB1-zOP
	.word	zB2-zOP
	.word	zB3-zOP
	.word	zB4-zOP
	.word	zB5-zOP
	.word	zB6-zOP
	.word	zB7-zOP
	.word	zB8-zOP
	.word	zB9-zOP
	.word	zBA-zOP
	.word	zBB-zOP
	.word	zBC-zOP
	.word	zBD-zOP
	.word	zBE-zOP
	.word	zBF-zOP
	.word	zC0-zOP
	.word	zC1-zOP
	.word	zC2-zOP
	.word	zC3-zOP
	.word	zC4-zOP
	.word	zC5-zOP
	.word	zC6-zOP
	.word	zC7-zOP
	.word	zC8-zOP
	.word	zC9-zOP
	.word	zCA-zOP
	.word	zCB-zOP
	.word	zCC-zOP
	.word	zCD-zOP
	.word	zCE-zOP
	.word	zCF-zOP
	.word	zD0-zOP
	.word	zD1-zOP
	.word	zD2-zOP
	.word	zD3-zOP
	.word	zD4-zOP
	.word	zD5-zOP
	.word	zD6-zOP
	.word	zD7-zOP
	.word	zD8-zOP
	.word	zD9-zOP
	.word	zDA-zOP
	.word	zDB-zOP
	.word	zDC-zOP
	.word	zDD-zOP
	.word	zDE-zOP
	.word	zDF-zOP
	.word	zE0-zOP
	.word	zE1-zOP
	.word	zE2-zOP
	.word	zE3-zOP
	.word	zE4-zOP
	.word	zE5-zOP
	.word	zE6-zOP
	.word	zE7-zOP
	.word	zE8-zOP
	.word	zE9-zOP
	.word	zEA-zOP
	.word	zEB-zOP
	.word	zEC-zOP
	.word	zED-zOP
	.word	zEE-zOP
	.word	zEF-zOP
	.word	zF0-zOP
	.word	zF1-zOP
	.word	zF2-zOP
	.word	zF3-zOP
	.word	zF4-zOP
	.word	zF5-zOP
	.word	zF6-zOP
	.word	zF7-zOP
	.word	zF8-zOP
	.word	zF9-zOP
	.word	zFA-zOP
	.word	zFB-zOP
	.word	zFC-zOP
	.word	zFD-zOP
	.word	zFE-zOP
	.word	zFF-zOP

/*
 * z-80 instructions
 */

| NOP
z00:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts

| LD BC,nnnn
z01:
	move.b	(%a3)+, regC(%a0)	| Load C
	move.b	(%a3)+, regB(%a0)	| Load B
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD (BC),A
z02:
	moveq	#0, %d0
	move.b	regBC(%a0), %d0		| high byte of address
	move.w	regBC(%a0), %d4		| address
	move.b	%d2, %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC BC
z03:
	addq.w  #1, regBC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC B
z04:
        addq.b  #1, regB(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| DEC B
z05:
        subq.b  #1, regB(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD B,nn
z06:
	move.b	(%a3)+, regB(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RLCA
z07:
        rol.b   #1, %d2
        scs	%d6			| Set or clear C
	moveq	#0, %d7			| Clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD (nnnn),SP  ---- special (old ex af,af)
z08:
	move.b  (%a3)+, %d0		| use d0 as temp. holder
	move.b	(%a3)+, -(%sp)
	move.w	(%sp)+, %d4
	bpl.s	z08Rom       		| Cant write to ROM
	move.b	%d0, %d4

	move.l	%a6, %d0		| see if SP in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	z08SPOk
	move.l	%a6, %d0		| See if SP in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	z08SPOk
	move.l	%a6, %d0		| In RAM
	sub.l	%a1, %d0
z08SPOk:
        move.b  %d0, (%a1,%d4.l)	| Save LSB of Stack
	move.w	%d0, -(%sp)		| Save SP
	move.b	(%sp)+, 1(%a1,%d4.l)	| Save MSB of Stack
	subq.b	#5,%d5			| Decrement Instruction Counter
	bpl	DoNext			| Decrement Inst. Counter, do another if >0
	rts

z08Rom:
	subq.b	#5,%d5			| Decrement Instruction Counter
	bpl	DoNext			| Decrement Inst. Counter, do another if >0
	rts

|	move.b	%d0, %d4

|	move.b	-1(%a3), %d0		| High byte of address

|	move.l	%a6, %d1
|	move.w	regSP(%a0), %d0		| Get SP
|	bmi.s	z08SPRRAM		| if SP in RAM
|	cmpi.w	#0x4000, %d0		| See if SP in Low ROM
|	blo.s	z08SPRLowROM
|	sub.l	%a5, %d1		| High ROM, get SP
|	bra.s	z08SPR
|z08SPRRAM:
|	sub.l	%a1, %d1		| RAM, get SP
|	bra.s	z08SPR
|z08SPRLowROM:
| 	sub.l	%a2, %d1		| Low ROM, get SP
|z08SPR:



| ADD HL,BC
z09:
	add.w	regBC(%a0), %d3
	scs	%d6			| Set or clear C
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD A,(BC)
z0A:
        move.w  regBC(%a0), %d0
	bpl.s	z0AROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blt.s	z0ARAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z0ARAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z0ARAMOk:
        move.b  (%a1, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z0AROM:
	cmpi.w	#0x4000, %d0		| See if Low ROM
	blo.s	z0ALowROM
	move.b	(%a5, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z0ALowROM:
	move.b	(%a2, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DEC BC
z0B:
        subq.w  #1, regBC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| INC C
z0C:
        addq.b  #1, regC(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| DEC C
z0D:
        subq.b  #1, regC(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD C,nn
z0E:
        move.b  (%a3)+, regC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| RRCA
z0F:
        ror.b   #1, %d2
        scs	%d6			| Set or clear C
	moveq	#0, %d7			| Clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| STOP --- special ??? (old djnz disp)
z10:
	/* GBC - HIGH SPEED SWITCH */
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	/* Wait for keypress */

	rts


| LD DE,nnnn
z11:
	move.b	(%a3)+, regE(%a0)	| Load E
	move.b	(%a3)+, regD(%a0)	| Load D
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| LD (DE),A
z12:
	move.w	regDE(%a0), %d0

	moveq	#0, %d0
	move.b	regDE(%a0), %d0		| high byte of address
	move.w	regDE(%a0), %d4		| address
	move.b	%d2, %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC DE
z13:
        addq.w  #1, regDE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| INC D
z14:
        addq.b  #1, regD(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| DEC D
z15:
        subq.b  #1, regD(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| LD D,nn
z16:
        move.b	(%a3)+,regD(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| RLA
z17:
        asr.b   #1, %d6			| Set carry in CCR
        roxl.b  #1, %d2
        scs	%d6			| Set or clear C
	moveq	#0, %d7			| Clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| JR disp
z18:
	move.b	(%a3)+, %d0
        ext.w	%d0      	        | extend sign (for - jumps)
	adda.w	%d0, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| ADD HL,DE
z19:
	add.w	regDE(%a0), %d3
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| LD A,(DE)
z1A:
        move.w  regDE(%a0), %d0
	bpl.s	z1AROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blt.s	z1ARAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z1ARAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z1ARAMOk:
	cmp.w	#0xA000, %d0		| See if reading Timer in MBC3 (Pokemon)
	bne.s	z1ARAMOk2
	move.b	CurRAMBank(%a0), %d1	| yes, get selected timer value
	cmp.b	#0x08, %d1
	blt.s	z1ARAMOk2
	subq	#8, %d1
	move.b	cTimerSec(%a0, %d1.w), %d2	
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z1ARAMOk2:
        move.b  (%a1, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z1AROM:
	cmpi.w	#0x4000, %d0		| See if Low ROM
	blo.s	z1ALowROM
	move.b	(%a5, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z1ALowROM:
	move.b	(%a2, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DEC DE
z1B:
        subq.w  #1, regDE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| INC E
z1C:
        addq.b  #1, regE(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| DEC E
z1D:
        subq.b  #1, regE(%a0)
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| LD E,nn
z1E:
        move.b  (%a3)+, regE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| RRA
z1F:
	asr.b   #1, %d6			| Set carry in CCR
	roxr.b  #1, %d2
	scs	%d6			| Set or clear C
	moveq	#0, %d7			| Clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| JR NZ,disp
z20:
	move.b	%d7, %d0		| only jump if Z reset (0)
	not	%d0
	and.b	(%a3)+, %d0
	ext.w	%d0      	        | extend sign (for - jumps)
	adda.w	%d0, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| LD HL,nnnn
z21:
	move.b	(%a3)+, %d0		| get low byte
	move.b	(%a3)+, -(%sp)		| get high byte
	move.w	(%sp)+, %d3		| 
	move.b	%d0, %d3		| add in low byte

	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| LDI (HL),A --- special (old ld (nnnn),hl)
z22:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	%d2, %d1		| byte to write
        addq.w  #1, %d3
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC  HL
z23:
        addq.w  #1, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| INC  H
z24:
	add.w	#0x0100, %d3
	cmpi.w	#0x0100, %d3
	scs	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| DEC H
z25:
	sub.w	#0x0100, %d3
	cmpi.w	#0x0100, %d3
	scs	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0

	rts


| LD H,nn
z26:
	move.b	%d3, %d0		| Save L
	move.b	(%a3)+,-(%sp)		| Get n to H
	move.w	(%sp)+, %d3		| 
	move.b	%d0, %d3		| Restore L
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| DAA
z27:
	moveq	#0, %d0
	move.b	regA_old(%a0), %d0	| get old A
	tst.b	%d6			| See if carry flag set
	beq.s	z27Cont
	cmp.b	%d0, %d2		| see which is smaller (old or new)
	bhs.s	z27sub			| old was smaller, so was subtract
					| else, add
z27add:
	exg	%d0, %d2		| swap values (smaller to d2)
	sub.b	%d2, %d0		| get difference to d0
	andi	#04, %CCR		| set Z-bit of CCR, reset X-bit
	abcd	%d0, %d2		| Do BCD add
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	moveq	#0, %d0

	rts

z27Cont:
	cmp.b	%d0, %d2		| see which is smaller (old or new)
	bhs.s	z27add			| old was smaller, so was add
					| else, subtract
z27sub:
	sub.b	%d2, %d0		| get difference to d0
	add.b	%d0, %d2		| make d2 old value
	andi	#04, %CCR		| set Z-bit of CCR, reset X-bit
	sbcd	%d0, %d2		| Do BCD subtract
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	moveq	#0, %d0

	rts




| JR Z,disp
z28:
        move.b  (%a3)+, %d0
        and.b	%d7, %d0		| only jump if Z set (1)
        ext.w	%d0    			| extend sign (for - jumps)
	adda.w	%d0, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| ADD  HL,HL
z29:
        add.w   %d3, %d3
        scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0

	rts


| LDI  A,(HL) ---- special (old ld hl,(nnnn))
z2A:
	move.w	%d3, %d0
	bpl.s	z2AROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z2ARAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z2ARAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z2ARAMOk:
        move.b  (%a1, %d0.l), %d2
        addq.w  #1, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z2AROM:
        addq.w  #1, %d3
	cmpi.w	#0x4000, %d0		| See if Low ROM
	blo.s	z2ALowROM
	move.b	(%a5, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z2ALowROM:
	move.b	(%a2, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DEC HL
z2B:
        subq.w  #1, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC L
z2C:
        addq.b  #1, %d3
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| DEC L
z2D:
        subq.b  #1, %d3
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD L,nn
z2E:
        move.b  (%a3)+, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| CPL
z2F:
        not.b   %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| JR NC,disp
z30:
	move.b	%d6, %d0
	not.b	%d0			| only jump if C reset (0)
	and.b	(%a3)+, %d0
        ext.w	%d0			| extend sign (for - jumps)
	adda.w	%d0, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD SP,nnnn
z31:
	move.b	(%a3)+, %d4
	move.b	(%a3)+, -(%sp)
	move.w	(%sp)+, %d0
	bpl.s	z31ROM
    move.b	%d4, %d0
	movea.l	%a1, %a6
	adda.l	%d0, %a6
|	move.w	%d0, regSP(%a0)		| Store for later calcs
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z31ROM:
    move.b	%d4, %d0
|	move.w	%d0, regSP(%a0)		| Store for later calcs
	cmpi.w	#0x4000, %d0		| See if Low ROM
	blo.s	z31LowROM
	movea.l	%a5, %a6
	adda.l	%d0, %a6
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z31LowROM:
	movea.l	%a2, %a6
	adda.l	%d0, %a6
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LDD  (HL),A --- special (old remapped ld (nnnn),a)
z32:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	%d2, %d1		| byte to write
        subq.w  #1, %d3
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC  SP
z33:
        addq.l  #1, %a6
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC  (HL)
z34:
	move.w	%d3, %d0
        bpl.s	z34ROM			| Cant write to ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z34RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z34RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z34RAMOk:
	move.b	(%a1, %d0.l), %d1
        addq.b  #1, %d1
	seq	%d7			| Set or clear Z

	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z34ROM:
	subq.b	#3, %d5			| Decrement Instruction Counter
	cmpi.w	#0x4000, %d0
	blo.s	z34LowROM		| If writing to Low ROM
	move.b	(%a5, %d0.l), %d1
	addq.b	#1, %d1
        seq	%d7			| Set or clear Z
	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


z34LowROM:
	move.b	(%a2, %d0.l), %d1
	addq.b	#1, %d1
        seq	%d7			| Set or clear Z
	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DEC (HL)
z35:
	move.w	%d3, %d0
        bpl.s	z35ROM			| Cant write to ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z35RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z35RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z35RAMOk:
	move.b	(%a1, %d0.l), %d1
        subq.b  #1, %d1
	seq	%d7			| Set or clear Z

	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z35ROM:
	subq.b	#3, %d5			| Decrement Instruction Counter
	cmpi.w	#0x4000, %d0
	blo.s	z35LowROM		| If writing to Low ROM
	move.b	(%a5, %d0.l), %d1
	subq.b	#1, %d1
        seq	%d7			| Set or clear Z
	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


z35LowROM:
	move.b	(%a2, %d0.l), %d1
	subq.b	#1, %d1
        seq	%d7			| Set or clear Z
	move.w	%d0, %d4		| address
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| LD   (HL),nn
z36:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	(%a3)+, %d1		| byte to write
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| SCF
z37:
        st	%d6			| Set C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| JR C,disp
z38:
        move.b  (%a3)+, %d0
        and.b	%d6, %d0		| only jump if C set (1)
        ext.w	%d0			| extend sign (for - jumps)
	adda.w	%d0, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| ADD  HL,SP
z39:
	move.l	%a6, %d0		| see if SP in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	z39SPOk
	move.l	%a6, %d0		| See if SP in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	z39SPOk
	move.l	%a6, %d0		| In RAM
	sub.l	%a1, %d0
z39SPOk:
        add.w   %d0, %d3
        scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LDD  A,(HL) --- special (old remapped ld a,(nnnn))
z3A:
	move.w	%d3, %d0
	bpl.s	z3AROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z3ARAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z3ARAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z3ARAMOk:
        move.b  (%a1, %d0.l), %d2
        subq.w  #1, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z3AROM:
        subq.w  #1, %d3
	cmpi.w	#0x4000, %d0
	blo.s	z3ALowROM
	move.b	(%a5, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

z3ALowROM:
	move.b	(%a2, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DEC SP
z3B:
        subq.l  #1, %a6
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| INC A
z3C:
	move.b	%d2, regA_old(%a0)
        addq.b  #1, %d2
        seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| DEC A
z3D:
	move.b	%d2, regA_old(%a0)
	subq.b  #1, %d2
	seq	%d7			| Set or clear Z
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD  A,nn
z3E:
	move.b  (%a3)+, %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| CCF
z3F:
        not.b	%d6
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,B
z40:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,C
z41:
        move.b  regC(%a0), regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,D
z42:
        move.b  regD(%a0), regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,E
z43:
        move.b  regE(%a0), regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,H
z44:
	move.w	%d3, -(%sp)
        move.b  (%sp)+, regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,L
z45:
        move.b	%d3, regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   B,(HL)
z46:
	move.w	%d3, %d0
	bpl.s	z46ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z46RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z46RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z46RAMOk:
        move.b  (%a1, %d0.l), regB(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z46ROM:
	cmpi.w	#0x4000, %d0
	blo.s	z46LowROM
	move.b	(%a5, %d0.l), regB(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z46LowROM:
	move.b	(%a2, %d0.l), regB(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   B,A
z47:
        move.b	%d2, regB(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,B
z48:
        move.b  regB(%a0), regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,C
z49:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,D
z4A:
        move.b  regD(%a0), regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,E
z4B:
        move.b  regE(%a0), regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,H
z4C:
	move.w	%d3, -(%sp)
        move.b  (%sp)+, regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,L
z4D:
        move.b  %d3, regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   C,(HL)
z4E:
	move.w	%d3, %d0
	bpl.s	z4EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z4ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z4ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z4ERAMOk:
        move.b  (%a1, %d0.l), regC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z4EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z4ELowROM
	move.b	(%a5, %d0.l), regC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z4ELowROM:
	move.b	(%a2, %d0.l), regC(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   C,A
z4F:
        move.b  %d2, regC(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,B
z50:
        move.b  regB(%a0), regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,C
z51:
        move.b  regC(%a0), regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,D
z52:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,E
z53:
        move.b  regE(%a0), regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,H
z54:
	move.w	%d3, -(%sp)
        move.b  (%sp)+, regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,L
z55:
        move.b  %d3, regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   D,(HL)
z56:
	move.w	%d3, %d0
	bpl.s	z56ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z56RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z56RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z56RAMOk:
        move.b  (%a1, %d0.l), regD(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z56ROM:
	cmpi.w	#0x4000, %d0
	blo.s	z56LowROM
	move.b	(%a5, %d0.l), regD(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z56LowROM:
	move.b	(%a2, %d0.l), regD(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   D,A
z57:
        move.b  %d2, regD(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,B
z58:
        move.b  regB(%a0), regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,C
z59:
        move.b  regC(%a0), regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,D
z5A:
        move.b  regD(%a0), regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,E
z5B:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,H
z5C:
	move.w	%d3, -(%sp)
        move.b  (%sp)+, regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,L
z5D:
        move.b	%d3, regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   E,(HL)
z5E:
	move.w	%d3, %d0
	bpl.s	z5EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z5ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z5ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z5ERAMOk:
        move.b  (%a1, %d0.l), regE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z5EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z5ELowROM
	move.b	(%a5, %d0.l), regE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z5ELowROM:
	move.b	(%a2, %d0.l), regE(%a0)
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   E,A
z5F:
        move.b  %d2, regE(%a0)
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,B
z60:
	move.b	%d3, %d0		| Store L
        move.w  regB(%a0), %d3
	move.b	%d0, %d3		| Restore L
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts
 

| LD   H,C
z61:
	move.b	%d3, %d0
        move.b  regC(%a0), -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,D
z62:
	move.b	%d3, %d0
        move.w  regD(%a0), %d3
	move.b	%d0, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,E
z63:
	move.b	%d3, %d0
        move.b  regE(%a0), -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,H
z64:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,L
z65:
	move.b	%d3, %d0
        move.b  %d3, -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   H,(HL)
z66:
	move.w	%d3, %d0
	bpl.s	z66ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z66RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z66RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z66RAMOk:
        move.b  (%a1, %d0.l), -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z66ROM:
	cmpi.w	#0x4000, %d0
	blo.s	z66LowROM
	move.b  (%a5, %d0.l), -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z66LowROM:
	move.b  (%a2, %d0.l), -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   H,A
z67:
	move.b	%d3, %d0
        move.b  %d2, -(%sp)
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,B
z68:
        move.b  regB(%a0), %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,C
z69:
        move.b  regC(%a0), %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,D
z6A:
        move.b  regD(%a0), %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,E
z6B:
        move.b	regE(%a0), %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,H
z6C:
	move.w	%d3, -(%sp)
        move.b  (%sp)+, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,L
z6D:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   L,(HL)
z6E:
	move.w	%d3, %d0
	bpl.s	z6EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z6ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z6ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z6ERAMOk:
        move.b  (%a1, %d0.l), %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z6EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z6ELowROM
	move.b	(%a5, %d0.l), %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z6ELowROM:
	move.b  (%a2, %d0.l), %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   L,A
z6F:
        move.b  %d2, %d3
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   (HL),B
z70:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	regBC(%a0), %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
 

| LD   (HL),C
z71:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	regBC+1(%a0), %d1	| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (HL),D
z72:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	regDE(%a0), %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (HL),E
z73:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	regDE+1(%a0), %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (HL),H
z74:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	%d0, %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (HL),L
z75:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	%d3, %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| HALT
z76:
	tst.b	regInt(%a0)		| See if interrupt enabled
	beq.s	z76NoInt
	subq.l	#1, %a3			| yes, Keep PC at Halt until Interrupt
	rts
z76NoInt:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   (HL),A
z77:
	move.w	%d3, %d4		| address

	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	move.b	%d2, %d1		| byte to write
	bsr	WriteMem
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   A,B
z78:
        move.b  regB(%a0), %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,C
z79:
        move.b  regC(%a0), %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,D
z7A:
        move.b  regD(%a0), %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,E
z7B:
        move.b  regE(%a0), %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,H
z7C:
	move.w	%d3, -(%sp)
        move.b	(%sp)+, %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,L
z7D:
        move.b	%d3, %d2
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| LD   A,(HL)
z7E:
	move.w	%d3, %d0
	bpl.s	z7EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z7ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z7ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z7ERAMOk:
        move.b  (%a1, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z7EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z7ELowROM
	move.b	(%a5, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z7ELowROM:
	move.b  (%a2, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   A,A
z7F:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,B
z80:
	move.b	%d2, regA_old(%a0)
	add.b	regB(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,C
z81:
	move.b	%d2, regA_old(%a0)
	add.b	regC(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,D
z82:
	move.b	%d2, regA_old(%a0)
	add.b	regD(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,E
z83:
	move.b	%d2, regA_old(%a0)
	add.b	regE(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,H
z84:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, -(%sp)
	add.b	(%sp)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,L
z85:
	move.b	%d2, regA_old(%a0)
	add.b	%d3, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADD  A,(HL)
z86:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, %d0
	bpl.s	z86ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z86RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z86RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z86RAMOk:
	add.b	(%a1, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z86ROM:
	cmpi.w	#0x4000, %d0
	blo.s	z86LowROM
	add.b   (%a5, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z86LowROM:
	add.b   (%a2, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| ADD  A,A
z87:
	move.b	%d2, regA_old(%a0)
	add.b	%d2, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts

| ADC  A,B
z88:
	move.b	%d2, regA_old(%a0)
	move.b  regB(%a0), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,C
z89:
	move.b	%d2, regA_old(%a0)
	move.b  regC(%a0), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,D
z8A:
	move.b	%d2, regA_old(%a0)
	move.b  regD(%a0), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,E
z8B:
	move.b	%d2, regA_old(%a0)
	move.b  regE(%a0), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,H
z8C:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, -(%sp)
	move.b  (%sp)+, %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,L
z8D:
	move.b	%d2, regA_old(%a0)
	asr.b	#1, %d6
	addx.b	%d3, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| ADC  A,(HL)
z8E:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, %d0
	bpl.s	z8EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z8ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z8ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z8ERAMOk:
	move.b	(%a1, %d0.l), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z8EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z8ELowROM
	move.b	(%a5, %d0.l), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z8ELowROM:
	move.b	(%a2, %d0.l), %d0
	asr.b	#1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| ADC  A,A
z8F:
	move.b	%d2, regA_old(%a0)
	asr.b	#1, %d6
	addx.b	%d2, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  B
z90:
	move.b	%d2, regA_old(%a0)
	sub.b	regB(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  C
z91:
	move.b	%d2, regA_old(%a0)
	sub.b	regC(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  D
z92:
	move.b	%d2, regA_old(%a0)
	sub.b	regD(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  E
z93:
	move.b	%d2, regA_old(%a0)
	sub.b	regE(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  H
z94:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, -(%sp)
	sub.b	(%sp)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  L
z95:
	move.b	%d2, regA_old(%a0)
	sub.b	%d3, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SUB  (HL)
z96:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, %d0
	bpl.s	z96ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z96RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z96RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z96RAMOk:
	sub.b	(%a1, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z96ROM:
	cmpi.w	#0x4000, %d0
	blo.s	z96LowROM
	sub.b	(%a5, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z96LowROM:
	sub.b	(%a2, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| SUB  A
z97:
	move.b	%d2, regA_old(%a0)
	sub.b	%d2, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, B
z98:
	move.b	%d2, regA_old(%a0)
	move.b  regB(%a0), %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, C
z99:
	move.b	%d2, regA_old(%a0)
	move.b  regC(%a0), %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, D
z9A:
	move.b	%d2, regA_old(%a0)
	move.b  regD(%a0), %d0
	asr.b	#1, %d6
	subx.b  %d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, E
z9B:
	move.b	%d2, regA_old(%a0)
	move.b  regE(%a0), %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, H
z9C:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, -(%sp)
	move.b  (%sp)+, %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, L
z9D:
	move.b	%d2, regA_old(%a0)
	asr.b	#1, %d6
	subx.b	%d3, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC A, (HL)
z9E:
	move.b	%d2, regA_old(%a0)
	move.w	%d3, %d0
	bpl.s	z9EROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	z9ERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	z9ERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
z9ERAMOk:
	move.b	(%a1, %d0.l), %d0
	asr.b	#1, %d6
	subx.b  %d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z9EROM:
	cmpi.w	#0x4000, %d0
	blo.s	z9ELowROM
	move.b	(%a5, %d0.l), %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
z9ELowROM:
	move.b	(%a2, %d0.l), %d0
	asr.b	#1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| SBC A, A
z9F:
	move.b	%d2, regA_old(%a0)
	asr.b	#1, %d6
	subx.b	%d2, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  B
zA0:
	and.b	regB(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  C
zA1:
	and.b   regC(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  D
zA2:
	and.b	regD(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  E
zA3:
	and.b	regE(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  H
zA4:
	move.w	%d3, -(%sp)
	and.b	(%sp)+, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  L
zA5:
	and.b	%d3, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| AND  (HL)
zA6:
	move.w	%d3, %d0
	bpl.s	zA6ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	zA6RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	zA6RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
zA6RAMOk:
	and.b   (%a1, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zA6ROM:
	cmpi.w	#0x4000, %d0
	blo.s	zA6LowROM
	and.b   (%a5, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zA6LowROM:
	and.b   (%a2, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| AND  A
zA7:
	tst.b	%d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  B
zA8:
	move.b	regB(%a0), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  C
zA9:
	move.b	regC(%a0), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  D
zAA:
	move.b	regD(%a0), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  E
zAB:
	move.b	regE(%a0), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  H
zAC:
	move.w	%d3, -(%sp)
	move.b	(%sp)+, %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  L
zAD:
	eor.b	%d3, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  (HL)
zAE:
	move.w	%d3, %d0
	bpl.s	zAEROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	zAERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	zAERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
zAERAMOk:
	move.b	(%a1, %d0.l), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zAEROM:
	cmpi.w	#0x4000, %d0
	blo.s	zAELowROM
	move.b	(%a5, %d0.l), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zAELowROM:
	move.b	(%a2, %d0.l), %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| XOR  A
zAF:
	clr.b	%d2
	st	%d7			| Set Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  B
zB0:
	or.b	regB(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  C
zB1:
	or.b	regC(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  D
zB2:
	or.b	regD(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  E
zB3:
	or.b	regE(%a0), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  H
zB4:
	move.w	%d3, -(%sp)
	or.b	(%sp)+, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  L
zB5:
	or.b	%d3, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| OR  (HL)
zB6:
	move.w	%d3, %d0
	bpl.s	zB6ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	zB6RAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	zB6RAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
zB6RAMOk:
	or.b	(%a1, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zB6ROM:
	cmpi.w	#0x4000, %d0
	blo.s	zB6LowROM
	or.b	(%a5, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zB6LowROM:
	or.b	(%a2, %d0.l), %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| OR  A
zB7:
	tst.b	%d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  B
zB8:
	cmp.b   regB(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  C
zB9:
	cmp.b   regC(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  D
zBA:
	cmp.b   regD(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  E
zBB:
	cmp.b   regE(%a0), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  H
zBC:
	move.w	%d3, -(%sp)
	cmp.b   (%sp)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  L
zBD:
	cmp.b   %d3, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP  (HL)
zBE:
	move.w	%d3, %d0
	bpl.s	zBEROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	zBERAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	zBERAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
zBERAMOk:
	cmp.b	(%a1, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zBEROM:
	cmpi.w	#0x4000, %d0
	blo.s	zBELowROM
	cmp.b	(%a5, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zBELowROM:
	cmp.b	(%a2, %d0.l), %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| CP  A
zBF:
	st	%d7			| Set Z
	moveq	#0, %d6			| Clear C
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts



| RET  NZ
zC0:
	tst.b	%d7			| dont return if Z is set
	beq	zRET
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| POP  BC
zC1:
	move.b	(%a6)+, regC(%a0)
	move.b	(%a6)+, regB(%a0)
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| JP   NZ,nnnn
zC2:
	tst.b	%d7			| don t jump if Z flag is 1
	beq.s	zDoJump
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


zDoJump:				| Conditional Jumps (1 xtra cycle)
	subq.b	#1, %d5
| JP   nnnn
zC3:
	move.b	1(%a3), -(%sp)		| Get jump location (high)
	move.w	(%sp)+, %d0		| 
	bmi.s	zC3RAM			| RAM Jump?
	move.b	(%a3), %d0		| No, Get jump location (low)
	move.l	%d0, %a3
	cmpi.w	#0x4000, %d0
	blo.s	zC3LowROM
	adda.l	%a5, %a3
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zC3LowROM:
	adda.l	%a2, %a3
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zC3RAM:
	move.b	(%a3), %d0		| Get jump location (low)
	move.l	%d0, %a3
	adda.l	%a1, %a3
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| CALL NZ,nnnn
zC4:
	tst.b	%d7			| don t call if Z flag is 1
	beq	zCD
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| PUSH BC
zC5:
	move.b	regB(%a0), -(%a6)
	move.b	regC(%a0), -(%a6)
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| ADD  A,nn
zC6:
	move.b	%d2, regA_old(%a0)
	add.b   (%a3)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  00H
zC7:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zC7PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zC7PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zC7PCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	move.l	%a2, %a3		| Jump to 0x00
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	moveq	#0, %d0
	rts


| RET  Z
zC8:
	tst.b	%d7			| Return if Z flag is 1
	bne.s	zRET
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


zRET:					| Conditional RETs (1 xtra cycle)
	subq.b	#1, %d5
| RET
zC9:
	move.b	1(%a6), -(%sp)		| Get address from stack (high)
	move.w	(%sp)+, %d0
	bmi.s	zRETToRAM
	move.b	(%a6), %d0		| (low)
	move.l	%d0, %a3
	addq.l  #2, %a6
	cmpi.w	#0x4000, %d0		| If reading from Bank 0
	blo.s	zC9LowROM
	adda.l  %a5, %a3		| Current Bank
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zC9LowROM:
	adda.l  %a2, %a3
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zRETToRAM:
	move.b	(%a6), %d0		| (low)
	addq.l  #2, %a6
	move.l	%d0, %a3
	adda.l  %a1, %a3
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| JP   Z,nnnn
zCA:
	tst.b	%d7			| jump if Z flag is 1
	bne	zDoJump
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| CB Codes
zCB:
	moveq	#0, %d0
	move.b	(%a3)+, %d0
	cmpi    #0x40, %d0
	bhs	zCBxBIT
	add.w	%d0, %d0		| x4 to get offset into zOPCB table
	move.w  zCBOP(%pc, %d0.w), %d0	| get address of code for instruction
	jmp     zCBOP(%pc, %d0.w)	| EXECUTE INSTRUCTION


zCBOP:
	.word	zCB00-zCBOP
	.word	zCB01-zCBOP
	.word	zCB02-zCBOP
	.word	zCB03-zCBOP
	.word	zCB04-zCBOP
	.word	zCB05-zCBOP
	.word	zCB06-zCBOP
	.word	zCB07-zCBOP
	.word	zCB08-zCBOP
	.word	zCB09-zCBOP
	.word	zCB0A-zCBOP
	.word	zCB0B-zCBOP
	.word	zCB0C-zCBOP
	.word	zCB0D-zCBOP
	.word	zCB0E-zCBOP
	.word	zCB0F-zCBOP
	.word	zCB10-zCBOP
	.word	zCB11-zCBOP
	.word	zCB12-zCBOP
	.word	zCB13-zCBOP
	.word	zCB14-zCBOP
	.word	zCB15-zCBOP
	.word	zCB16-zCBOP
	.word	zCB17-zCBOP
	.word	zCB18-zCBOP
	.word	zCB19-zCBOP
	.word	zCB1A-zCBOP
	.word	zCB1B-zCBOP
	.word	zCB1C-zCBOP
	.word	zCB1D-zCBOP
	.word	zCB1E-zCBOP
	.word	zCB1F-zCBOP
	.word	zCB20-zCBOP
	.word	zCB21-zCBOP
	.word	zCB22-zCBOP
	.word	zCB23-zCBOP
	.word	zCB24-zCBOP
	.word	zCB25-zCBOP
	.word	zCB26-zCBOP
	.word	zCB27-zCBOP
	.word	zCB28-zCBOP
	.word	zCB29-zCBOP
	.word	zCB2A-zCBOP
	.word	zCB2B-zCBOP
	.word	zCB2C-zCBOP
	.word	zCB2D-zCBOP
	.word	zCB2E-zCBOP
	.word	zCB2F-zCBOP
	.word	zCB30-zCBOP
	.word	zCB31-zCBOP
	.word	zCB32-zCBOP
	.word	zCB33-zCBOP
	.word	zCB34-zCBOP
	.word	zCB35-zCBOP
	.word	zCB36-zCBOP
	.word	zCB37-zCBOP
	.word	zCB38-zCBOP
	.word	zCB39-zCBOP
	.word	zCB3A-zCBOP
	.word	zCB3B-zCBOP
	.word	zCB3C-zCBOP
	.word	zCB3D-zCBOP
	.word	zCB3E-zCBOP
	.word	zCB3F-zCBOP


| RLC  B
zCB00:
	move.b	regB(%a0), %d0
	rol.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  C
zCB01:
	move.b	regC(%a0), %d0
	rol.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  D
zCB02:
	move.b	regD(%a0), %d0
	rol.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  E
zCB03:
	subq.b	#4, %d5
	move.b	regE(%a0), %d0
	rol.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  H
zCB04:
	ror.w	#8, %d3
	rol.b	#1, %d3
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  L
zCB05:
	rol.b	#1, %d3
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  (HL)
zCB06:
	move.w	%d3, %d4
	bpl.s	zCB06Cont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb06RAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb06RAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb06RAMOk:
	move.b	(%a1, %d4.l), %d1
	rol.b	#1, %d1			| byte to write
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB06Cont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RLC  A
zCB07:
	rol.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  B
zCB08:
	move.b	regB(%a0), %d0
	ror.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  C
zCB09:
	move.b	regC(%a0), %d0
	ror.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  D
zCB0A:
	move.b	regD(%a0), %d0
	ror.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  E
zCB0B:
	move.b	regE(%a0), %d0
	ror.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  H
zCB0C:
	ror.w	#8, %d3
	ror.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  L
zCB0D:
	ror.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  (HL)
zCB0E:
	move.w	%d3, %d4
	bpl.s	zCB0ECont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb0ERAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb0ERAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb0ERAMOk:
	move.b	(%a1, %d4.l), %d1
	ror.b	#1, %d1			| byte to write
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB0ECont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RRC  A
zCB0F:
	ror.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RL   B
zCB10:
	move.b	regB(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   C
zCB11:
	move.b	regC(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   D
zCB12:
	move.b	regD(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   E
zCB13:
	move.b	regE(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   H
zCB14:
	ror.w	#8, %d3
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   L
zCB15:
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   (HL)
zCB16:
	move.w	%d3, %d4
	bpl.s	zCB16Cont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb16RAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb16RAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb16RAMOk:
	move.b	(%a1, %d4.l), %d1
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d1
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB16Cont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RL   A
zCB17:
	asr.b	#1, %d6			| Get C flag to (x)
	roxl.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   B
zCB18:
	move.b	regB(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   C
zCB19:
	move.b	regC(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   D
zCB1A:
	move.b	regD(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   E
zCB1B:
	move.b	regE(%a0), %d0
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   H
zCB1C:
	ror.w	#8, %d3
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   L
zCB1D:
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   (HL)
zCB1E:
	move.w	%d3, %d4
	bpl.s	zCB1ECont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb1ERAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb1ERAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb1ERAMOk:
	move.b	(%a1, %d4.l), %d1
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d1
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB1ECont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| RR   A
zCB1F:
	asr.b	#1, %d6			| Get C flag to (x)
	roxr.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| SLA  B
zCB20:
	move.b	regB(%a0), %d0
	asl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  C
zCB21:
	move.b	regC(%a0), %d0
	asl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  D
zCB22:
	move.b	regD(%a0), %d0
	asl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  E
zCB23:
	move.b	regE(%a0), %d0
	asl.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  H
zCB24:
	ror.w	#8, %d3
	asl.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  L
zCB25:
	asl.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  (HL)
zCB26:
	move.w	%d3, %d4
	bpl.s	zCB26Cont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb26RAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb26RAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb26RAMOk:
	move.b	(%a1, %d4.l), %d1
	asl.b	#1, %d1
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB26Cont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SLA  A
zCB27:
	asl.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  B
zCB28:
	move.b	regB(%a0), %d0
	asr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  C
zCB29:
	move.b	regC(%a0), %d0
	asr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  D
zCB2A:
	move.b	regD(%a0), %d0
	asr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  E
zCB2B:
	move.b	regE(%a0), %d0
	asr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  H
zCB2C:
	ror.w	#8, %d3
	asr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  L
zCB2D:
	asr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  (HL)
zCB2E:
	move.w	%d3, %d4
	bpl.s	zCB2ECont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb2ERAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb2ERAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb2ERAMOk:
	move.b	(%a1, %d4.l), %d1
	asr.b	#1, %d1
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB2ECont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRA  A
zCB2F:
	asr.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP B -- special (old sll)
zCB30:
	move.b	regB(%a0), %d0
	ror.b	#4, %d0
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP C -- special ""
zCB31:
	move.b	regC(%a0), %d0
	ror.b	#4, %d0
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP D -- special ""
zCB32:
	move.b	regD(%a0), %d0
	ror.b	#4, %d0
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP E -- special ""
zCB33:
	move.b	regE(%a0), %d0
	ror.b	#4, %d0
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP H -- special ""
zCB34:
	ror.w	#8, %d3
	ror.b	#4, %d3
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP L -- special ""
zCB35:
	ror.b	#4, %d3
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP (HL) -- special ""
zCB36:
	move.w	%d3, %d4
	bpl.s	zCB36Cont		| Cant write to ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb36RAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb36RAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb36RAMOk:
	move.b	(%a1, %d4.l), %d1
	ror.b   #4, %d1
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB36Cont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SWAP A -- special ""
zCB37:
	ror.b	#4, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| Clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  B
zCB38:
	move.b	regB(%a0), %d0
	lsr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regB(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  C
zCB39:
	move.b	regC(%a0), %d0
	lsr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regC(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  D
zCB3A:
	move.b	regD(%a0), %d0
	lsr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regD(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  E
zCB3B:
	move.b	regE(%a0), %d0
	lsr.b	#1, %d0
	scs	%d6			| Set or clear C
	move.b	%d0, regE(%a0)
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  H
zCB3C:
	ror.w	#8, %d3
	lsr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	ror.w	#8, %d3
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  L
zCB3D:
	lsr.b	#1, %d3
	scs	%d6			| Set or clear C
|	tst.b	%d3
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  (HL)
zCB3E:
	move.w	%d3, %d4
	bpl.s	zCB3ECont		| Cant shift ROM
	cmp.w	#0xE000, %d4		| See if Echo RAM (E000-FF00)
	blo.s	cb3ERAMOk
	cmpi.w	#0xFE00, %d4
	bge.s	cb3ERAMOk
	andi.w	#0xDFFF, %d4		| Yes, mask bit
cb3ERAMOk:
	move.b	(%a1, %d4.l), %d1
	lsr.b	#1, %d1
	scs	%d6			| Set or clear C
	seq	%d7			| Set or clear Z
	moveq	#0, %d0
	move.w	%d3, -(%sp)		| high byte of address
	move.b	(%sp)+, %d0
	bsr	WriteMem
zCB3ECont:
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

| SRL  A
zCB3F:
	lsr.b	#1, %d2
	scs	%d6			| Set or clear C
|	tst.b	%d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| (All CB instructs 0x40 - 0xFF)      
zCBxBIT:
	move.w	%d0, %d1
	andi.b	#7, %d0
	cmpi.b	#6, %d0			| If (HL)
	beq	zCBxHL

	cmpi.b	#7, %d0			| See if Reg A
	beq.s	zCBxregA

	move.w	%d3, regHL(%a0)		| move HL to storage in case H or L

	cmpi.b	#0x80, %d1		| BIT
	bhs.s	zCBxRES
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	btst	%d1, regB(%a0, %d0.w)	| Check selected register
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxRES:
	cmpi.b	#0xC0, %d1		| RES
	bhs.s	zCBxSET
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	bclr	%d1, regB(%a0, %d0.w)	| Clear selected register
	move.w	regHL(%a0), %d3		| move HL from storage in case H or L
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxSET:
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	bset	%d1, regB(%a0, %d0.w)	| SET selected register
	move.w	regHL(%a0), %d3		| move HL from storage in case H or L
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxregA:
	cmpi.b	#0x80, %d1		| BIT A
	bhs.s	zCBxARES
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	btst	%d1, %d2
	seq	%d7			| Set or clear Z
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zCBxARES:
	cmpi.b	#0xC0, %d1		| RES A
	bhi.s	zCBxASET
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	bclr	%d1, %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zCBxASET:
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	bset	%d1, %d2		| SET A
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxHL:
	cmpi.b	#0x80, %d1		| BIT (HL)
	bhs.s	zCBxHLRES
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1
	move.w	%d3, %d0
	bpl.s	zCBxHLBROM		| see if testing ROM
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	cbxHLBRAMOk
	cmpi.w	#0xFE00, %d0
	bge.s	cbxHLBRAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
cbxHLBRAMOk:
	btst	%d1, (%a1, %d0.l)
	seq	%d7			| Set or clear Z
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zCBxHLBROM:
	cmpi.w	#0x4000, %d0
	blo.s	zCBxHLLowROM
	btst.b	%d1, (%a5, %d0.l)
	seq	%d7			| Set or clear Z
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxHLLowROM:
	btst.b	%d1, (%a2, %d0.l)
	seq	%d7			| Set or clear Z
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zCBxHLRES:
	cmpi	#0xC0, %d1		| RES (HL)
	bhi.s	zCBxHLSET
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1

	moveq	#0, %d0
	move.w	%d3, -(%sp)
	move.b	(%sp)+, %d0		| High byte of address
	bpl.s	zHLRESROM		| ROM
	move.b	%d1, %d4
	move.b	(%a1, %d3.l), %d1
	bclr	%d4, %d1		| get updated value
	move.w	%d3, %d4		| address
	bsr	WriteMem
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zHLRESROM:
	moveq	#0, %d1			| RES in ROM, just write a #0
	move.w	%d3, %d4
	bsr	WriteMem
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCBxHLSET:
	lsr.b	#3, %d1			| Extract bit #
	andi.b	#7, %d1

	moveq	#0, %d0
	move.w	%d3, -(%sp)
	move.b	(%sp)+, %d0		| High byte of address
	bpl.s	zHLSETROM		| ROM
	move.b	%d1, %d4
	move.b	(%a1, %d3.l), %d1
	bset	%d4, %d1		| get updated value
	move.w	%d3, %d4		| address
	bsr	WriteMem
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zHLSETROM:
	moveq	#0, %d4			| SET in ROM
	bset	%d1, %d4
	move.b	%d4, %d1
	move.w	%d3, %d4
	bsr	WriteMem
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| CALL Z,nnnn
zCC:
	tst.b	%d7			| call only if Z flag is 1
	bne.s	zCD
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| CALL nnnn
zCD:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zCDPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zCDPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zCDPCOk:
	addq.w	#2, %d0			| Increment current address
	move.w	%d0, -(%sp)		| Save to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)

	move.b	1(%a3), -(%sp)		| Get jump address (high)
	move.w	(%sp)+, %d0		| 
	bmi.s	zCallToRAM		| If RAM Jump?
	move.b	(%a3), %d0		| No, Get jump address (low)
	cmpi.w	#0x4000, %d0		| See if Call to Low ROM
	blo.s	zCDLowROM
	add.l	%a5, %d0
	move.l	%d0, %a3		| Put jump address in a3
	subq.b	#6, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zCDLowROM:
	add.l	%a2, %d0
	move.l	%d0, %a3		| Put jump address in a3
	subq.b	#6, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

zCallToRAM:
	move.b	(%a3), %d0
	add.l	%a1, %d0
	move.l	%d0, %a3		| Put jump address in a3
	subq.b	#6, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| ADC  A,nn
zCE:
	move.b	%d2, regA_old(%a0)
	move.b  (%a3)+, %d0
	asr.b   #1, %d6
	addx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  8
zCF:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zCFPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zCFPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zCFPCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x08(%a2), %a3		| jump to 0x8
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


|RET  NC
zD0:
	tst.b	%d6			| dont return if C is set
	beq	zRET
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| POP  DE
zD1:
	move.b	(%a6)+, regE(%a0)
	move.b	(%a6)+, regD(%a0)
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| JP   NC,nnnn
zD2:
	tst.b	%d6			| don t jump if C flag is 1
	beq	zDoJump
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| z- ---- ??? (old out (nn),a)
zD3:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CALL NC,nnnn
zD4:
	tst.b	%d6			| don t call if C flag is 1
	beq	zCD
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| PUSH DE
zD5:
	move.b  regD(%a0), -(%a6)
	move.b  regE(%a0), -(%a6)
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| SUB  nn
zD6:
	move.b	%d2, regA_old(%a0)
	sub.b	(%a3)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  10H
zD7:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zD7PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zD7PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zD7PCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x10(%a2), %a3		| jump to 0x10
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RET  C
zD8:
	tst.b	%d6			| return if C flag is 1
	bne	zRET
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RETI ---- remapped (old exx)
zD9:
	move.b	1(%a6), -(%sp)		| Get address from stack (high)
	move.w	(%sp)+, %d0
	bmi.s	zRETIRAM
	move.b	(%a6), %d0		| (low)
	addq.l  #2, %a6
	move.l	%d0, %a3
	cmpi.w	#0x4000, %d0
	blo.s	zD9LowROM
	adda.l  %a5, %a3
	bra	zFB			| Enable Interrupt
zD9LowROM:
	adda.l  %a2, %a3
	bra	zFB			| Enable Interrupt
zRETIRAM:
	move.b	(%a6), %d0		| (low)
	addq.l  #2, %a6
	move.l	%d0, %a3
	adda.l  %a1, %a3
	bra	zFB			| Enable Interrupt


| JP   C,nnnn
zDA:
	tst.b	%d6			| don t jump if C flag is 0
	bne	zDoJump
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| - --- ??? (old in a,(nn))
zDB:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CALL C,nnnn
zDC:
	tst.b	%d6			| don t call if C flag is 0
	bne	zCD
	addq.l	#2, %a3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| - --- ??? (old ix-commands)
zDD:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| SBC  A,nn
zDE:
	move.b	%d2, regA_old(%a0)
	move.b  (%a3)+, %d0
	asr.b   #1, %d6
	subx.b	%d0, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  18H
zDF:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zDFPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zDFPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zDFPCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x18(%a2), %a3		| jump to 0x20
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (0xFF00+nn),A ---- special (old ret po)
zE0:
	move.w	#0xFF00, %d4
	move.b	(%a3)+, %d4
	move.b	%d2, %d1
	bsr	WRAM_io
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| POP  HL
zE1:
	move.b	(%a6)+, %d0		| Pop L
	move.b	(%a6)+, -(%sp)		| Pop H
	move.w	(%sp)+, %d3
	move.b	%d0, %d3
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   (0xFF00+C),A  ---- special (old jp po,nnnn)
zE2:
	move.w	#0xFF00, %d4
	move.b	regC(%a0), %d4
	move.b	%d2, %d1
	bsr	WRAM_io
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts



| - --- ??? (old ex (sp),hl)
zE3:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts

| - --- ??? (old call po,nnnn)
zE4:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| PUSH HL
zE5:
	move.w	%d3, -(%sp)
	move.b  (%sp)+, -(%a6)
	move.b	%d3, -(%a6)
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| AND  nn
zE6:
	and.b	(%a3)+, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  20H
zE7:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zE7PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zE7PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zE7PCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x20(%a2), %a3		| jump to 0x20
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| ADD  SP,dd --- special (old ret pe) (nocash extended as shortint)
zE8:
	move.b	(%a3)+, %d0
	ext.w	%d0	  		| extend sign
	adda.w	%d0, %a6

|	VGB Doesnt seem to set these flags
|       No$GMB clears the Z flag, sets the C flag on addition, clears it on subtraction
|	scs	%d6			| Set or clear C
|	moveq	#0, %d7			| Clear Z
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| JP   HL
zE9:
	tst.w	%d3
	bmi.s	zE9RAM			| RAM Jump?
	cmpi.w	#0x4000, %d3
	bhs.s	zE9HighROM
	lea	(%a2, %d3.l), %a3
	dbra	%d5, DoNext		| Decrement Counter, do another if >0
	rts
zE9HighROM:
	lea	(%a5, %d3.l), %a3
	dbra	%d5, DoNext		| Decrement Counter, do another if >0
	rts

zE9RAM:
	lea.l	(%a1, %d3.l), %a3
	dbra	%d5, DoNext		| Decrement Counter, Do another if >0
	rts



| LD   (nnnn),A  --- special (old jp pe,nnnn)
zEA:
	moveq	#0, %d0
	move.b	(%a3)+, %d1		| Get Low byte to temp
	move.b	(%a3)+, %d0		| Get High byte
	move.b	%d0, -(%sp)
	move.w	(%sp)+, %d4		| address to write to
	move.b	%d1, %d4
	move.b	%d2, %d1		| value to write
	bsr	WriteMem
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts




| - --- ??? (old ex de,hl)
zEB:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts

| - --- ??? (old call pe,nnnn)
zEC:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts

| - --- ??? (old ed-commands)
zED:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| XOR  nn
zEE:
	move.b	(%a3)+, %d0
	eor.b	%d0, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  28H
zEF:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zEFPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zEFPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zEFPCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x28(%a2), %a3		| jump to 0x28
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   A,(0xFF00+nn) -- special (old ret p)
zF0:
	moveq	#0, %d0
	move.w	#0xFF00, %d0
	move.b	(%a3)+, %d0
	move.b	(%a1, %d0.l), %d2
	cmpi.b	#0x44, %d0
	beq.s	zF0Exit
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zF0Exit:
	rts


| POP  AF
zF1:
	move.b	(%a6)+, %d0
	btst	#7, %d0
	sne	%d7			| Set or clear Z
	btst	#4, %d0
	sne	%d6			| Set or clear C
	move.b	(%a6)+, %d2		| Get A
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   A,(FF00+C) -- special (old jp p,nnnn)
zF2:
	moveq	#0, %d0
	move.w	#0xFF00, %d0
	move.b	regC(%a0), %d0
	move.b	(%a1, %d0.l), %d2
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| DI
zF3:
|  IMPORTANT:   CANCEL ANY PENDING EI
	clr.b	regInt(%a0)		| disable Interrupts

	

	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| - --- ??? (old call p,nnnn)
zF4:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| PUSH AF
zF5:
	move.b	%d6, %d0		| Get C flag
	andi.b	#0x20, %d0
	asr.b	#1, %d7			| Get Z flag to (x)
	roxr.b	#1, %d0			| Move (x) to d0
	move.b  %d2, -(%a6)
	move.b  %d0, -(%a6)
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| OR   nn
zF6:
	or.b	(%a3)+, %d2
	seq	%d7			| Set or clear Z
	moveq	#0, %d6			| clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| RST  30H
zF7:
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zF7PCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zF7PCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zF7PCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x30(%a2), %a3		| jump to 0x30
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   HL,SP+dd -- special (old ret m) (nocash corrected)
zF8:
	move.b	(%a3)+, %d3
	ext.w	%d3			| extend sign

	move.l	%a6, %d0		| see if SP in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zF8SPOk
	move.l	%a6, %d0		| See if SP in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zF8SPOk
	move.l	%a6, %d0		| In RAM
	sub.l	%a1, %d0
zF8SPOk:
	add.w	%d0, %d3
	scs	%d6			| Set or clear C
	moveq	#0, %d7			| Clear Z
	subq.b	#3, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   SP,HL
zF9:
	movea.l	%a1, %a6
	tst.w	%d3
	bmi.s	zF9InRAM
	movea.l	%a2, %a6
	cmpi.w	#0x4000, %d3		| If ROM 0
	blo.s	zF9InRAM
	movea.l	%a5, %a6		| current Bank
zF9InRAM:
	adda.l	%d3, %a6
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| LD   A,(nnnn) -- special (old jp m,nnnn)
zFA:
	move.b	(%a3)+, %d2		| Get Low byte to temp
	move.b	(%a3)+, -(%sp)		| Get High byte
	move.w	(%sp)+, %d0
	bpl.s	zFAROM			| Is ROM?
	move.b	%d2, %d0		| No, get low byte
	cmp.w	#0xE000, %d0		| See if Echo RAM (E000-FF00)
	blo.s	zFARAMOk
	cmpi.w	#0xFE00, %d0
  	bge.s	zFARAMOk
	andi.w	#0xDFFF, %d0		| Yes, mask bit
zFARAMOk:
	move.b  (%a1, %d0.l), %d2
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zFAROM:
	move.b	%d2, %d0
	cmpi.w	#0x4000, %d0
	blo.s	zFALowROM
	move.b  (%a5, %d0.l), %d2
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts
zFALowROM:
	move.b  (%a2, %d0.l), %d2
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts


| EI
zFB:
	/*  VGB - If already enabled, do nothing */
	moveq	#0, %d1
	move.w	#0xFF00, %d1
	move.b	0x0F(%a1, %d1.l), %d0	| do logical AND of IE, IF
	move.b	#0xFF, %d1
	and.b	(%a1, %d1.l), %d0	| Did interrupt occur
	bne.s	zFBIntr			| yes, branch
	st	regInt(%a0)		| no, Enable Interrupt
	dbra	%d5, DoNext		| no, Decrement Inst. Counter, 
	rts				| do another if >0

zFBIntr:
					| Interrupt not enabled until instruction after EI
	move.w	%d0, -(%sp)		| save IE (+) IF
	move.w	%d5, -(%sp)		| save counter
	clr.w	%d5
	bsr	DoNext			| Do one more instruction
zFBRet:
	clr.b	regInt(%a0)		| disable Interrupts
	moveq	#0, %d1
	move.w	#0xFF00, %d1

	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zFBPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zFBPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zFBPCOk:
	move.w	%d0, -(%sp)		| high byte
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)		| low byte

	move.w	(%sp)+, %d5		| restore inst.  counter
	move.w	(%sp)+, %d0		| restore IE (+) IF

	btst	#0, %d0			| V-Blank?
	beq.s	zFBNextI1		| no
	lea.l	0x40(%a2), %a3		| yes, jump to interrupt handler
	bclr.b	#0, 0x0F(%a1, %d1.l)
	bra.s	zFBExit
zFBNextI1:
	btst	#1, %d0			| LCDC?
	beq.s	zFBNextI2		| no
	lea.l	0x48(%a2), %a3		| yes, jump to interrupt handler
	bclr.b	#1, 0x0F(%a1, %d1.l)
	bra.s	zFBExit
zFBNextI2:
	btst	#2, %d0			| Timer?
	beq.s	zFBNextI3
	lea.l	0x50(%a2), %a3		| jump to interrupt handler
	bclr.b	#2, 0x0F(%a1, %d1.l)
	bra.s	zFBExit
zFBNextI3:
	btst	#3, %d0			| Serial?
	beq.s	zFBNextI4
	lea.l	0x58(%a2), %a3		| jump to interrupt handler
	bclr.b	#3, 0x0F(%a1, %d1.l)
	bra.s	zFBExit
zFBNextI4:
	btst	#4, %d0			| Joystick?
	beq.s	zFBNextI5
	lea.l	0x60(%a2), %a3		| jump to interrupt handler
	bclr.b	#4, 0x0F(%a1, %d1.l)
|	bra.s	zFBExit
zFBNextI5:

zFBExit:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts



| - --- ??? (old call m,nnnn)
zFC:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| - --- ??? (old iy-commands)
zFD:
	dbra	%d5, DoNext		| Decrement Inst. Counter, do another if >0
	rts


| CP   nn
zFE:
	cmp.b   (%a3)+, %d2
	seq	%d7			| Set or clear Z
	scs	%d6			| Set or clear C
	subq.b	#2, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another
	rts



| RST  38H
zFF:
|	trap	#8
	move.l	%a3, %d0		| see if PC in low ROM
	sub.l	%a2, %d0
	cmpi.l	#0x4000, %d0
	bcs.s	zFFPCOk
	move.l	%a3, %d0		| See if PC in high ROM
	sub.l	%a5, %d0
	cmpi.l	#0x8000, %d0
	bcs.s	zFFPCOk
	move.l	%a3, %d0		| In RAM
	sub.l	%a1, %d0
zFFPCOk:
	move.w	%d0, -(%sp)		| move current address to stack
	move.b	(%sp)+, -(%a6)
	move.b	%d0, -(%a6)
	lea.l	0x38(%a2), %a3		| jump to 0x38
	subq.b	#4, %d5			| Decrement Instruction Counter
	bpl	DoNext			| Do another if >0
	rts

