/*
 *
 * MIT License
 * Copyright (c) 2022-2023 ARM Limited.
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

#include "mpu_defs.h"
#include "mpu_prog.h"
#include <stdio.h>
#include <assert.h>

#define PRIVILEGED_LOCATION 0x5800E000;


int main (void) {

  /* ===========================================
   * This example aims to show:
   *
   * Basic programming of MPU regions.
   * How to enable the MPU and MemManage faults?
   * Trigger MemManage faults to show how memory
   * is protected by the MPU using two scenarios
   *
   * Scenario:1 - Write to a Read-only memory
   * Scenario:2 - Read privileged memory from
   *              unprivileged state
   * =========================================== */

    printf("Example Project: trap_access Start \n");

    /* Set MPU regions and enable MPU */
    setMPU();

    /* Switch to unprivileged mode */
    __set_CONTROL( __get_CONTROL( ) | CONTROL_nPRIV_Msk ) ;

 
    /* ==========================================================
     * NOTE: 
     * Uncomment any one scenario that will cause MemManage Fault
     * By default, Scenario:1 will be executed. If Scenario:2 
     * needs to be run, then comment Scenario:1 (Lines 89:90) and 
     * uncomment Scenario:2 (Lines 94:95)
     * ========================================================== */

    int x = 1;

    /* Scenario:1 - Write to a Read-only memory */

    int* test1 = (int*)&Image$$ER_ROM$$Base;
    *test1 = x;

    /* Scenario:2 - Read privileged memory from unprivileged state */

    /* int* test2 = (int*)PRIVILEGED_LOCATION;
       x= *test2;                   */

    return 0;
}

/*----------------------------------------------------------------------------
  MemManage Handler
 *----------------------------------------------------------------------------*/
extern void MemManage_Handler(void){
    printf("We are in the MemManage handler. \n");
    printf("    MemManage Fault Address Register: \n        SCB->MMFAR = 0x%08x\n", SCB->MMFAR );
    printf("    MemManage Fault Status Register: \n        SCB->CFSR->MMFSR = 0x%08x\n", (0x000000FF)&(SCB->CFSR));
    printf("Example Project: trap_access End \n");
    assert(0);
}
