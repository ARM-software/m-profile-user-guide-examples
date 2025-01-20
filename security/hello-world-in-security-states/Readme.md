# Example Project - hello-world-in-security-states

This example demonstrates details on memory security configuration and protection available in Cortex-M processors with Security Extension.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.c
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.19
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))


## Purpose and scope

This example aims to show:

+  How to build a Secure image 
   * Program the SAU
   * Configure system control registers
   * Configure interrupts and exceptions for a security domain
   * Enable SecureFault and writing a basic SecureFault handler
   * Create an import library to export to the Non-secure image
   * Configure Non-secure registers
   * Call a Non-secure image
         
+ How to build a Non-secure image that can call a Secure image
                

More details about this example can be found in Armv8-M Security Extension User Guide - chapter:8 - Use case examples

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Arm Development Studio project.

To import both projects, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en).

To build the projects within the IDE:

1. In the Project Explorer view, select the Secure project(hello-world-in-security-states_s), select **Project → Build Project**.
2. In the Project Explorer view, select the Non-Secure project (hello-world-in-security-states_ns), import the interface.h and hello-world-in-security-states-CMSE-Lib.o from Secure project into Non-Secure project, follow the guidelines in the section ["Using the Import wizard"](https://developer.arm.com/documentation/101469/2022-2/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Using-the-Import-wizard?lang=en). 
   hello-world-in-security-states-CMSE-Lib.o file is added to Non-Secure project root directory.
3. In the Project Explorer view, select the Non-secure project(hello-world-in-security-states_ns), select **Project → Build Project**

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.

## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *hello-world-in-security-states.launch* is provided.

1. Select **Run → Debug Configurations...**.
2. Select *hello-world-in-security-states.launch* from the list of Generic Arm C/C++ Application configurations.
3. Click on **Debug** to start debugging. The executable image will be downloaded to the target and the program counter set to `Reset_Handler`.
4. Run the executable (press F8). Text output appears in the Target Console view.

## Program execution flow
The flow of program execution is shown below:

      Reset_Handler   (Secure) startup_ARMv81MML.c: 
            |          Set and seal MSP and PSP
            |          Initialize system, optional FPU enable, enable Loop and branch info cache
         __main        Arm C library initialization for Secure world
            |
         main         main_s.c:
                        Print message
                        Secure system Initialization
                           Program the SAU
                           Configure system control registers
                           Configure interrupts and exceptions for a security domain
                        Configure Non-secure registers VTOR_NS and MSP_NS
                        Call a Non-secure image
                           Get Non-secure reset handler from Non-secure vector table
                           Call the Non-secure reset handler to start Non-secure software application
            |
         S -> NS
            |
      Reset_Handler   (Non-secure) startup_ARMv81MML.c: Initialization of Non-secure world
            |
         __main        Arm C library initialization for Non-secure world
            |
         main         main_ns.c:
                        Print message
                        Call a simple Non-secure Callable functions
                        

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)


[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

## Output in Target Console:
Example Project: hello-world-in-security-states Start 
S: Hello World in Secure State 
NS: Hello World in Non-secure State 
S: Calling Secure function from Non-secure state
Example Project: hello-world-in-security-states End