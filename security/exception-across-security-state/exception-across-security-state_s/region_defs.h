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
#ifndef REGION_DEFS_H
#define REGION_DEFS_H

/*--------------------- Secure Flash Configuration ----------------------------------*/

#define __ROM_BASE_S                   0x10000000
#define __ROM_SIZE_S                   0x00200000

/*--------------------- Secure RAM Configuration ---------------------------*/

#define __RAM_BASE_S                   0x30000000
#define __RAM_SIZE_S                   0x00200000

/*--------------------- Secure Stack / Heap Configuration ---------------------------*/
#define __STACK_SIZE                   0x00000400
#define __HEAP_SIZE                    0x00000C00
#define __PSP_STACK_SIZE               0x00000400

/*--------------------- Secure NSC Region Configuration ---------------------------*/
#define __CMSEVENEER_BASE              0x101FFC00
#define __CMSEVENEER_SIZE              0x00000400

/*--------------------- Non-secure ROM Region Configuration ---------------------------*/
#define __ROM_BASE_NS                  0x00200000
#define __ROM_SIZE_NS                  0x00200000

/*--------------------- NOn-secure RAM Region Configuration ---------------------------*/
#define __RAM_BASE_NS                  0x20200000
#define __RAM_SIZE_NS                  0x00200000


#endif
