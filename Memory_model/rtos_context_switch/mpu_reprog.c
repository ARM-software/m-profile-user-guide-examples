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

#include "mpu_defs.h"
#include "threadDefs.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


/**
  \brief        Reconfigure MPU with new set of memory attributes
  \details      Reprogram Base and limit address of MPU regions with new configuration
  \return       0, successfully set
 */

int reprogMPU(MpuRegionCfg  *mpuConf){

    for(unsigned int i = 0; i < MAX_MPU_REGIONS; i++){

        ARM_MPU_SetRegion(i,
                mpuConf[i].rbar,
                mpuConf[i].rlar
        );

    }

    __DSB();

    return 0;
}

