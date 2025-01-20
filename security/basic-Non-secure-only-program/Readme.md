# Example Project - basic-Non-secure-only-program

This example demonstrates how only a small portion of secure boot code is required to switch the Cortex-M processors with TrustZone support to Non-secure state.
The boot code contains some initializations and secure settings, and these are done before the C library gets initialized.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.c
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.19
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

Build Secure image 
        Basic programming of SAU
        Set up required system configuration in Secure side
        Launch Non-secure image from Secure side 
Build Non-secure image
        Access Non-secure address.

More details about this example can be found in Armv8-M Security User Guide - chapter:8 - Use case examples

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Arm DS project.

To import projects, follow the guidelines in the section ["Import Existing projects into Workspace" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the Secure project you want to build.
2. Select Project → Build Secure Project first -> Build Non-secure project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *basic-Non-secure-only-program_m55_FVP.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *basic-Non-secure-only-program_m55_FVP.launch* from the list of Generic Arm C/C++ Application configurations.
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
S: hello from secure world 
NS: hello from non-secure world 

