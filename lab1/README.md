# Laboratory assignment 1

Goal of this laboratory exercise is to deepen understanding of parallel computing critical section.
To achieve this goal this task entails creating a real world computation simulation that suffers from the data race problem and how to solve it.

### Project: Logging to a file.

Imagine you have a system where multiple threads are logging messages to a file.
Without proper synchronization, the log messages could get mixed up and result in a corrupted log file.

### Correct program output

Thread 1 logging message 0
Thread 2 logging message 0
Thread 1 logging message 1
Thread 1 logging message 2
Thread 1 logging message 3
Thread 1 logging message 4
Thread 2 logging message 1
Thread 2 logging message 2
Thread 2 logging message 3
Thread 2 logging message 4

### Incorrect program output

Thread 1 logging message 0
Thread 1 logging message 1
Thread 1 logging message 2
Thread 1 logging message 3
Thread Thread 1 logging message 2 logging message 4
0
Thread 2 logging message 1
Thread 2 logging message 2
Thread 2 logging message 3
Thread 2 logging message 4

### Notes on the solution

The critical section without mutual exclusion causes data race, so we need to add a synchronization mechanism to it.
In C++ we have access to the mutex library that easily locks the critical section. I also added a custom mutex implementation.
In it, I use an atomic flag provided from atomic library, as boolean is not an atomic operation, meaning that it can be changed by other threads,
thus having the same data race problem.
