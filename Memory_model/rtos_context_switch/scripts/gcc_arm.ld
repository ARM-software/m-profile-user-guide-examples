/******************************************************************************
 * @file     gcc_arm.ld
 * @brief    GNU Linker Script for Cortex-M based device
 * @version  V2.2.0
 * @date     16. December 2020
 ******************************************************************************/
/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
__ROM_SIZE = 0x00040000;

/*--------------------- Embedded RAM Configuration ----------------------------
  <h> RAM Configuration
    <o0> RAM Base Address    <0x0-0xFFFFFFFF:8>
    <o1> RAM Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
 -----------------------------------------------------------------------------*/
__RAM_BASE = 0x20000000;
__RAM_SIZE = 0x00040000;

/*--------------------- Stack / Heap Configuration ----------------------------
  <h> Stack / Heap Configuration
    <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
    <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
  </h>
  -----------------------------------------------------------------------------*/
__STACK_SIZE = 0x00000200;
__HEAP_SIZE  = 0x00000C00;


__THREAD_A_ROM_BASE = 0x00100000;
__THREAD_A_ROM_SIZE = 0x00040000;

__THREAD_B_ROM_BASE = 0x00200000;
__THREAD_B_ROM_SIZE = 0x00040000;

__UNPRIV_ROM_BASE = 0x00300000;
__UNPRIV_ROM_SIZE = 0x00040000;

__THREAD_A_RAM_BASE = 0x21000000;
__THREAD_A_RAM_SIZE = 0x00040000;

__THREAD_B_RAM_BASE = 0x21100000;
__THREAD_B_RAM_SIZE = 0x00040000;

__UNPRIV_RAM_BASE = 0x21200000;
__UNPRIV_RAM_SIZE = 0x00040000;

/*
 *-------------------- <<< end of configuration section >>> -------------------
 */

/* ARMv8-M stack sealing:
   to use ARMv8-M stack sealing set __STACKSEAL_SIZE to 8 otherwise keep 0
 */
__STACKSEAL_SIZE = 0;


