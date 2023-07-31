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

#include "Excep_prog.h"
#include <stdio.h>
#include <string.h>

/* Definition of new vector table with attribute */
VECTOR_TABLE_Type new_vectorTable[VECTORTABLE_SIZE] __attribute__((used, section("NewVT")));


/**
  \brief        Relocate vector table
  \details      Create a new vector table at ITCM and change the
                VTOR from origin vector table address to new one.
 */
void Vector_Table_Relocation(void){
  /* Copy the original handler address into new vector table */
  memcpy(new_vectorTable, __VECTOR_TABLE, sizeof(VECTOR_TABLE_Type)*VECTORTABLE_SIZE);

  /* Get information about vector table */
  printf("Vector table address is 0x%08x\n", SCB->VTOR);

  /* Change the VTOR into new vector table address */
  SCB->VTOR = (uint32_t)&new_vectorTable;
  __DSB();
  __ISB();

  printf("New vector table address is 0x%08x\n", SCB->VTOR );
}
