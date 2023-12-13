# rtos_context_switch

The goal of this example is to show a simple and easy to understand the real-time kernel context switching operations; using MPU regions concept available in Cortex-M processors. 
This goal is accomplished by having two threads A and B, that switches alternatively between them and a Systick Interrupt Service Routine(ISR) that acts as a kernel code.

This example project is created, built and run using:

- Arm Development Studio 2022.1
- Arm Compiler for Embedded 6.18
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.8.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))
- GCC Toolchain version: 10.3

## Purpose and scope

This is an example that shows how to create a very simple RTOS (Real Time Operating System) capable of dealing with context switching and thread isolation. For simplicity, only two threads, thread A and thread B, are created.

Basic context switching and thread isolation requirements considered for this example are listed below:

- Two isolated threads called as thread A and thread B will be created
- Both thread A and thread B will be executed in unprivileged mode.
- Each thread has its own dedicated process stack.
- The MPU regions are setup in such a way that the data and code corresponding to Thread A is not accessible to Thread B and vice-versa.
- A System Tick Timer (SysTick) generates interrupts to switch between threads.
- The SysTick handler acts as a real-time kernel code and is responsible for context switching. The Systick handler is also responsible for MPU reprogramming that is needed for thread isolation.
- The MPU regions for SysTick handler is setup in such a way that it neither of the threads can access the memory used by kernel code (i.e.) SysTick handler.

More details about this example can be found in Chapter: Use-Case-Examples of [Armv8-M Memory Model and MPU User Guide](https://developer.arm.com/documentation/107565/latest/)

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *rtos_context_switch.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *rtos_context_switch* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `main`.
4. Run the executable (press F8). Text output appears in the Target Console view.

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

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

## Extension - build and run example with GCC 

In addition to build with Arm compiler for embedded, the projects can also be built with GCC compiler through Arm Development Studio. Steps are shown below.

1. Import the project firstly, right-click the project, select **Properties -> C/C++ Build -> Tool Chain Editor**, switch to the GCC compiler (version 10.3.1) at Current toolchain option. 

2. At Properties menu, choose the **settings**, and make sure the following settings have been configured.
   ```
   // GCC C Compiler 10.3
   Target:  -march=armv8-m.main
   Optimization: -O1

   // GCC Assembler 10.3
   Target:  -march=armv8-m.main

   // GCC C Linker 10.3
   Image: Linker script=import the scripts/gcc_arm.ld
   Miscellaneous: other flags -march=armv8-m.main --specs=rdimon.specs 
   ```

3. Finally, select **Project → Build Project** and get the output folder `Debug`

4. To run the example at FVP, open **rtos_context_switch.launch**, at **Debugger Interface**, click **Run target initialization debugger script** and import **scripts/paddron.ds**, which makes sure the data is loaded right. 
   ```
   // paddron.ds
   set elf load-segments-at-p_paddr on
   ```
