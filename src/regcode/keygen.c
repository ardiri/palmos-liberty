/*
 * @(#)keygen.c
 *
 * Copyright 2000, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                 Michael Ethetton (mailto:methetton@gambitstudios.com)
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_IDLENGTH 8

int
main(int argc, char *argv[])
{
  char hex[MAX_IDLENGTH+1] = {};
  char userID[256]         = {};

  printf("keygen Gmbt\n");
  printf("  Copyright 2000 Aaron Ardiri (ardiri@palmgear.com)\n\n");

  // do we ask for it?
  if (argc < 2) { 
    fprintf(stdout, "userID:   ");
    fscanf(stdin, "%s", userID);
  }

  // supplied at commandline
  else {
    sscanf(argv[1], "%s", userID);
  }

  // maybe it is in hex?
  if (sscanf(userID, "%x:%x:%x:%x:%x:%x:%x:%x:%x", 
             &hex[0], &hex[1], &hex[2], 
             &hex[3], &hex[4], &hex[5], 
             &hex[6], &hex[7], &hex[8]) == 9) {

    int i, checksum = 0;
    for (i=0; i<MAX_IDLENGTH; i++) {
      userID[i] = hex[i];
      checksum  ^= hex[i];
    }

    // lets check the checksum
    if (checksum != hex[MAX_IDLENGTH]) {
      fprintf(stdout, "ERROR: checksum digit invalid\n");
      exit(1);
    }
  }

  {
    FILE           *file;
    unsigned char  key;
    unsigned char  dhdr[88]     = {}; // header
    unsigned char  data[0x7FFF] = {}; // 32K of memory
    unsigned char  regi[0x7FFF] = {}; // 32K of memory
    unsigned short i, index, regiLength, dataLength;
  
    // read in the "encryption" key
    file       = fopen("@message","rb");
    dataLength = fread(data,1,0x7fff,file);
    fclose(file);

    // read in the data to encrypt
    file       = fopen("code0007.bin","rb");
    strcpy(regi, "|HaCkMe|");
    regiLength = fread(regi+16,1,0x7fff,file) + 16;
    fclose(file);
 
    strcpy(dhdr, "Gmbt keygen");
    dhdr[0x20] = 0x80;
    dhdr[0x24] = 0xff; dhdr[0x25] = 0xff; dhdr[0x26] = 0xff; dhdr[0x27] = 0xff;
    dhdr[0x28] = 0xff; dhdr[0x29] = 0xff; dhdr[0x2a] = 0xff; dhdr[0x2b] = 0xff;
    dhdr[0x3c] = '_';  dhdr[0x3d] = 'k';  dhdr[0x3e] = 'e';  dhdr[0x3f] = 'y'; 
    dhdr[0x40] = 'G';  dhdr[0x41] = 'm';  dhdr[0x42] = 'b';  dhdr[0x43] = 't'; 
    dhdr[0x4D] = 0x01;  // one record
    dhdr[0x51] = 0x58;  // offset (hard coded)

    // starting key = checksum
    key = 0;
    for (i=0; i<MAX_IDLENGTH; i++) {
      key += userID[i];
    }
    key = (key ^ 0xff) & 0xff;
    if (key == 0) key = 0x20; // key *cannot* be zero

    // encrypt the chunk
    index = key;
    for (i=0; i<regiLength; i++) {

      // adjust the byte
      regi[i] ^= key;

      // dynamically update the key
      do {
        index = (index + key + 1) % dataLength;
        key   = data[index];
      } while (key == 0);

      data[index] = (unsigned char)((key + index) & 0xff);
    }

    file = fopen("Gmbt_keygen.pdb","wb");

    // write the data header 
    fwrite(dhdr,1,88,file);

    // write the new code into file
    fwrite(regi,1,regiLength,file);
    fclose(file);

    exit(0);
  }
}
