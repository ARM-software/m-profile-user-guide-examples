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

#ifndef EXCEP_PROG_H_
#define EXCEP_PROG_H_

/* Priority for exception */
#define IRQ0_PRI                0x1F
#define MEM_PRI                 0x06
#define SVC_PRI                 0x02


/* Parameters for context stack */
#define PS_STACK_SIZE           0x100
#define CalleeRegNum            8

/* Index for caller register of SVC stack */
#define STK_FRAME_R0            0
#define STK_FRAME_R1            1
#define STK_FRAME_R2            2
#define STK_FRAME_R3            3
#define STK_FRAME_R12           4
#define STK_FRAME_LR            5
#define STK_FRAME_RET_ADDR      6
#define STK_FRAME_XPSR          7

/* Declaration of variables used in unprivileged mode */
extern uint32_t deprivThreadStack[];
extern uint32_t *deprivThreadStackPtr;

uint32_t SVCHandlerMain(uint32_t *svc_StackFrame, uint32_t *msp, uint32_t EXCReturn);
void depriv_service(void);

#endif /* EXCEP_PROG_H_ */


