/*--------------------------------------------------------------------*/
/* testminiassembler.c                                                */
/* Author: Bob Dondero                                                */
/*--------------------------------------------------------------------*/

#include "miniassembler.h"
#include <stdio.h>

/*--------------------------------------------------------------------*/

/* Test the MiniAssembler module by calling its functions with a 
   variety of arguments and writing the returned values to stdout.
   Return 0. */
   
int main(void)
{
   int i1;
   int i2;
   int i3;
   
   unsigned int auiRegisters[] = {0, 1, 15, 29, 30};
   unsigned int auiAddresses[] = {0x00000000, 0x00000001, 0x001ffffe,
      0x001fffff, 0x01234567, 0x76543210}; 
   unsigned int auiImmeds[] = {0, 1, 32767, 65534, 65535};

   int iRegisterCount =(int)(sizeof(auiRegisters)/sizeof(unsigned int));
   int iAddressCount = (int)(sizeof(auiAddresses)/sizeof(unsigned int));
   int iImmedCount = (int)(sizeof(auiImmeds)/sizeof(unsigned int));
      
   /* Test MiniAssembler_mov. */
   
   for (i1 = 0; i1 < iRegisterCount; i1++)
      for (i2 = 0; i2 < iImmedCount; i2++)
      {
         unsigned int uiRegister = auiRegisters[i1];
         unsigned int uiImmed = auiImmeds[i2];
         printf("MiniAssembler_mov(%u, %u): 0x%08x\n",
            uiRegister, uiImmed,
            MiniAssembler_mov(uiRegister, uiImmed));         
      }
      
   /* Test MiniAssembler_adr. */
   
   for (i1 = 0; i1 < iRegisterCount; i1++)
      for (i2 = 0; i2 < iAddressCount; i2++)
         for (i3 = 0; i3 < iAddressCount; i3++)
         {
            unsigned int uiRegister = auiRegisters[i1];
            unsigned int uiAddress1 = auiAddresses[i2];
            unsigned int uiAddress2 = auiAddresses[i3];
            printf("MiniAssembler_adr(%u, 0x%08x, 0x%08x): 0x%08x\n",
               uiRegister, uiAddress1, uiAddress2,
               MiniAssembler_adr(uiRegister, uiAddress1, uiAddress2));
         }
         
   /* Test MiniAssembler_strb. */
   
   for (i1 = 0; i1 < iRegisterCount; i1++)
      for (i2 = 0; i2 < iRegisterCount; i2++)
      {
         unsigned int uiRegister1 = auiRegisters[i1];
         unsigned int uiRegister2 = auiRegisters[i2];
         printf("MiniAssembler_strb(%u, %u): 0x%08x\n",
            uiRegister1, uiRegister2,
            MiniAssembler_strb(uiRegister1, uiRegister2));
      }
         
   /* Test MiniAssembler_b. */

   for (i1 = 0; i1 < iAddressCount; i1++)
      for (i2 = 0; i2 < iAddressCount; i2++)
      {
         unsigned int uiAddress1 = auiAddresses[i1];
         unsigned int uiAddress2 = auiAddresses[i2];
         printf("MiniAssembler_b(0x%08x, 0x%08x): 0x%08x\n",
            uiAddress1, uiAddress2,
            MiniAssembler_b(uiAddress1, uiAddress2));
      }
   
   return 0; 
}
