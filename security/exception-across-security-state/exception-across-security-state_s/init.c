/*
 *
 * MIT License
 *
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
#include <stdio.h>
#include <stdlib.h>
#include <arm_cmse.h>
#if defined (ARMv81MML_DSP_DP_MVE_FP)
  #include "ARMv81MML_DSP_DP_MVE_FP.h"
#else
  #error device not specified!
#endif
#include "partition_ARMv81MML.h"
#include "init.h"
#include "interface.h"


/**
  \brief        Configure Exceptions and interrupts
  \details      Target selected IRQ0, IRQ2 and IRQ4 to non-secure with each priority
                Set all other interrupts to secure
                Set up SecureFault priority and enable SecureFault
 */
void Exception_Config()
{
  /* Set IRQ0, IRQ2 and IRQ4 as non-secure interrupt */
  NVIC->ITNS[0] = 0x15;

  /* Set priority to exceptions  */
  NVIC_SetPriority(Interrupt1_IRQn, 0x40);
  NVIC_SetPriority(Interrupt3_IRQn, 0x85);
  NVIC_SetPriority(SecureFault_IRQn, 0x00);
  NVIC_SetPriority(MemoryManagement_IRQn, 0x00);
  NVIC_SetPriority(BusFault_IRQn, 0x00);

  /* Enable secure exceptions */
  NVIC->ISER[0] |= 1 << (uint32_t)Interrupt1_IRQn|
                   1 << (uint32_t)Interrupt3_IRQn;

  /* Enable MemManage Fault, SecureFault and BusFault*/
  SCB->SHCSR = SCB_SHCSR_SECUREFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk |
               SCB_SHCSR_BUSFAULTENA_Msk;
}

/**
  \brief        Configure system control registers
  \details      Set up SLEEPDEEP, SYSRESETREQ are only configured from Secure state
                Enable Non-secure access to FPU
                Set up FPU context control register and power control register
                Target exception(BusFault,HardFault, NMI) to Secure state
                Non-secure exceptions are de-prioritized
 */
void System_Config()
{

  /* SCB->SLEEPDEEP bit is only configurable from the Secure state */
#if defined (SCB_CSR_AIRCR_INIT) && (SCB_CSR_AIRCR_INIT == 1U)
  SCB->SCR   = (SCB->SCR   & ~(SCB_SCR_SLEEPDEEPS_Msk    )) |
               ((SCB_CSR_DEEPSLEEPS_VAL     << SCB_SCR_SLEEPDEEPS_Pos)     &
               SCB_SCR_SLEEPDEEPS_Msk);

  /* Set Application Interrupt and Reset Control Register as below:
   * SYSRESETREQ accessible from Secure state
   * Non-secure exceptions are de-prioritized
   * BusFault, HardFault, and NMI are Secure
    */
  SCB->AIRCR = (SCB->AIRCR & ~
               (SCB_AIRCR_VECTKEY_Msk       | SCB_AIRCR_SYSRESETREQS_Msk   |
                SCB_AIRCR_BFHFNMINS_Msk     | SCB_AIRCR_PRIS_Msk          ))|
               ((0x05FAU                    << SCB_AIRCR_VECTKEY_Pos)      &
                SCB_AIRCR_VECTKEY_Msk)      |
               ((SCB_AIRCR_SYSRESETREQS_VAL << SCB_AIRCR_SYSRESETREQS_Pos) &
                SCB_AIRCR_SYSRESETREQS_Msk) |
               ((SCB_AIRCR_PRIS_VAL         << SCB_AIRCR_PRIS_Pos)         &
                SCB_AIRCR_PRIS_Msk)         |
               ((SCB_AIRCR_BFHFNMINS_VAL    << SCB_AIRCR_BFHFNMINS_Pos)    &
                SCB_AIRCR_BFHFNMINS_Msk);
#endif

#if (((defined (__FPU_USED) && (__FPU_USED == 1U))              || \
    (defined (__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE > 0))) && \
    (defined (TZ_FPU_NS_USAGE) && (TZ_FPU_NS_USAGE == 1U)))

  /* Enable Non-secure access to Floating-point Extension */
  SCB->NSACR = (SCB->NSACR & ~(SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk)) |
               ((SCB_NSACR_CP10_11_VAL << SCB_NSACR_CP10_Pos) &
               (SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk));

  /* Set Floating-Point Context Control Register as below:
   * Treat Floating-point registers as Secure
   * CLRONRET is read-only from Non-secure state
   * Clear floating-point caller saved registers on exception return is enabled
   */
  FPU->FPCCR = (FPU->FPCCR & ~(FPU_FPCCR_TS_Msk | FPU_FPCCR_CLRONRETS_Msk |
                FPU_FPCCR_CLRONRET_Msk)) |
               ((FPU_FPCCR_TS_VAL        << FPU_FPCCR_TS_Pos       ) &
                 FPU_FPCCR_TS_Msk       ) |
               ((FPU_FPCCR_CLRONRETS_VAL << FPU_FPCCR_CLRONRETS_Pos) &
                 FPU_FPCCR_CLRONRETS_Msk) |
               ((FPU_FPCCR_CLRONRET_VAL  << FPU_FPCCR_CLRONRET_Pos ) &
                 FPU_FPCCR_CLRONRET_Msk );

