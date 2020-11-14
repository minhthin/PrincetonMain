/*--------------------------------------------------------------------*/
/* strp.c                                             		     	  */
/* Author: Minh-Thi Nguyen                                            */
/*--------------------------------------------------------------------*/

#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "str.h"
/*--------------------------------------------------------------------*/

size_t Str_getLength(const char *pcSrc)
{
   const char *pcEnd; 
	 
   assert((long)pcSrc);
	 
   pcEnd = pcSrc;
   while (*pcEnd != '\0')
      pcEnd++;
	 
   return (size_t)(pcEnd - pcSrc);
}

/*--------------------------------------------------------------------*/
char *Str_copy(char *dest, const char *pcSrc)
{
	char *output = dest; 
	
	assert((long)pcSrc);
	assert((long)dest);
	
	while (*pcSrc != '\0')
	{
		*dest = *pcSrc;
		dest++;
		pcSrc++;
	}
	
	*dest = '\0';
	
	return output;
}

/*--------------------------------------------------------------------*/
char *Str_concat(char *dest, const char *pcSrc)
{
	char *output = dest;
	
	assert((long)pcSrc);
	assert((long)dest);
	
	while (*dest != '\0')
	{
		dest++;
	}
	
	while (*pcSrc != '\0') 
	{
		*dest = *pcSrc;
		pcSrc++;
		dest++;
	}
	
	*dest = '\0';
	
	return output;
	
}

/*--------------------------------------------------------------------*/
int Str_compare(const char *s1, const char *s2) 
{
	assert((long)s1);
	assert((long)s2);
	
	while (*s1 == *s2)
	{
		if (*s1 == '\0') return 0;
		s1++;
		s2++;
	}
	
	if (*s1 < *s2) return -1;
	else return 1;
	
}

/*--------------------------------------------------------------------*/
char *Str_search(const char *pcSrc, const char *sub) 
{
	const char *foundMatch; /* pointer to first occurrence */
	const char *searchSub; /* pointer to search subarray */
	
	size_t i;
	size_t subLength = Str_getLength(sub);
	
	assert((long)pcSrc);
	assert((long)sub);
	
	/* check if no string to be searched */
	if (subLength == 0) return (char*)pcSrc; 
	
	while (*pcSrc != '\0')
	{
		if (*pcSrc == *sub)
		{
			/* search for substring once match found */
			i = 0;
			foundMatch = pcSrc;
			searchSub = sub;
			
			while (*searchSub != '\0' && *foundMatch == *searchSub)
			{
				foundMatch++;
				searchSub++;
				i++;
			}
			
			if (i == subLength) 
			{
				return (char*)pcSrc;
			}
		}
		
		pcSrc++;
	}
	return NULL;
	
}
