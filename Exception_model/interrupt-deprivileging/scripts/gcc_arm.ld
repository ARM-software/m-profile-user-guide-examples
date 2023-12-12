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

/*
 *-------- <<< Use Configuration Wizard in Context Menu >>> -------------------
 */

/*---------------------- Flash Configuration ----------------------------------
  <h> Flash Configuration
    <o0> Flash Base Address <0x0-0xFFFFFFFF:8>
    <o1> Flash Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__ROM_BASE = 0x00000000;
__ROM_SIZE = 0x00080000;

__UNPRIV_ROM_BASE = 0x10000000;
__UNPRIV_ROM_SIZE = 0x00020000;

/*--------------------- Embedded RAM Configuration ----------------------------
  <h> RAM Configuration
    <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
    <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
 -----------------------------------------------------------------------------*/
__RAM_BASE = 0x20000000;
__RAM_SIZE = 0x00040000;

__UNPRIV_RAM_BASE = 0x21000000;
__UNPRIV_RAM_SIZE = 0x00040000;

/*--------------------- Stack / Heap Configuration ----------------------------
  <h> Stack / Heap Configuration
    <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
    <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__STACK_SIZE = 0x00010000;
__HEAP_SIZE  = 0x00000C00;

/*
 *-------------------- <<< end of configuration section >>> -------------------
 */

MEMORY
{
  ER_ROM (rx)  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
  UNPRIV_ROM (rx)  : ORIGIN = __UNPRIV_ROM_BASE, LENGTH = __UNPRIV_ROM_SIZE
  RW_RAM   (rwx) : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE 
  UNPRIV_RAM  (rwx): ORIGIN = __UNPRIV_RAM_BASE, LENGTH = __UNPRIV_RAM_SIZE
}

/* Linker script to place sections and symbol values. Should be used together
 * with other linker script that defines memory regions FLASH and RAM.
 * It references following symbols, which must be defined in code:
 *   Reset_Handler : Entry of reset handler
 *
 * It defines following symbols, which code can use without definition:
 *   __exidx_start
 *   __exidx_end
 *   __copy_table_start__
 *   __copy_table_end__
 *   __zero_table_start__
 *   __zero_table_end__
 *   __etext
 *   __data_start__
 *   __preinit_array_start
 *   __preinit_array_end
 *   __init_array_start
 *   __init_array_end
 *   __fini_array_start
 *   __fini_array_end
 *   __data_end__
 *   __bss_start__
 *   __bss_end__
 *   __end__
 *   end
 *   __HeapLimit
 *   __StackLimit
 *   __StackTop
 *   __stack
 *   __StackSeal      (only if ARMv8-M stack sealing is used)
 */
ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    Image$$ER_ROM$$Base = .;
    KEEP(*(.vectors))
    logs/PrivilegedFuncs.o (.text*)
    logs/main.o (.text*)
    logs/mpu_prog.o (.text*)
    logs/system.o (.text*)
    logs/startup.o (.text*)  
    . = ALIGN(4);
  } > ER_ROM

  .copy.table :
  {
    . = ALIGN(4);
    __copy_table_start__ = .;
    LONG (__etext)
    LONG (__data_start__)
    LONG ((__data_end__ - __data_start__) / 4)
    __copy_table_end__ = .;
  } > ER_ROM

  .zero.table :
  {
    . = ALIGN(4);
    __zero_table_start__ = .;
    __zero_table_end__ = .;
    Image$$ER_ROM$$Limit = .;
  } > ER_ROM

  .unprivileged_code :
  {
    . = ALIGN(4);
    Image$$UNPRIV_ROM$$Base = .;
    *(EXCLUDE_FILE(
    logs/PrivilegedFuncs.o 
    logs/main.o 
    logs/mpu_prog.o 
    logs/system.o
    logs/startup.o
    ) .text*)
    KEEP(*(.init))
    KEEP(*(.fini))
    
    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)
    *(.rodata*)
    KEEP(*(.eh_frame*))
    Image$$UNPRIV_ROM$$Limit = Image$$UNPRIV_ROM$$Base + __UNPRIV_ROM_SIZE;
    . = ALIGN(4);
  } > UNPRIV_ROM

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > UNPRIV_ROM

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > UNPRIV_ROM
  __exidx_end = .; 
  
  /**
   * Location counter can end up 2byte aligned with narrow Thumb code but
   * __etext is assumed by startup code to be the LMA of a section in RAM
   * which must be 4byte aligned
   */
  __etext = ALIGN (4);

  .data : AT (__etext)
  {
    __data_start__ = .;
    Image$$RW_RAM$$Base = .;
    *(vtable)
    
    logs/PrivilegedFuncs.o (.data*)
    logs/main.o (.data*)
    logs/mpu_prog.o (.data*)
    logs/system.o (.data*)
    logs/startup.o (.data*)

    . = ALIGN(4);
    /* preinit data */
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    /* init data */
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    /* finit data */
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    KEEP(*(.jcr*))
    . = ALIGN(4);
    /* All data end */
    __data_end__ = .;
  } > RW_RAM

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    
    logs/PrivilegedFuncs.o (.bss*)
    logs/main.o (.bss*)
    logs/mpu_prog.o (.bss*)
    logs/system.o (.bss*)
    logs/startup.o (.bss*)

    *(COMMON)
    Image$$RW_RAM$$Limit = .;
    . = ALIGN(4);
    __bss_end__ = .;
    
  } > RW_RAM AT > RW_RAM
  
  .stack (ORIGIN(RW_RAM) + LENGTH(RW_RAM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __StackTop = ORIGIN(RW_RAM) + LENGTH(RW_RAM);
    __StackLimit = __StackTop - __STACK_SIZE;
    Image$$ARM_LIB_STACK$$ZI$$Limit = __StackTop;
    . = . + __STACK_SIZE;    
    . = ALIGN(8);
  } > RW_RAM
  PROVIDE(__stack = __StackTop);

  .unprivileged_data :
  {
    . = ALIGN(4);
    Image$$UNPRIV_RAM$$Base = .;
    *(EXCLUDE_FILE(
    logs/PrivilegedFuncs.o 
    logs/main.o 
    logs/mpu_prog.o 
    logs/system.o
    logs/startup.o
    ) .data*)
    . = ALIGN(4);
  } > UNPRIV_RAM
  
  .unprivileged_bss :
  {
    . = ALIGN(4);
    *(EXCLUDE_FILE(
    logs/PrivilegedFuncs.o 
    logs/main.o 
    logs/mpu_prog.o 
    logs/system.o
    logs/startup.o
    :q) .bss*)
    Image$$UNPRIV_RAM$$Limit = .;
    . = ALIGN(4);
  } > UNPRIV_RAM

  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > UNPRIV_RAM AT > UNPRIV_RAM

  .PSP_stack (ORIGIN(UNPRIV_RAM) + LENGTH(UNPRIV_RAM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __PSPStackTop = ORIGIN(UNPRIV_RAM) + LENGTH(UNPRIV_RAM);
    Image$$PSP_STACK$$ZI$$Limit = __PSPStackTop;
    __PSPStackLimit = __PSPStackTop - __STACK_SIZE;
    Image$$PSP_STACK$$ZI$$Base = __PSPStackLimit;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
  } > UNPRIV_RAM
  PROVIDE(__psp_stack = __PSPStackTop);
  
  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__PSPStackLimit >= __HeapLimit, "region RAM overflowed with stack")  
}
