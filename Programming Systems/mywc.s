/*--------------------------------------------------------------------*/
/* mywc.c                                                             */
/* Author: Minh-Thi Nguyen and Marti Vives                            */
/*--------------------------------------------------------------------*/
 
        .equ    FALSE, 0
        .equ    TRUE, 1
        .equ    EOF, -1

//----------------------------------------------------------------------

        .section .rodata

printfFormatStr:
        .string "%7ld %7ld %7ld\n"

//----------------------------------------------------------------------

        .section .data

lLineCount:
        .quad   0
lWordCount:
        .quad   0
lCharCount:
        .quad   0
iInWord:
        .word   FALSE

//----------------------------------------------------------------------

        .section .bss

iChar:
        .skip   4

//----------------------------------------------------------------------

        .section .text

        //--------------------------------------------------------------
        // Write to stdout counts of how many lines, words, and 
        // characters are in stdin. A word is a sequence of 
        // non-whitespace characters. Whitespace is defined by the 
        // isspace() function. Return 0.
        //--------------------------------------------------------------

        // Must be a multiple of 16
        .equ    MAIN_STACK_BYTECOUNT, 16

        .global main

main:

        // Prolog
        sub     sp, sp, MAIN_STACK_BYTECOUNT
        str     x30, [sp]

loop1:

        // iChar = getchar();
        bl      getchar         // call getchar
        adr     x1, iChar       // place in x1 the address denoted by iChar              
        str     w0, [x1]        // store 4 bytes from w0 to memory addressed by iChar

        // if (iChar == EOF) goto endloop1;
        adr     x1, iChar       // place in x1 the address denoted by iChar 
        ldr     w0, [x1]        // load 4 bytes of memory addressed in x1 (iChar) to w0      
        cmp     w0, EOF         // assembler shorthand for subs wzr, w0, -1; compares operands and set condition flags
        beq     endloop1        // if equal go to end of loop 

        // lCharCount++
        adr     x0, lCharCount  // place in x0 the address denoted by lCharCount
        ldr     x1, [x0]        // load 8 bytes of memory addressed 
        add     x1, x1, 1       // add 1 to x1
        str     x1, [x0]        // store 8 bytes from x1 to memory addressed by x0

        // if (!isspace(iChar)) goto else1;
        adr     x1, iChar       // place in x1 the address denoted by iChar        
        ldr     w0, [x1]        // load from address held in x1
        bl      isspace         // function call to isspace, returns 0 if not a space
        cmp     w0, 0           // compare function result and 0
        beq     else1           // if is not a space branch to else1

        // if (!iInWord) goto endif1
        adr     x1, iInWord
        ldr     w0, [x1]
        cmp     w0, TRUE
        bne     endif1

        // lWordCount++
        adr     x0, lWordCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

        // iInWord = FALSE
        mov     w0, FALSE
        adr     x1, iInWord
        str     w0, [x1]

        // goto endif1
        b       endif1


endloop1:
        // if (!iInWord) goto endif2
        adr     x1, iInWord
        ldr     w0, [x1]
        cmp     w0, TRUE
        bne     endif2

        // lWordCount++;
        adr     x0, lWordCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

        // goto endif2;
        b       endif2


else1:

        // if (iInWord) goto endif1
        adr     x1, iInWord
        ldr     w0, [x1]
        cmp     w0, TRUE
        beq     endif1

        // iInWord = TRUE
        mov     w0, TRUE
        adr     x1, iInWord
        str     w0, [x1]

        // goto endif1
        b       endif1

endif1:

        // if (iChar != '\n') goto loop1;
        adr     x1, iChar
        ldr     w0, [x1]
        cmp     w0, '\n'
        bne     loop1

        // lLineCount++
        adr     x0, lLineCount
        ldr     x1, [x0]
        add     x1, x1, 1
        str     x1, [x0]

        // goto loop1
        b       loop1

endif2:

        // printf("%7ld %7ld %7ld\n", lLineCount, lWordCount, lCharCount);
        adr     x0, printfFormatStr
        adr     x1, lLineCount
        ldr     x1, [x1]
        adr     x2, lWordCount
        ldr     x2, [x2]
        adr     x3, lCharCount
        ldr     x3, [x3]
        bl      printf

        // Epilog and return 0
        mov     x0, 0 
        ldr     x30, [sp]   
        add     sp, sp, MAIN_STACK_BYTECOUNT               
        ret 

        .size   main, (. - main)






        