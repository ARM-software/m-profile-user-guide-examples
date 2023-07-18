# Example Project - context-switch-fp

At an OS, Round-robin scheduling is usually used to arrange task equally by setting time slices. The time slice is defined by a special timer - SysTick which is a free-run timer to produce a periodical interrupt. At SysTick exception, PendSV with lower priority will be triggered to achieve the context switching so that move to a next task. Typically, R4-R11 will be saved at context switching. However, when FPU is enabled with addition of 34 registers, the context of FPU also needs to be stacked when switching tasks. Since FPU registers are not often used in ISRs, there is an optimization - Lazy Floating-point state preservation, that can be enabled which defers the actual saving of the FPU registers on the stack until a floating point instruction is used in the exception. So that, interrupt latency can be reduced by the push and pop these registers, which is important in the RTOS. 

This example demonstrates how to achieve the tasks switching by SysTick exception and how to complete the FPU context saved with Lazy Floating-point state preservation.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

- SysTick as a timer of scheduler
- FPU context switching

More details about this example can be found in Armv8-M Exception Handling User Guide - chapter:5 - Use case examples

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.

## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *context-switch-fp.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *context-switch-fp* from the list of Generic Arm C/C++ Application configurations.
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

The execution flow can be seen at the following figure.
![Execution flow chart for context switching for FPU](context_switch_fp_process.png "Execution flow chart for context-switch-fp example")

When SysTick exception happens, the state context including R0-R3, R12, LR, PC and xPSR will be stacked into PSP automatically and the space for FPU state context (S0-S15, FPSCR and VPR if MVE is implemented) will be reserved. At SysTick_handler, we trigger a PendSV with lower priority to complete context switching. So the whole process will be executed periodically.  

The context switching is implemented in the PendSV_Handler() like the following figure.
![Execution flow chart for pendsv](PendSV_Handler.png "PendSV Execution flow chart for context-switch-fp example")
The additional context (R4-R11 and S16-S31) will be saved manually. With pushing context into stack, the PSP is decreased, so the pointer address will be recorded finally in order to executing the next context switching. Then, select the next task and restore its context step by step.


Showing the following output in target console:

```
Example Project: context-switch-fp Start
Start Scheduler ! 

we are in SysTick handler ! 

The diagonal of a square with side=1: 0.000000 

The diagonal of a square with side=2: 1.414214 

The diagonal of a square with side=3: 2.828427 

The diagonal of a square with side=4: 4.242641 
we are in SysTick handler ! 

The area of a circle with r=2: 3.141593 

The area of a circle with r=3: 12.566371 

The area of a circle with r=4: 28.274334 

The area of a circle with r=5: 50.265484 

...
 ```

The first output of SysTick handler shows the program switches to task1 from main thread. Then, like a Round-Robin scheduling, the two task with main thread are done in a circular order. 
 
 ...[Note]: There is no guarantee that the messages in output stream are completed. You mostly see the messages are interrupted into pieces because different tasks will try to use one sharing output.