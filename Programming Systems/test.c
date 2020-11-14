#include <stddef.h>
#include <assert.h>
#include <stdio.h>
#include "string.h"

size_t Str_getLength(const char pcSrc[])
{
	size_t uLength = 0;
	assert(pcSrc != NULL);
	while (pcSrc[uLength] != '\0')
		uLength++;
	return uLength;
}


char *Str_copy(char dest[], const char pcSrc[])
{
	assert(pcSrc != NULL);
	assert(dest != NULL);
	
	size_t ind = 0;
	
	while (pcSrc[ind] != '\0') 
	{
		dest[ind] = pcSrc[ind];
		ind++;
	}
	
	dest[ind] = '\0';
	
	return dest;
}

char *Str_concat(char dest[], const char pcSrc[])
{
	assert(pcSrc != NULL);
	assert(dest != NULL);
	
	size_t ind = 0;
	size_t indDest = 0;
	
	while (dest[indDest] != '\0') 
	{
		indDest++;
	}
	
	while (pcSrc[ind] != '\0') 
	{
		dest[indDest] = pcSrc[ind];
		indDest++;
		ind++;
	}
	
	dest[indDest] = '\0';
	
	return dest;
}

int Str_compare(const char s1[], const char s2[]) 
{
	assert(s1 != NULL);
	assert(s2 != NULL);
	
	size_t ind = 0;
	
	while (s1[ind] == s2[ind]) 
	{
		if (s1[ind] == '\0') return 0;
		ind++;
	}
	
	if (s1[ind] > s2[ind]) return 1;
	else if (s1[ind] < s2[ind]) return -1;
}

char *Str_search(const char pcSrc[], const char sub[]) 
{
	assert(pcSrc != NULL);
	assert(sub != NULL);
	
	if (Str_getLength(sub) == 0) return pcSrc;
	
	size_t ind = 0;
	
	char *foundMatch = malloc(Str_getLength(pcSrc));
		
	while (pcSrc[ind] != '\0')
	{
		while (pcSrc[ind + i] != '\0' && pcSrc[ind + i] == sub[i])
		{
			foundMatch[i] = pcSrc[ind + i];
			i++;
		}
		
		if (i == Str_getLength(sub))
		{
			return foundMatch;
		}
		ind++;
	}
	return NULL;
}


int main(void) 
{
	char string1[256];
	scanf( "%s" , string1 );
	char string2[256];
	scanf( "%s" , string2 );
	
	char *ptr;
	ptr = Str_search(string1, string2);
	printf("\n%s", ptr);
	
	char *realSearch = strstr(string1, string2);
	printf("\n%s", realSearch);
	
	printf("\n%d", realSearch == ptr);
	
}
