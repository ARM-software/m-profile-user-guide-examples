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

/* Definition of global variables - New vector table and IRQ pending request array */
uint32_t new_vectorTable[VECTORTABLE_SIZE] __attribute__((aligned (VECTORTABLE_ALIGNMENT)));
uint32_t IRQPendRequests[IRQ_PENDNum];


/**
  \brief        Print the exception number of pending interrupt
  \details      Read the NVIC_ISPR to get the pending exception number 
  \param [in]   PendNum - Value read from IRQPendRequests array.
 */
void Print_PendIRQ(uint32_t PendNum){
  uint32_t count = 0;

  /* Extract the Pending bit that is set in ISPR */
  while(PendNum){
    if ((PendNum & 0x1))
       printf("Setting IRQ %d to pend \n", count);

    PendNum = PendNum >> 1;
    count++;
  }
}


/**
  \brief        Print the active and pending status of interrupts
  \details      Get the status of IRQs by reading ICSR register.
 */
void Print_PendAndActiveStatus(){
  /* Read status of IRQs with exception number from ICSR register. */
  uint32_t ExcepNumPend    = (SCB->ICSR & SCB_ICSR_VECTPENDING_Msk)
                              >> SCB_ICSR_VECTPENDING_Pos;
  uint32_t NumsActive      = (SCB->ICSR & SCB_ICSR_RETTOBASE_Msk)
                              >> SCB_ICSR_RETTOBASE_Pos;
  uint32_t ExcepNumActive  = SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk;

  /* Check if there is any pending IRQ
   * If so, print the exception number of pending IRQ with highest priority */
  if(ExcepNumPend > 0)
    printf("The number of the highest priority pending exception is %d \n",
          ExcepNumPend);

  /* Check if there is any active IRQ
   * If so, decide how many IRQs are active and
   * print the exception number of active IRQ with highest priority */
  if(ExcepNumActive > 0){
    if(NumsActive == 0)
      printf("There is more than one active exception. \n");
    else
      printf("There is only one active exception. \n");

    printf("The number of the highest priority active exception is %d \n",
          ExcepNumActive);
  }
}


/**
  \brief        Generic interrupt handler
  \details      Unify the operations handled by multiple interrupt handlers
                in a single function
 */
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


/**
  \brief        Relocate vector table
  \details      Create a new vector table at RAM with new elements and change the
                VTOR from origin vector table address to new one.
 */
void Vector_Table_Relocation(void){
  /* Copy the original handler address into new vector table */
  memcpy(new_vectorTable, __VECTOR_TABLE, sizeof(uint32_t));

  /* Replace the element with new handler address */
  new_vectorTable[Interrupt0_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;
  new_vectorTable[Interrupt1_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;
  new_vectorTable[Interrupt2_IRQn + IRQ_offset] = (uint32_t)Interrupt_Handler;

  /* Get information about vector table */
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


/**
  \brief        Encode Priority
  \details      Encodes the priority for an interrupt with the given priority group,
                preemptive priority value, and sub priority value.
  \param [in]   PriorGroup  Used priority group.
  \param [in]   PreemPrio   Preemptive priority value (starting from 0).
  \param [in]   SubPrio     Sub priority value (starting from 0).
  \return       Encoded priority. Value can be used in the function \ref Set_Pri_IRQn().
 */
uint32_t EncodePrio_GROUP(uint32_t PrioGroup, uint32_t PreemPrio, uint32_t SubPrio){
  uint32_t PriorityGroupTmp = (PrioGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  /* Calculate the fields to set group priority */
  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(PriorityBits)) ?
          (uint32_t)(PriorityBits) : (uint32_t)(7UL - PriorityGroupTmp);

  /* Calculate the fields to set sub priority */
  SubPriorityBits     = ((PriorityGroupTmp + (uint32_t)(PriorityBits)) < (uint32_t)7UL) ?
          (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(PriorityBits));

  return (((PreemPrio & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
          ((SubPrio   & (uint32_t)((1UL << (SubPriorityBits    )) - 1UL))));
}


/**
  \brief        Set Priority
  \details      Set the priority for an interrupt with the given priority group,
                preemptive priority value, and sub priority value.
  \param [in]   IRQn        Interrupt to be set priority
  \param [in]   PriorGroup  Used priority group.
  \param [in]   PreemPrio   Preemptive priority value (starting from 0).
  \param [in]   SubPrio     Sub priority value (starting from 0).
 */
void Set_Pri_IRQn(IRQn_Type IRQn, uint32_t PrioGroup, uint32_t PreemPrio,
                uint32_t SubPrio){

  uint32_t priorityGroup;

  /* Set priority group
   * such as PrioGroup=0b011, the highest 4 bits of NVIC_IPR are set group priority
   */
  NVIC_SetPriorityGrouping(PrioGroup);

  /* Variables to store priority group */
  priorityGroup =  NVIC_GetPriorityGrouping();

  /* Encode priority with sub priority number and group priority number */
  /* Note: priority depends on the used priority grouping */
  uint32_t priority;
  priority = EncodePrio_GROUP(priorityGroup, PreemPrio, SubPrio);
  SetPrio(IRQn, priority);
}


/**
  \brief        Clear IRQs
  \details      Clear IRQs by writing to ICPR register.
 */
void Clr_IRQs(){

  for(int i = 0;i < IRQ_PENDNum;i++)
    IRQPendRequests[i] = 0;

  NVIC->ICPR[0] = 0xFFFFFFFF;
}
