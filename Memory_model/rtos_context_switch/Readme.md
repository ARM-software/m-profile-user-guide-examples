<!--
SPDX-FileCopyrightText: Copyright 2022 Arm Limited <open-source-office@arm.com>
SPDX-License-Identifier: MIT
-->


# Example Project - rtos_context_switch

This example demonstrates the functionality of the Memory Protection Unit (MPU). It should not be used as a test to validate all aspects of the MPU.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.1
- Arm Compiler for Embedded 6.18
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.8.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This is an example that shows how to create a very simple RTOS (Real Time Operative System) capable of dealing with context switching and thread isolation. For simplicity, only two threads, thread A and thread B, are created.

In this example:

- Two threads, thread A and thread B, are created. 
- Thread A and thread B are executed in unprivileged mode.
- Data and code corresponding to one of the threads is not accessible to the other thread and vice-versa.
- Each thread has its own process stack. The process stack pointer (PSP) for the corresponding thread is used when running a thread. 
- A System Tick Timer (SysTick) generates interrupts to switch between threads.
- The SysTick handler is responsible for context switching and MPU reprogramming. Note that the Systick handler is always executed in privileged mode and uses the main stack pointer (MSP). MPU reprogramming is needed for process isolation between threads.
- A scatter file is used to place code and data in specific memory areas of the device. The symbols used  in the scatter-file to describe the different load and execution regions are used when programming and reprogramming the MPU regions.

More elaborate details about this example can be found in [Armv8-M Memory Model and MPU User Guide - Use case examples](https://developer.arm.com/documentation/107565/0100/Use-case-examples/rtos-context-switch).


## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *m33connection.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *m33connection* from the list of Generic Arm C/C++ Application configurations.
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

This is part of the output in the Target Console view shown when running this example:

```
Pentagonal number: 2262 

Pentagonal number: 2380 

Pentagonal number: 2501 

...

Fibonacci number: 0 

Fibonacci number: 1 

Fibonacci number: 2 

Fibonacci number: 3 

...

Pentagonal number: 2752 

Pentagonal number: 2882 

Pentagonal number: 3015 

Pentagonal number: 3151 

...

Fibonacci number: 8 

Fibonacci number: 13 

Fibonacci number: 21 

Fibonacci number: 34  
```

### Triggering a MemManage fault {.reference}

If you uncomment the following code in *threadB.c*:

```
// Uncomment to trigger MemManage fault
threadA_stk[1] = 0x0;
```

thread B tries to write on thread A stack and that triggers a MemManage fault. The code will branch to the MemManage handler, which prints a message:

```
We are in the MemManage handler. 
    MemManage Fault Address Register: 
        SCB->MMFAR = 0x21000010
    MemManage Fault Status Register: 
        SCB->CFSR->MMFSR = 0x00000082
```

- The `MMFAR` register shows the address causing the MemManage fault.
- The `MMFSR` register provides the following information of the MemManage fault, from the value `0x82`:
	- The processor attempted a load or store at a location that does not permit the operation.
	- `MMFAR` holds a valid fault address.



