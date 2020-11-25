/*--------------------------------------------------------------------*/
/* bigintadd.c                                                        */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "bigint.h"
#include "bigintprivate.h"
#include <string.h>
#include <assert.h>

/* In lieu of a boolean data type. */
enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

/* Return the larger of lLength1 and lLength2. */

static long BigInt_larger(long lLength1, long lLength2)
{
   long lLarger;

   if(lLength1 <=lLength2) goto elsecondition;
   lLarger = lLength1;
   goto endif;

   elsecondition:
        lLarger = lLength2;
        goto endif;

    endif:
        return lLarger;
}

/*--------------------------------------------------------------------*/

/* Assign the sum of oAddend1 and oAddend2 to oSum.  oSum should be
   distinct from oAddend1 and oAddend2.  Return 0 (FALSE) if an
   overflow occurred, and 1 (TRUE) otherwise. */

int BigInt_add(BigInt_T oAddend1, BigInt_T oAddend2, BigInt_T oSum)
{
    // use adcs instead of ulCarry 
   unsigned long ulCarry;
   unsigned long ulSum;
   long lIndex;
   long lSumLength;

   /* Determine the larger length. */
   lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength);

   /* Clear oSum's array if necessary. */
   if (oSum->lLength <= lSumLength) goto endif1;
   memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long));
   goto endif1;

endif1:

    /* Perform the addition */
    ulCarry = 0;
    lIndex = 0; 
    goto forloop;

forloop:
    if (lIndex >= lSumLength) goto forloopend;
    ulSum = ulCarry;
    ulCarry = 0;

    ulSum += oAddend1->aulDigits[lIndex];
    if (ulSum >= oAddend1->aulDigits[lIndex]) goto endif2;
    ulCarry = 1;
    goto endif2;

endif2:
    ulSum += oAddend2->aulDigits[lIndex];
    if(ulSum >= oAddend2->aulDigits[lIndex]) goto endif3;
    ulCarry = 1;
    goto endif3;

endif3:
    oSum->aulDigits[lIndex] = ulSum;
    lIndex++;
    goto forloop;


forloopend:
    if (ulCarry != 1) goto endif4;
    if (lSumLength != MAX_DIGITS) goto endif5;
    return FALSE;
    goto endif5;

endif5:
    oSum->aulDigits[lSumLength] = 1;
    lSumLength++;
    goto endif4;

endif4:
    oSum->lLength = lSumLength;
    return TRUE;
}
