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


#include "SelectSVCNumber.h"
#include <stdio.h>

int main(){
  /* ===========================================
   * The example demonstrates how to select different functions 
   * by using a SVC <num> as a parameter
   * =========================================== */

  printf("Example Project: svc-number-as-parameter Start \n");

  /* Step1: Call different SVC subroutine */
  __asm(
    "MOV R0,  #4      \n"
    "MOV R1,  #8      \n"
    "SVC      #1      \n"
  );
  printf("The first routine is completed !\n");


  __asm(
    "MOV R0,  #6      \n"
    "MOV R1,  #6      \n"
    "SVC      #2      \n"
  );
  printf("The second routine is completed !\n");


  __asm(
    "MOV R0,  #83     \n"
    "MOV R1,  #7      \n"
    "SVC      #3      \n"
  );
  printf("The third routine is completed !\n");


  printf("Example Project: svc-number-as-parameter End \n");
}
