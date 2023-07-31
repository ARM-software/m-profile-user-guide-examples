/*
 *
 * MIT License
 * Copyright (c) 2023 ARM Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#if defined (ARMv8MML)
  #include "ARMv8MML.h"
#elif defined (ARMv8MML_DSP)
  #include "ARMv8MML_DSP.h"
#elif defined (ARMv8MML_SP)
  #include "ARMv8MML_SP.h"
#elif defined (ARMv8MML_DSP_SP)
  #include "ARMv8MML_DSP_SP.h"
#elif defined (ARMv8MML_DP)
  #include "ARMv8MML_DP.h"
#elif defined (ARMv8MML_DSP_DP)
  #include "ARMv8MML_DSP_DP.h"
#else
  #error device not specified!
#endif


#include <stdio.h>
#include "SelectSVCNumber.h"


/**
  \brief        Specific exception handling
  \details      Get the SVC number and call the sub routines to get the result of
                arithmetic operation.
  \param [in]   svc_StackFrame    stack saved by hardware automatically.
  \param [in]   EXC_return_value  state to return after finishing exception handling.
 */
void SVC_Handler_Main(uint32_t *svc_StackFrame, uint32_t EXC_return_value)
{
  /* First argument (r0) is svc_StackFrame
   * Stack contains: r0, r1, r2, r3, r12, LR, PC and xPSR
   * Second argument (r1) is EXC_return_value */
  uint32_t svc_number;
  uint32_t res;
  uint32_t argValue0 = svc_StackFrame[STK_FRAME_R0];
  uint32_t argValue1 = svc_StackFrame[STK_FRAME_R1];


  printf("The return address is 0x%08x \n",
          svc_StackFrame[STK_FRAME_RET_ADDR]);
  printf("The stacked return state is 0x%08x \n",
          EXC_return_value);

  /* Extract lower byte of the SVC opcode to get SVC number */
  svc_number = *((uint16_t *)svc_StackFrame[STK_FRAME_RET_ADDR] - 1) & 0xFF;
  printf("svc_number is %d \n", svc_number);

  /* Switch the number and execute subroutine based on SVC_number */
  switch(svc_number)
  {
     case 1:
       res = argValue0 + argValue1;
       printf("The result of R0+R1 is %d!\n", res);
       break;
     case 2:
       res = argValue0 * argValue1;
       printf("The result of R0*R1 is %d!\n", res);
       break;
     case 3:
       res = argValue0 % argValue1;
       printf("The result of R0 mod R1 is %d!\n", res);
       break;
     case 0xF:
       res = argValue0 - argValue1;
       printf("The output of SVC_0xF is %d!\n", res);
       break;
     default:
       printf("Unknown SVC \n");
       break;
  }
}

/**
  \brief        Entry point of SVC handler
  \details      Get the stacked value of registers and pass it on as 
                parameters to SVCHandlerMain.
 */

extern void SVC_Handler(void){
  __asm(
    ".global SVC_Handler_Main        \n"
    "TST      LR, #0x4               \n"  /* Called from Process stack pointer? */
    "ITE      EQ                     \n"
    "MRSEQ    R0, MSP                \n"  /* Using MSP,Z==1 and record the MSP */
    "MRSNE    R0, PSP                \n"  /* Using PSP,Z==0 and record the PSP */
    "TST      LR, #0x20              \n"  /* Enable the default caller register stacking? */
    "IT       EQ                     \n"
    "ADDEQ    R0, R0, #0x40          \n"  /* Not enable, reserve the space to store the caller register */
    "MOV      R1, LR                 \n"  /* Save the exc_return into R1 */
    "B        SVC_Handler_Main       \n"
  );
}
