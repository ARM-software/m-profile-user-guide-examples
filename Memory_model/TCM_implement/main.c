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

#if defined (ARMCM55)
  #include "ARMCM55.h"
#endif

#include <stdio.h>
#include "mpu_prog.h"
#include "Excep_prog.h"

#define MAX_RPMS 100
#define RPMS_TO_INC 15

int rpms = 0;
int stir = 0;

int main(){
  /* ===========================================
   * NOTE:
   * This example is to show how to configure TCM memory, and set up
   * latency critical code at TCM.
   * Please refer the configurations at RTE\Device\ARMCM55_ac6.sct
   * =========================================== */

  printf("Example Project: TCM-implement Start\n");
  /* Step1: Set MPU regions and enable MPU */
  setMPU();
  SCB_EnableDCache();
  SCB_EnableICache();

  /* Step2: relocate vector table into TCM */
  Vector_Table_Relocation();

  /* Step3: Generate SysTick interrupt */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/200);

  /* Start background sequence */
  while(1){
    if(rpms < MAX_RPMS){
        stir = 1;
        rpms += RPMS_TO_INC;
    }
    else{
        stir = 0;
        rpms = 0;
    }
    printf("Current revolutions per minute: %d \n", rpms);
  }

  return 0;
}
