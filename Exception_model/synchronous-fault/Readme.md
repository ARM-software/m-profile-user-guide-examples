# Example Project - synchronous-fault

UsageFault as one of individual fault exceptions, can be worked around by emulating some powering off hardware functionality in software. It can handle various programming errors including undefined or unsupported instruction opcodes. This example is to show how to trigger an UsageFault by executing floating point instruction while powering off FPU by software implicitly and how to fix the fault in its handler.  

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

- Triggering a simple UsageFault by executing undefined floating-point instruction when powering off the FPU by software.
- Solving the fault in handler.

More details about this example can be found in Armv8-M Exception Handling User Guide - chapter:5 - Use case examples

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.

## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *synchronous-fault.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *synchronous-fault* from the list of Generic Arm C/C++ Application configurations.
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

At this example, the project is built at ARMv8MML_DP platform. We need to enable UsageFault and power off FPU manually. For UsageFault, it is controlled by System Handler Control and State Register (SHCSR). For FPU, the SU10 ans SU11 are set in Coprocessor Power Control Register (CPPWR). When running a floating-point instruction like *VMOV*, it will trigger UsageFault like the output of console showing:


```
Example Project: synchronous-fault Start
FPU is enable!
UsageFault entered!
The UsageFault status register:      
UFSR is 0x8 
The floating add result is 2.125000 
Example Project: synchronous-fault End 
```

We print the fault exception status by reading Configurable Fault Status Register (CFSR). Bits [31:16] in CFSR is UsageFault Status Register (UFSR). Bit [3] NOCP in UFSR shows whether a coprocessor disabled or not present error has occurred. Here, we set the SU10 and SU11 as 0x1, which illustrates all accesses to the floating-point extension and MVE is permitted to become unknown. In this case, it notices the UsageFault.
Entering the UsageFault handler, the CP10 and CP11 is re-enabled. The program will return to where the fault happens (the *VMOV* instruction) and re-execute. 
Then we will get the right results of executing FP instructions.


