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
#include <assert.h>


/**
  \brief        Overwrite SysTick handler
  \details      print the exception status and pend PendSV.
                PendSV doesn't preempt the SysTick since PendSV has lower exception number.
 */
extern void SysTick_Handler(void){
  printf("We are in SysTick_Handler! \n");
  printf("The pending and active status are \n        "
         "SCB->ICSR = 0x%08x\n", SCB->ICSR);

  /* Set the pending bit of PendSV and it will generate PendSV exception */
  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
  __DSB();
  __ISB();

  printf("The pending and active status are \n        "
         "SCB->ICSR = 0x%08x\n", SCB->ICSR);
  printf("We are in SysTick_Handler end! \n");


  /* Disable the SysTick */
  SysTick->CTRL = 0UL;
}


/**
  \brief        Overwrite PendSV handler
  \details      print the exception status.
 */
extern void PendSV_Handler(void){
  printf("We are in PendSV_Handler! \n");
  printf("The pending and active status are \n        "
         "SCB->ICSR = 0x%08x\n", SCB->ICSR);
  printf("Example Project: system-exceptions End \n");
}


int main(){
  /* ===========================================
   * This example demonstrates how to generate built-in system exceptions 
   * such as PendSV and SysTick that are commonly used in OS environment.
   * =========================================== */

  printf("Example Project: system-exceptions Start \n");

  /* Step1: Set priority of PendSV and SysTick */
  NVIC_SetPriority(PendSV_IRQn,  0xFF);
  NVIC_SetPriority(SysTick_IRQn, 0x11);

  /* Step2: The RVR is 24bit counter with setting maximum value */
  SysTick->LOAD = 0x00FFFFFF;

  /* Step3: Generate a SysTick exception after counting 24bit maximum value */
  /* Load the SysTick counter value */
  SysTick->VAL  = 0UL;

  /* Enable SysTick Exception and SysTick Timer */
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_TICKINT_Msk |
                  SysTick_CTRL_ENABLE_Msk;

  __DSB();
  __ISB();

  while(1);
}
