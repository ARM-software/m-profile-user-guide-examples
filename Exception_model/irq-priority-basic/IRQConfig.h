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

#ifndef IRQCONFIG_H_
#define IRQCONFIG_H_

/* Parameters for new vector table */
#define VECTORTABLE_SIZE              48
#define VECTORTABLE_ALIGNMENT         0x100U

/* Parameters for pending different interrupts at each IRQ handler */
#define IRQ_PENDNum                   3 /* Number of IRQs to trigger */

#define InIRQ0Handler                 0 /* Index for pending request array */
#define InIRQ1Handler                 1
#define InIRQ2Handler                 2

#define IRQ_offset                   16 /* Exception Number offset for external interrupts */

#define IRQs_preemption               1 /* Definition for each case test */
#define IRQs_tail_chaining            2
#define Set_group_prio                3
#define GroupPrio_test                1
#define SubPrio_test                  2
#define ExcepNum_test                 3
#define MAX_CASES                     3
#define MAX_SUBCASES                  3

/* Parameters for group priority setting */
#define PriorityBits                  8
#define PriGroup                      3 /* Group priority [7:4], sub priority [3:0] */

/* Functions for setting group priority */
uint32_t EncodePrio_GROUP(uint32_t PrioGroup, uint32_t PreemPrio, uint32_t SubPrio);
void Set_Pri_IRQn(IRQn_Type IRQn, uint32_t PrioGroup, uint32_t PreemPrio, uint32_t SubPrio);


/**
  \brief       Set Interrupt Priority
  \details     Sets the priority of an IRQ by using all bits of IPR
  \param [in]  IRQn      Interrupt number.
  \param [in]  priority  Priority to set.
 */
__STATIC_INLINE void SetPrio(IRQn_Type IRQn, uint32_t priority){
  NVIC->IPR[(uint32_t)(IRQn)] = (uint8_t)((priority << (8U - PriorityBits)) & (uint32_t)0xFFUL);
}

/* Declaration of origin vector table and pend request array */
extern uint32_t __VECTOR_TABLE[];
extern uint32_t IRQPendRequests[];


void Print_PendIRQ(uint32_t PendNum);
void Print_PendAndActiveStatus();
void Interrupt_Handler(void);
void Vector_Table_Relocation(void);
void Clr_IRQs();

#endif /* IRQCONFIG_H_ */
