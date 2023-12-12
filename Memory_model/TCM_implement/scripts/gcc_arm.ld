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
__ROM_BASE = 0x10000000;
__ROM_SIZE = 0x00080000;


/*--------------------- ITCM Configuration ----------------------------------
; <h> ITCM Configuration - 256KB
;   <o0> ITCM Base Address    <0x0-0xFFFFFFFF:8>
;   <o1> ITCM Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
 *----------------------------------------------------------------------------*/
__ITCM_BASE = 0x00000000;
__ITCM_SIZE = 0x00040000;


/*--------------------- Embedded RAM Configuration ----------------------------
  <h> RAM Configuration
    <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
    <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
 -----------------------------------------------------------------------------*/
__RAM_BASE = 0x21000000;
__RAM_SIZE = 0x00040000;


/*--------------------- DTCM Configuration ----------------------------------
; <h> DTCM Configuration - 256KB
;   <o0> DTCM Base Address    <0x0-0xFFFFFFFF:8>
;   <o1> DTCM Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>
 *----------------------------------------------------------------------------*/
__DTCM_BASE = 0x20000000;
__DTCM_SIZE = 0x00040000;


/*--------------------- Stack / Heap Configuration ----------------------------
  <h> Stack / Heap Configuration
    <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
    <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__STACK_SIZE = 0x00010000;
__HEAP_SIZE  = 0x00000C00;


__VectorTable_SIZE = 0x00000800;

/*
 *-------------------- <<< end of configuration section >>> -------------------
 */

/* ARMv8-M stack sealing:
   to use ARMv8-M stack sealing set __STACKSEAL_SIZE to 8 otherwise keep 0
 */
__STACKSEAL_SIZE = 0;


MEMORY
{
  ITCM (rx)  : ORIGIN = __ITCM_BASE, LENGTH = __VectorTable_SIZE
  ITCM_CODE (rx) : ORIGIN = __ITCM_BASE + __VectorTable_SIZE, LENGTH = __ITCM_SIZE
  ER_ROM (rx)  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
  DTCM (rwx) : ORIGIN = __DTCM_BASE, LENGTH = __DTCM_SIZE
  RW_RAM   (rwx) : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE
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
    logs/main.o (.text*)
    logs/mpu_prog.o (.text*)
    logs/system.o (.text*)
    logs/startup.o (.text*)
    *(EXCLUDE_FILE(logs/Excep_prog.o) .NewVT*)
    
    . = ALIGN(4);
  } > ER_ROM

  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > ER_ROM

  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > ER_ROM
  __exidx_end = .;

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

  .New_Vt :
  {
    . = ALIGN(4);
    Image$$ITCM$$Base = .;
    _srelocate = .;
    *(.NewVT);
    Image$$ITCM$$Limit = .;
    . = ALIGN(4);
    _erelocate = .;
  } > ITCM
  
  
  .itcm_code :
  {
    . = ALIGN(4);
    Image$$ITCM_Code$$Base = .;
    
    *(EXCLUDE_FILE(
    logs/main.o 
    logs/mpu_prog.o 
    logs/Excep_prog.o
    logs/system.o
    logs/startup.o) .text*)

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
    Image$$ITCM_Code$$Limit = .;
    . = ALIGN(4);
  } > ITCM_CODE

  /**
   * Location counter can end up 2byte aligned with narrow Thumb code but
   * __etext is assumed by startup code to be the LMA of a section in RAM
   * which must be 4byte aligned
   */
  __etext = ALIGN (4);

  .data : AT (__etext)
  {
    . = ALIGN(4);
    __data_start__ = .;
    Image$$RW_RAM$$Base = .;
    
    logs/Excep_prog.o (.data*)
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
    
    logs/Excep_prog.o (.bss*)
    logs/main.o (.bss*)
    logs/mpu_prog.o (.bss*)
    logs/system.o (.bss*)
    logs/startup.o (.bss*)

    Image$$RW_RAM$$Limit = .;
    __bss_end__ = .;
    . = ALIGN(4);
  } > RW_RAM

  .Reset_stack (ORIGIN(RW_RAM) + LENGTH(RW_RAM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __ResetStackTop = ORIGIN(RW_RAM) + LENGTH(RW_RAM);
    __ResetStackLimit = __ResetStackTop - __STACK_SIZE;
    Image$$RESET_MSP_STACK$$ZI$$Limit = __ResetStackTop;
    . = . + __STACK_SIZE;    
    . = ALIGN(8);
  } > RW_RAM
  PROVIDE(__Resetstack = __ResetStackTop);

  .dtcm_data : 
  {
    __dtcm_data_start__ = .;
    Image$$DTCM$$Base = .;  
     
    *(EXCLUDE_FILE(
    logs/Excep_prog.o 
    logs/main.o 
    logs/mpu_prog.o  
    logs/system.o
    logs/startup.o) .data*)
    *(.dtcm*)
    
    __dtcm_data_end__ = .;
  } > DTCM

  .dtcm_bss :
  {
    . = ALIGN(4);
    __dtcm_bss_start__ = .;
    
    *(EXCLUDE_FILE(
    logs/Excep_prog.o 
    logs/main.o 
    logs/mpu_prog.o 
    logs/system.o
    logs/startup.o) .bss*)
    
    Image$$DTCM$$Limit = .;
    . = ALIGN(4);
    __dtcm_bss_end__ = .;
  } > DTCM AT > DTCM
  
  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    __HeapLimit = .;
  } > DTCM AT > DTCM

  .stack (ORIGIN(DTCM) + LENGTH(DTCM) - __STACK_SIZE) (COPY) :
  {
    . = ALIGN(8);
    __StackTop = ORIGIN(DTCM) + LENGTH(DTCM);
    Image$$ARM_LIB_STACK$$ZI$$Limit = __StackTop;
    __StackLimit = __StackTop - __STACK_SIZE;
    Image$$ARM_LIB_STACK$$ZI$$Base = __StackLimit;
    . = . + __STACK_SIZE;
    . = ALIGN(8);
  } > DTCM
  PROVIDE(__stack = __StackTop);
  
  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")  
}
