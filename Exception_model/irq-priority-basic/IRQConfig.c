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
#include <string.h>


uint32_t new_vectorTable[VECTORTABLE_SIZE] __attribute__((aligned (VECTORTABLE_ALIGNMENT)));
uint32_t IRQPendRequests[IRQ_PENDNum];


void Print_PendIRQ(uint32_t PendNum){
/* Extract the Pending bit that is set in ISPR */
  uint32_t count = 0;

  while(PendNum){
    if ((PendNum & 0x1))
       printf("Setting IRQ %d to pend \n", count);

    PendNum = PendNum >> 1;
    count++;
  }
}


void Print_PendAndActiveStatus(){
/* Read status from ICSR register. */
  uint32_t ExcepNumPend    = (SCB->ICSR & SCB_ICSR_VECTPENDING_Msk)
                              >> SCB_ICSR_VECTPENDING_Pos;
  uint32_t NumsActive      = (SCB->ICSR & SCB_ICSR_RETTOBASE_Msk)
                              >> SCB_ICSR_RETTOBASE_Pos;
  uint32_t ExcepNumActive  = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;


  if(ExcepNumPend > 0)
    printf("The excep num of the highest priority pending excep is %d \n",
          ExcepNumPend);

  if(ExcepNumActive > 0){
    if(NumsActive == 0)
      printf("There is more than one active exception. \n");
    else
      printf("There is only one active exception. \n");

    printf("The excep num of the highest priority active excep is %d \n",
          ExcepNumActive);
  }
}


void Interrupt_Handler(void) {
  /* Get the current active exception number */
  uint32_t IRQNum = __get_IPSR() - IRQ_offset;
  uint32_t IRQsToPend = IRQPendRequests[IRQNum];

  printf("We are in IRQ %d Handler!\n", IRQNum);
  Print_PendIRQ(IRQsToPend);

  /* Force the new pended interrupts to be recognized */
  NVIC->ISPR[0] = IRQsToPend;

  __DSB();
  __ISB();

  Print_PendAndActiveStatus();
}


void Vector_Table_Relocation(void){
  /* Copy the original handler address into new vector table */
  memcpy(new_vectorTable, __VECTOR_TABLE, sizeof(uint32_t));

  /* Replace the element with new handler address */
  new_vectorTable[Interrupt0_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;
  new_vectorTable[Interrupt1_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;
  new_vectorTable[Interrupt2_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;

  /* Change the VTOR */
  printf("Vector table address is 0x%08x\n", SCB->VTOR);
  printf("The IRQ0's vector address is 0x%08x\n",
        (uint32_t)&__VECTOR_TABLE[Interrupt0_IRQn+IRQ_offset]);

  /* Change the VTOR into new vector table address */
  SCB->VTOR = (uint32_t)&new_vectorTable;
  __DSB();
  __ISB();

  printf("New vector table address is 0x%08x\n", SCB->VTOR );
  printf("The new IRQ0's vector address is 0x%08x\n",
        (uint32_t)&new_vectorTable[Interrupt0_IRQn+IRQ_offset]);
}


uint32_t EncodePrio_GROUP(uint32_t PrioGroup, uint32_t PreemPrio, uint32_t SubPrio){
  uint32_t PriorityGroupTmp = (PrioGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(PriorityBits)) ?
          (uint32_t)(PriorityBits) : (uint32_t)(7UL - PriorityGroupTmp);

  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(PriorityBits)) < (uint32_t)7UL) ?
          (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(PriorityBits));

  return (((PreemPrio & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
          ((SubPrio   & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL))));
}


void Set_Pri_IRQn(IRQn_Type IRQn, uint32_t PrioGroup, uint32_t PreemPrio,
                uint32_t SubPrio){

  uint32_t priorityGroup;

  /* Set priority group to 0b011 */
  NVIC_SetPriorityGrouping(PrioGroup);

  /* Variables to store priority group */
  priorityGroup =  NVIC_GetPriorityGrouping();

  /* Encode priority with subpriority number and group priority number */
  /* Note: priority depends on the used priority grouping */
  uint32_t priority;
  priority = EncodePrio_GROUP(priorityGroup, PreemPrio, SubPrio);
  SetPrio(IRQn, priority);
}


void Clr_IRQs(){
/* Clear the pending request array and clearing pending bit */
  for(int i = 0;i < IRQ_PENDNum;i++)
    IRQPendRequests[i] = 0;

  NVIC->ICPR[0] = 0xFFFFFFFF;
}
