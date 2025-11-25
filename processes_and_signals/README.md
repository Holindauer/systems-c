# Processes and Signals

On linux, a process is a program with its own execution context. A process has its own stack, heap, address space, etc.

A child process is created by calling **fork()** from within a parent process. Calling fork() will copy the memory image of the calling process. Execution will contiue as normal from then on. Only, there will now be two versions of the same process executing concurrently. The exact details of what concurrent execution means in this context depends on how the operating system scheduler handles multiple processes. As such, the exact execution order of forked processes is non deterministic.

