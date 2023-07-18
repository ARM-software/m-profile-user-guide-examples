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

extern unsigned int Image$$ITCM$$Base;
extern unsigned int Image$$ITCM$$Limit;

extern unsigned int Image$$ITCM_Code$$Base;
extern unsigned int Image$$ITCM_Code$$Limit;

extern unsigned int Image$$RW_RAM$$Base;
extern unsigned int Image$$RESET_MSP_STACK$$ZI$$Limit;

extern unsigned int Image$$ER_ROM$$Base;
extern unsigned int Image$$ER_ROM$$Limit;

extern unsigned int Image$$DTCM$$Base;
extern unsigned int Image$$ARM_LIB_STACK$$ZI$$Limit;

int setMPU(void);
