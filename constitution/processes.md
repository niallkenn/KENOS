Process: An independent execution context with its own virtual address space and kernel state.
Address space: Each process owns a private user address space and shares the kernel's higher-half mapping.
Privilege: User code executes in Ring 3; kernel code executes in Ring 0.
Creation: Processes are created through kernel process-management primitives.
Execution: Each process contains one or more threads of execution.
Context: A context contains the CPU state required to resume execution.
Kernel stack: Every thread has a dedicated kernel stack.
Scheduling: Runnable processes/threads are selected by the scheduler.
Blocking: A blocked process is removed from the runnable set until its wait condition is satisfied.
Termination: A terminated process releases its resources and remains available for parent observation until reaped.
Unix model: The eventual process interface will provide fork, exec, wait, and exit semantics.