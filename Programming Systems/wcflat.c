/*--------------------------------------------------------------------*/
/* wcflat.c                                                           */
/* Author: Minh-Thi Nguyen and Marti Vives                            */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <ctype.h>

/*--------------------------------------------------------------------*/
/* In lieu of a boolean data type. */
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

static long lLineCount = 0;      /* Bad style. */
static long lWordCount = 0;      /* Bad style. */
static long lCharCount = 0;      /* Bad style. */
static int iChar;                /* Bad style. */
static int iInWord = FALSE;      /* Bad style. */

/*--------------------------------------------------------------------*/

/* Write to stdout counts of how many lines, words, and characters
   are in stdin. A word is a sequence of non-whitespace characters.
   Whitespace is defined by the isspace() function. Return 0. */

int main(void)
{
    loop1:
        iChar = getchar();

        if (iChar == EOF) goto endloop1;

        lCharCount++;

        if (!isspace(iChar)) goto else1;
        if (!iInWord) goto endif1;
        lWordCount++;
        iInWord = FALSE;
        goto endif1;

    endloop1:
        if (!iInWord) goto endif2;
        lWordCount++;
        goto endif2;

    else1:
        if (iInWord) goto endif1;
        iInWord = TRUE;
        goto endif1;

    endif1:
        if (iChar != '\n') goto loop1;
        lLineCount++;
        goto loop1;

    endif2:
        printf("%7ld %7ld %7ld\n", lLineCount, lWordCount, lCharCount);
        return 0;
}
