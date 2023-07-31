# Example Project - svc-number-as-parameter

Cortex-M cores support the SuperVisor Call (SVC) instruction, which triggers an SVC exception. This is useful if the core is in unprivileged mode, but the program needs to access features of the core (e.g special registers) that only can be accessed in privileged mode. SVC instruction also has a parameter (for Thumb code 0-255) to select different SVC functions like enabling privileged mode. 
This example demonstrates how to select different functions built-in within a program by using SVC number as a parameter.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

- Executing different SVC instructions and designating SVC #number for different functions.
- Getting the stacked return state and address. 

More details about this example can be found in Chapter:Use-Case-Examples of [Armv8-M Exception Model User Guide](https://developer.arm.com/documentation/107706/latest/)

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *svc-number-as-parameter.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *svc-number-as-parameter* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `main`.
4. Run the executable (press F8). Text output appears in the Target Console view.

> [NOTE]In Breakpoints view at Arm DS, you can use the "Manage Signals" feature to trap exceptions in Debugger. Code execution will stop when a selected exception occurs, so you can clearly see exactly when an exception occurs. 

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

## Output in Target Console:

At this example, three different SVC exception are triggered in turn. Each number of SVC #<num> presents a kind of arithmetic. For instance, executing *SVC #1* will switch into *add* subroutine. The console will repeat the following output block:


```
Example Project: svc-number-as-parameter Start 
The return address is 0x00001140 
The stacked return state is 0xfffffff9 
svc_number is 1 
The result of R0+R1 is 12!
The first routine is completed !
The return address is 0x00001156 
The stacked return state is 0xfffffff9 
svc_number is 2 
The result of R0*R1 is 36!
The second routine is completed !
The return address is 0x00001166 
The stacked return state is 0xfffffff9 
svc_number is 3 
The result of R0 mod R1 is 6!
The third routine is completed !
Example Project: svc-number-as-parameter End 
```

The SVC exception causes the processor to create a stack frame on the currently active stack, to preserve information about the state of the currently executing code stream. Basically, callee registers will be saved into stack automatically, which can be judged by EXC_RETURN value. 

The EXC_RETURN value is stored at LR register. Bit [5] is the value that indicates whether the default stacking rule should apply. Bit [3] is the mode to return to, bit [2] is stack pointer selection, bit [1] is reserved bit and bit [0] indicated the security domain the exception is taken to. In this example, EXC_RETURN value is 0xffff_fff9, which means that SVC is taken from secure world, Main stack pointer is used and thread mode will be returned to and callee registers have been stacked default.

The return address is also stacked into SP, which can be read as an argument. Then the SVC number can be extracted from the lower byte of SVC opcode at this address. Based on SVC number, the program will switch to execute subroutine. The parameter "1" is to select add operation. The parameter "2" is to choose sub operation. The parameter "3" is to select multiply operation.

Then, we can abstract the value stacked into SVC_STK_FRAME, execute each subroutine and print the related result.


