/*
 *
 * MIT License
 * Copyright (c) 2025 ARM Limited.
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
#ifndef INIT_H
#define INIT_H

/*SecureFault priority value*/
#define SecureFaultPriority            0


/*CPPWR: SUS10 Position */
#define SCnSCB_CPPWR_SUS10_Pos         21U

/*CPPWR: SUS10 Enable: the SU10 field is only accessible from the Secure state.*/
#define SCnSCB_CPPWR_SUS10_Msk         (1UL << SCnSCB_CPPWR_SUS10_Pos)

/*CPPWR: SUS11 Position */
#define SCnSCB_CPPWR_SUS11_Pos         23U

/*CPPWR: SUS11 Enable: the SU11 field is only accessible from the Secure state.*/
#define SCnSCB_CPPWR_SUS11_Msk         (1UL << SCnSCB_CPPWR_SUS11_Pos)

/* Set 1 in each bit to target each interrupt to Secure*/
#define NVIC_INIT_SECURE_VAL           0XFFFFFFFF

/* Symbols extracted from scatter file for Arm Compiler 6 toolchain and linker script for GNU
 * toolchain */
extern int32_t  Image$$ER_CMSE_VENEER$$Base;
extern int32_t  Image$$ER_CMSE_VENEER$$Limit;

extern int32_t  Image$$PSP_STACK$$ZI$$Limit;
extern int32_t  Image$$PSP_STACK$$ZI$$Base;
extern int32_t  Image$$PSP_STACKSEAL$$Base;

/*  PSP Stack address*/
#define INITIAL_PSP                    Image$$PSP_STACK$$ZI$$Limit

#define PSP_STACK_LIMIT                Image$$PSP_STACK$$ZI$$Base

#define PSP_STACK_SEAL                 Image$$PSP_STACKSEAL$$Base


void Secure_System_Init();

#endif  /* INIT_H */
