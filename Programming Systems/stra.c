/*--------------------------------------------------------------------*/
/* stra.c                                             		     	  */
/* Author: Minh-Thi Nguyen                                            */
/*--------------------------------------------------------------------*/

#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include "str.h"
/*--------------------------------------------------------------------*/

size_t Str_getLength(const char pcSrc[])
{
	size_t uLength = 0; /* index relative to beginning of string */
	assert(pcSrc != NULL);
	
	while (pcSrc[uLength] != '\0')
		uLength++;
	return uLength;
}

/*--------------------------------------------------------------------*/
char *Str_copy(char dest[], const char pcSrc[])
{
	size_t uLength = 0; 
	
	assert(pcSrc != NULL);
	assert(dest != NULL);
	
	while (pcSrc[uLength] != '\0') 
	{
		dest[uLength] = pcSrc[uLength];
		uLength++;
	}
	
	dest[uLength] = '\0';
	
	return dest;
}

/*--------------------------------------------------------------------*/
char *Str_concat(char dest[], const char pcSrc[])
{
	/* index relative to beginning of second string */
	size_t uLength = 0; 
	/* index relative to beginning of final string */
	size_t uLengthDest = 0; 
	
	assert(pcSrc != NULL);
	assert(dest != NULL);
	
	while (dest[uLengthDest] != '\0') 
	{
		uLengthDest++;
	}
	
	while (pcSrc[uLength] != '\0') 
	{
		dest[uLengthDest] = pcSrc[uLength];
		uLengthDest++;
		uLength++;
	}
	
	dest[uLengthDest] = '\0';
	
	return dest;
}

/*--------------------------------------------------------------------*/
int Str_compare(const char s1[], const char s2[]) 
{
	size_t uLength = 0; /* index relative to beginning of each string */
	
	assert(s1 != NULL);
	assert(s2 != NULL);
	
	while (s1[uLength] == s2[uLength]) 
	{
		if (s1[uLength] == '\0') return 0;
		uLength++;
	}
	
	if (s1[uLength] > s2[uLength]) return 1;
	else return -1;
}

/*--------------------------------------------------------------------*/
char *Str_search(const char pcSrc[], const char sub[]) 
{
	size_t uLength = 0; /* index relative to beginning of primary string */
	size_t uLengthSub; /* index relative to beginning of sub string */
	
	const char *foundMatch; /* Pointer to primary string if match found */
	
	/* check arguments are not null */
	assert(pcSrc != NULL);
	assert(sub != NULL);
	
	/* check if there is a string to be searched */
	if (Str_getLength(sub) == 0) {
		return (char*)pcSrc;
	}
	
	
	/* traverse primary array to find complete match*/
	while (pcSrc[uLength] != '\0')
	{
		uLengthSub = 0;
		
		while (pcSrc[uLength + uLengthSub] != '\0' && 
			pcSrc[uLength + uLengthSub] == sub[uLengthSub]) 
			uLengthSub++;
		
		if (uLengthSub == Str_getLength(sub))
		{
			/* point to where sub string occurs */
			foundMatch = pcSrc + uLength; 
			return (char*)foundMatch;
		}
		uLength++;
	}
	return NULL;
}
