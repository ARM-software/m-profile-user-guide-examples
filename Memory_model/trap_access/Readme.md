# Example Project - trap_access

This example demonstrates the functionality of the Memory Protection Unit (MPU). It should not be used as a test to validate all aspects of the MPU.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.1
- Arm Compiler for Embedded 6.18
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.8.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example aims to show:

- Basic programming of MPU regions.
- How to enable the MPU and MemManage faults?
- Trigger MemManage faults to show how memory is protected by MPU

More details about this example can be found in Chapter: Use-Case-Examples of [Armv8-M Memory Model and MPU User Guide](https://developer.arm.com/documentation/107565/latest/)

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *trap_access.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *trap_access* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `main`.
4. Run the executable (press F8). Text output appears in the Target Console view.

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

## Output in Target Console:

Consider the following scenarios:

- Scenario:1 - Perform a write to a Read-only memory. This results in MemManage fault showing following output in target console:

```
Example Project: trap_access Start
We are in the MemManage handler. 
    MemManage Fault Address Register: 
        SCB->MMFAR = 0x00000000
    MemManage Fault Status Register: 
        SCB->CFSR->MMFSR = 0x00000082
Example Project: trap_access End
```

- Scenario:2 - Read privileged memory from unprivileged state. This results in a  MemManage fault showing following output in target console

```
Example Project: trap_access Start
We are in the MemManage handler. 
    MemManage Fault Address Register: 
        SCB->MMFAR = 0x5800e000
    MemManage Fault Status Register: 
        SCB->CFSR->MMFSR = 0x00000082
Example Project: trap_access End 
```
