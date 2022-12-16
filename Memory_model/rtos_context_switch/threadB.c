/* 
 *
 * MIT License
 * Copyright (c) 2022 ARM Limited.
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

#include <stdint.h>
#include <stdio.h>

#include "threadDefs.h"

uint32_t threadB_stk[THREAD_STACK_SIZE];

uint32_t pent;
uint32_t idx_p;


void thrB (void) {
    pent = 1;
    idx_p = 1;

    // This is Thread B, which calculates and prints the numbers in the Pentagonal sequence
    while(1){

        pent = (3*idx_p*idx_p - idx_p)/2;
        idx_p++;
        if(pent > 0xFFFFFFF){
            pent = 1;
            idx_p = 1;
        }
        printf("\nPentagonal number: %u \n", pent);

        // Uncomment to trigger MemManage fault
        // threadA_stk[1] = 0x0;
    }

}



