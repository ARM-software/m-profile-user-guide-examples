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
#include "scheduler.h"


int main(){
  /* =====================================================
   * The goal of this example is to show a simple and easy
   * to understand the context switching operations.
   * This goal is accomplished by having two threads A and B, 
   * that switches alternatively between them and uses 
   * Floating-point operations in each thread. 
   * ====================================================== */

  printf("Example Project: context-switch-fp Start \n");
  /* Step 1: As SCB->CPACR has been set at system_ARMv8MML.c
   *         only set FPCCR.CLRONRET bit */
  FPU->FPCCR |= FPU_FPCCR_CLRONRET_Msk;

  /* PendSV is set to the lowest priority level that a context switching done
   * from PendSV will always context switch the background thread, not another handler. */
  NVIC_SetPriority(PendSV_IRQn,  0xFF);
  NVIC_SetPriority(SysTick_IRQn, 0x11);

  /* Step 2: create two tasks to execute the float point operation.*/
  InitTask(task1, ID_TASK1);
  InitTask(task2, ID_TASK2);

  /* Step 3: start switching task.*/
  /* The main program is in idle thread. As we start the scheduler,
   * the SysTick will be generated as a Timer. For the first SysTick,
   * the PendSV will be triggered to switch idle thread into task1. */
  StartScheduler();
}
