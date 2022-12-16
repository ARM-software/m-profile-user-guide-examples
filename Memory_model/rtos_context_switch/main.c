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

#include "myRTOS.h"
#include "mpu_defs.h"
#include "mpu_configs.h"

ThreadContext threadAContext;
ThreadContext threadBContext;


int main (void) {

    initMPUConfigs();

    newThread(thrA, threadA_stk, &threadAContext, mpuCfgA, ID_THREAD_A);
    newThread(thrB, threadB_stk, &threadBContext, mpuCfgB, ID_THREAD_B);

    // First thread to execute is Thread A. We set the current context to Thread B,
    // so it is as if Thread B was the last thread executed.
    // Then, the context switching routine knows it is now turn for thread A be executed.

    currentContext = &threadBContext;
    __set_PSP((uint32_t)(currentContext -> sp));
    __set_PSPLIM ((uint32_t)(currentContext -> spLimit));

    startOS();

    return 0;
}
