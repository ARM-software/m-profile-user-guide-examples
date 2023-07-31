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

#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif


#include <stdio.h>
#include "excep_prog.h"
#include "mpu_prog.h"


int main(){
  /* ===========================================
   * This example demonstrates the concept of
   * interrupt deprivileging. This concept is 
   * useful for creating sandboxes and is 
   * used to enable services of peripherals 
   * where some of them communicate through interrupts.
   * 
   * For platforms that use Arm7-M based devices,
   * enable CCR.NONBASETHRDENA bit
   * =========================================== */

  printf("Example Project: interrupt-deprivileging Start \n");
  /* Step1: in privileged background, set priority for IRQ, MemManage fault and SVC */
  setMPU();
  NVIC_SetPriority(Interrupt0_IRQn, IRQ0_PRI);
  NVIC_SetPriority(SVCall_IRQn, SVC_PRI);
  NVIC_SetPriority(MemoryManagement_IRQn, MEM_PRI);

  /* Step2: Enable and pend IRQ0. */
  NVIC_EnableIRQ(Interrupt0_IRQn);
  NVIC_SetPendingIRQ(Interrupt0_IRQn);

  /* Step3: print the status of execution mode */
  printf("we are back from IRQ!\n");
  printf("The status is 0x%01x \n", __get_CONTROL());

  printf("Example Project: interrupt-deprivileging End \n");
}
