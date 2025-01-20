/*
 *
 * MIT License
 * Copyright (c) 2025 ARM Limited.
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

#include <stdio.h>
#include <arm_cmse.h>
#include "interface.h"
#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif


/* typedef for Non-secure callback function */
typedef funcptr funcptr_NS __attribute__((cmse_nonsecure_call));

/**
  \brief        Trigger IRQs according different cases
  \details      Case1: trigger both IRQ0 and IRQ1, check that secure IRQ1 handler
                is triggered first and tail-chained by non-secure IRQ0 handler.
                Case2: trigger both IRQ2 and IRQ3, check that non-secure IRQ2 handler
                is triggered first and tail-chained by secure IRQ3 handler.
                Case3: check that secure fault is preempted from non-secure IRQ4
                even though secure fault priority is lower that IRQ4.
 */
void __attribute__((cmse_nonsecure_entry)) ns_callable_pend_IRQs(){

  printf("\n%s: Case1: start !     \n", get_state);
  NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                   1 << (uint32_t)Interrupt1_IRQn;

  printf("\n%s: Case2: start !     \n", get_state);
  NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt2_IRQn|
                   1 << (uint32_t)Interrupt3_IRQn;

  printf("\n%s: Case3: start !     \n", get_state);
  NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt4_IRQn;
}


/**
  \brief        Print the active and pending status of interrupts
  \details      Get the status of IRQs by reading ICSR register.
 */
void __attribute__((cmse_nonsecure_entry)) Print_PendAndActiveStatus(void){
  /* Read status of IRQs with exception number from ICSR register. */
  uint32_t ExcepNumPend    = (SCB->ICSR & SCB_ICSR_VECTPENDING_Msk)
                              >> SCB_ICSR_VECTPENDING_Pos;
  uint32_t NumsActive      = (SCB->ICSR & SCB_ICSR_RETTOBASE_Msk)
                              >> SCB_ICSR_RETTOBASE_Pos;
  uint32_t ExcepNumActive  = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;

  /* Check if there is any pending IRQ
   * If so, print the exception number of pending IRQ with highest priority */
  if(ExcepNumPend > 0)
    printf("%s: The number of the highest priority pending exception is %d \n",
          get_state, ExcepNumPend);

  /* Check if there is any active IRQ
   * If so, decide how many IRQs are active and
   * print the exception number of active IRQ with highest priority */
  if(ExcepNumActive > 0){
    if(NumsActive == 0)
      printf("%s: There is more than one active exception. \n", get_state);
    else
      printf("%s: There is only one active exception. \n", get_state);

    printf("%s: The number of the highest priority active exception is %d \n", get_state,
          ExcepNumActive);
  }
}
