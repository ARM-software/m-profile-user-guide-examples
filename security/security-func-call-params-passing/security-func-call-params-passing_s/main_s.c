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
#include <init.h>

/* address for the NS vector table */
uint32_t *VectorTable_NS = (uint32_t *)0x00200000;

/* typedef for Non-secure callback functions */
typedef void (*funcptr_void) (void) __attribute__((cmse_nonsecure_call));

/**
  \brief        SecureFault handling
  \details      Print SecureFault address & status register
                Write 1 to clear fault status
                Exit program for demo purpose
 */
void SecureFault_Handler()
{
  printf("S: SecureFault occurred \n");
  printf("S: SecureFault address Register: \n  SCB->SFAR = 0x%08x\n", SCB->SFAR );
  printf("S: SecureFault status Register: \n   SCB->SFSR= 0x%08x\n", (0x000000FF)&(SCB->SFSR));
  SCB->SFSR |= 0xFFFFFFFF;
  exit(0);
}

/* Secure main() */
int32_t main(void) {

   /* ===========================================
    * This example demonstrates function calls with parameter passing across a security
    * boundary:
        * Program the SAU, configure system control registers, configure interrupts and
        * exceptions for a security domain, enable SecureFault and writing a basic SecureFault
        * handler
        * Implement Secure function ns_callable_fn1(),ns_callable_fn2(),ns_callable_fn3(),
        * ns_callable_init() with various input parameters
        * Create a CMSE import library to export to the Non-secure image
        * Configure Non-secure registers and call a Non-secure image
        * In Non-secure side, it calls Secure functions
        * In Secure side, it calls Non-secure function as callback function
        * =========================================== */

  funcptr_void NonSecure_ResetHandler;
  printf("Example Project: security-func-call-params-passing Start\n");

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
