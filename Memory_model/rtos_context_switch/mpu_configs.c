/*
 *
 * MIT License
 * Copyright (c) 2022-2023 ARM Limited.
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


#if defined (ARMv8MML)
#include "ARMv8MML.h"
#elif defined (ARMv8MML_DSP)
#include "ARMv8MML_DSP.h"
#elif defined (ARMv8MML_SP)
#include "ARMv8MML_SP.h"
#elif defined (ARMv8MML_DSP_SP)
#include "ARMv8MML_DSP_SP.h"
#elif defined (ARMv8MML_DP)
#include "ARMv8MML_DP.h"
#elif defined (ARMv8MML_DSP_DP)
#include "ARMv8MML_DSP_DP.h"
#else
#error device not specified!
#endif

#include "myRTOS.h"
#include "mpu_defs.h"
#include "mpu_configs.h"


MpuRegionCfg  mpuCfgA [MAX_MPU_REGIONS];
MpuRegionCfg  mpuCfgB [MAX_MPU_REGIONS];

void initMPUConfigs(){

    unsigned int unpROMAddr = (unsigned int) &Image$$UNPRIV_ROM$$Base;
    unsigned int unpROMLimit = (unsigned int) &Image$$UNPRIV_ROM$$Limit;
    unsigned int unpRAMAddr = (unsigned int) &Image$$UNPRIV_RAM$$Base;
    unsigned int unpRAMLimit = (unsigned int) &Image$$ARM_LIB_HEAP$$ZI$$Limit;

    // MPU Configuration for Thread A
    unsigned int tAROMAddr = (unsigned int) &Image$$THREAD_A_ROM$$Base;
    unsigned int tAROMLimit = (unsigned int) &Image$$THREAD_A_ROM$$Limit;
    unsigned int tARAMAddr = (unsigned int) &Image$$THREAD_A_RAM$$Base;
    unsigned int tARAMLimit = (unsigned int) &Image$$THREAD_A_RAM$$ZI$$Limit;


    for(int i = 4; i < MAX_MPU_REGIONS; i++){
        mpuCfgA[i].rlar = 0; //Enable bit set to 0
    }
    // Region 0
    mpuCfgA[0].rbar = ARM_MPU_RBAR(tAROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC);
    mpuCfgA[0].rlar = ARM_MPU_RLAR(tAROMLimit, 0UL);

    // Region 1
    mpuCfgA[1].rbar = ARM_MPU_RBAR(tARAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN);
    mpuCfgA[1].rlar = ARM_MPU_RLAR(tARAMLimit, 0UL);

    // Region 2
    mpuCfgA[2].rbar = ARM_MPU_RBAR(unpROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC);
    mpuCfgA[2].rlar = ARM_MPU_RLAR(unpROMLimit, 0UL);

    // Region 3
    mpuCfgA[3].rbar = ARM_MPU_RBAR(unpRAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN);
    mpuCfgA[3].rlar = ARM_MPU_RLAR(unpRAMLimit, 0UL);


    // MPU Configuration for Thread B
    unsigned int tBROMAddr = (unsigned int) &Image$$THREAD_B_ROM$$Base;
    unsigned int tBROMLimit = (unsigned int) &Image$$THREAD_B_ROM$$Limit;
    unsigned int tBRAMAddr = (unsigned int) &Image$$THREAD_B_RAM$$Base;
    unsigned int tBRAMLimit = (unsigned int) &Image$$THREAD_B_RAM$$ZI$$Limit;

    for(int i = 4; i < MAX_MPU_REGIONS; i++){
        mpuCfgB[i].rlar = 0;
    }

    // Region 0
    mpuCfgB[0].rbar = ARM_MPU_RBAR(tBROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC);
    mpuCfgB[0].rlar = ARM_MPU_RLAR(tBROMLimit, 0UL);

    // Region 1
    mpuCfgB[1].rbar = ARM_MPU_RBAR(tBRAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN);
    mpuCfgB[1].rlar = ARM_MPU_RLAR(tBRAMLimit, 0UL);

    // Region 2
    mpuCfgB[2].rbar = ARM_MPU_RBAR(unpROMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RO, ARM_MPU_NON_PRIV, ARM_MPU_EXEC);
    mpuCfgB[2].rlar = ARM_MPU_RLAR(unpROMLimit, 0UL);

    // Region 3
    mpuCfgB[3].rbar = ARM_MPU_RBAR(unpRAMAddr, ARM_MPU_SH_OUTER, ARM_MPU_RW, ARM_MPU_NON_PRIV, ARM_MPU_XN);
    mpuCfgB[3].rlar = ARM_MPU_RLAR(unpRAMLimit, 0UL);

}
