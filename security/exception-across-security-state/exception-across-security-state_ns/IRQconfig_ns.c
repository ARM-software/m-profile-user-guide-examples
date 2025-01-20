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
#include <stdint.h>
#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif
#include "interface.h"


/**
  \brief        Initialization of non-secure IRQs
  \details      Priority setting and enabling for non-secure IRQs.
                Set the priority threshold of non-secure interrupt.
 */
void NS_Exception_Config(){
  /* Set priority to exceptions  */
  NVIC_SetPriority(Interrupt0_IRQn, 0x12);
  NVIC_SetPriority(Interrupt2_IRQn, 0x40);
  NVIC_SetPriority(Interrupt4_IRQn, 0x63);

  __set_BASEPRI(0x80);

  /* Enable secure exceptions */
  NVIC->ISER[0] |= 1 << (uint32_t)Interrupt0_IRQn|
                   1 << (uint32_t)Interrupt2_IRQn|
                   1 << (uint32_t)Interrupt4_IRQn;
}


/**
  \brief        Read secure data at non-secure state
  \details      Try to read the secure vector table at non-secure world
 */
void read_secure_data(){
  unsigned int *VectorTable_S = ((uint32_t *)0x10000000);
  unsigned int Initial_SP = VectorTable_S[0];
  printf("S: Initial SP of secure vector table is 0x%08x \n", Initial_SP);
}


/**
  \brief        Non-secure IRQ0 handling
  \details      Call the Print_PendAndActiveStatus() to get the active
                and pending status of IRQs
 */
extern void Interrupt0_Handler(void) {
  Print_PendAndActiveStatus();
}


/**
  \brief        Non-secure IRQ2 handling
  \details      Call the Print_PendAndActiveStatus() to get the active
                and pending status of IRQs
 */
extern void Interrupt2_Handler(void) {
  Print_PendAndActiveStatus();
}


/**
  \brief        Non-secure IRQ4 handling
  \details      Try to access the secure data at non-secure state, which will trigger
                secure fault.
                Call the Print_PendAndActiveStatus() to get the active
                and pending status of IRQs
 */
extern void Interrupt4_Handler(void) {
  printf("We are accessing secure memory \n");
  read_secure_data();
}
