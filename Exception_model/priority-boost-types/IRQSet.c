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

#include "IRQSet.h"
#include <stdio.h>
#include <string.h>

/* Definition of global variables - New vector table and IRQ pending request array */
uint32_t new_vectorTable[VECTORTABLE_SIZE] __attribute__((aligned (VECTORTABLE_ALIGNMENT)));
uint32_t IRQPendRequests[IRQ_PENDNum];


/**
  \brief        Print the exception number of pending interrupt
  \details      Read the NVIC_ISPR to get the pending exception number
  \param [in]   PendNum  Values read from IRQPendRequests array.
 */
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

  /* Decide if there is any pending IRQ
   * If so, print the exception number of pending IRQ with highest priority */
  if(ExcepNumPend > 0)
    printf("The number of the highest priority pending exception is %d \n",
          ExcepNumPend);

  /* Decide if there is any active IRQ
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

  /* Change the VTOR into new vector table address */
  SCB->VTOR = (uint32_t)&new_vectorTable;
  __DSB();
  __ISB();

}
