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
        
        // Local variable stack offsets:
        .equ    LLARGER,    8   
 
        // Parameter stack offsets:
        .equ    LLENGTHONE, 16
        .equ    LLENGTHTWO, 24


BigInt_larger:

        // Prolog 
        sub     sp, sp, BIGINT_STACK_BYTECOUNT
        str     x30, [sp]
        str     x0, [sp, LLENGTHONE]
        str     x1, [sp, LLENGTHTWO]

        // long lLarger

        // if(lLength1 <= lLength2) goto elsecondition
        ldr     x0, [sp, LLENGTHONE]
        ldr     x1, [sp, LLENGTHTWO]
        cmp     x0, x1
        ble     elsecondition

        // lLarger = lLength1
        ldr     x0, [sp, LLENGTHONE]
        str     x0, [sp, LLARGER]

        // goto endif
        b       endif

elsecondition:

        // lLarger = lLength2
        ldr     x0, [sp, LLENGTHTWO]
        str     x0, [sp, LLARGER]
        b       endif

endif:

        // return lLarger
        ldr     x0, [sp, LLARGER]
        ldr     x30, [sp]
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
        
        // Local variable stack offsets:
        .equ    ULCARRY,    8   
        .equ    ULSUM,      16 
        .equ    LINDEX,     24
        .equ    LSUMLENGTH, 32
 
        // Parameter stack offsets:
        .equ    oADDENDONE, 40
        .equ    oADDENDTWO, 48
        .equ    oSUM,       56 
        
        // structure field offsets 
        // structure field offsets 
        .equ    SIZE_ULONG_POW, 3
        .equ    LLENGTH,    0
        .equ    AULDIGITS,  8 
        .equ    MAX_DIGITS, 32768  
        .global BigInt_add

BigInt_add:

        // Prolog
        sub     sp, sp, BIGINTADD_STACK_BYTECOUNT
        str     x30, [sp]
        str     x0, [sp, oADDENDONE]
        str     x1, [sp, oADDENDTWO]
        str     x2, [sp, oSUM]


        // unsigned long ulCarry;
        // unsigned long ulSum;
        // long lIndex;
        // long lSumLength;

        // lSumLength = BigInt_larger(oAddend1->lLength, oAddend2->lLength)
        ldr     x0, [sp, oADDENDONE]    // load lLength of oAddend1 in x0
        // addendone is only a pointer, so we need to get the actual value
        ldr     x0, [x0, LLENGTH]
        ldr     x1, [sp, oADDENDTWO]    // load lLength of oAddend1 in x1
        ldr     x1, [x1, LLENGTH]
        bl      BigInt_larger           // call BigInt_larger
        str     x0, [sp, LSUMLENGTH]    // store function return to LSUMLENGTH 
        
        // if (oSum->lLength <= lSumLength) goto endif1;
        ldr     x0, [sp, oSUM]          // load lLength of oSUM in x0
        ldr     x1, [sp, LSUMLENGTH]    // load lLength of LSUMLENGTH in x1
        cmp     x0, x1                  // compare x0 and x1
        ble     endif1                  // branch to endif1 if x0<=x1

        // memset(oSum->aulDigits, 0, MAX_DIGITS * sizeof(unsigned long))
        ldr     x0, [sp, oSUM]
        add     x0, x0, AULDIGITS
        mov     x1, xzr
        mov     x2, MAX_DIGITS
        lsl     x2, x2, SIZE_ULONG_POW
        bl      memset

endif1:

        // ulCarry = 0
        str     xzr, [sp, ULCARRY]

        // lIndex = 0
        str     xzr, [sp,LINDEX]

