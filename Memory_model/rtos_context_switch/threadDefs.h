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

#include <stdint.h>

#define THREAD_STACK_SIZE  2048
#define NUM_CALLEE_REGS 8
#define MY_EXC_RETURN 0xFFFFFFFD  /* Return to thread mode using process stack and restore basic stack frame */
#define NORM_STACK_FRAME_SIZE           8
#define NORM_STACK_FRAME_XPSR_OFFSET    7
#define NORM_STACK_FRAME_PC_OFFSET      6
#define NORM_STACK_FRAME_LR_OFFSET      5
#define NORM_STACK_FRAME_R12_OFFSET     4
#define NORM_STACK_FRAME_R3_OFFSET      3
#define NORM_STACK_FRAME_R2_OFFSET      2
#define NORM_STACK_FRAME_R1_OFFSET      1
#define NORM_STACK_FRAME_R0_OFFSET      0
#define DEF_MY_XPSR                     0x01000000
#define MAX_MPU_REGIONS                 8

/* MPU Region Configuration struct definition */
typedef struct{
	uint32_t rbar;
	uint32_t rlar;
} MpuRegionCfg;

/* Thread Context Type definition */
typedef struct{
	uint32_t  calleeRegs[NUM_CALLEE_REGS]; //r4-r11
    uint32_t  *sp;
    uint32_t  *spLimit;
    uint32_t  *pc;
    unsigned int  lr;
    MpuRegionCfg  *mpuCfg;
    unsigned int  id;
} ThreadContext;

/* Thread functions references */
extern void thrA (void);
extern void thrB (void);

/* Thread stacks references */
extern uint32_t threadA_stk[THREAD_STACK_SIZE];
extern uint32_t threadB_stk[THREAD_STACK_SIZE];

/* Thread contexts */
extern ThreadContext threadAContext;
extern ThreadContext threadBContext;

/* MPU configurations for each thread */
extern MpuRegionCfg  mpuCfgA [MAX_MPU_REGIONS];
extern MpuRegionCfg  mpuCfgB [MAX_MPU_REGIONS];

/* IDs for each thread */
#define ID_THREAD_A  1
#define ID_THREAD_B  2
