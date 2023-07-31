# Example Project - priority-boost-types

This example demonstrates the function of BASEPRI. BASEPRI acts as a threshold, so that when privileged code sets BASEPRI to a higher priority, a lower value than the current execution priority, additional interrupts are masked if they have priority value greater than or equal to BASEPRI. In some circumstances, BASEPRI is useful to prevent higher priority interrupts from preempting a critical section of code.

To guarantee that the example works, the same versions of the tools must be used. The example may work using other versions of the tools but it is not guaranteed. This example project was created, built and run using:

- Arm Development Studio 2022.2
- Arm Compiler for Embedded 6
- Fast Models Fixed Virtual Platforms (FVP) 11.18
- CMSIS 5.9.0 (available in [GitHub repository](https://github.com/ARM-software/CMSIS_5))

## Purpose and scope

This example demonstrates how to use BASEPRI register setting and its effect on exceptions.


More details about this example can be found in Chapter:Use-Case-Examples of [Armv8-M Exception Model User Guide](https://developer.arm.com/documentation/107706/latest/)

## Building the example

This example can be built with Arm Compiler for Embedded 6 using the supplied Eclipse project.

To import this project, follow the guidelines in the section ["Import an existing Eclipse project" in the Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469/2022-1/Projects-and-examples-in-Arm-Development-Studio/Importing-and-exporting-projects/Import-an-existing-Eclipse-project?lang=en). 

To build the projects within the IDE:

1. In the Project Explorer view, select the project you want to build.
2. Select Project → Build Project.

This example is intended to be built with Arm Compiler for Embedded 6. If you wish to modify and rebuild the example, you must use Arm Compiler for Embedded 6 to rebuild it.


## Running the example

The executable is intended for running on an Armv8-M FVP model supplied with Arm Development Studio. A ready-made launch configuration *priority-boost-types.launch* is provided.

1. Select Run → Debug Configurations....
2. Select *priority-boost-types* from the list of Generic Arm C/C++ Application configurations.
3. Click on Debug to start debugging. The executable image will be downloaded to the target and the program counter set to `main`.
4. Run the executable (press F8). Text output appears in the Target Console view.

> [NOTE]In Breakpoints view at Arm DS, you can use the "Manage Signals" feature to trap exceptions in Debugger. Code execution will stop when a selected exception occurs, so you can clearly see exactly when an exception occurs. 

Additional Material:

[Arm Development Studio Getting Started Guide](https://developer.arm.com/documentation/101469)

[Arm Development Studio User Guide](https://developer.arm.com/documentation/101470)

[Arm Development Studio Debugger Command Reference](https://developer.arm.com/documentation/101471)

## Output in Target Console:

At this example, three IRQ interrupts are enable with priority setting - IRQ2 = 0x22 > IRQ1 = 0x33 > IRQ0 = 0xFF. As setting BASEPRI as 0x10000000 (the priority between IRQ1 and IRQ0 with NVIC_PRIO_BITS = 3), we pend IRQ0 and IRQ1 firstly, and the IRQ2 will be triggered at IRQ1_Handler. The output of console show:


```
Example Project: priority-boost-types Start 
BASEPRI is set with triggering IRQ0 and IRQ1! 
We are in IRQ 1 Handler!
Setting IRQ 2 to pend 
We are in IRQ 2 Handler!
The number of the highest priority pending exception is 16 
There is more than one active exception. 
The number of the highest priority active exception is 18 
The number of the highest priority pending exception is 16 
There is only one active exception. 
The number of the highest priority active exception is 17 
The number of the highest priority pending exception is 16 
BASEPRI is clear! 
We are in IRQ 0 Handler!
There is only one active exception. 
The number of the highest priority active exception is 16 
Example Project: priority-boost-types End 
```

The priority of IRQ1 is higher than BASEPRI and the priority of IRQ0 is lower than BASEPRI, so IRQ0 will be masked and pended. As we trigger IRQ2 at IRQ1_Handler because the priority of IRQ2 is higher than BASEPRI and IRQ1's, so it will preempt the IRQ1 handling.

As the result shows, when the execution is in IRQ2 handler, there is more than one active exception and the highest priority active exception is IRQ2, with only one pending exception IRQ0. After handling IRQ2, the program is going to handle IRQ1, then it will print the active and pending status of exception at IRQ1 handler.

As we clear the BASEPRI, the rest pending exception will be handled following the value of preemptive priority. The final output lines mean that IRQ0 is active and is going to be handled.
