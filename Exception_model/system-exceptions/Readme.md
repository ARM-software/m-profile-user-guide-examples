# Example Project - system-exceptions

This example demonstrates how to generate built-in system exceptions that are commonly used in OS environment such as PendSV and SysTick.The SysTick interrupt is enabled that includes an instance of the SysTick timer if privileged software sets the ENABLE and TICKINT bits in the SysTick Control and Status Register, SYST_CSR. The PendSV exception is invoked by privileged software setting ICSR.PENDSEVSET. The intention is to achieve task switching in a multi-tasking environment. SysTick exception provides a periodic interrupt triggers the thread scheduler that is used to allocate chunks of execution time to a thread, while PendSV is to performs the actual context switch between threads.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6.18
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

- Triggering a SysTick exception when the time counter is overflow.
- Generating a PendSV exception by configuring ICSR register.
- Observing the status of built-in system exceptions.

More details about this example can be found in Armv8-M Exception Handling User Guide - chapter:5 - Use case examples

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *system-exceptions.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *system-exceptions* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `main`.
4. Run the executable (press F8). Text output appears in the Target Console view.

Additional Material:

Arm Development Studio Getting Started Guide
https://developer.arm.com/documentation/101469

Arm Development Studio User Guide
https://developer.arm.com/documentation/101470

Arm Development Studio Debugger Command Reference
https://developer.arm.com/documentation/101471

## Output in Target Console:

The console will repeat the following output block:

```
Example Project: system-exceptions Start
We are in SysTick_Handler! 
The pending and active status are 
        SCB->ICSR = 0x0000080f
The pending and active status are 
        SCB->ICSR = 0x1000e80f
We are in SysTick_Handler end! 
We are in PendSV_Handler! 
The pending and active status are 
        SCB->ICSR = 0x0000080e
Example Project: system-exceptions End
```

For ICSR register, bits [8:0] show active status, bits [20:12] show the exception number of the highest priority pending and enabled interrupt, bit [26] shows SysTick pending status, bit [28] shows PendSV pending status. 

The first line of output - SCB->ICSR = 0x0000_080f shows the SysTick exception is active with the exception number 15, and bit [11] shows that there is only one exception active. 

We generate a PendSV exception at SysTick handler by set ICSR.PENDSVSET bit (bit [28]) and the priority of PendSV is lower than SysTick. PendSV will be pending and the program will go on handling the SysTick exception. The output - SCB->ICSR = 0x1000_e80f shows the SysTick exception is active and there is another exception pending which the number is 14. After finishing the SysTick, the program will handle the number 14 exception - PendSV.

In PendSV_Handler, the status shows the active exception is number 14 and there is only one exception active. 
