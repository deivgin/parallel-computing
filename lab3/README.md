# Bubble sort parallel implementation

## Motivation

The purpose of the assignment is to design and implement efficient parallel algorithm for bubble sort algorithm using
shared memory multithreaded parallel system and experimentally investigate the speedup and scalability of the
application.

## Problem

## Experiment:

In this experiment I wanted to find out what would be the average run time for each defined bubble sort algorithm.

Experiment details:

- Each algorithm was run 100 times.
- Data set was 10000
- For parallel algorithms 8 cores were used

### Bubble sort

Average time taken to sort with 10000 dataset: 0.420109 seconds

### Bubble sort with swapped flag:

Average time taken to sort with 10000 dataset: 0.421125 seconds

### Parallel bubble sort

Average time taken to sort with 10000 dataset: 0.662394 seconds

### Parallel bubble sort with swapped flag

Average time taken to sort with 10000 dataset: 1.07725 seconds