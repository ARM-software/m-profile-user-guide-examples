# TCM_implement

This example demonstrates how to configure and use the Tightly Coupled Memory(TCM). TCM provides guaranteed low-latency memory access that the core can use without the unpredictability of access time with cached main memory. At this example, vector table and exception handlers which need deterministic timing and cannot wait for cache misses can be put at TCM.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example shows how to create software that uses TCMs for latency critical code, while the rest of the code is placed in cacheable memory. Cortex-M55 is the target used for this example, as the Cortex-M55 processor can optionally include Tightly Coupled Memories (TCMs).

- The code tries to represent a very simplified control system for a deposit, which controls filling and emptying the deposit depending on the volume and the speed used when stirring the content of the deposit.
- The code that controls the stirring is part of `main` and is not latency critical, so it uses cacheable memory 
- The code that controls filling and emptying the deposit is latency critical code. Therefore, the code is placed in ITCM and the variables used are placed in the DTCM. This code corresponds to the SysTick handler also at TCM. The volume of the deposit is checked and filled at a defined interval, when the SysTick interrupt is triggered.
- The MPU is programmed so the regions for the ITCM and DTCM in the scatter file are non-cacheable, while the rest of the memory is cacheable.

An execution flow chart for this example project is here shown:

![Execution Flow Chart](flowchart.png)

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.

The executable is intended for running on an Arm Cortex-M55 FVP model supplied with Arm Development Studio.

A ready-made launch configuration *TCM_implement.launch* is provided.

## Project structure

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

The source files structure of this example project is here shown:

```
│   check_is_full.c
│   main.c
│   mpu_defs.h
│   mpu_prog.c
│   mpu_prog.h
│   Excep_prog.c
│   Excep_prog.h
└───RTE
    │   RTE_Components.h
    └───Device
        └───ARMCM55
                ARMCM55_ac6.sct
                startup_ARMCM55.c
                system_ARMCM55.c
```

- **main.c**: Sets the MPU, enables caches, sets up SysTick exception. It contains a loop with non-latency critical code for controlling the stirring of the deposit. 
- **mpu_prog.c**: MPU regions programming.
- **Excep_prog.c**: Relocation of vector table. For new vector table, it locates at ITCM.
- **mpu_defs.h**: Definitions for MPU region attributes.
- **check_is_full.c**: Latency critical function that checks deposit level to set a flag to stop or continue filling the deposit. It also contains the SysTick handler with the latency critical code to control filling of the deposit.
- **RTE/Device/ARMCM55/startup_ARMCM55.c**: Vector table, interrupt/exception handlers. Reset handler in which ITCM and DTCM are enabled and caches are invalidated before branching to `main`.
- **RTE/Device/ARMCM55/ARMCM55_ac6.sct**: Scatter file.
- **RTE/Device/ARMCM55/system_ARMCM55.c**: Target definitions.

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To build the supplied projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

## Running the example

1. Select Run → Debug Configurations....
2. Select *TCM_implement.launch* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to the entry point of the application.
4. Run the executable (press F8). Text output appears in the Target Console view.


Additional Material:

Arm Development Studio Getting Started Guide
https://developer.arm.com/documentation/101469

Arm Development Studio User Guide
https://developer.arm.com/documentation/101470

Arm Development Studio Debugger Command Reference
https://developer.arm.com/documentation/101471


## Output in Target Console:

This is part of the output in the Target Console view shown when running this example:

```
Example Project: TCM-implement Start
Vector table address is 0x10000000
New vector table address is 0x00000000
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
CurrNumber of checks in SysTick handler: 0 
```

From the output, the monitoring of the deposit system will be observed. For each minutes, the revolutions will be printed and if it is over the maximum revolution, it will be zero, which means we can not store anything into the system. SysTick exception is triggered to check if the deposit is full periodically. The current volume and the number of exception will be printed.

## Running the project in MPS3-Board - AN555

For executing the project at AN555 image (Cortex-M85), the memory regions need to be changed, which will follow the memory map in AN555 image. 

Once we relocate the VTOR at real board, the booting address for next execution will be changed too. If we don't connect the debugger (Arm Dstream), the booting address will be read from the board configuration, so we need to change the config file; while, the booting address is loaded by debugger, which we don't revise manually.

With the above changes, open the semihosting debug in the project, and connect with debugger, the following output will be shown in the app console:

```
Arm MPS3 FPGA Prototyping Board Test Suite
Version 1.2.6 Build date: Jun  9 2023
Copyright (C) Arm Ltd 2023. All rights reserved.

Program started.
Vector table address is 0x11000000
New vector table address is 0x10000000
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: 0 
Current revolutions per minute: 15 
Current revolutions per minute: 30 
Current revolutions per minute: 45 
Current revolutions per minute: 60 
Current revolutions per minute: 75 
Current revolutions per minute: 90 
Current revolutions per minute: 105 
Current revolutions per minute: Number of checks in SysTick handler: 0 
...
```