#endif

}

/**
  \brief        Secure system initialization
  \details      Set up SAU regions
                Configure system control registers
                Configure Exceptions and interrupts
 */
void Secure_System_Init()
{
  TZ_SAU_Setup();
  System_Config();
  Exception_Config();
  __DSB();
  __ISB();
}


/**
  \brief        Secure IRQ1 handling
  \details      Call the Print_PendAndActiveStatus() to get the active
                and pending status of IRQs
 */
extern void Interrupt1_Handler(void) {
  Print_PendAndActiveStatus();
}


/**
  \brief        Secure IRQ3 handling
  \details      Call the Print_PendAndActiveStatus() to get the active
                and pending status of IRQs
 */
extern void Interrupt3_Handler(void) {
  Print_PendAndActiveStatus();
}


/**
  \brief        SecureFault handling
  \details      Print active and pending status of exceptions
                Print SecureFault address & status register
                Write 1 to clear fault status
                Exit program for demo purpose
 */
extern void SecureFault_Handler(void) {
  Print_PendAndActiveStatus();

  printf("%s: SecureFault occurred \n", get_state);
  printf("%s: SecureFault address Register: \n  SCB->SFAR = 0x%08x\n", get_state, SCB->SFAR );
  printf("%s: SecureFault status Register: \n  SCB->SFSR = 0x%08x\n",get_state,
        (0x000000FF)&(SCB->SFSR));
  SCB->SFSR |= 0xFFFFFFFF;

  /* Once a secure fault has been triggered, it's not safe to return the context
   * that created secure fault. With exit, the exception handler will stop. */
  exit(0);
}

/**
  \brief        $Sub$$main() in Arm Compiler 6 performs tasks immediately before the main
                application execution
  \details      Set up PSP stack and PSP limit
                Seal PSP stack
                Switch to use PSP stack
                Call original main
  */

#if defined(__ARMCC_VERSION)
extern void $Super$$main(void);

void $Sub$$main(void)
{
  __set_PSP((uint32_t)(&INITIAL_PSP));
  __set_PSPLIM((uint32_t)(&PSP_STACK_LIMIT));
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __TZ_set_STACKSEAL_S((uint32_t *)(&PSP_STACK_SEAL));
#endif
  __set_CONTROL( __get_CONTROL( ) | CONTROL_SPSEL_Msk );
  $Super$$main();
}

#elif defined ( __GNUC__ )
extern void __real_main();
/* __wrap_main() in GNU Compiler Collection performs tasks immediately before the main
 * application execution */
void __wrap_main()
{
  __set_PSP((uint32_t)(&INITIAL_PSP));
  __set_PSPLIM((uint32_t)(&PSP_STACK_LIMIT));
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
  __TZ_set_STACKSEAL_S((uint32_t *)(&PSP_STACK_SEAL));
#endif
  __set_CONTROL( __get_CONTROL( ) | CONTROL_SPSEL_Msk );
  __real_main();
}
#else
  #warning Not supported compiler type
#endif

