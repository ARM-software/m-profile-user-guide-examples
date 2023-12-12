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

#include "funcs.h"
#include <stdio.h>

// Flag of UsageFault
uint32_t flag = 0;

/**
  \brief        Overwrite UsageFault handler
  \details      Enable FPU and re-execute the FPU operations again
 */
extern void UsageFault_Handler(){
  /* Power the FPU in both privileged and user modes by clearing bits 20 of CPPWR
   * when the NOCP is UsageFault
   */
  __asm volatile(
    "LDR r2, =0xE000E00C    \n" // Load Coprocessor Power Control Register
    "MOV R3, #0x00000000    \n"
    "STR R3, [R2]           \n"
    "DSB                    \n"
    "ISB                    \n"
    "ADD R0, R0, #1         \n" // Increase flag if UsageFault has happened
    "BX  LR                 \n"
  );
}


void print_status(){
  printf("UsageFault entered \n");
}

int main(){

  /* ===========================================
   * This example triggers a UsageFault by executing 
   * a floating-point instruction while the FPU is disabled. 
   * This example also show how UsageFault exception handler 
   * can be used to fix the fault.
   * =========================================== */
  printf("Example Project: synchronous-fault Start \n");

  volatile float result;

  /* Step1: Enable UsageFault */
  SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk ;

  /* Step2: Make sure that power off FPU functionality */
  SCnSCB->CPPWR |= ((0x1 << 10*2) |
                    (0x1 << 11*2));

  __DSB();
  __ISB();


  /* Step3: Execute FP instructions
   * It will trigger the UsageFault when powering off the FPU.
   * After returning from UsageFault handler, the program will re-execute
   * these FP instructions again successfully.
   */
  result = Call_FPU(flag);

  printf("The floating result is %f \n", result);

  printf("Example Project: synchronous-fault End \n");
}
