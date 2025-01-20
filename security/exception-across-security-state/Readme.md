# Example Project - exception-across-security-state

This example demonstrates some use cases on exceptions happened at different secure mode and how the Arm v8-M system handle these exceptions. 

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.c
- Arm Compiler for Embedded 6.18
- Fast Models Fixed Virtual Platforms (FVP) 11.19
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope
This example demonstrates:

+  How to build a Secure image 
    * Program the SAU
    * Configure system control registers
    * Configure interrupts and exceptions for a security domain
    * Enable SecureFault and writing a basic SecureFault handler
    * Implement secure functions that can be called by Non-secure side:
        * ns_callable_pend_IRQs:  
            * pend interrupts in specific order

        * Print_PendAndActiveStatus:  
            * gets the status of IRQs by reading the ICSR register and print the active and pending status of exceptions

    * Create an import library to export to the Non-secure image
    * Configure Non-secure registers and Non-secure vector table
    * Call a Non-secure image
    
+ How to build a Non-secure image
    * Set priority and enable Non-secure interrupt
    * Re-write the Non-secure exception handler
    * call ns_callable_pend_IRQs
    * When handling the Non-secure interrupt 4, try to access the secure data and trigger SecureFault

More details about this example can be found in Armv8-M Security Extension User Guide - chapter:8 - Use case examples.
 
## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Arm DS project.

To import this project, follow the guidelines in the section ["Import Existing projects into Workspace" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-2/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the Secure project (exception-across-security-state_s) and select Project → Build Project.
2. In the Project Explorer view, select the Non-Secure project (exception-across-security-state_ns) and select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.

## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *exception-across-security-state.launch* is provided.

1. Select Run ->  Debug Configurations....
2. Select *exception-across-security-state.launch* from the list of Generic Run -> Debug Configurations ->Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `Reset_Handler`.
4. Run the executable (press F8). Text output appears in the Target Console view.

Additional Material:

Arm Development Studio Getting Started Guide
https://developer.arm.com/documentation/101469

Arm Development Studio User Guide
https://developer.arm.com/documentation/101470

Arm Development Studio Debugger Command Reference
https://developer.arm.com/documentation/101471

## Output in Target Console:
Example Project: exception-across-security-state Start
S: Hello World in Secure State 

NS: Case1: start !     
S: The number of the highest priority pending exception is 16 
S: There is only one active exception. 
S: The number of the highest priority active exception is 17 
NS: There is only one active exception. 
NS: The number of the highest priority active exception is 16 

NS: Case2: start !     
NS: The number of the highest priority pending exception is 19 
NS: There is only one active exception. 
NS: The number of the highest priority active exception is 18 
S: There is only one active exception. 
S: The number of the highest priority active exception is 19 

NS: Case3: start !     
We are accessing secure memory 
S: There is more than one active exception. 
S: The number of the highest priority active exception is 7 
S: SecureFault occurred 
S: SecureFault address Register: 
  SCB->SFAR = 0x10000000
S: SecureFault status Register: 
  SCB->SFSR = 0x00000048
-------------------------------------------------------