MEMORY
{
  ER_ROM (rx)  : ORIGIN = __ROM_BASE, LENGTH = __ROM_SIZE
  THREAD_A_ROM (rx)  : ORIGIN = __THREAD_A_ROM_BASE, LENGTH = __THREAD_A_ROM_SIZE
  THREAD_B_ROM (rx)  : ORIGIN = __THREAD_B_ROM_BASE, LENGTH = __THREAD_B_ROM_SIZE
  UNPRIV_ROM (rx)  : ORIGIN = __UNPRIV_ROM_BASE, LENGTH = __UNPRIV_ROM_SIZE
  
  RW_RAM   (rwx) : ORIGIN = __RAM_BASE, LENGTH = __RAM_SIZE
  THREAD_A_RAM (rwx)  : ORIGIN = __THREAD_A_RAM_BASE, LENGTH = __THREAD_A_RAM_SIZE
  THREAD_B_RAM (rwx)  : ORIGIN = __THREAD_B_RAM_BASE, LENGTH = __THREAD_B_RAM_SIZE
  UNPRIV_RAM (rwx)  : ORIGIN = __UNPRIV_RAM_BASE, LENGTH = __UNPRIV_RAM_SIZE
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
    logs/main.o      (.text*)
    logs/mpu_configs.o    (.text*)
    logs/mpu_prog.o    (.text*)
    logs/mpu_reprog.o    (.text*)
    logs/myRTOS.o    (.text*)
    logs/system.o   (.text*)
    logs/startup.o   (.text*)
    
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


  .thread_A_code :
  {
    . = ALIGN(4);
    Image$$THREAD_A_ROM$$Base = .;
    
    logs/threadA.o   (.text*)
    
    Image$$THREAD_A_ROM$$Limit = .;
    . = ALIGN(4);
  } > THREAD_A_ROM
  
 
  .thread_B_code :
  {
    . = ALIGN(4);
    Image$$THREAD_B_ROM$$Base = .;
    
    logs/threadB.o   (.text*)
    
    Image$$THREAD_B_ROM$$Limit = .;
    . = ALIGN(4);
  } > THREAD_B_ROM
  
  
  .unpriv_code :
  {
    . = ALIGN(4);
    Image$$UNPRIV_ROM$$Base = .;
    
    *(EXCLUDE_FILE(
    logs/main.o 
    logs/mpu_prog.o
    logs/mpu_configs.o
    logs/myRTOS.o 
    logs/mpu_reprog.o
    logs/threadA.o
    logs/threadB.o
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
    
    Image$$UNPRIV_ROM$$Limit = .;
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
    . = ALIGN(4);
    __data_start__ = .;
    Image$$RW_RAM$$Base = .;
    
    *(vtable)
    
    logs/main.o      (.data*)
    logs/mpu_configs.o    (.data*)
    logs/mpu_prog.o    (.data*)
    logs/mpu_reprog.o    (.data*)
    logs/myRTOS.o    (.data*)
    logs/system.o   (.data*)
    logs/startup.o   (.data*)
 
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

    logs/main.o      (.bss*)
    logs/mpu_configs.o    (.bss*)
    logs/mpu_prog.o    (.bss*)
    logs/mpu_reprog.o    (.bss*)
    logs/myRTOS.o    (.bss*)
    logs/system.o   (.bss*)
    logs/startup.o   (.bss*)
    
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


  .thread_A_data : 
  {
    __thread_A_data_start__ = .;
    Image$$THREAD_A_RAM$$Base = .;  
     
    logs/threadA.o    (.data*)
    
    __thread_A_data_end__ = .;
  } > THREAD_A_RAM

  .thread_A_bss :
  {
    . = ALIGN(4);
    __thread_A_bss_start__ = .;
    
    logs/threadA.o    (.bss*)
    
    Image$$THREAD_A_RAM$$ZI$$Limit = .;
    . = ALIGN(4);
    
    __thread_A_bss_end__ = .;
  } > THREAD_A_RAM AT > THREAD_A_RAM


  .thread_B_data : 
  {
    __thread_B_data_start__ = .;
    Image$$THREAD_B_RAM$$Base = .;  
     
    logs/threadB.o    (.data*)
    
    __thread_B_data_end__ = .;
  } > THREAD_B_RAM

  .thread_B_bss :
  {
    . = ALIGN(4);
    __thread_B_bss_start__ = .;
    
    logs/threadB.o    (.bss*)
    
    Image$$THREAD_B_RAM$$ZI$$Limit = .;
    . = ALIGN(4);
    
    __thread_B_bss_end__ = .;
  } > THREAD_B_RAM AT > THREAD_B_RAM  
  
  
  .unpriv_data : 
  {
    __unpriv_data_start__ = .;
    Image$$UNPRIV_RAM$$Base = .;  
     
    *(EXCLUDE_FILE(
    logs/main.o 
    logs/mpu_prog.o
    logs/mpu_configs.o
    logs/myRTOS.o 
    logs/mpu_reprog.o
    logs/threadA.o
    logs/threadB.o
    logs/system.o
    logs/startup.o) .data*)
    
    __unpriv_data_end__ = .;
  } > UNPRIV_RAM

  .unpriv_bss :
  {
    . = ALIGN(4);
    __unpriv_bss_start__ = .;
    
    *(EXCLUDE_FILE(
    logs/main.o 
    logs/mpu_prog.o
    logs/mpu_configs.o
    logs/myRTOS.o 
    logs/mpu_reprog.o
    logs/threadA.o
    logs/threadB.o
    logs/system.o
    logs/startup.o) .bss*)
    
    . = ALIGN(4);
    
    __unpriv_bss_end__ = .;
  } > UNPRIV_RAM
  
  
  .heap (COPY) :
  {
    . = ALIGN(8);
    __end__ = .;
    PROVIDE(end = .);
    . = . + __HEAP_SIZE;
    . = ALIGN(8);
    
    Image$$ARM_LIB_HEAP$$ZI$$Limit = .;
    __HeapLimit = .;   
  } > UNPRIV_RAM AT > UNPRIV_RAM
}
