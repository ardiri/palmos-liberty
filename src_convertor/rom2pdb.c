/*
 * @(#)rom2pdb.c
 *
 * Copyright 2000, Aaron Ardiri     (mailto:aaron@ardiri.com)
 *                 Michael Ethetton (mailto:methetton@gambitstudios.com)
 * All rights reserved.
 * 
 * This file was generated as part of the "rom2pdb" program that generates
 * PDB data files for the "liberty" program developed for the Palm Computing 
 * Platform designed by Palm: http://www.palm.com/ 
 *
 * The contents of this file is confidential and proprietrary in nature 
 * ("Confidential Information"). Redistribution or modification without 
 * prior consent of the original author(s) is prohibited.
 *
 * REVISIONS:
 * ----------
 *
 *  7-Jul-2000 Aaron Ardiri Creation
 *  8-Jul-2000 Aaron Ardiri PDB file is generated in same directory as rom
 * 13-Jul-2000 Aaron Ardiri updates to compile under Borland C 5.02
 * 26-Jul-2000 Aaron Ardiri TETRIS timing problem:
 *                          - JR Z, 0xFB can be replaced with HALT
 *                          - ofs: 0x02F0, 0x28 --> 0x76
 * 30-Jul-2000 Aaron Ardiri generation using smaller rom file format
 * 15-Oct-2000 Aaron Ardiri "illegal" character removal from ROM name(s)
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int
main(int argc, char *argv[])
{
  int result = 0;

  printf("GameBoy (tm) ROM Convertor for Liberty\n");
  printf("  Copyright 2000 Gambit Studios, LLC\n\n");
  printf("Gambit Studios does not endorse the illegal use of commercial\n");
  printf("games. DO NOT request illegal images from us - we dont't know\n");
  printf("where you can get them, and will not respond to your requests.\n\n");

  // called correctly?
  if (argc > 1) {

    unsigned short i;

    // process all data files passed in
    for (i=1; i<argc; i++) {

      FILE           *inFile          = NULL;
      FILE           *outFile         = NULL;
      unsigned char  romName[32]      = { 0 };
      unsigned char  outFileName[512] = { 0 };
      unsigned char  buffer[0x2000]   = { 0 }; // 8K of memory
      unsigned char  bank1a[0x2000]   = { 0 }; // 8K of memory
      unsigned char  bank1b[0x2000]   = { 0 }; // 8K of memory
      unsigned short j, offset, numRec;

      // open the "rom" file
      inFile = fopen(argv[i],"rb");
     
      // does it exist?
      if (inFile != NULL) {

        unsigned long romSize = 0;

        // lets check the size
        fseek(inFile, 0L, SEEK_END); romSize = ftell(inFile); rewind(inFile);

        // is it a valid size?
        if ((romSize != 0) && (romSize % 32768) == 0) {

          unsigned char tmp = 0;

          printf("[%s]:\n", argv[i]);

          // ok.. lets get the rom name
	  fseek(inFile, 308, SEEK_SET);
          fread(buffer,1,16,inFile); buffer[15] = '\0';
	  memset(romName, 0, 32); sprintf(romName, "Gmbt_%s", buffer);

          // lets remove any "erroneous" characters from the file name
          //
          // "*", "?", "<", ">" + non_printable ( < 0x20 + > 0x7f )
          // - replace with "_"

          for (j=0; ((j<15) && (buffer[j] != '\0')); j++) {
            if (
                (buffer[j] <= 0x20) || (buffer[j] >= 0x7f) || // unprintable
		(buffer[j] == '*' ) || (buffer[j] == '?' ) ||
		(buffer[j] == '.' ) || (buffer[j] == ',' ) ||
		(buffer[j] == ':' ) || (buffer[j] == ';' ) ||
		(buffer[j] == '+' ) || (buffer[j] == '-' ) ||
		(buffer[j] == '<' ) || (buffer[j] == '>' ) ||
		(buffer[j] == '(' ) || (buffer[j] == ')' ) ||
		(buffer[j] == '{' ) || (buffer[j] == '}' ) ||
		(buffer[j] == '[' ) || (buffer[j] == ']' ) ||
		(buffer[j] == '\\') || (buffer[j] == '/' ) ||
		(buffer[j] == '|' ) || (buffer[j] == ':' ) ||
		(buffer[j] == '~' ) || (buffer[j] == '`' ) ||
		(buffer[j] == '!' ) || (buffer[j] == '$' ) ||
		(buffer[j] == '^' ) || (buffer[j] == '&' ) ||
		(buffer[j] == '\'') || (buffer[j] == '"' )    // illegal chars
               ) 
            buffer[j] = '_';
	  }

          // lets generate the correct FULL path for the pdb file generation
	  {
	    unsigned char *ptrChr;
            memset(outFileName, 0, 512); 
	    strcpy(outFileName, argv[i]);
	    ptrChr = outFileName + strlen(outFileName)-1;
	    while (
	           (*ptrChr != '/')  &&    // unix
		   (*ptrChr != '\\') &&    // windows
		   (ptrChr > outFileName)  // dont go too far back!
		  ) {
	      ptrChr--;
	    }
	    if (ptrChr > outFileName) 
              sprintf(ptrChr+1, "%s.pdb", buffer);
            else
              sprintf(outFileName, "%s.pdb", buffer);
          }	      

          // ok.. lets get the cartidge type
	  fseek(inFile, 327, SEEK_SET);
	  fread(&tmp,1,1,inFile);

          printf("CARTRIDGE TYPE: ");
	  switch (tmp) 
	  {
            case 0:   printf("ROM only"); break;
            case 1:   printf("ROM+MBC1"); break; 
	    case 2:   printf("ROM+MBC1+RAM"); break;
            case 3:   printf("ROM+MBC1+RAM+BATTERY"); break;
            case 5:   printf("ROM+MBC2"); break;
            case 6:   printf("ROM+MBC2+BATTERY"); break;
            case 8:   printf("ROM+RAM"); break;
            case 9:   printf("ROM+RAM+BATTERY"); break;
            case 11:  printf("ROM+MMM01"); break;
            case 12:  printf("ROM+MMM01+SRAM"); break;
            case 13:  printf("ROM+MMM01+SRAM+BATTERY"); break;
            case 15:  printf("ROM+MBC3+TIMER+BATTERY"); break;
            case 16:  printf("ROM+MBC3+TIMER+RAM+BATTERY"); break;
            case 17:  printf("ROM+MBC3"); break;
            case 18:  printf("ROM+MBC3+RAM"); break;
            case 19:  printf("ROM+MBC3+RAM+BATTERY"); break;
            case 25:  printf("ROM+MBC5"); break;
            case 26:  printf("ROM+MBC5+RAM"); break;
            case 27:  printf("ROM+MBC5+RAM+BATTERY"); break;
            case 28:  printf("ROM+MBC5+RUMBLE"); break;
            case 29:  printf("ROM+MBC5+RUMBLE+SRAM"); break;
            case 30:  printf("ROM+MBC5+RUMBLE+SRAM+BATTERY"); break;
            case 31:  printf("POCKET CAMERA"); break;
            case 253: printf("Bandai TAMA5"); break;
            case 254: printf("Hudson HuC-3"); break;
            case 255: printf("Hudson HuC-1 ROM+RAM+BATTERY"); break;
	    default:  printf("UNKNOWN CARTRIDGE"); break;
	  }
          printf("\n");

          // ok.. lets get the rom size
	  fseek(inFile, 328, SEEK_SET);
	  fread(&tmp,1,1,inFile);

          printf("ROM SIZE:       ");
	  switch (tmp) 
	  {
	    case 0:   printf("32Kb"); break;
	    case 1:   printf("64Kb"); break;
	    case 2:   printf("128Kb"); break;
	    case 3:   printf("256Kb"); break;
	    case 4:   printf("512Kb"); break;
	    case 5:   printf("1Mb"); break;
	    case 6:   printf("2Mb"); break;
	    case 52:  printf("1.1Mb"); break;
	    case 53:  printf("1.2Mb"); break;
	    case 54:  printf("1.4Mb"); break;
	    default:  printf("unknown"); break;
	  }
          printf("\n");

          // ok.. lets get the ram size
	  fseek(inFile, 329, SEEK_SET);
	  fread(&tmp,1,1,inFile);

          printf("RAM SIZE:       ");
	  switch (tmp) 
	  {
	    case 0:   printf("None"); break;
	    case 1:   printf("2Kb"); break;
	    case 2:   printf("8Kb"); break;
	    case 3:   printf("32Kb"); break;
	    case 4:   printf("128Kb"); break;
	    default:  printf("unknown"); break;
	  }
          printf("\n");

          // open the "pdb" file
          outFile = fopen(outFileName,"wb");

	  // write the HEADER
          fwrite(romName,1,32,outFile);              // database name
	  buffer[0x00] = 0x80; buffer[0x01] = 0x01; 
	  buffer[0x02] = 0x00; buffer[0x03] = 0x01;  // flags + version
	  buffer[0x04] = 0xAD; buffer[0x05] = 0xC0;
	  buffer[0x06] = 0xBE; buffer[0x07] = 0xA0;  // creation time
	  buffer[0x08] = 0xAD; buffer[0x09] = 0xC0;
	  buffer[0x0A] = 0xBE; buffer[0x0B] = 0xA0;  // modification time
	  buffer[0x0C] = 0xAD; buffer[0x0D] = 0xC0;
	  buffer[0x0E] = 0xBE; buffer[0x0F] = 0xA0;  // backup time
	  buffer[0x10] = 0x00; buffer[0x11] = 0x00;
	  buffer[0x12] = 0x00; buffer[0x13] = 0x00;  // modification number
	  buffer[0x14] = 0x00; buffer[0x15] = 0x00;
	  buffer[0x16] = 0x00; buffer[0x17] = 0x00;  // app info
	  buffer[0x18] = 0x00; buffer[0x19] = 0x00;
	  buffer[0x1A] = 0x00; buffer[0x1B] = 0x00;  // sort info
	  buffer[0x1C] = 'g';  buffer[0x1D] = 'R';
	  buffer[0x1E] = 'O';  buffer[0x1F] = 'M';   // type
	  buffer[0x20] = 'G';  buffer[0x21] = 'm';
	  buffer[0x22] = 'b';  buffer[0x23] = 't';   // creator
	  buffer[0x24] = 0x00; buffer[0x25] = 0x00;
	  buffer[0x26] = 0x00; buffer[0x27] = 0x00;  // unique ID seed
	  buffer[0x28] = 0x00; buffer[0x29] = 0x00;
	  buffer[0x2A] = 0x00; buffer[0x2B] = 0x00;  // next record list
          fwrite(buffer,1,0x2C,outFile);

          numRec = (unsigned short)(romSize / 16384);
          printf("RECORD COUNT:   %d\n", numRec);

          buffer[0x00] = numRec / 256;
          buffer[0x01] = numRec % 256;
          fwrite(buffer,1,2,outFile);                // number of reocrds

          // write the resource headers
	  offset = 0x50 + (numRec * 10);
	  for (j=0; j<numRec; j++) {
            
	    buffer[0x00] = '_';  buffer[0x01] = 'd';
	    buffer[0x02] = 'a';  buffer[0x03] = 't';
	    buffer[0x04] = 0x00; buffer[0x05] = (unsigned char)j;
	    buffer[0x06] = 0x00; buffer[0x07] = (unsigned char)j/4;

	    switch (j % 4) 
	    {
	      case 0: buffer[0x08] = (offset / 256);
                      buffer[0x09] = offset % 256;
		      break;

	      case 1: buffer[0x08] = 0x40 + (offset / 256);
                      buffer[0x09] = offset % 256;
		      break;

	      case 2: buffer[0x08] = 0x80 + (offset / 256);
                      buffer[0x09] = offset % 256;
		      break;

	      case 3: buffer[0x08] = 0xC0 + (offset / 256);
                      buffer[0x09] = offset % 256;
		      break;
	    }

            fwrite(buffer,1,0x0A,outFile);
	  }

          buffer[0x00] = 0x00; buffer[0x01] = 0x00;
          fwrite(buffer,1,2,outFile);

	  // lets write the CHUNKS!
	  rewind(inFile);
	  fread(bank1a,1,8192,inFile);
	  fread(bank1b,1,8192,inFile);

          // apply TETRIS timing fix (as done by NO$GB) 
	  // -- Aaron Ardiri, 2000
	  if ((strcmp(romName, "Gmbt_TETRIS") == 0) && (bank1a[0x2f0] == 0x28)) 
            bank1a[0x2f0] = 0x76;

          // transfer over data
          fwrite(bank1a,1,8192,outFile);
          fwrite(bank1b,1,8192,outFile);  // first two rom banks

	  while (fread(buffer,1,8192,inFile) != 0) {
	    fwrite(buffer,1,8192,outFile);
	  }

          // close output file
          fclose(outFile);
	}
	else 
          printf("ERROR: [%s] INVALID SIZE\n", argv[i]);

        // close input file
        fclose(inFile);
      }
      else
        printf("ERROR: [%s] FILE NOT FOUND\n", argv[i]);
    }
    printf("\n");
  }
  else {
    printf("USAGE:\n");
    printf("  rom2pdb { file1 file2 ... fileN }\n\n");

    result = 1;
  }

  return result;
}
