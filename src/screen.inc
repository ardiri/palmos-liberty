/*
 * @(#)screen.inc	
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


| *******************************************************************
| *                                                                 *
| *           DrawLine                                              *
| *******************************************************************



DoBlankLines:
	move.b	0x7f44(%a3), %d1
DoBLine:
	moveq	#0, %d0
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+
	move.l	%d0, (%a4)+

	addq.b	#1, StartDLine(%a3)		| Increment BG Drawing Line
	cmp.b	StartDLine(%a3), %d1
	bhi.s	DoBLine				| do all lines we are drawing

	move.l	%a4, ptrCurLine(%a0)		| Increment BG Drawing Line

	movem.l	(%sp)+,%d2-%d4/%d6-%d7/%a1-%a6	|restore registers
	moveq	#0, %d0
	moveq	#0, %d1
	moveq	#0, %d5
	rts



| d0.l = temp
| d1.l = temp
| d2.b = temp 			LCDC value
| d3.l = temp
| d4.w = (LOW) bit offset (rotate #)
| d4.w = (HIGH) Current Y line
| d5.w = (LOW) remaining bytes to write
| d5.w = (HIGH) offset for BG table for current Y
| d6.w = (LOW) bytes to end of tile row in data table
| d6.w = (HIGH) stored Y offset
| d7.l = tile negate byte

| a1   = pointer to preconverted tile table
| a2   = pointer to BG data table
| a3   = pointer to RAM
| a4   = current screen write location
| a5   = holder for a2
| a6   = pointer to Window data table

DrawLines:
	exg	%a4, %d1
	move.l	ptr32KRam(%a0), %a4		| RAM ptr
	lea.l	0x7F00(%a4), %a4		| a4 = ptr to FF00
	move.b	0x44(%a4), %d0
	cmpi.b	#0x90, %d0			| See if past screen
	bcc.s	DontDraw			| yes, dont draw
	cmp.b	0x4c(%a4), %d0			| see if all lines already drawn
	bhi.s	DrawEm				| no, draw them
DontDraw:
	exg	%a4, %d1
	rts					| yes, return
DrawEm:
	exg	%a4, %d1
	movem.l	%d2-%d4/%d6-%d7/%a1-%a6, -(%sp)	| save registers we will trash

	move.l	ptrCurLine(%a0), %a4		| Get current line in screen buffer

	move.l	ptr32KRam(%a0), %a3

	move.b	0x7F40(%a3), %d2		| Check out LCDC

	bpl.s	DoBlankLines			| If Screen is off, dont draw the line
	tst.b	0x7F47(%a3)			| Is palette 00?
	beq.s	DoBlankLines			| If so, dont draw
	btst	#0, %d2				| See if background is on
	beq.s	DoBlankLines

	moveq	#0, %d5
	moveq	#0, %d4

	move.b	xByteOff(%a0), %d5		| Get x byte offset to tile start
	move.w	#0x1F, %d6
	sub.b	%d5, %d6			| Save byte offset (0x1F-offset)
	swap	%d6

	move.b	StartDLine(%a3), %d4		| Get start Y line
	move.w	%d4, %d0

	move.b	0x7F42(%a3), %d6		| Get Scroll Y
	add.w	%d6, %d0
	add.w	%d0, %d0			| * 2 bits/pixel

	move.w	%d0, %d6
	and.w	#0x0F, %d6			| Get remainder
	
	sub.b	%d6, %d0			| 8 lines/ tile * 32 tiles/line
	add.w	%d0, %d0
	add.w	%d0, %d5			| add Y byte offset to tile start

	add.w	%d5, %d5			| x2 for preconverted tile table

	andi.w	#0x7FF, %d5			| If past end of table, wrap to 1st row

	swap	%d4
	move.b	xBitOff(%a0), %d4		| Get Scroll X bit, byte offsets
	swap	%d4

	move.l	ptrTileTable(%a0), %a2
	btst	#3, %d2				| BG Data Select (0=0x9800, 1=0x9C00)
	beq.s	LBGSCont
	lea	0x800(%a2), %a2			| add 0x400
LBGSCont:

	moveq	#0, %d7				| Used in negating tile #s, if required

	lea	0x6000(%a3), %a1		| move to preconverted tile area
	btst	#4, %d2				| BG & Window Tile Data Select
	bne.s	LBGTDS0				| If 0, stick with 0x8000
	lea	0x800(%a1), %a1			| else, add 0x800
	move.l	#0x0800, %d7			| Negate tile #s below
LBGTDS0:
	add.w	%d6, %a1			| Add in y line offset
	swap	%d6				| Save the Y remainder for the Window code

	movea.l	%a2, %a5			| Store pointer to BG tile map
	adda.w	%d5, %a2			| Add tile start for current row
	swap	%d5				| save offset


	btst	#5, 0x7F40(%a3)			| Is Window on?
	beq	DrawBGOnly			| no, no need to check each loop
	cmpi.b	#166,0x7F4B(%a3)		| See if Window X>166
	bhi	DrawBGOnly

|	lea	0x1800(%a3), %a6		| 0x1800 - 1 (starts offscreen horiz.)
	move.l	ptrTileTable(%a0), %a6
	btst	#6, 0x7F40(%a3)			| Win Data Select (0=0x9800, 1=0x9C00)
	beq.s	LBGWCont
	lea	0x800(%a6), %a6			| add 0x400 to %a6
LBGWCont:

DoALine:
	move.w	#20, %d5			| BG byte counter
	move.b	%d5, xOffset(%a0)
	cmp.b	0x7F4A(%a3), %d4		| If Window y>current Y, then dont Draw it
	blo.s	NoWindow
	move.b	wxByteOff(%a0), %d5		| get WX byte offset
	beq	DrawWin				| If 0, then no BG- all Window on line
	sub.b	%d5, xOffset(%a0)		| Save byte offset for window
	subq	#1, %d5
NoWindow:
	swap	%d4
	bra	DrawBG
DNextLine:
	swap	%d4
	add.l	#44, ptrCurLine(%a0)		| Increment BG Drawing Line
	addq	#2, %a4
	move.l	ptrCurLine(%a0), %a4		| Get current line in screen buffer
	movea.l	%a5, %a2			| restore BG tile map pointer
	swap	%d5
	swap	%d6
	addq.b	#2, %d6
	addq.l	#2, %a1
	cmpi.b	#0x10, %d6
	bne.s	DTileTblOk
	clr.w	%d6
	suba.l	#0x10, %a1
	add.w	#0x40, %d5			| update tile start
	andi.w	#0x7FF, %d5			| If past end of table, wrap to 1st row
DTileTblOk:
	add.w	%d5, %a2			| Add tile start for current row
	swap	%d5
	swap	%d6
	move.w	#0x1F, %d6
	sub.b	xByteOff(%a0), %d6		| Save byte offset (0x1F-offset)
	addq	#1, %d4				| increment current line
	cmp.b	0x7f44(%a3), %d4		| See if finished drawing
	bls.s	DoALine
	bsr	DrawJustSprites
	move.b	%d4, StartDLine(%a3)		| Save as next row to draw

DLineExit:
	movem.l	(%sp)+,%d2-%d4/%d6-%d7/%a1-%a6	|restore registers
	moveq	#0, %d0
	moveq	#0, %d1
	moveq	#0, %d5
	rts



| ** Draw all Background Lines on screen with Window off

DrawBGOnly:
	movea.l	%a2, %a6
DrawBGLines:
	move.w	#20, %d5			| BG byte counter
	swap	%d4
BGDraw:
	moveq	#0, %d0
	move.w	(%a2)+, %d0			| get the ptr to the tile data table
	eor.w	%d7, %d0			| Negate tile #, if needed

	| Do tile word
	move.w	(%a1,%d0.w), %d0		| get 2 bytes from preconverted tile
	ror.l	%d4, %d0			| apply offset
	or.w	%d1, %d0			| get carried bits
	move.w	%d0, (%a4)+			| Write Pattern, increment screen buffer pointer
	swap	%d0				| move carry bits to low word

	subq.b	#1, %d5
	dbcs	%d6, BGNotEnd
	bcs.s	BGDone
	suba.l	#0x40, %a2			| at end, subtract 0x20 from row (wrap to beginning of row)

BGNotEnd:
	moveq	#0, %d1
	move.w	(%a2)+, %d1			| get the ptr to the tile data table
	eor.w	%d7, %d1			| Negate tile #, if needed

	| Do tile word
	move.w	(%a1,%d1.w), %d1		| get 2 bytes from preconverted tile
	ror.l	%d4, %d1			| apply offset
	or.w	%d0, %d1			| get carried bits
	move.w	%d1, (%a4)+			| Write Pattern, increment screen buffer pointer
	swap	%d1				| move carry bits to low word

	subq.b	#1, %d5
	dbcs	%d6, BGDraw			| Horizontal byte ctr until end of row
	bcs.s	BGDone				| If end of line, exit
	suba.l	#0x40, %a2			| at end, subtract 0x20 from row (wrap to beginning of row)
	bra.s	BGDraw				| Do Next tile in row

BGDone:
	swap	%d4
	addq	#2, %a4
	movea.l	%a6, %a2			| restore BG tile map pointer
	swap	%d6
	addq.b	#2, %d6				| Increment Y row Offset
	addq.l	#2, %a1				| Increment tile row
	cmpi.b	#0x10, %d6			| See if back to start of row
	bne.s	BGDTileTblOk
	clr.w	%d6				| yes, Y row offset=0
	suba.l	#0x10, %a1
	swap	%d5
	add.w	#0x40, %d5			| update tile start
	andi.w	#0x7FF, %d5			| If past end of table, wrap to 1st row
	movea.l	%a5, %a2
	add.w	%d5, %a2
	movea.l	%a2, %a6
	swap	%d5
BGDTileTblOk:
	swap	%d6
	move.w	#0x1F, %d6
	sub.b	xByteOff(%a0), %d6		| Save byte offset (0x1F-offset)
	addq	#1, %d4				| increment current line
	cmp.b	0x7f44(%a3), %d4		| See if finished drawing
	bls	DrawBGLines
	bsr	DrawJustSprites
	move.b	%d4, StartDLine(%a3)		| Save as next row to draw
	move.l	%a4, ptrCurLine(%a0)		| Get current line in screen buffer

	movem.l	(%sp)+,%d2-%d4/%d6-%d7/%a1-%a6	|restore registers
	moveq	#0, %d0
	moveq	#0, %d1
	moveq	#0, %d5
	rts





| ** Draw A Single line of Background on screen with Window on

DrawBG:
	moveq	#0, %d0
	move.w	(%a2)+, %d0			| get the ptr to the tile data table
	eor.w	%d7, %d0			| Negate tile #, if needed

	| Do tile word
	move.w	(%a1,%d0.w), %d0		| get 2 bytes from preconverted tile

	ror.l	%d4, %d0			| apply offset
	or.w	%d1, %d0			| get carried bits
	move.w	%d0, (%a4)+			| Write Pattern, increment screen buffer pointer
	swap	%d0				| move carry bits to low word

	subq.b	#1, %d5
	dbcs	%d6, DBGNotEnd
	bcs.s	DBGDone1
	suba.l	#0x40, %a2			| at end, subtract 0x20 from row (wrap to beginning of row)

DBGNotEnd:
	moveq	#0, %d1
	move.w	(%a2)+, %d1			| get the ptr to the tile data table
	eor.w	%d7, %d1			| Negate tile #, if needed

	| Do tile word
	move.w	(%a1,%d1.w), %d1		| get 2 bytes from preconverted tile

	ror.l	%d4, %d1			| apply offset
	or.w	%d0, %d1			| get carried bits
	move.w	%d1, (%a4)+			| Write Pattern, increment screen buffer pointer
	swap	%d1				| move carry bits to low word

	subq.b	#1, %d5
	dbcs	%d6, DrawBG			| Horizontal byte ctr until end of row
	bcs.s	DBGDone2			| If end of line, exit
	suba.l	#0x40, %a2			| at end, subtract 0x20 from row (wrap to beginning of row)
	bra.s	DrawBG				| Do Next tile in row

DBGDone2:
	move.w	%d1, %d0			| get carry bits ready for Window (just in case)
DBGDone1:
	cmpi.b	#20, xOffset(%a0)		| See if drawing window
	bhs	DNextLine			| No, do next line


|  * *  Drawing Window after BG  * *
						| (get remaining BG word, since window
						| could start in a few bits)
	moveq	#0, %d1
	move.w	(%a2)+, %d1			| get the ptr to the tile data table
	eor.w	%d7, %d1			| Negate tile #, if needed

	move.w	(%a1,%d1.w), %d1		| get 2 bytes from tile
	ror.l	%d4, %d1			| apply offset
	or.w	%d0, %d1			| get carried bits

| ** Draw A Single Line of Window
	swap	%d4
DrawWin:
	swap	%d4
	move.b	wxBitOff(%a0), %d4		| Get window bit offset
	moveq	#-1, %d0
	clr.w	%d0				| Set d0 to 0xFFFF0000 for shifting
	lsr.l	%d4, %d0
	and.w	%d0, %d1			| mask out BG bits, leaving only bits to display

	clr.w	%d6
	swap	%d6
|	move.l	%a1, %a6
	suba.l	%d6, %a1			| Remove Y offset from pointer to Tile Data
	swap	%d6

	moveq	#0, %d0
	move.b	ScreenWinY(%a3), %d0		| Get current Window Y
	addq.b	#1, ScreenWinY(%a3)
	add.w	%d0, %d0			| * 2

	move.b	%d0, %d6
	and.b	#0x0F, %d6			| Get remainder

	adda.w	%d6, %a1			| add to tile data pointer to get offset

	sub.b	%d6, %d0			| 8 lines/ tile * 32 tiles/line
	add.w	%d0, %d0
	add.w	%d0, %d0			| x2 for preconverted tile data table

	move.l	%a6, %a2
	adda.l	%d0, %a2			| add Y byte offset to tile start

	move.b	xOffset(%a0), %d5


DrawWinLoop:
	moveq	#0, %d0
	move.w	(%a2)+, %d0			| get the ptr to the tile data table
	eor.w	%d7, %d0			| Negate tile #, if needed

	| Do tile row
	move.w	(%a1,%d0.w), %d0		| get 2 bytes from tile

	ror.l	%d4, %d0			| apply offset
	or.w	%d1, %d0			| get carried bits

	move.w	%d0, (%a4)+			| Write Pattern, increment screen buffer pointer

	swap	%d0				| move carry bits to low word
	move.w	%d0, %d1			| save carried bits

	dbra	%d5, DrawWinLoop		| Horizontal byte ctr until end of row
DrawWinExit:
	suba.w	%d6, %a1
	swap	%d6
	adda.w	%d6, %a1
	swap	%d6
	move.b	xBitOff(%a0), %d4		| Get Scroll X bit, byte offsets
	bra	DNextLine






DrawJustSprites:
	movem.l	%d2-%d4/%d6-%d7/%a1-%a6,-(%sp)	|save registers we will trash

DrawLineSprites:
	move.l	ptr32KRam(%a0), %a4	
	move.b	0x7F40(%a4), %d5	| Get LCDC
	btst	#1, %d5			| See if Sprites are on
	beq.s	DrawLineExit		| No, dont draw

	move.b	StartDLine(%a4), %d4	| Get starting Y Line to d4 High
	addq.b	#1, %d4
	swap	%d4
	move.b	0x7F44(%a4), %d4	| Get current Y line
	addq.b	#1, %d4
					| MJE 2/16/02 Fix sprites not showing up
	btst	#2, %d5			| See if sprites are 8 pixels high
	bne.s	Not8Pix
	addq.b	#8, %d4			| If so, make sure to draw them
Not8Pix:

|	beq.s	DrawLineExit		| Dont draw if on line zero

					| Draw all tiles ending on current row
	swap	%d5			| Save LCDC to High word
	lea	0x7Ea0(%a4), %a2	| Start at end of table (for priority)
	move.w	#39, %d5		| Check 40 sprites (Low word of %d5)
xDSLoop:
	subq.l	#4, %a2
	moveq	#0, %d0
	move.b	(%a2), %d0
	cmp.b	%d0, %d4		| See if > current Y row
	bcs.s	DSNoDraw
	swap	%d4
	cmp.b	%d0, %d4		| See if < starting line
	bcc.s	DSNoDraw2
	move.l	%a2, %d6		| save %a2
					| Get screen line to write to
	add.w	%d0, %d0		| get y*44
	add.w	%d0, %d0		| first, *4
	move.w	%d0, %d3
	add.w	%d3, %d3		| + (y*4)*2  [8]
	add.w	%d3, %d0
	add.w	%d3, %d3		| + (y*8)*4  [32]
	add.w	%d3, %d3
	add.w	%d3, %d0		| total= y*44
	move.l	ptrScreen(%a0), %a6	| Get Screen Pointer
	add.w	%d0, %a6		| (no need to add 704, because sprites start at 0)
	bra.s	DrawOne			| draw sprite
DrawOneRet:
	move.l	ptr32KRam(%a0), %a4	| restore %a4
	exg	%a2, %d6		| restore %a2
DSNoDraw2:
	swap	%d4
DSNoDraw:
	subq.b	#1, %d5			| decrement sprite counter
	bcc.s	xDSLoop


DrawLineExit:
	movem.l	(%sp)+,%d2-%d4/%d6-%d7/%a1-%a6	|restore registers
	moveq	#0, %d5
	moveq	#0, %d0
	rts



DrawOne:
	move.w	(%a2), %d0		| Get sprite y, x bytes
	move.w	2(%a2), %d2		| Move Pattern, Flags to %d2

	moveq	#0, %d3			| X pos
	move.b	%d0, %d3
	beq.s	DrawOneRet		| If x=0, or
	cmpi.b	#167, %d3		| If x=>167, not on screen
	bhi.s	DrawOneRet
	lsr.w	#3, %d3			| / 8
	add.b	%d3, %d3		| * 2 bytes per 8 pixels

	adda.w	%d3, %a6		| add in x byte offset

	move.b	%d0, %d3
	andi.b	#07, %d3		| get remainder in %d3 (offset)
	add.b	%d3, %d3		| (offset * 2)

	moveq	#0, %d0
	move.b	2(%a2), %d0		| Get Pattern #

	btst	#18, %d5		| See if 8x16 sprites
	beq.s	DSNot16			| If not, do nothing
	andi.b	#0xFE, %d0		| If 8x16, ignore low bit
DSNot16:
	lsl.w	#4, %d0			| x 16


	movea.l	ptrObjPal0(%a0), %a1	| assume Palette 0
	btst	#4, %d2			| Check
	beq.s	DSPal0
	movea.l	ptrObjPal1(%a0), %a1	| no, get palette 1
DSPal0:
|	btst	#7, %d2			| Is priority=behind BG?
|	bne	DrawOneBG		| yes, draw BG

	btst	#5, %d2			| Is X flipped?
	bne	DSFlippedX

|	movea.l	ptrHLConv(%a0), %a2 	| location of HL Conversion table (used in writing bytes)

	movea.l	ptrBGTileRam(%a0), %a4	| Converted Tile ptr
	adda.w	%d0, %a4

	lea.l	masktable(%pc), %a3	| location of mask table (used in writing bytes)

	btst	#6, %d2			| Is Y flipped?
	bne	DSflippedY


| X=NORMAL, Y=NORMAL
	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	bsr	WriteBytes

	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	| Do tile rows 7,8
 	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	btst	#18, %d5		| See if 8x16 sprites
	beq	DrawOneRet		| If not, skip to next tile

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	| Do tile rows 15,16
	move.l	(%a4), %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytes

	bra	DrawOneRet


DSFlippedX:
	lea.l	revpixel(%pc), %a2 	| location of HL Conversion table (used in writing bytes)
	lea.l	revmask(%pc), %a3	| location of reversed mask table
	adda.w	%d0, %a4

	btst	#6, %d2			| Is Y flipped?
	bne	DSflippedXY

| X=FLIPPED, Y=NORMAL
	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	bsr	WriteBytesR

	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	btst	#18, %d5		| See if 8x16 sprites
	beq	DrawOneRet		| If not, skip to next tile

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	| Do tile rows 15,16
	move.l	(%a4), %d1		| get 4 bytes from tile
	lea	44(%a6), %a6		| add 44
	bsr	WriteBytesR

	bra	DrawOneRet


DSflippedXY:
	| X=FLIPPED, Y=FLIPPED

	btst	#18, %d5		| See if 8x16 sprites
	beq	DSfXYNext		| If not, only do 8 lines

	| Do tile rows 15,16
	lea	16(%a4), %a4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBytesR

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR

	lea	308(%a6), %a6		| set up y loc for next 82
	suba.l	#32, %a4

DSfXYNext:
	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		|
	swap	%d1			| going up
	bsr	WriteBytesR


	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR


	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR

	| Do tile rows 1,2
	move.l	(%a4), %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytesR

	bra	DrawOneRet


DSflippedY:
	| X=NORMAL, Y=FLIPPED
	btst	#18, %d5		| See if 8x16 sprites
	beq	DSfYNext		| If not, only do 8 lines

	| Do tile rows 15,16
	lea	16(%a4), %a4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		|
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBytes

	lea	308(%a6), %a6		| set up y loc for next 82
	lea.l	-32(%a4), %a4

DSfYNext:
	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBytes

	| Do tile rows 1,2
	move.l	(%a4), %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBytes

	bra	DrawOneRet





| ************************************************
| *     Write 8 pixels (2 bytes) to screen
| *     %a1 - location of pixel conversion table
| *	%a2 - Pointer to ptrHLConv table
| *     %a3 - location of mask table
| *     %a6 - Byte loc. to write to
| *     %d1 - pattern to write (2 bytes, HHLL)
| *	%d2 - used as Temp var.
| *     %d3 - pixel Offset (x2)
| *	%d7 - used as Temp var.

WriteBytes:

	swap	%d1	

	moveq	#0, %d2
	move.w	%d1, -(%sp)		| save high byte of pattern
	move.b	(%sp)+, %d2

	move.b	(%a3,%d2.w), -(%sp)	| Get High Mask
	move.w	(%sp)+, %d7
	
	move.b	(%a1,%d2.w), -(%sp)	| Get High pattern
	move.w	(%sp)+, %d2

	andi.w	#0xFF, %d1

	move.b	(%a3,%d1.w), %d7	| Get Low Mask
	move.b	(%a1,%d1.w), %d2	| Get low pattern

	swap	%d7			| move mask to high word
	move.w	#0xFFFF, %d7		| put 0xFFFF in low order word of mask
	swap	%d2			| move bytes to high word
	ror.l	%d3, %d7		| apply offset
	ror.l	%d3, %d2
	and.l	%d7, (%a6)		| Mask BG
	or.l	%d2, (%a6)		| Write pattern


	swap	%d1
	lea	44(%a6), %a6		| get next screen line

	moveq	#0, %d2
	move.w	%d1, -(%sp)		| save high byte of pattern
	move.b	(%sp)+, %d2

	move.b	(%a3,%d2.w), -(%sp)	| Get High Mask
	move.w	(%sp)+, %d7
	
	move.b	(%a1,%d2.w), -(%sp)	| Get High pattern
	move.w	(%sp)+, %d2

	andi.w	#0xFF, %d1

	move.b	(%a3,%d1.w), %d7	| Get Low Mask
	move.b	(%a1,%d1.w), %d2	| Get low pattern

	swap	%d7			| move mask to high word
	move.w	#0xFFFF, %d7		| put 0xFFFF in low order word of mask
	swap	%d2			| move bytes to high word
	ror.l	%d3, %d7		| apply offset
	ror.l	%d3, %d2
	and.l	%d7, (%a6)		| Mask BG
	or.l	%d2, (%a6)		| Write pattern

	rts



WriteBytesR:

	swap	%d1
	moveq	#0, %d2
	ror.w	#4, %d1			| Get High bits together
	ror.b	#4, %d1
	rol.w	#4, %d1
	move.b	%d1, %d2
	move.b	(%a3,%d2.w), -(%sp)	| get high mask byte
	move.w	(%sp)+, %d7
	move.b	(%a2,%d2.w), %d2	| Convert high byte
	move.b	(%a1,%d2.w), %d1	| get palette

	ror.w	#8, %d1

	move.b	%d1, %d2
	move.b	(%a3,%d2.w), %d7	| get low mask byte
	move.b	(%a2,%d2.w), %d2	| Convert Low byte
	move.b	(%a1,%d2.w), %d1	| get palette

	move.w	%d1, %d2
	swap	%d7			| move mask to high word
	move.w	#0xFFFF, %d7		| put 0xFFFF in low order word of mask
	swap	%d2			| move bytes to high word
	clr.w	%d2			| clear low word
	ror.l	%d3, %d7		| apply offset
	ror.l	%d3, %d2
	and.l	%d7, (%a6)		| Mask BG
	or.l	%d2, (%a6)		| Write pattern


	swap	%d1
	lea.l	44(%a6), %a6		| get next screen line
	moveq	#0, %d2
	ror.w	#4, %d1			| Get High bits together
	ror.b	#4, %d1
	rol.w	#4, %d1
	move.b	%d1, %d2
	move.b	(%a3,%d2.w), -(%sp)	| get high mask byte
	move.w	(%sp)+, %d7
	move.b	(%a2,%d2.w), %d2	| Convert high byte
	move.b	(%a1,%d2.w), %d1	| get palette

	ror.w	#8, %d1

	move.b	%d1, %d2
	move.b	(%a3,%d2.w), %d7	| get low mask byte
	move.b	(%a2,%d2.w), %d2	| Convert Low byte
	move.b	(%a1,%d2.w), %d1	| get palette

	move.w	%d1, %d2
	swap	%d7			| move mask to high word
	move.w	#0xFFFF, %d7		| put 0xFFFF in low order word of mask
	swap	%d2			| move bytes to high word
	clr.w	%d2			| clear low word
	ror.l	%d3, %d7		| apply offset
	ror.l	%d3, %d2
	and.l	%d7, (%a6)		| Mask BG
	or.l	%d2, (%a6)		| Write pattern

	rts



revmask:
	dc.b	0xFF,0x3F,0xCF,0x0F,0xF3,0x33,0xC3,0x03,0xFC,0x3C,0xCC,0x0C,0xF0,0x30,0xC0,0x00
	dc.b	0x3F,0x3F,0x0F,0x0F,0x33,0x33,0x03,0x03,0x3C,0x3C,0x0C,0x0C,0x30,0x30,0x00,0x00
	dc.b	0xCF,0x0F,0xCF,0x0F,0xC3,0x03,0xC3,0x03,0xCC,0x0C,0xCC,0x0C,0xC0,0x00,0xC0,0x00
	dc.b	0x0F,0x0F,0x0F,0x0F,0x03,0x03,0x03,0x03,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00
	dc.b	0xF3,0x33,0xC3,0x03,0xF3,0x33,0xC3,0x03,0xF0,0x30,0xC0,0x00,0xF0,0x30,0xC0,0x00
	dc.b	0x33,0x33,0x03,0x03,0x33,0x33,0x03,0x03,0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00
	dc.b	0xC3,0x03,0xC3,0x03,0xC3,0x03,0xC3,0x03,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00
	dc.b	0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	dc.b	0xFC,0x3C,0xCC,0x0C,0xF0,0x30,0xC0,0x00,0xFC,0x3C,0xCC,0x0C,0xF0,0x30,0xC0,0x00
	dc.b	0x3C,0x3C,0x0C,0x0C,0x30,0x30,0x00,0x00,0x3C,0x3C,0x0C,0x0C,0x30,0x30,0x00,0x00
	dc.b	0xCC,0x0C,0xCC,0x0C,0xC0,0x00,0xC0,0x00,0xCC,0x0C,0xCC,0x0C,0xC0,0x00,0xC0,0x00
	dc.b	0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00
	dc.b	0xF0,0x30,0xC0,0x00,0xF0,0x30,0xC0,0x00,0xF0,0x30,0xC0,0x00,0xF0,0x30,0xC0,0x00
	dc.b	0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00
	dc.b	0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00
	dc.b	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00


| * Used to convert a byte from from hhhhllll to hlhlhlhlhlhlhlhl


revpixel:
	dc.b	0x00,0x40,0x10,0x50,0x04,0x44,0x14,0x54,0x01,0x41,0x11,0x51,0x05,0x45,0x15,0x55
	dc.b	0x80,0xC0,0x90,0xd0,0x84,0xC4,0x94,0xd4,0x81,0xC1,0x91,0xd1,0x85,0xC5,0x95,0xd5
	dc.b	0x20,0x60,0x30,0x70,0x24,0x64,0x34,0x74,0x22,0x62,0x32,0x72,0x25,0x65,0x35,0x75
	dc.b	0xa0,0xE0,0xB0,0xF0,0xa4,0xE4,0xB4,0xF4,0xa2,0xE2,0xB2,0xF2,0xa5,0xE5,0xB5,0xF5
	dc.b	0x08,0x48,0x18,0x58,0x0C,0x4C,0x1C,0x5C,0x06,0x46,0x16,0x56,0x0D,0x4D,0x1D,0x5D
	dc.b	0x88,0xC8,0x98,0xD8,0x8C,0xCC,0x9C,0xDC,0x86,0xC6,0x96,0xd6,0x8D,0xCD,0x9D,0xDD
	dc.b	0x28,0x68,0x38,0x78,0x2C,0x6C,0x3C,0x7C,0x26,0x66,0x36,0x76,0x2D,0x6D,0x3D,0x7D
	dc.b	0xA8,0xE8,0xB8,0xF8,0xAC,0xEC,0xBC,0xFC,0xa6,0xE6,0xB6,0xF6,0xAD,0xED,0xBD,0xFD
	dc.b	0x02,0x42,0x12,0x52,0x06,0x46,0x16,0x56,0x03,0x43,0x13,0x53,0x07,0x47,0x17,0x57
	dc.b	0x82,0xC2,0x92,0xd2,0x86,0xC6,0x96,0xd6,0x83,0xC3,0x93,0xd3,0x87,0xC7,0x97,0xd7
	dc.b	0x22,0x62,0x32,0x72,0x26,0x66,0x36,0x76,0x23,0x63,0x33,0x73,0x27,0x67,0x37,0x77
	dc.b	0xa2,0xE2,0xB2,0xF2,0xa6,0xE6,0xB6,0xF6,0xa3,0xE3,0xB3,0xF3,0xa7,0xE7,0xB7,0xF7
	dc.b	0x0A,0x4A,0x1A,0x5A,0x0E,0x4E,0x1E,0x5E,0x07,0x47,0x17,0x57,0x0F,0x4F,0x1F,0x5F
	dc.b	0x8A,0xCA,0x9A,0xDA,0x8E,0xCE,0x9E,0xDE,0x87,0xC7,0x97,0xd7,0x8F,0xCF,0x9F,0xDF
	dc.b	0x2A,0x6A,0x3A,0x7A,0x2E,0x6E,0x3E,0x7E,0x27,0x67,0x37,0x77,0x2F,0x6F,0x3F,0x7F
	dc.b	0xAA,0xEA,0xBA,0xFA,0xAE,0xEE,0xBE,0xFE,0xa7,0xE7,0xB7,0xF7,0xAF,0xEF,0xBF,0xFF


masktable:
	dc.b	0xFF,0xFC,0xFC,0xFC,0xF3,0xF0,0xF0,0xF0,0xF3,0xF0,0xF0,0xF0,0xF3,0xF0,0xF0,0xF0
	dc.b	0xCF,0xCC,0xCC,0xCC,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0
	dc.b	0xCF,0xCC,0xCC,0xCC,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0
	dc.b	0xCF,0xCC,0xCC,0xCC,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0,0xC3,0xC0,0xC0,0xC0
	dc.b	0x3F,0x3C,0x3C,0x3C,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x3F,0x3C,0x3C,0x3C,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x3F,0x3C,0x3C,0x3C,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30,0x33,0x30,0x30,0x30
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00
	dc.b	0x0F,0x0C,0x0C,0x0C,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00




*  Draw Sprites Behind Background

DrawOneBG:

	btst	#5, %d2			| Is X flipped?
	bne	DSBFlipX
|	movea.l	ptrHLConv(%a0), %a2 	| location of HL Conversion table (used in writing bytes)
	movea.l	ptrBGTileRam(%a0), %a1	| Converted Tile ptr

	lea.l	masktable(%pc), %a3	| location of mask table (used in writing bytes)

	movea.l	ptrBGTileRam(%a0), %a4	| Converted Tile ptr
	adda.w	%d0, %a4

	btst	#6, %d2			| Is Y flipped?
	bne	DSBFlipY

	| X=NORMAL, Y=NORMAL
	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	bsr	WriteBBytes

	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	btst	#18, %d5		| See if 8x16 sprites
	beq	DrawOneRet		| If not, skip to next tile

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	| Do tile rows 15,16
	move.l	(%a4), %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytes

	bra	DrawOneRet


DSBFlipX:
	lea.l	revpixel(%pc), %a2 	| location of HL Conversion table (used in writing bytes)
	lea.l	masktable(%pc), %a3	| location of BG mask table
	adda.w	%d0, %a4

	btst	#6, %d2			| Is Y flipped?
	bne	DSBFlipXY

	| X=FLIPPED, Y=NORMAL
	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	bsr	WriteBBytesR

	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	btst	#18, %d5		| See if 8x16 sprites
	beq	DrawOneRet		| If not, skip to next tile

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	| Do tile rows 15,16
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	44(%a6), %a6
	bsr	WriteBBytesR

	bra	DrawOneRet


DSBFlipXY:
	| X=FLIPPED, Y=FLIPPED
	btst	#18, %d5		| See if 8x16 sprites
	beq	DSBfXYNext		| If not, only do 8 lines

	| Do tile rows 15,16
	lea	16(%a4), %a4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBBytesR

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBBytesR

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBBytesR

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132,%a6
	swap	%d1			| going up
	bsr	WriteBBytesR

	lea	308(%a6), %a6		| set up y loc
	lea	-32(%a4), %a4

DSBfXYNext:
	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBBytesR


	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytesR


	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytesR

	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytesR

	bra	DrawOneRet


DSBFlipY:
	| X=NORMAL, Y=FLIPPED
	btst	#18, %d5		| See if 8x16 sprites
	beq	DSBfYNext		| If not, only do 8 lines

	| Do tile rows 15,16
	lea	16(%a4), %a4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBBytes

	| Do tile rows 13,14
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes

	| Do tile rows 11,12
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes

	| Do tile rows 9,10
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes

	lea	308(%a6), %a6		| set up y loc for next 82
	lea	-32(%a4), %a4
DSBfYNext:
	| Do tile rows 7,8
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	lea	264(%a6), %a6		| row 7
	swap	%d1			| going up
	bsr	WriteBBytes


	| Do tile rows 5,6
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes


	| Do tile rows 3,4
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes

	| Do tile rows 1,2
	move.l	(%a4)+, %d1		| get 4 bytes from tile
	suba.l	#132, %a6
	swap	%d1			| going up
	bsr	WriteBBytes

	bra	DrawOneRet



| ************************************************
| *     Write 8 pixels (2 bytes) to screen BEHIND BACKGROUND
| *     %a1 - location of pixel conversion table
| *	%a2 - Pointer to ptrHLConv table
| *	%a3 - Pointer to Masktable
| *     %a6 - Byte loc. to write to
| *     %d1 - pattern to write (2 bytes, HHLL)
| *     %d3 - pixel Offset (x2)

WriteBBytes:
	swap	%d1
|	move.w	%d1,%d2			| copy
|	andi.w	#0x7FFE, %d2		| make +
|	eor.w	%d2, %d1		| save sign, low bit only (H7L0)

|	or.w	(%a2, %d2.w), %d1	| convert HHLL to hlhlhlhlhlhlhlhl

	moveq	#0, %d2
	move.w	%d1, -(%sp)		| save high byte
	move.b	(%sp)+, %d2

	move.b	(%a3, %d2.w), -(%sp)	| Get High pattern
	move.w	(%sp)+, %d2

	andi.w	#0xFF, %d1

	move.b	(%a1,%d1.w), %d2	| Get low pattern

	swap	%d2			| move bytes to high word
	ror.l	%d3, %d2		| apply offset


|	move.l	(%a6), %d7		| Get BG Mask
|	clr.w	%d1
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 3
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 1
|	ror.l	#8, %d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 0
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 2
|	rol.l	#8, %d7
|	and.l	%d7, %d2		| Apply mask
|	or.l	%d2, (%a6)		| Write pattern

	lea	44(%a6), %a6		| get next screen line
	swap	%d1

|	move.w	%d1,%d2			| copy
|	andi.w	#0x7FFE, %d2		| make +
|	eor.w	%d2, %d1		| save sign, low bit only (H7L0)

|	or.w	(%a2, %d2.w), %d1	| convert HHLL to hlhlhlhlhlhlhlhl

	moveq	#0, %d2
	move.w	%d1, -(%sp)		| save high byte
	move.b	(%sp)+, %d2

	move.b	(%a1, %d2.w), -(%sp)	| Get High pattern
	move.w	(%sp)+, %d2

	andi.w	#0xFF, %d1

	move.b	(%a1,%d1.w), %d2	| Get low pattern

	swap	%d2			| move bytes to high word
	ror.l	%d3, %d2		| apply offset


|	move.l	(%a6), %d7		| Get BG Mask
|	clr.w	%d1
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 3
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 1
|	ror.l	#8, %d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 0
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 2
|	rol.l	#8, %d7
|	and.l	%d7, %d2		| Apply mask
|	or.l	%d2, (%a6)		| Write pattern

	rts


WriteBBytesR:
	swap	%d1
	moveq	#0, %d2
	ror.w	#4, %d1			| Get High bits together
	ror.b	#4, %d1
	rol.w	#4, %d1
	move.b	%d1, %d2
	move.b	(%a2,%d2.w), %d2	| Convert high byte
	move.b	(%a1,%d2.w), %d1	| get palette

	ror.w	#8, %d1
	move.b	%d1, %d2
	move.b	(%a2,%d2.w), %d2	| Convert Low byte
	move.b	(%a1,%d2.w), %d1	| get palette

	move.w	%d1, %d2

	swap	%d2			| move bytes to high word
	ror.l	%d3, %d2		| apply offset

|	move.l	(%a6), %d7		| Get BG Mask
|	clr.w	%d1
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 3
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 1
|	ror.l	#8, %d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 0
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 2
|	rol.l	#8, %d7
|	and.l	%d7, %d2		| Apply Mask
|	or.l	%d2, (%a6)		| Write pattern

	lea	44(%a6), %a6		| get next screen line
	swap	%d1

	moveq	#0, %d2
	ror.w	#4, %d1			| Get High bits together
	ror.b	#4, %d1
	rol.w	#4, %d1
	move.b	%d1, %d2
	move.b	(%a2,%d2.w), %d2	| Convert high byte
	move.b	(%a1,%d2.w), %d1	| get palette

	ror.w	#8, %d1
	move.b	%d1, %d2
	move.b	(%a2,%d2.w), %d2	| Convert Low byte
	move.b	(%a1,%d2.w), %d1	| get palette

	move.w	%d1, %d2

	swap	%d2			| move bytes to high word
	ror.l	%d3, %d2		| apply offset

|	move.l	(%a6), %d7		| Get BG Mask
|	clr.w	%d1
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 3
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 1
|	ror.l	#8, %d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 0
|	swap	%d7
|	move.b	%d7, %d1
|	move.b	(%a3,%d1.w), %d7	| byte 2
|	rol.l	#8, %d7
|	and.l	%d7, %d2		| Apply Mask
|	or.l	%d2, (%a6)		| Write pattern

	rts
