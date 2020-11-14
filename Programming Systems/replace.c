/*--------------------------------------------------------------------*/
/* replace.c                                                          */
/* Author: Minh-Thi Nguyen                                                       */
/*--------------------------------------------------------------------*/

#include "str.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*/

/* If pcFrom is the empty string, then write string pcLine to stdout
   and return 0.  Otherwise write string pcLine to stdout with each
   distinct occurrence of string pcFrom replaced with string pcTo,
   and return a count of how many replacements were made.  Make no
   assumptions about the maximum number of replacements or the
   maximum number of characters in strings pcLine, pcFrom, or pcTo. */

static size_t replaceAndWrite(const char *pcLine,
                              const char *pcFrom, const char *pcTo)
{
	size_t frequency = 0; /* Frequency of occurrence of pcFrom*/
	size_t i; /* for loop index */
	size_t begLength; /* Lengths of beginning string part */
		
	const char *searchString; /* Search string initially set to pcLine */

	/* Temporary pointers to strings to concatinate*/
	char *foundMatch;
	
	/* Check for null */
	assert((long)pcLine);
	assert((long)pcFrom);
	assert((long)pcTo);

	
	/* Check if there is anything to replace*/
	if (*pcFrom == '\0')
	{
		printf("%s", pcLine);
		return frequency;
	}
	
	searchString = pcLine;

	/* Count number of replacements */
	while (*searchString != '\0') 
	{
		/* Find the occurrence */
		foundMatch = Str_search(searchString, pcFrom);
		
		/* Break if no match found */
		if (foundMatch == NULL) break;
		
		/* Update the count of replacements */
		frequency++;
		
		/* Print the part before the match occurrence*/
		begLength = Str_getLength(searchString) - Str_getLength(foundMatch);
		
		for (i = 0; i < begLength; i++)
		{
			fprintf(stdout, "%s", &(*(searchString + i)));
		}
		
		/* Print out replacement */
		fprintf(stdout, "%s", pcTo);
		
		searchString = foundMatch + Str_getLength(pcFrom);
	}
	
	fprintf(stdout, "\n");

	return frequency;
}


/*--------------------------------------------------------------------*/
/* If argc is unequal to 3, then write an error message to stderr and
   return EXIT_FAILURE.  Otherwise...
   If argv[1] is the empty string, then write each line of stdin to
   stdout, write a message to stderr indicating that 0 replacements
   were made, and return 0.  Otherwise...
   Write each line of stdin to stdout with each distinct occurrence of
   argv[1] replaced with argv[2], write a message to stderr indicating
   how many replacements were made, and return 0.
   Assume that no line of stdin consists of more than MAX_LINE_SIZE-1
   characters. */
int main(int argc, char *argv[])
{
   enum {MAX_LINE_SIZE = 4096};
   enum {PROPER_ARG_COUNT = 3};
   char acLine[MAX_LINE_SIZE];
   char *pcFrom;
   char *pcTo;
   size_t uReplaceCount = 0;
   if (argc != PROPER_ARG_COUNT)
   {
      fprintf(stderr, "usage: %s fromstring tostring\n", argv[0]);
      return EXIT_FAILURE;
   }
   pcFrom = argv[1];
   pcTo = argv[2];
   
   while (fgets(acLine, MAX_LINE_SIZE, stdin) != NULL)
   {
	   if (Str_getLength(pcFrom) == 0) 
	   {
		   fprintf(stdout, acLine);
		   fprintf(stderr, "0 replacements were made.");
		   return 0;
	   }
	   
	   if (Str_getLength(pcFrom) != 0)
	   {
		   uReplaceCount += replaceAndWrite(acLine, pcFrom, pcTo);
	   }
   }
   
   fprintf(stderr, "%lu replacements\n", (unsigned long)uReplaceCount);
   return 0;
}
