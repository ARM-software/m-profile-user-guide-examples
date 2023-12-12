# Example Project - synchronous-fault

This example triggers a UsageFault by executing a floating-point instruction while the FPU is disabled. This example also show how UsageFault exception handler can be used to fix the fault.

This example is built using:
- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))
- GCC Toolchain version:10.3

## Purpose and scope

This example aims to show:

- How to enable Floating point unit?
- How to fix a fault in UsageFault handler?

More details about this example can be found in Chapter:Use-Case-Examples of [Armv8-M Exception Model User Guide](https://developer.arm.com/documentation/107706/latest/)

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

> [NOTE]In Breakpoints view at Arm DS, you can use the "Manage Signals" feature to trap exceptions in Debugger. Code execution will stop when a selected exception occurs, so you can clearly see exactly when an exception occurs. 

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

## Output in Target Console:

At this example, the project is built at ARMv8MML_DP platform. We need to enable UsageFault and power off FPU manually. For UsageFault, it is controlled by System Handler Control and State Register (SHCSR). For FPU, the SU10 and SU11 are set in Coprocessor Power Control Register (CPPWR). When running a floating-point instruction like *VMOV*, it will trigger UsageFault like the output of console showing:


```
Example Project: synchronous-fault Start
UsageFault entered!
The floating add result is 10.000000 
Example Project: synchronous-fault End 
```

We can get the fault exception status by reading Configurable Fault Status Register (CFSR) when program enters in UsageFault Handler. Bits [31:16] in CFSR is UsageFault Status Register (UFSR). Bit [3] NOCP in UFSR shows whether a coprocessor disabled or not present error has occurred. Here, we set the SU10 and SU11 as 0x1, which illustrates all accesses to the floating-point extension and MVE is permitted to become unknown. In this case, it notices the UsageFault.
Entering the UsageFault handler, the CP10 and CP11 is re-enabled. The program will return to where the fault happens (the *VMOV* instruction) and re-execute. 
Then we will get the right results of executing FP instructions.

## Extension - build and run example with GCC 

In addition to build with Arm compiler for embedded, the projects can also be built with GCC compiler. 

   Import the project firstly, right-click the project, select Properties -> C/C++ Build -> Tool Chain Editor. We can switch to the GCC compiler at Current toolchain option. Then, accroding to the build_gcc.sh at scripts folder, we need to re-configure the build setting. Finally, Select Project → Build Project.

   To run the example at FVP, we can follow the steps of 'Running the example' section. But it is important to import the paddron.ds at scripts folder to Debugger interface, which makes sure the data is loaded right. 

   ```
   // paddron.ds
   set elf load-segments-at-p_paddr on
   ```
