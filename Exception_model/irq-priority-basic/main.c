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


#include "IRQConfig.h"
#include <stdio.h>



int main(){
  /* ===========================================
   * NOTE:
   * This example includes 3 cases to show the basic priority setting,
   * vector table relocation, IRQs preemption, tail-chaining behavior
   * and effect of group priority and sub priority.
   * =========================================== */

  printf("Example Project: irq-priority-basic Start\n");

  /* Step1: Apply a region for new vector table */
  Vector_Table_Relocation();

  /* Enable 3 IRQs */
  NVIC->ISER[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                   1 << (uint32_t)Interrupt1_IRQn|
                   1 << (uint32_t)Interrupt2_IRQn;
  __DSB();
  __ISB();

  for(uint32_t cases=IRQs_preemption; cases <= MAX_CASES;cases++){
    printf("\n");

    switch (cases){
      case 1:
        /* ==================
         * Case1: IRQs preemption
         * Trigger 3 interrupts in turn and observe the preemption behavior
         * ================== */

        /* Step2: Set priorities for IRQs */
        NVIC_SetPriority(Interrupt0_IRQn, 0xFF);
        NVIC_SetPriority(Interrupt1_IRQn, 0x66);
        NVIC_SetPriority(Interrupt2_IRQn, 0x44);

        /* Pend IRQ1 at IRQ0 handler, pend IRQ2 at IRQ1 handler */
        IRQPendRequests[InIRQ0Handler] = 1 << (uint32_t)Interrupt1_IRQn;
        IRQPendRequests[InIRQ1Handler] = 1 << (uint32_t)Interrupt2_IRQn;

        /* Step3: Trigger IRQ0 firstly */
        NVIC_SetPendingIRQ(Interrupt0_IRQn);
        printf("Case:1 is completed! \n");

        break;

      case 2:
        /* ==================
         * Case2: IRQs tail-chaining
         * Trigger 3 interrupts and observe the tail-chaining behavior
         * ================== */

        /* Step2: Set priorities for IRQs */
        NVIC_SetPriority(Interrupt0_IRQn, 0x44);
        NVIC_SetPriority(Interrupt1_IRQn, 0x66);
        NVIC_SetPriority(Interrupt2_IRQn, 0xFF);

        /* Pend IRQ1,2 at IRQ0 handler */
        IRQPendRequests[InIRQ0Handler] = 1 << (uint32_t)Interrupt1_IRQn|
                                         1 << (uint32_t)Interrupt2_IRQn;

        /* Step3: Pend all IRQs at once */
        NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                         1 << (uint32_t)Interrupt1_IRQn|
                         1 << (uint32_t)Interrupt2_IRQn;
        __DSB();
        __ISB();
        printf("Case:2 is completed! \n");

        break;

      case 3:
        /* ==================
         * Case3: Effect of group priority and sub priority
         * Programmable priority scheme as a 8-bit value, which is split into two
         * fields -Group priority and sub priority.
         * Preemption is based on only group priority. If multiple IRQs with same
         * group priority, the order of handling IRQ depends sub priority.
         * ================== */

        /* Step2: Set priorities for IRQs */
        /* IRQ0: group priority 3, sub priority 0 */
        Set_Pri_IRQn(Interrupt0_IRQn, PriGroup, 3, 0);
        Set_Pri_IRQn(Interrupt1_IRQn, PriGroup, 3, 1);
        Set_Pri_IRQn(Interrupt2_IRQn, PriGroup, 3, 0);

        /* Step3: trigger different IRQs */
        for(uint32_t sub_cases=GroupPrio_test; sub_cases <= MAX_SUBCASES;sub_cases++){
          printf("\n");

          switch (sub_cases){
            case 1:
              IRQPendRequests[InIRQ1Handler] = 1 << (uint32_t)Interrupt0_IRQn;
              NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt1_IRQn;
              __DSB();
              __ISB();
              printf("Group priority test is completed! \n");
              break;

            case 2:
              NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                               1 << (uint32_t)Interrupt1_IRQn;
              __DSB();
              __ISB();
              printf("Sub priority test is completed! \n");
              break;

            case 3:
              NVIC->ISPR[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                               1 << (uint32_t)Interrupt2_IRQn;
              __DSB();
              __ISB();
              printf("Exception number test is completed! \n");
              break;
            }
          Clr_IRQs();
        }
        printf("Case:3 is completed! \n");

        break;
    }
    Clr_IRQs();
  }
  printf("Example Project: irq-priority-basic End \n");
}
