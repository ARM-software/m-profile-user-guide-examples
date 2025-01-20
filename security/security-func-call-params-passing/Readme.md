# Example Project - security-func-call-params-passing

This example demonstrates some use cases for function calls with parameter passing across a security boundary within an Armv8-M processor configured with the Security Extension.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was tested using:

- Arm Development Studio 2022.c
- Arm Compiler for Embedded 6
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
        * ns_callable_fn1:  
            * support four integer parameters
            * add the received parameters 
            * return the result

        * ns_callable_fn2:  
            * support a structure pointer as an input parameter that points to a structure with more than four elements
            * call a CMSE intrinsic to check the structure read permission from Non-secure state
            * sum up the structure elements
            * return the result

        * ns_callable_init: 
            * support a function pointer type parameter
            * calls a CMSE intrinsic to create a Non-secure function pointer with the lease significant bit (LSB) cleared
            * saves the Non-secure function pointer to a global callback function pointer (callback_NS)

        * ns_callable_fn3:  
           * supports a data pointer type and length parameters
           * call a CMSE intrinsic to check the address range has read permission from Non-secure state
           * process data block after checking
           * call the Non-secure callback function to pass the processing result to Non-secure side    
    * Create an import library to export to the Non-secure image
    * Configure Non-secure registers
    * Call a Non-secure image
    
+ How to build a Non-secure image
    * call ns_callable_fn1 with four integer arguments and print the function return value
    * call ns_callable_fn2 with a structure pointer argument that points to a structure with more than four elements. Then print value returned from the secure function
    * implement func_ns as a Non-secure function that is passed one integer argument, and print this value
    * call ns_callable_init with the Non-secure function pointer (func_ns) as an input argument
    * call ns_callable_fn3 with a data pointer and length arguments

More details about this example can be found in Armv8-M Security Extension User Guide - chapter:8 - Use case examples.

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Arm Development Studio  project.

To import both projects, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en).

To build the projects within the IDE:

1. In the Project Explorer view, select the Secure project (security-func-call-params-passing_s) and select **Project** → **Build Project**.
2. In the Project Explorer view, select the Non-Secure project (security-func-call-params-passing_ns), import the interface.h and security-func-call-params-passing-CMSE-Lib.o from Secure project into Non-Secure project, follow the guidelines in the section ["Using the Import wizard"](https://developer.arm.com/documentation/101469/2022-2/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Using-the-Import-wizard?lang=en).
   security-func-call-params-passing-CMSE-Lib.o is added to Non-Secure project root directory. 
3. Build the Non-secure project, select **Project** → **Build Project**.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *security-func-call-params-passing.launch* is provided.

1. Select **Run** ->  **Debug Configurations...**.
2. Select *security-func-call-params-passing.launch* from the list of Generic **Run** -> **Debug Configurations** ->**Arm C/C++ Application configurations**.
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
                        Call ns_callable_fn1 and print the function return value
                        Call ns_callable_fn2 and print value returned from the secure function
                        call ns_callable_init with the Non-secure function pointer (func_ns) as an input argument
                        call ns_callable_fn3 with a data pointer and length as input arguments

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

[Arm Compiler for Embedded User Guide](https://developer.arm.com/documentation/100748/latest/)

[Fast Models Reference Guide](https://developer.arm.com/documentation/100964/1119/Microcontroller-Prototyping-System-2/MPS2---memory-maps/MPS2---memory-map-for-models-with-the-Armv8-M-additions?lang=en)

[Arm®v8-M Security Extensions - Requirements on Development Tools ](https://arm-software.github.io/acle/cmse/)

## Output in Target Console:
Example Project: security-func-call-params-passing Start
S: Hello World in Secure State 
NS: Hello World in Non-secure State 
NS: call Secure function
NS: get add result from Secure side: 1 + 2 + 3 + 5.800000 = 11.800000
NS: call Secure function with more input parameters
NS: get add result from Secure side: 1 + 2 + 3 + 4 + 5 = 15
NS: call Secure function with Non-secure function pointer as input parameter 
NS: call Secure function with Non-secure data pointer as input parameter 
S: check Non-secure permission to read the data region 
S: process Non-secure data in Secure side
NS: Non-secure callback function get Secure processing result = 15
Example Project: security-func-call-params-passing End