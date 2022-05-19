#
# @(#)Makefile
#
# Copyright 2000-2002, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                      Michael Ethetton (mailto:methetton@gambitstudios.com)
# All rights reserved.
#
# This file was generated as part of the "liberty" program developed for 
# the Palm Computing Platform designed by Palm: http://www.palm.com/ 
#
# The contents of this file is confidential and proprietrary in nature 
# ("Confidential Information"). Redistribution or modification without 
# prior consent of the original author(s) is prohibited.
#
# NOTE:
# The following portions of code found within this source code file are
# owned exclusively by Michael Ethetton, and shall not be used in or
# sold to other projects (internal or external) without the written
# permission of Michael Ethetton.
#
# - z-80 CPU Emulation
# - Nintendo Gameboy Emulation System
#
# The following portions of code found within this source code file are
# owned exclusively by Aaron Ardiri, and shall not be used in or 
# sold to other projects (internal or external) without the written 
# permission of Aaron Ardiri.
#
# - GNU prc-tools 2.0 application framework
# - Help System 
# - Device Configuration Module
# - Multiple Code Segments Solution
#
# It shall be noted that Aaron Ardiri has licensed the above source code
# and framework to Michael Ethetton for use only in the "liberty" project.
#

# compiler commands 
PREFIX   = 
CC       = $(PREFIX)m68k-palmos-gcc
PILRC    = $(PREFIX)pilrc
BUILDPRC = $(PREFIX)build-prc
PRC2BIN  = $(PREFIX)prc2bin
ENCRYPT  = regcode/encrypt

# source / outputs
BINFILE = liberty
EXEC    = $(BINFILE)_$(LANG)
MSEG    = $(EXEC)-sections.o $(EXEC)-sections.ld
OBJS    = palm.o device.o game.o help_$(LANG).o gameboy.o register.o $(MSEG)
TEMP    = tmp.out

# compiler flags 
CCFLAGS = -O2 -Wall -palmos4 -DLANG_$(LANG) -DPROTECTION_$(PROTECTION)

# compile requirements
all:
	LANG=en PROTECTION=OFF make liberty_unprotected
	LANG=en PROTECTION=ON  make liberty
	LANG=fr PROTECTION=ON  make liberty
	LANG=de PROTECTION=ON  make liberty
	LANG=es PROTECTION=ON  make liberty

liberty: $(EXEC) $(EXEC).def bin.stamp
	$(BUILDPRC) $(EXEC).def $(BINFILE) *.bin
	$(PRC2BIN) $(EXEC).prc > $(TEMP)
	cp @message code0007.bin regcode/
	$(ENCRYPT) code0002.bin code0006.bin
	cp @message code0007.bin
	$(BUILDPRC) $(EXEC).def *.bin
	make clean
	cp $(EXEC).prc ..

liberty_unprotected: $(EXEC) $(EXEC).def bin.stamp
	$(BUILDPRC) $(EXEC).def $(BINFILE) *.bin
	make clean
	cp $(EXEC).prc ../$(EXEC)_reg.prc

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(CCFLAGS) -o $(BINFILE)

$(MSEG): $(EXEC).def
	m68k-palmos-multigen $(EXEC).def
	$(CC) -c -o $(EXEC)-sections.o $(EXEC)-sections.s

bin.stamp: $(EXEC).rcp
	$(PILRC) -q -D PROTECTION_$(PROTECTION) $(EXEC).rcp

# compile rules
.SUFFIXES: .c .cc .o

.c.o:
	$(CC) -c $(CCFLAGS) $<
.cc.o:
	$(CC) -E $(CCFLAGS) $< > $<.s
	$(CC) -c -o $@ $(CCFLAGS) $<.s

# clean-up funtions
clean:
	rm -f *.[oa] *sections.* *.bin *.hdr bin.res *.grc *.s *~ $(TEMP) $(BINFILE)

distclean: 
	rm -f *.[oa] *sections.* *.bin *.hdr bin.res *.[pg]rc *.s *~ $(TEMP) $(BINFILE)
