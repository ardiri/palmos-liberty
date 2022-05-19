#
# @(#)Makefile
#
# Copyright 2000, Aaron Ardiri     (mailto:aaron@ardiri.com)
#                 Michael Ethetton (mailto:methetton@gambitstudios.com)
# All rights reserved.
# 
# This file was generated as part of the "rom2pdb" program that generates
# PDB data files for the "liberty" program developed for the Palm Computing 
# Platform designed by Palm: http://www.palm.com/ 
#
# The contents of this file is confidential and proprietrary in nature 
# ("Confidential Information"). Redistribution or modification without 
# prior consent of the original author(s) is prohibited.
#
# REVISIONS:
# ----------
#
# 13-Jul-2000 Aaron Ardiri Creation
#

# compiler commands 
PREFIX   = 
CC       = $(PREFIX)gcc

# source / outputs
SRCS     = rom2pdb.c
EXEC     = rom2pdb

# compiler flags 
CCFLAGS = -Wall 

# compile requirements
all:
	$(CC) $(CCFLAGS) -o $(EXEC) $(SRCS)

# clean-up funtions
clean:
	rm -f *.[oa] *~

distclean: 
	rm -f *.[oa] *~ $(EXEC)
