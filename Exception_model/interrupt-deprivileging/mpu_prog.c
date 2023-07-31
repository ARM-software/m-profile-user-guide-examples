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

/* For more specific information, refer to
 * https://github.com/ARM-software/m-profile-user-guide-examples/blob/main/Memory_model/ */

#include "mpu_prog.h"
#include <stdio.h>


/* Memory region symbols extracted from scatter file*/
unsigned int ROMAddr  = (unsigned int) &Image$$ER_ROM$$Base;
unsigned int ROMLimit = (unsigned int) &Image$$ER_ROM$$Limit;
unsigned int RAMAddr  = (unsigned int) &Image$$RW_RAM$$Base;
unsigned int RAMLimit = (unsigned int) &Image$$ARM_LIB_STACK$$ZI$$Limit;
unsigned int UnprivROMAddr  = (unsigned int) &Image$$UNPRIV_ROM$$Base;
unsigned int UnprivROMLimit = (unsigned int) &Image$$UNPRIV_ROM$$Limit;
unsigned int UnprivRAMAddr  = (unsigned int) &Image$$UNPRIV_RAM$$Base;
unsigned int UnprivRAMLimit = (unsigned int) &Image$$PSP_STACK$$ZI$$Limit;


/**
  \brief        Setup MPU regions and its memory attributes to each region
  \details      The base and limit addresses of MPU regions are set based on symbols 
                created by the linker based on the scatter file. Using this approach 
                helps in keeping the MPU configuration in sync with the addresses being
                used, and removes the need to duplicate the information.
                Configure memory attributes in Memory Attribute Indirection Registers.
  \return       0, successfully set
 */
void setMPU(){
  /* Set memory attributes in Memory Attribute Indirection Registers */
  /* Set Attribute 0 */
  ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(    /* Normal memory */
    /* Outer Write-Through non-transient with read allocate */
    ARM_MPU_ATTR_MEMORY_(1UL, 0UL, 1UL, 0UL),
    /* Inner Write-Through non-transient with read allocate */
    ARM_MPU_ATTR_MEMORY_(1UL, 0UL, 1UL, 0UL)
  ));

  /* Set region 0 - privileged ROM */
  ARM_MPU_SetRegion(0UL,
    ARM_MPU_RBAR(ROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_PRIV, ARM_MPU_EXEC),
    ARM_MPU_RLAR(ROMLimit, 0UL)
  );

  /* Set region 1 - privileged RAM */
  ARM_MPU_SetRegion(1UL,
    ARM_MPU_RBAR(RAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_PRIV, ARM_MPU_XN),
    ARM_MPU_RLAR(RAMLimit, 0UL)
  );

  /* Set region 2 - unprivileged ROM */
  ARM_MPU_SetRegion(2UL,
    ARM_MPU_RBAR(UnprivROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_PRIV, ARM_MPU_EXEC),
    ARM_MPU_RLAR(UnprivROMLimit, 0UL)
  );

  /* Set region 3 - unprivileged RAM */
  ARM_MPU_SetRegion(3UL,
    ARM_MPU_RBAR(UnprivRAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_PRIV, ARM_MPU_XN),
    ARM_MPU_RLAR(UnprivRAMLimit, 0UL)
  );

  /* Enable MemManage Faults */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

  /* Enable MPU with all region definitions and background regions for privileged access. */
  /* Exceptions are protected by MPU. */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}


/**
  \brief        Re-configure memory region with another attribute.
  \details      With the execution switching between different privileged modes,
                the MPU need to be re-configured to new attributes.
  \param [in]   PRI_ATTR privilege level /Privileged or unprivileged
 */
void config_MPU(uint32_t PRI_ATTR){
  /* Change the memory attribute of unprivileged ROM */
  ARM_MPU_SetRegion(2UL,
    ARM_MPU_RBAR(UnprivROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, PRI_ATTR, ARM_MPU_EXEC),
    ARM_MPU_RLAR(UnprivROMLimit, 0UL)
  );

  ARM_MPU_SetRegion(3UL,
    ARM_MPU_RBAR(UnprivRAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, PRI_ATTR, ARM_MPU_XN),
    ARM_MPU_RLAR(UnprivRAMLimit, 0UL)
  );

  /* Enable MPU with all region definitions and background regions for privileged access. */
  ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk | MPU_CTRL_HFNMIENA_Msk);
}
