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


#ifndef IRQSET_H_
#define IRQSET_H_

/* Params for new vector table */
#define VECTORTABLE_SIZE              48
#define VECTORTABLE_ALIGNMENT         0x100U

/* Params for pending different interrupts at each IRQ handler */
#define IRQ_PENDNum                   3 /* Number of IRQs to trigger */

#define InIRQ0Handler                 0 /* Index for pending request array */
#define InIRQ1Handler                 1
#define InIRQ2Handler                 2

#define IRQ_offset                   16 /* Exception Number offset for external interrupts */

extern uint32_t __VECTOR_TABLE[];
extern uint32_t IRQPendRequests[];

void Interrupt_Handler(void);
void Vector_Table_Relocation(void);
void Print_PendAndActiveStatus();

#endif /* IRQSET_H_ */
