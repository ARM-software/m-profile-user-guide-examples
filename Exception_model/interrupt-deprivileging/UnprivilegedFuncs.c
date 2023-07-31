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

/* Definition of variables used in deprivileged mode */
uint32_t deprivThreadStack[PS_STACK_SIZE] __attribute__((aligned (8)));
uint32_t *deprivThreadStackPtr = &deprivThreadStack[PS_STACK_SIZE];


/**
  \brief        Function to trigger deprivileging request
  \details      Execute SVC #1 to trigger deprivileging request
  \Note         depriv_return function will be executed in deprivileged mode
 */
__attribute__((naked)) void depriv_return(){
  __asm volatile(
    "SVC     #1       \n"
  );
}


/**
  \brief        Deprivileged service
  \details      Call depriv_return function in deprivileged thread mode.
 */
void depriv_service(void){
  printf("we are in deprivileging thread mode !\n");

  depriv_return();
}

