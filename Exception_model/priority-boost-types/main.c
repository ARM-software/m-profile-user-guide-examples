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
#include "IRQSet.h"


int main(){
  /* ===========================================
   * This example demonstrates BASEPRI functionality
   * * =========================================== */

  printf("Example Project: priority-boost-types Start \n");
  Vector_Table_Relocation();

  /* Step1: Set priorities for IRQs and enable IRQs */
  NVIC->ISER[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                   1 << (uint32_t)Interrupt1_IRQn|
                   1 << (uint32_t)Interrupt2_IRQn;

  NVIC_SetPriority(Interrupt0_IRQn, 0xFF);
  NVIC_SetPriority(Interrupt1_IRQn, 0x33);
  NVIC_SetPriority(Interrupt2_IRQn, 0x22);

  /* Pend IRQ2 at IRQ1 handler */
  IRQPendRequests[InIRQ1Handler] = 1 << (uint32_t)Interrupt2_IRQn;

  /* Step2: Set BASEPRI
   * As we want to mask IRQ0, the priority value is 128. */
  __set_BASEPRI(0x80);
  __DSB();

  /* Step3: Trigger IRQ0 and IRQ1
   * print the status of IRQs */
  printf("BASEPRI is set with triggering IRQ0 and IRQ1! \n");
  NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                   1 << (uint32_t)Interrupt1_IRQn;
  __DSB();
  __ISB();
  Print_PendAndActiveStatus();

  /* Step4: Clear BASEPRI */
  printf("BASEPRI is clear! \n");
  __set_BASEPRI(0);
  __DSB();

  printf("Example Project: priority-boost-types End \n");
}
