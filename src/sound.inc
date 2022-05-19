/*
 * @(#)sound.inc
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

DoSound:
	tst.b	cfgSoundMute(%a0)	| Is sound muted?
	bne	DoSoundRet		| yes, skip sound playing

DoSound1:
	move.w	#0xFF00, %d4
	btst	#7, 0x26(%a1, %d4)	| Are sound circuits on?
	beq	DoSoundRet		| No, skip sound

	movem.l	%d0-%d4/%a0-%a4, -(%sp)

	tst.b	cfgSoundChan2(%a0)	| no, Is channel 2 enabled?
	bne.s	DoSndCh2		| yes, play it
ChkSndCh1:
	tst.b	cfgSoundChan1(%a0)	| 2 not enabled.  Is channel 1 enabled?
	bne.s	DoSndCh1		| yes, play it
	movem.l	(%sp)+, %d0-%d4/%a0-%a4
	bra	DoSoundRet

					| ***************************
					|   UPDATE SOUND CHANNEL 1
					| ***************************

DoSndCh1:
|	moveq	#0, %d0			| CHANNEL 1
|	move.w	snd1Freq(%a0), %d0	| Get current freq.
|	lsr.w	#1, %d0			| If freq=0, dont play
	move.l	#0x00020000, %d0
	move.w	#0x0800, %d1
	sub.w	snd1Freq(%a0), %d1
	divu	%d1, %d0
	and.l	#0x0000FFFF, %d0
	beq	SndUpdate

	moveq	#0, %d1
	move.b	snd1Vol(%a0), %d1	| amplitude
	move.b	cfgSoundVolume(%a0), %d5
	lsl.b	%d5, %d1		| shift GB vol. level to Palm level
	lsr.b	#1, %d1			| (need to allow vol multipliers of
					| 1/2, 1, 2, 4)
	beq	SndUpdate		| If vol=0, dont play
PlayCh1:
	lea	-10(%sp), %sp		| Get space for sound structure
	move.l	%sp, %a1
	move.w	#0,-(%sp)		| sync/async
	move.l	%a1, -(%sp)		| ptr to sound structure
	move.l	#0,-(%sp)		| sound channel

	move.w	#0x0303, (%a1)+		| Command: sndFreqDurationAmp
	move.l	%d0, (%a1)+		| freq
	move.w	#0x0050, (%a1)+		| duration
	move.w	%d1, (%a1)		| vol


	trap	#15			| Call SndDoCmd
	.word	0xA233
	lea	20(%sp),%sp

	bra	SndUpdate


					| ***************************
					|   UPDATE SOUND CHANNEL 2
					| ***************************
DoSndCh2:
|	moveq	#0, %d0			| CHANNEL 2
|	move.w	snd2Freq(%a0), %d0	| Get current freq.
|	lsr.w	#1, %d0			| If freq=0, dont play
	move.l	#0x00020000, %d0
	move.w	#0x0800, %d1
	sub.w	snd1Freq(%a0), %d1
	divu	%d1, %d0
	and.l	#0x0000FFFF, %d0
	beq.s	ChkSndCh1		|

	moveq	#0, %d1
	move.b	snd2Vol(%a0), %d1	| amplitude
	move.b	cfgSoundVolume(%a0), %d5
	lsl.b	%d5, %d1		| shift GB vol. level to Palm level
	lsr.b	#1, %d1			| (need to allow vol multipliers of
					| 1/2, 1, 2, 4)
	beq	ChkSndCh1		| if vol=0, dont play


	tst.b	cfgSoundChan1(%a0)	| Is channel 1 enabled?
	beq.s	Ch1NoCheck		| no, skip freq comparision

	move.b	snd1Vol(%a0), %d2	| Check Channel 1 amplitude
	move.b	cfgSoundVolume(%a0), %d5
	lsl.b	%d5, %d2		| shift GB vol. level to Palm level
	lsr.b	#1, %d2			| (need to allow vol multipliers of
					| 1/2, 1, 2, 4)
	beq.s	Ch1NoCheck		| If vol=0, playing ch2

|	move.w	snd1Freq(%a0), %d5
|	lsr.w	#1, %d5
	move.l	#0x00020000, %d5
	move.w	#0x0800, %d3
	sub.w	snd1Freq(%a0), %d3
	divu	%d3, %d5
	and.l	#0x0000FFFF, %d5

	beq.s	Ch1NoCheck		| If freq=0, playing ch2
	cmp.w	%d0, %d5		| If ch1 Freq => ch2 Freq,
	bge.s	Ch1NoCheck		| do channel 2
	move.b	%d2, %d1		| else, swap vol
	move.w	%d5, %d0		| swap freq
					| Play channel 1

Ch1NoCheck:
	lea	-10(%sp), %sp		| Get space for sound structure
	move.l	%sp, %a1
	move.w	#0,-(%sp)		| sync/async
	move.l	%a1, -(%sp)		| ptr to sound structure
	move.l	#0,-(%sp)		| sound channel

	move.w	#0x0303, (%a1)+		| Command: sndFreqDurationAmp
	move.l	%d0, (%a1)+		| freq
	move.w	#0x0050, (%a1)+		| duration
	move.w	%d1, (%a1)		| vol

	trap	#15			| Call SndDoCmd
	.word	0xA233
	lea	20(%sp),%sp


SndUpdate:
	movem.l	(%sp)+, %d0-%d4/%a0-%a4
					| *** Update Sound Channel Values ***
	tst.b	snd1Freq(%a0)		| See if ch#1 Freq. changing
	beq.s	snd1chkEnv
	subq.b	#1, snd1FCtr(%a0)	| Decrement counter
	bne.s	snd1chkEnv
	move.b	snd1Freq(%a0), snd1FCtr(%a0)	| restore counter

	move.w	snd1Freq(%a0), %d0	| Get Freq
	move.b	snd1Div(%a0), %d5	| Get divisor
	lsr.w	%d5, %d0		| Get freq. change
	move.w	#0xFF00, %d4
	btst.b	#3, 0x10(%a1, %d4)	| See if freq. up or down
	bne.s	snd1Fdec
	add.w	%d0, snd1Freq(%a0)	| Increase Freq
	cmpi.w	#0x07FF, snd1Freq(%a0)	| see if max freq
	blt.s	snd1chkEnv
	clr.w	snd1Freq(%a0)		| yes, no more changes
	bra.s	snd1chkEnv
snd1Fdec:
	sub.w	%d0, snd1Freq(%a0)	| Decrease Freq
	bgt.s	snd1chkEnv		| if at min. freq
	clr.w	snd1Freq(%a0)		| no more changes

snd1chkEnv:
	tst.b	snd1Env(%a0)		| See if ch#1 envelope changing
	beq.s	snd1done
	subq.b	#1, snd1ECtr(%a0)	| Decrement counter
	bne.s	snd1done
	move.b	snd1Env(%a0), snd1ECtr(%a0)	| restore counter
	move.w	#0xFF00, %d4
	btst.b	#3, 0x12(%a1, %d4)	| See if attenuate or amplify
	beq.s	snd1dec
	addq.b	#1, snd1Vol(%a0)	| Amplify by 1 step
	cmpi.b	#15, snd1Vol(%a0)	| see if max volume
	blt.s	snd1done
	clr.b	snd1Env(%a0)		| yes, no more changes
	bra.s	snd1done
snd1dec:
	subq.b	#1, snd1Vol(%a0)	| attenuate by 1 step
	bgt.s	snd1done		| if at min. volume,
	clr.b	snd1Env(%a0)		| no more changes

snd1done:

snd2chkEnv:
	tst.b	snd2Env(%a0)		| See if ch#2 envelope changing
	beq.s	snd2done
	subq.b	#1, snd2ECtr(%a0)	| Decrement counter
	bne.s	snd2done
	move.b	snd2Env(%a0), snd2ECtr(%a0)	| restore counter
	move.w	#0xFF00, %d4
	btst.b	#3, 0x17(%a1, %d4)	| See if attenuate or amplify
	beq.s	snd2dec
	addq.b	#1, snd2Vol(%a0)	| Amplify by 1 step
	cmpi.b	#15, snd2Vol(%a0)	| see if max volume
	blt.s	snd2done
	clr.b	snd2Env(%a0)		| yes, no more changes
	bra.s	snd2done
snd2dec:
	subq.b	#1, snd2Vol(%a0)	| attenuate by 1 step
	bgt.s	snd2done		| if at min. volume,
	clr.b	snd2Env(%a0)		| no more changes

snd2done:

DoSoundRet:
	rts
