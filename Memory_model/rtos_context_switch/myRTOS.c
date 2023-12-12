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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "threadDefs.h"
#include "mpu_prog.h"
#include "mpu_reprog.h"

ThreadContext *currentContext;

/**
  \brief        Start Systick timer for context switching
  \details      Setup MPU regions and switch to unprivileged mode to run threads continuously 
  \return       -
 */
void startOS(){

    /* Set MPU regions, enable MPU and enable MemManage Faults */
    setMPU();

    /* Start SysTick timer for context switching */
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock/2000);

    /* Switch to unprivileged mode to run threads */
    __set_CONTROL( __get_CONTROL( ) | CONTROL_nPRIV_Msk ) ;

    /* Idle thread */
    while(1);
}

/**
  \brief        Create New Thread context
  \details      Initialize the stack in such a way that new context is created. 
  \param        function       - Value of Program counter
  \param        threadStackLim - Stack Pointer Limit value
  \param        threadContext  - New thread context to be initialized
  \param        mpuConf        - MPU configuration for new context
  \param        threadID       - Thread identification
  \return       0, successfully set
*/

int newThread(void * function, uint32_t * threadStackLim, ThreadContext *threadContext, MpuRegionCfg  *mpuConf,  unsigned int  threadID){

    for(int i = 0; i < NUM_CALLEE_REGS; i++){
        threadContext->calleeRegs[i] = 0x0;
    }

    threadContext -> spLimit  = threadStackLim;
    threadContext -> pc = function;
    threadContext -> lr = MY_EXC_RETURN;
    threadContext -> mpuCfg = mpuConf;
    threadContext -> id = threadID;

    // Initialise thread stack
    threadContext -> sp  = threadStackLim + THREAD_STACK_SIZE - NORM_STACK_FRAME_SIZE;
    threadContext -> sp[NORM_STACK_FRAME_XPSR_OFFSET] = DEF_MY_XPSR;
    threadContext -> sp[NORM_STACK_FRAME_PC_OFFSET] = (uint32_t)function;

    return 0;
}

/**
  \brief        Thread context switch happens in SysTick Handler Body
  \details      Once a SysTick interrupt occurs, the processor hardware automatically 
                stacks an exception frame (RETPSR, PC, LR, r12 and r3-r0) onto the Process Stack 
                (PSP) and branches to the SysTick handler routine in Handler Mode (which uses the Main Stack). 
                Initialize the stack in such a way that new context is created. 
  \param        lr        - Link Register value
  \param        calleeRegisters[] 
  \return       0, successfully set
*/

int SysTick_Handler_body(unsigned int lr, unsigned int calleeRegisters[]){
    /* Context switching code:
     * Returns the new LR value that must be used to return from the exception. */

    /* Save PSP */
    currentContext -> sp = (uint32_t  *)__get_PSP();

    /* Save callee registers */
    for(int i = 0; i < NUM_CALLEE_REGS; i++){
        currentContext->calleeRegs[i] = calleeRegisters[i];
    }

    /* Change current thread context */
    if(currentContext -> id == ID_THREAD_A){
        currentContext = &threadBContext;
    }
    else if(currentContext -> id == ID_THREAD_B){
        currentContext = &threadAContext;
    }
    else{
        printf("Sth went wrong \n");
        return -1;
    }

    reprogMPU(currentContext -> mpuCfg);

    /* Restore callee registers */
    for(int i = 0; i < NUM_CALLEE_REGS; i++){
        calleeRegisters[i] = currentContext->calleeRegs[i];
    }

    /* Set PSP and PSP Limit */
    __set_PSPLIM ((uint32_t)(currentContext -> spLimit));
    __set_PSP((uint32_t)(currentContext -> sp));

    return currentContext -> lr;
}


/**
  \brief        SysTick Handler 
  \details      Save callee saved registers to the state, so that C function 
                SysTick_Handler_body can get access to them and perform context switches 
*/

extern __attribute__((naked)) void SysTick_Handler(){

    __asm(
            /* Push the callee saved registers to the stack, so the C function, SysTick_Handler_body,
             * can get access to them and perform context switches */
            "PUSH    {r4-r11}\n"
            "MOV     r1,sp\n"
            /* Call the C handler, SysTick_Handler_body, allowing it to
             * modify the return value used by the exception handler */
            "MOV     r0,lr\n"
            "BL      SysTick_Handler_body\n"
            /* Restore the new set of callee registers and the new LR value */
            "POP     {r4-r11}\n"
            /* Return from the exception */
            "BX      r0"
    );

}

/*----------------------------------------------------------------------------
  MemManage Handler
 *----------------------------------------------------------------------------*/
extern void MemManage_Handler(void){
    printf("We are in the MemManage handler. \n");
    printf("    MemManage Fault Address Register: \n        SCB->MMFAR = 0x%08x\n", SCB->MMFAR );
    printf("    MemManage Fault Status Register: \n        SCB->CFSR->MMFSR = 0x%08x\n", (0x000000FF)&(SCB->CFSR));

    assert(0);
}
