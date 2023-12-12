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


/* As we use arm compiler, it will automatically relocate the latency critical
 * code from ROM to ITCM when doing scatter loading. However, if we use other
 * tool chains like GCC, it won't do the relocate automatically so that we need
 * to finish it manually like relocating vector table.
 */
#if defined (ARMCM55)
#include "ARMCM55.h"
#endif

#define DEP_LIMIT 100
#define LITRES_TO_ADD 10

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

int __attribute__((section(".dtcm"))) volume = 0;
int __attribute__((section(".dtcm"))) checks = 0;


/**
  \brief        Checks if the volume of a deposit is beyond a limit
  \details      Check if current volume with other to add is beyond limit
  \return       1, the deposit is full
                0, the deposit is not full
 */
bool check_is_full(){
  if ((volume + LITRES_TO_ADD)>= DEP_LIMIT)
    return true;
  else
    return false;
}


/**
  \brief        Overwrite of SysTick handler
  \details      Get the result of check_is_full() and go on the next step to fill
                or stop based on it.
 */
extern void SysTick_Handler(void){
  printf("Number of checks in SysTick handler: %d \n", checks);
  checks += 1;

  /* Checks if the deposit is full */
  /* Fills or stops filling the deposit depending whether it is full or not */
  if (check_is_full()){
    volume = 0;
  }
  else{
    volume += LITRES_TO_ADD;
  }

  printf("Current volume: %d \n", volume);
}
