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

#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif

#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "mpu_prog.h"
#include "excep_prog.h"


__attribute((naked)) void Interrupt0_Handler(void) {
  __asm volatile(
    "PUSH    {R4-R12, LR}                    \n" /* Push the callee regs and keep stack pointer 8 byte alignment. */
    "CLRM    {R1-R12}                        \n" /* Clear all the regs */
    "SVC     #0                              \n" /* Request a depriv of the execution */
    "POP     {R4-R12, LR}                    \n"
    "BX      LR                              \n"
  );
}


uint32_t SVCHandlerMain(uint32_t *svc_StackFrame, uint32_t *msp, uint32_t EXCReturn) {
  uint32_t *psp;
  uint32_t *pspLim;
  uint32_t excReturn;
  uint32_t svc_number;

  /* Different actions based on SVC call number */
  svc_number = *((uint16_t *)svc_StackFrame[STK_FRAME_RET_ADDR] - 1) & 0xFF;

  switch (svc_number) {
    case 0: /* IRQ depriv request */
       /* From handler mode to unpriv thread mode.
        * Record the PSP to restore the state when SVC happens. */
       psp    = (uint32_t *)__get_PSP();
       pspLim = (uint32_t *)__get_PSPLIM();

       /* Re-use the SVC caller register stack
        * R1 stores current psp, R2 stores psplim
        * Refresh the LR, return address and xPSR */
       svc_StackFrame[STK_FRAME_R1] = (uint32_t) psp;
       svc_StackFrame[STK_FRAME_R2] = (uint32_t) pspLim;
       svc_StackFrame[STK_FRAME_LR] = EXCReturn;

       deprivThreadStackPtr        -= CalleeRegNum;
       psp                          = deprivThreadStackPtr;

       psp[STK_FRAME_RET_ADDR]      = (uint32_t)&depriv_service;
       psp[STK_FRAME_XPSR]          = svc_StackFrame[STK_FRAME_XPSR] & 0xFFFFFE00;

       /* Preparation for fake exception return
        * Switch to the unprivileged memory region */
       config_MPU(ARM_MPU_NON_PRIV);
       __set_PSP((uint32_t)psp);
       __set_PSPLIM((uint32_t)deprivThreadStack);
       __set_CONTROL(__get_CONTROL() | CONTROL_nPRIV_Msk);

       excReturn = 0xFFFFFFFD;
       break;

    case 1: /* IRQ repriv request */
       /* From thread mode to handler mode, return to IRQ
        * Re-use the SVC #0 caller register stack to complete a fake exception return */
       excReturn                 = msp[STK_FRAME_LR];
       psp                       = (uint32_t *) msp[STK_FRAME_R1];
       pspLim                    = (uint32_t *) msp[STK_FRAME_R2];
       deprivThreadStackPtr      = svc_StackFrame + CalleeRegNum;

       /* Preparation for fake exception return */
       config_MPU(ARM_MPU_PRIV);
       __set_PSP((uint32_t)psp);
       __set_PSPLIM((uint32_t)pspLim);
       __set_CONTROL(__get_CONTROL() & ~CONTROL_nPRIV_Msk);
       break;

    default:
       assert(false);
    }
    return excReturn;
}


__attribute((naked)) void SVC_Handler(void) {
/* The execution will enter into SVC_Handler twice.
 * The background SP in use when the SVC is entered is
 * MSP for the first case, and PSP for the second case.*/
  __asm volatile(
    "TST     LR, #0x4                 \n"
    "ITE     EQ                       \n"
    "MRSEQ   R0, MSP                  \n"
    "MRSNE   R0, PSP                  \n"
    "TST     LR, #0x20                \n"
    "IT      EQ                       \n"
    "ADDEQ   R0, R0, #0x40            \n"
    "MRS     R1, MSP                  \n"
    "MOV     R2, LR                   \n"
    "BL      SVCHandlerMain           \n"
    "BX      R0                       \n" /* new excReturn value from svcHandler */
  );
}
