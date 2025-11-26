# Processes and Signals

On linux, a process is a program with its own execution context. A process has its own stack, heap, address space, etc.

A child process is created by calling **fork()** from within a parent process. Calling fork() will copy the memory image of the calling process into a new process. Execution will contiue as normal from then on. Only, there will now be two versions of the same process executing concurrently. The details of what concurrent execution means in this context depends on how the operating system scheduler handles multiple processes. As such, the exact execution order of forked processes is non deterministic.

![process_graph](process_graph.png)

When fork is called, the program execution bifurcates into a **process graph**. While the exact order of execution in such a graph is non deterministic, there are certain predictable behaviors. 

For example, in the above function *fork2()*, the parent process is forked twice in succession. Before the final print statement "Bye" can be reached, two child processes must be spawned, so we know that at the very least, before "Bye" can be printed, we must see "L0" and "L1" in the terminal output.

![zombie_process](orphan_process.png)

When forking processes, sometimes the parent process may exit before the child process. This is called an **orphan process**.
In the above function *fork8*, the parent process terminates, but the child process will continue to run.

![zombie_process](zombie_process.png)

There are also **zombie processes**, which refers to when a parent process does not properly handle the exit codes of a child process after it terminates. A parent process must **reap** child processes after they exit else they will continue to occupy a process ID.

The function *fork9* above prevents a zombie process by calling **wait()**, which frees the PID of the child processes from the parent process once it has completed.