forloop:

        // if (lIndex >= lSumLength) goto forloopend;
        ldr     x0, [sp,LINDEX]          
        ldr     x1, [sp,LSUMLENGTH]
        cmp     x0, x1              
        bge     forloopend

        // ulSum = ulCarry;
        ldr     x0, [sp, ULCARRY]
        str     x0, [sp, ULSUM]

        // ulCarry = 0
        str     xzr, [sp, ULCARRY]

        // ulSum += oAddend1->aulDigits[lIndex];
        ldr     x0, [sp,oADDENDONE]
        add     x0, x0, AULDIGITS
        ldr     x1, [sp,LINDEX]
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        ldr 	x1, [sp,ULSUM]
        add 	x1, x1, x0
        str 	x1, [sp,ULSUM]

        // if (ulSum >= oAddend1->aulDigits[lIndex]) goto endif2
        ldr     x0, [sp,oADDENDONE]
        add     x0, x0, AULDIGITS
        ldr     x1, [sp,LINDEX]
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        ldr     x1, [sp,ULSUM]
        cmp     x1, x0
        bhs     endif2

        // ulCarry = 1
        mov     x0, 1 // x0 contains 1
        str     x0, [sp, ULCARRY]	  
        
endif2:

        // ulSum += oAddend2->aulDigits[lIndex];
        ldr     x0, [sp,oADDENDTWO]
        add     x0, x0, AULDIGITS
        ldr     x1, [sp,LINDEX]
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        ldr 	x1, [sp,ULSUM]
        add 	x1, x1, x0
        str 	x1, [sp,ULSUM]

        // if(ulSum >= oAddend2->aulDigits[lIndex]) goto endif3;
        ldr     x0, [sp,oADDENDTWO]
        add     x0, x0, AULDIGITS
        ldr     x1, [sp,LINDEX]
        ldr     x0, [x0,x1,lsl SIZE_ULONG_POW]
        ldr     x1, [sp,ULSUM]
        cmp     x1, x0
        bhs     endif3

        // ulCarry = 1
        mov     x0, 1 // x0 contains 1
        str     x0, [sp, ULCARRY]
        
endif3:
        // oSum->aulDigits[lIndex] = ulSum;
        ldr     x0, [sp, ULSUM] // x0 contains ulSUM
        ldr     x1, [sp, oSUM] // x1 contains oSUM (pointer)
        add     x1, x1, AULDIGITS // x1 contains oSum->aulDigits 
        ldr     x2, [sp, LINDEX] // x2 contains lIndex
        str     x0, [x1, x2, lsl SIZE_ULONG_POW] // x1 contains oSum->aulDigits[lIndex]

        // lIndex++: lIndex = lIndex + 1
        ldr     x0, [sp, LINDEX] // x0 contains lIndex
        add     x0, x0, 1 // x0 contains lIndex + 1
        str     x0, [sp, LINDEX]

        // goto forloop;
        b       forloop
        
forloopend:

        // if (ulCarry != 1) goto endif4;
        mov     x0, 1
        ldr     x1, [sp, ULCARRY]
        cmp     x0, x1
        bne     endif4

        // if (lSumLength != MAX_DIGITS) goto endif5;
        ldr     x0, [sp, LSUMLENGTH]
        mov     x1, MAX_DIGITS
        cmp     x0, x1
        bne     endif5

        // epilog and return FALSE;
        mov     x0, FALSE
        ldr     x30, [sp]
        add     sp, sp, BIGINTADD_STACK_BYTECOUNT
        ret
        
        // .size   BigInt_add, (. - BigInt_add)
        
endif5:
        // oSum->aulDigits[lSumLength] = 1;
        mov     x0, 1 // x0 contains ulSUM
        ldr     x1, [sp, oSUM] // x1 contains oSUM
        add     x1, x1, AULDIGITS // x1 contains oSum->aulDigits 
        ldr     x2, [sp, LSUMLENGTH] // x2 contains lIndex
        str     x0, [x1, x2, lsl SIZE_ULONG_POW] // x1 contains oSum->aulDigits[lIndex]

        // lSumLength++;
        ldr     x0, [sp, LSUMLENGTH] // x0 contains lIndex
        add     x0, x0, 1 // x0 contains lIndex + 1
        str     x0, [sp, LSUMLENGTH]  
        
endif4:
        // oSum->lLength = lSumLength;        
        ldr     x0, [sp, LSUMLENGTH] // x0 contains lSumLength
        ldr     x1, [sp, oSUM] // x1 contains oSUM
        str     x0, [x1, LLENGTH] // x1 contains oSum->lLength     

        // return TRUE;
        mov     x0, TRUE
        ldr     x30, [sp]
        add     sp, sp, BIGINTADD_STACK_BYTECOUNT
        ret     
        
        .size   BigInt_add, (. - BigInt_add)
        