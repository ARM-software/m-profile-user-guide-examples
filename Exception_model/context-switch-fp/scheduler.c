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


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "scheduler.h"

uint32_t CurrentTaskID = 0;
uint32_t* PSPBase = (uint32_t*) &Image$$PSP_STACK$$ZI$$Base;
struct TaskContext Tasks[TASK_NUM_MAX];


void TerminateTask(void){
/* Since none of the threads in this example exit,
 * so this function hasn't been implemented. */
  assert(0);
}


void InitTask(void* task, uint32_t TaskID){
/* When adding a task, we initialize the PSP value of the new task
 * at the start address of new 2 KB RAM. Record the address of context. */

  if(TaskID >= TASK_NUM_MAX) {
    printf("Can not register a new task anymore!\n");
    return;
  }

  /* Detailed information is referred to the explanation in user guide */
  Tasks[TaskID].sp        = PSPBase + TaskID * TASK_STACK_SIZE - NORM_STACK_FRAME_SIZE;
  Tasks[TaskID].spLimit   = PSPBase + (TaskID - 1) * TASK_STACK_SIZE;
  Tasks[TaskID].excReturn = EXC_RETURN_THREAD_S_PSP;

  /* Initialize thread stack
   * Set LR to a function that will terminate the thread so that if
   * the thread entry point function "task" returns, the thread will terminate. */
  Tasks[TaskID].sp[STK_FRAME_XPSR]     = XPSR_THREAD;
  Tasks[TaskID].sp[STK_FRAME_RET_ADDR] = (uint32_t)task;
  Tasks[TaskID].sp[STK_FRAME_LR]       = (uint32_t)TerminateTask;
}


uint32_t PendSV_Handler_Main(uint32_t* sp, uint32_t* sp_lim, uint32_t exc_Return){
  /* Save PSP value to the slot at CurrentTaskID index */
  Tasks[CurrentTaskID].sp              = sp;
  Tasks[CurrentTaskID].spLimit         = sp_lim;
  Tasks[CurrentTaskID].excReturn       = exc_Return;

  /* Select next task */
  CurrentTaskID++;

  /* Check if a task is register at current slot */
  if (CurrentTaskID >= TASK_NUM_MAX) {
      CurrentTaskID = 0;
  }

  return (uint32_t)&Tasks[CurrentTaskID];
}


void SysTick_Handler(void){
/* In SysTick handler, with privileged access level,
 * We can directly trigger PendSV by setting pending bit with lowest priority level.
 * Therefore, the context switching will be done in PendSV Handler
 * after there is no interrupt handler in active state. */
  printf("\n we are in SysTick handler ! \n");

  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
  __DSB();
}


__attribute__ ((naked)) void PendSV_Handler(void) {
  __asm(
    /* Push the callee saved registers to the stack
     * From Idle thread to task thread or from task1 to task2 */
    "TST      LR,     #0x4                \n"  /* Check EXC_RETURN to determine which SP the current thread is using */
    "ITTEE    EQ                          \n"
    "MRSEQ    R0,     MSP                 \n"  /* Get current SP */
    "MRSEQ    R1,     MSPLIM              \n"  /* Get current SPLim */
    "MRSNE    R0,     PSP                 \n"
    "MRSNE    R1,     PSPLIM              \n"
    "MOV      R2,     LR                  \n"  /* Get current EXC_RETURN */

    "STMDB    R0!,    {R4-R11}            \n"  /* Save R4-R11 to SP Frame Stack */
    "TST      LR,     #0x10               \n"  /* Check EXC_RETURN.Ftype bit to see if the current thread has a FP context */
    "IT       EQ                          \n"
    "VSTMDBEQ R0!,    {S16-S31}           \n"  /* If so, save S16-S31 FP addition context, that will also trigger lazy fp
                                                  context preservation of S0-S15 */

    "BL       PendSV_Handler_Main         \n"  /* Save SP value of current task */
    "LDR      LR,     [R0, #8]            \n"  /* Get the EXC_RETURN value */
    "LDR      R1,     [R0, #4]            \n"  /* Get the SP_LIM value */
    "LDR      R0,     [R0, #0]            \n"  /* Get the SP value */

    "TST      LR,     #0x10               \n"  /* Check EXC_RETURN.Ftype bit to see if the next thread has a FP context */
    "IT       EQ                          \n"
    "VLDMIAEQ R0!,    {S16-S31}           \n"  /* If so, restore S16-S31 */
    "LDMIA    R0!,    {R4-R11}            \n"  /* Restore R4-R11 */

    "TST      LR,     #0x4                \n"  /* Check EXC_RETURN to determine which SP the next thread is using */
    "ITT      NE                          \n"
    "MSRNE    PSPLIM, R1                  \n"  /* Update the SPLim and SP since MSP won't be changed*/
    "MSRNE    PSP,    R0                  \n"
    "BX       LR                          \n"
  );
}


void StartScheduler() {
  printf("Start Scheduler ! \n");

  /* Start SysTick timer for context switching */
  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock/1000);

  /* Wait in Idle thread, until the SysTick exception happens to call the task1 */
  while(1);
}

