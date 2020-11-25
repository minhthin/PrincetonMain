//----------------------------------------------------------------------
// bigintadd.s
// Author: Minh-Thi Nguyen and Marti Vives
//----------------------------------------------------------------------

        .equ    FALSE, 0
        .equ    TRUE, 1

//----------------------------------------------------------------------

        .section .rodata

//----------------------------------------------------------------------

        .section .data

//----------------------------------------------------------------------

        .section .bss

//----------------------------------------------------------------------

        .section .text

        //--------------------------------------------------------------
        // Return the larger of lLength1 and lLength2
        //--------------------------------------------------------------
        
        // Must be a multiple of 16
        .equ    BIGINT_STACK_BYTECOUNT, 32  
 
        // Registers for parameters 
        lLength1    .req x19
        lLength2    .req x20    

        // Register for local variables 
        lLarger     .req x21


BigInt_larger:

        // Prolog 
        sub     sp, sp, BIGINT_STACK_BYTECOUNT
        str     x30, [sp] // save x30
        str     x19, [sp, 8] // save x19
        str     x20, [sp, 16] // save x20
        str     x21, [sp, 24] // save x21
        mov     lLength1, x0 // store lLength1 in x19
        mov     lLength2, x1 // store lLength2 in x20

        // long lLarger

        // if(lLength1 <= lLength2) goto elsecondition
        mov     x0, lLength1
        mov     x1, lLength2
        cmp     x0, x1
        ble     elsecondition

        // lLarger = lLength1
        mov     x0, lLength1
        mov     lLarger, x0

        // goto endif
        b       endif

elsecondition:

        // lLarger = lLength2
        mov     x0, lLength2
        mov     lLarger, x0
        b       endif

endif:

        // return lLarger
        mov     x0, lLarger
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16] 
        ldr     x21, [sp, 24] 
        add     sp, sp, BIGINT_STACK_BYTECOUNT
        ret

        .size   BigInt_larger, (. - BigInt_larger)

        //--------------------------------------------------------------
        // Assign the sum of oAddend1 and oAddend2 to oSum.  
        // oSum should be distinct from oAddend1 and oAddend2.  
        // Return 0 (FALSE) if an overflow occurred, and 1 
        // (TRUE) otherwise.
        //--------------------------------------------------------------

        // Must be a multiple of 16
        .equ    BIGINTADD_STACK_BYTECOUNT, 64

        // structure field offsets 
        // structure field offsets 
        .equ    SIZE_ULONG_POW, 3
        .equ    LLENGTH,    0
        .equ    AULDIGITS,  8 
        .equ    MAX_DIGITS, 32768  
        .global BigInt_add

        // Registers for parameters
        oAddend1    .req x19
        oAddend2    .req x20
        oSum        .req x21

        // Registers for local variables 
        ulCarry     .req x22
        ulSum       .req x23
        lIndex      .req x24 
        lSumLength  .req x25
        
BigInt_add:

        // Prolog
        sub     sp, sp, BIGINTADD_STACK_BYTECOUNT
        str     x30, [sp]
        str     x19, [sp, 8]
        str     x20, [sp, 16]
        str     x21, [sp, 24]
        str     x22, [sp, 32]
        str     x23, [sp, 40]
        str     x24, [sp, 48]
        str     x25, [sp, 56]
        mov     oAddend1, x0
        mov     oAddend2, x1
        mov     oSum, x2

        // unsigned long ulCarry;
        // unsigned long ulSum;
        // long lIndex;
        // long lSumLength;

        // lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength)
        mov     x0, oAddend1    // load lLength of oAddend1 in x0
        // addendone is only a pointer, so we need to get the actual value
        ldr     x0, [x0, LLENGTH]
        mov     x1, oAddend2    // load lLength of oAddend1 in x1
        ldr     x1, [x1, LLENGTH]
        bl      BigInt_larger           // call BigInt_larger
        mov     lSumLength, x0  
        
        // if (oSum->lLength <= lSumLength) goto endif1;
        mov     x0, oSum        // load lLength of oSUM in x0
        ldr     x0, [x0, LLENGTH]
        mov     x1, lSumLength    // load lLength of LSUMLENGTH in x1
        cmp     x0, x1                  // compare x0 and x1
        ble     endif1                  // branch to endif1 if x0<=x1

        // memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long))
        mov     x0, oSum
        add     x0, x0, AULDIGITS
        mov     x1, xzr
        mov     x2, MAX_DIGITS
        lsl     x2, x2, SIZE_ULONG_POW
        bl      memset

