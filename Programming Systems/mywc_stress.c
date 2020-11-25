/*--------------------------------------------------------------------*/
/* mywc_stress.c                                                      */
/* Author: Marti Vives and Minh-Thi Nguyen                            */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

int main(void)
{
	int i,num,nchars;
	i = 0;
	scanf("%d",&nchars);

	while (i<nchars){
		num = rand()%0x7F;
		if ( (num == 0x09) || (num == 0x0A) || ((num >= 0x20) && 
			(num <= 0x7E)) ) {
			printf("%c", num);
			i++;
		}
	}
	return 0;
}
