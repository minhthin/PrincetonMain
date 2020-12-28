/*--------------------------------------------------------------------*/
/* miniassembler.c                                                    */
/* Author: Minh-Thi Nguyen and Marti Vives                            */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_mov(unsigned int uiReg,
   unsigned int uiImmed)
   {
      unsigned int uiInstr; 
      unsigned int uiImmed16; 
      unsigned int opc; 
      unsigned int shift;
      unsigned int hw; 

      /* Construct machine instruction */
      uiInstr = 0x10000000;

      /* Add Rd */
      uiInstr |= uiReg;

      /* Add imm16 */
      uiImmed16 = uiImmed & 0x0000ffff;
      uiImmed16 = uiImmed16 << 5; 
      uiInstr |= uiImmed16; 

      /* Add opc */
      /* sf = 0 */
      opc = 6;
      opc = opc << 29; 
      uiInstr |= opc;

      /* Add shift */
      shift = 0x25;
      shift = shift << 23; 
      uiInstr |= shift; 

      /* Add hw */
      hw = uiImmed >> 16; 
      hw = hw << 21; 
      uiInstr |= hw; 

      return uiInstr;
   }

/*--------------------------------------------------------------------*/


unsigned int MiniAssembler_adr(unsigned int uiReg, unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
{
   unsigned int uiInstr; 
   unsigned int uiDisp; 
   unsigned int uiDispLo; 
   unsigned int uiDispHi; 
         
   /* Construct machine instruction */
   uiInstr = 0x10000000;  
   
   /* Add Rd */
   uiInstr |= uiReg; 

   /* Compute imm value */ 
   uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

   /* Add immlo */ 
   uiDispLo = uiDisp & 0x3; 
   uiDispLo = uiDispLo << 29; 
   uiInstr |= uiDispLo; 
   
   /* Add immhi*/ 
   uiDispHi = uiDisp >> 2; 
   uiDispHi = uiDispHi << 5; 
   uiDispHi &= 0x00ffffe0; 
   uiInstr |= uiDispHi; 
   
   return uiInstr;

}

unsigned int MiniAssembler_strb(unsigned int uiFromReg,
   unsigned int uiToReg)
   {
      unsigned int uiInstr; 
      unsigned int beg; 
      
      /* Construct machine instruction */
      uiInstr = 0x10000000;  

      /* Add Rt */
      uiInstr |= uiFromReg;

      /* Add Rn */
      uiToReg = uiToReg << 5; 
      uiInstr |= uiToReg;

      /* Add size, opc, imm12 = 0 (default) */
      beg = 0x39;
      beg = beg << 24;
      uiInstr |= beg;

      return uiInstr;
   }

/*--------------------------------------------------------------------*/

unsigned int MiniAssembler_b(unsigned long ulAddr,
   unsigned long ulAddrOfThisInstr)
   {
      unsigned int uiInstr; 
      unsigned int uiDisp; 
      unsigned int uiDisp26; 
      unsigned int beg;
            
      /* Construct machine instruction */
      uiInstr = 0x10000000; 

      /* Compute imm value */ 
      uiDisp = (unsigned int)(ulAddr - ulAddrOfThisInstr);

      /* Add imm26: find imm28, right shift by 2 */ 
      uiDisp26 = uiDisp & 0x0fffffff;
      uiDisp26 = uiDisp26 >> 2;
      uiInstr |= uiDisp26; 

      /* Add op*/ 
      beg = 5;
      beg = beg << 26; 
      uiInstr |= beg;

      return uiInstr;
   }
