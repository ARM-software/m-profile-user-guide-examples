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

#if defined (ARMCM55)
  #include "ARMCM55.h"
#endif

#include "mpu_defs.h"
#include "mpu_prog.h"


int setMPU(void){
  /* Set memory attributes in Memory Attribute Indirection Registers */

  /* Set Attr 0 - used for cached memory */
  ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(       /* Normal memory */
    ARM_MPU_ATTR_MEMORY_(1UL, 1UL, 1UL, 1UL), /* Outer Write-Through non-transient */
    ARM_MPU_ATTR_MEMORY_(1UL, 1UL, 1UL, 1UL)  /* Inner Write-Through non-transient */
  ));

  /* Set Attr 1 - used for TCM */
  ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(       /* Normal memory non-cacheable */
    ARM_MPU_ATTR_NON_CACHEABLE,
    ARM_MPU_ATTR_NON_CACHEABLE
  ));

  /* Set MPU regions
   * The symbols used to obtain the following addresses come from
   * the information in \RTE\Device\ARMCM55\ARMCM55_ac6.sct. */

  /* Set region 0 ITCM with Attr 1 */
  unsigned int ITCMAddr = (unsigned int) &Image$$ITCM$$Base;
  unsigned int ITCMLimit = (unsigned int) &Image$$ITCM$$Limit;

  ARM_MPU_SetRegion(0UL,
    ARM_MPU_RBAR(ITCMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_EXEC),
    ARM_MPU_RLAR(ITCMLimit, 1UL)
  );


  /* Set region 1 ITCM_Code with Attr 1 */
  unsigned int ITCMCodeAddr = (unsigned int) &Image$$ITCM_Code$$Base;
  unsigned int ITCMCodeLimit = (unsigned int) &Image$$ITCM_Code$$Limit;

  ARM_MPU_SetRegion(1UL,
    ARM_MPU_RBAR(ITCMCodeAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC),
    ARM_MPU_RLAR(ITCMCodeLimit, 1UL)
  );


  /* Set region 2 ROM with Attr 0 */
  unsigned int ROMAddr = (unsigned int) &Image$$ER_ROM$$Base;
  unsigned int ROMLimit = (unsigned int) &Image$$ER_ROM$$Limit;

  ARM_MPU_SetRegion(2UL,
    ARM_MPU_RBAR(ROMAddr, ARM_MPU_SH_NON, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC),
    ARM_MPU_RLAR(ROMLimit, 0UL)
  );

  /* Set region 3 DTCM with Attr 1 */
  unsigned int DTCMAddr = (unsigned int) &Image$$DTCM$$Base;
  unsigned int DTCMLimit = (unsigned int) &Image$$ARM_LIB_STACK$$ZI$$Limit;

  ARM_MPU_SetRegion(3UL,
    ARM_MPU_RBAR(DTCMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN),
    ARM_MPU_RLAR(DTCMLimit, 1UL)
  );

  /* Set region 4 SRAM with Attr 0 */
  unsigned int RAMAddr = (unsigned int) &Image$$RW_RAM$$Base;
  unsigned int RAMLimit = (unsigned int) &Image$$RESET_MSP_STACK$$ZI$$Limit;

  ARM_MPU_SetRegion(4UL,
    ARM_MPU_RBAR(RAMAddr, ARM_MPU_SH_NON, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN),
    ARM_MPU_RLAR(RAMLimit, 0UL)
  );

  /* Enable MPU with all region definitions and background regions for privileged access. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

  return 0;
}

