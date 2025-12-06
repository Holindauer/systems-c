# Concurrency

### Threads

Multiple threads in a single process enable multiple execution streams for the same process. If the main process spawns a new thread, it gets its own distinct stack from the parent thread. However, all threads of a process have access to the same address space. That is, threads share the same heap and PID, but get their own stack.

This feature gives rise to certain issues when multi-threading a process. For example, it is possible to incur a **race condition**. A race condition occurs when two or more threads are able to mutate the same memory location (which can be on the heap, stack, or in global memory). This causes a situation where one thread may read a value, and while it is mutating it, another thread reads the unmutated value before the other thread has the chance to write back the update. Race conditions emerge when multiple threads concurrently mutate shared data with multi-instruction operations under the assumption they are **atomic**. Atomic operations are indivisible—they execute as a single, uninterruptible operation that cannot be partially completed or interleaved with other operations. 

For example, consider the following assembly pseudocode:

    mov a b 
    add a a 1 
    mov b a

The move of b into a as well as a into b happen as distinct events compared to the increment of a. 

Say we wish to increment twice across two threads. At clock cycle 2, the increment of a in the first thread happens at the same time as the move of b into a for thread 2. When the first thread writes back the value of a to b, the second thread will be incrementing the original value of b, not the incremented value. This will cause the value of b in the write back of thread 2 to be the same as that of thread 1, only incrementing the value of b once, as opposed to twice.

    cycle   thread_1    thread_2

    1       mov a b 
    2       add a a 1   mov a b
    3       mov b a     add a a 1 
    4                   mov b a

### Locks

Locks are a tool to prevent race conditions. The most basic type of lock is a **binary lock**, also known as a **mutex** or **binary semaphore**. A mutex is a single-owner binary lock that protects a critical section of code. When a thread acquires a mutex, it gains exclusive ownership, and all other threads must wait until the original thread releases the mutex before they can acquire it and modify the protected data.


    pthread_mutex_t lock;
    pthread_mutex_lock(&lock);

    x = x + 1; // critical section

    pthread_mutex_unlock(&lock)


The code between the lock and unlock of the mutex is known as the **critical section** because it is the code that modifies shared data.

There are multiple types of locks. Another synchronization mechanism is **condition variables**, which are always used together with a mutex and a predicate (a boolean condition). A condition variable allows threads to wait until a specific condition becomes true. The waiting thread must check the predicate in a loop, as spurious wakeups can occur. When the condition changes, another thread signals the condition variable to wake up waiting threads.

    // puts thread to sleep until condition is true (must be called with mutex held)
    int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

    // wakes up one waiting thread when signal is sent
    int pthread_cond_signal(pthread_cond_t *cond);


Another type of synchronization primitive is the **semaphore**. Note that a mutex is a binary semaphore (a semaphore with value 1). A semaphore can be initialized as follows:

    #include <semaphore.h>

    sem_t s;
    sem_init(&s, 0, 1); // init s to value 1

The semaphore is an integer counter that has some value at a particular time. To interact with the semaphore, you can call 

    sem_wait(&s)

Which will decrement the semaphore value (if it's greater than 0) or block the thread until the semaphore value becomes greater than 0, then decrement it. Other threads will call 

    sem_post(&s) 

Which will increment the semaphore value and notify one waiting thread (if any) that a change has occurred and it should wake up.


A classic semaphore synchronization problem is the **dining philosophers**. Say there are 5 philosophers at dinner. The philosophers cannot eat when they speak, and because of this they need two forks to eat fast enough while listening when the others are speaking. *Note, these are not fork() calls, these are actual forks you would use to eat*. 

The difficulty is that there are only five forks, arranged between each philosopher. When it is time for a philosopher to eat, he will grab the fork to his left and right. 

    while (1) {
        think();
        getforks();
        eat();
        putforks();
    }

    int left(int p) { 
        return p; 
    }

    int right(int p) {
        return (p + 1) % 5;
    }

Each philosopher is a thread. If both the left and right fork of a philosopher is not available, he must wait until the other philosophers have finished eating for both forks to be placed down again.


    void getforks() { 
        sem_wait(forks[left(p)]); 
        sem_wait(forks[right(p)]); 
    } 
    
    void putforks() { 
        sem_post(forks[left(p)]); 
        sem_post(forks[right(p)]); 
    }

However, there is a bug in the above code. It is possible for all philosophers to simultaneously grab the fork to their left. Once this happens, each philosopher is waiting for the fork to their right, which is held by the philosopher to their right. This creates a circular dependency: philosopher 0 waits for fork 1 (held by philosopher 1), philosopher 1 waits for fork 2 (held by philosopher 2), and so on, with philosopher 4 waiting for fork 0 (held by philosopher 0). Since all philosophers are waiting and none can proceed, the system is stuck. When a circular dependency like this emerges where all threads are blocked waiting for resources held by other threads, it is called a **deadlock**.


    void getforks() { 
        if (p == 4) { 
            sem_wait(forks[right(p)]); 
            sem_wait(forks[left(p)]); 
        } else { 
            sem_wait(forks[left(p)]); 
            sem_wait(forks[right(p)]); 
        }
    }

To fix this, we can break the circular dependency by having one philosopher (philosopher 4) grab forks in the opposite order—right fork first, then left fork. This ensures that at least one philosopher can always acquire both forks, preventing the circular wait condition that causes deadlock.