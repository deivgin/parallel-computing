# Bubble sort parallel implementation

## Motivation

The purpose of the assignment is to design and implement efficient parallel algorithm for bubble sort algorithm using
shared memory multithreaded parallel system and experimentally investigate the speedup and scalability of the
application.

## Problem

## Experiment:

### Sequential bubble sort:

Time taken to sort with 1000 dataset: 0.005348 seconds
Time taken to sort with 10000 dataset: 0.428394 seconds
Time taken to sort with 100000 dataset: 41.6731 seconds

### Parallel with 2 cores

Time taken to sort with 1000 dataset: 0.017565 seconds
Time taken to sort with 10000 dataset: 0.451674 seconds
Time taken to sort with 100000 dataset: 21.1798 seconds

### Parallel with 4 cores

Time taken to sort with 1000 dataset: 0.043706 seconds
Time taken to sort with 10000 dataset: 0.611198 seconds
Time taken to sort with 100000 dataset: 16.4869 seconds

### Parallel with 8 cores

Time taken to sort with 1000 dataset: 0.103566 seconds
Time taken to sort with 10000 dataset: 1.0967 seconds
Time taken to sort with 100000 dataset: 19.4803 seconds