endif1:

        // ulCarry = 0
        mov     ulCarry, xzr

        // lIndex = 0
        mov     lIndex, xzr

forloop:

        // if (lIndex >= lSumLength) goto forloopend;
        mov     x0, lIndex       
        mov     x1, lSumLength
        cmp     x0, x1              
        bge     forloopend

        // ulSum = ulCarry;
        mov     x0, ulCarry
        mov     ulSum, x0

        // ulCarry = 0
        mov     ulCarry, xzr

        // ulSum += oAddend1->aulDigits[lIndex];
        mov     x0, oAddend1
        add     x0, x0, AULDIGITS
        mov     x1, lIndex
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        mov 	x1, ulSum
        add 	x1, x1, x0
        mov 	ulSum, x1

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto endif2
        mov     x0, oAddend1
        add     x0, x0, AULDIGITS
        mov     x1, lIndex
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        mov 	x1, ulSum
        cmp     x1, x0
        bhs     endif2

        // ulCarry = 1
        mov     x0, 1 // x0 contains 1
        mov     ulCarry, x0
        
endif2:

        // ulSum += oAddend2->aulDigits[lIndex];
        mov     x0, oAddend2
        add     x0, x0, AULDIGITS
        mov     x1, lIndex
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        mov 	x1, ulSum
        add 	x1, x1, x0
        mov 	ulSum, x1

        // if(ulSum >= oAddend2->aulDigits[lIndex]) goto endif3;
        mov     x0, oAddend2
        add     x0, x0, AULDIGITS
        mov     x1, lIndex
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        mov 	x1, ulSum
        cmp     x1, x0
        bhs     endif3

        // ulCarry = 1
        mov     x0, 1 // x0 contains 1
        mov     ulCarry, x0
        
endif3:
        // oSum->aulDigits[lIndex] = ulSum;
        mov     x0, ulSum // x0 contains ulSUM
        mov     x1, oSum // x1 contains oSUM (pointer)
        add     x1, x1, AULDIGITS // x1 contains oSum->aulDigits 
        mov     x2, lIndex // x2 contains lIndex
        str     x0, [x1, x2, lsl SIZE_ULONG_POW] // x1 contains oSum->aulDigits[lIndex]

        // lIndex++: lIndex = lIndex + 1
        mov     x0, 1
        add     lIndex, lIndex, x0

        // goto forloop;
        b       forloop
        
forloopend:

        // if (ulCarry != 1) goto endif4;
        mov     x0, 1
        mov     x1, ulCarry
        cmp     x0, x1
        bne     endif4

        // if (lSumLength != MAX_DIGITS) goto endif5;
        mov     x0, lSumLength
        mov     x1, MAX_DIGITS
        cmp     x0, x1
        bne     endif5

        // epilog and return FALSE;
        mov     x0, FALSE
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16]
        ldr     x21, [sp, 24]
        ldr     x22, [sp, 32]
        ldr     x23, [sp, 40]
        ldr     x24, [sp, 48]
        ldr     x25, [sp, 56]
        add     sp, sp, BIGINTADD_STACK_BYTECOUNT
        ret
                
endif5:
        // oSum->aulDigits[lSumLength] = 1;
        mov     x0, 1 // x0 contains ulSUM
        mov     x1, oSum // x1 contains oSUM
        add     x1, x1, AULDIGITS // x1 contains oSum->aulDigits 
        mov     x2, lSumLength // x2 contains lIndex
        str     x0, [x1, x2, lsl SIZE_ULONG_POW] // x1 contains oSum->aulDigits[lIndex]

        // lSumLength++;
        mov     x0, 1
        add     lSumLength, lSumLength, x0 
        
endif4:
        // oSum->lLength = lSumLength;        
        mov     x0, lSumLength // x0 contains lSumLength
        mov     x1, oSum // x1 contains oSUM
        str     x0, [x1, LLENGTH] // x1 contains oSum->lLength     

        // return TRUE;
        mov     x0, TRUE
        ldr     x30, [sp]
        ldr     x19, [sp, 8]
        ldr     x20, [sp, 16]
        ldr     x21, [sp, 24]
        ldr     x22, [sp, 32]
        ldr     x23, [sp, 40]
        ldr     x24, [sp, 48]
        ldr     x25, [sp, 56]
        add     sp, sp, BIGINTADD_STACK_BYTECOUNT
        ret     
        
        .size   BigInt_add, (. - BigInt_add)
        