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
#include <arm_cmse.h>
#include <stdlib.h>
#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif
#include "init.h"
#include "interface.h"


/* VectorTable_NS: array of non-secure vector table */
uint32_t *VectorTable_NS = (uint32_t *)0x00200000;

/* typedef for non-secure callback functions */
typedef void (*funcptr_void) (void) __attribute__((cmse_nonsecure_call));


/* Secure main() */
int32_t main(void) {

 /* ===========================================
   * This Secure example aims to show exceptions handling between Secure and Non-secure states
   * Set up Secure system configuration
   * Implement Secure function ns_callable_pend_IRQs(), Print_PendAndActiveStatus()
   * Branch from Secure to Non-secure image
   * In Non-secure state, it triggers secure exceptions and non-secure exceptions firstly
   * in different cases
   * =========================================== */

  funcptr_void NonSecure_ResetHandler;
  printf("Example Project: exception-across-security-state Start\n");

  printf("S: Hello World in Secure State \n");

  /* Add user setup code for Secure part here*/
  Secure_System_Init();

  __TZ_set_MSP_NS((uint32_t)(VectorTable_NS[0]));
  SCB_NS->VTOR = (uint32_t)VectorTable_NS;

  /* Get Non-secure reset handler address from Non-secure vector table */
  NonSecure_ResetHandler = (funcptr_void)(VectorTable_NS[1]);

  /* Start Non-secure state software application */
  NonSecure_ResetHandler();

  return 0;
}
