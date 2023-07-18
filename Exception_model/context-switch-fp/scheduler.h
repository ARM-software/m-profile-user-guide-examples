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
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/* Params for Task Context definition */
/* Two task threads and one idle thread */
#define TASK_NUM_MAX                 3
#define TASK_STACK_SIZE              2048u
/* Index 0 for task array presents idle thread */
#define ID_TASK1                     1
#define ID_TASK2                     2

struct TaskContext{
    uint32_t  *sp;
    uint32_t  *spLimit;
    uint32_t  excReturn;
};

void task1(void);
void task2(void);

/* Context stack for tasks */
#define NUM_CALLEE_REGS              24
#define NORM_STACK_FRAME_SIZE        50

/* Refresh value for exception return */
#define XPSR_THREAD                  0x01000000
#define EXC_RETURN_THREAD_S_PSP      0xFFFFFFED

/* Index for caller register of SVC stack with offset-the number of callee registers */
#define STK_FRAME_R0                 (0+NUM_CALLEE_REGS)
#define STK_FRAME_R1                 (1+NUM_CALLEE_REGS)
#define STK_FRAME_R2                 (2+NUM_CALLEE_REGS)
#define STK_FRAME_R3                 (3+NUM_CALLEE_REGS)
#define STK_FRAME_R12                (4+NUM_CALLEE_REGS)
#define STK_FRAME_LR                 (5+NUM_CALLEE_REGS)
#define STK_FRAME_RET_ADDR           (6+NUM_CALLEE_REGS)
#define STK_FRAME_XPSR               (7+NUM_CALLEE_REGS)

extern uint32_t Image$$PSP_STACK$$ZI$$Base;

void TerminateTask(void);
void InitTask(void* task, uint32_t TaskID);
uint32_t PendSV_Handler_Main(uint32_t* sp, uint32_t* sp_lim, uint32_t exc_Return);
void StartScheduler();

#endif /* SCHEDULER_H_ */
