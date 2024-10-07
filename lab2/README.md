# Barrier implementation for image processing

This simple program demonstrates a simple barrier class that synchronises threads to wait
for when all threads have finished their work.

The test is done by simulating image processing - each thread works on an image chunk.
We then wait when all chunks are done to continue on other processes.

Min parts of the code:
1. Barrier - class that makes threads wait and not continue work until all threads have finished.
2. processImageChunk - function that simulates image chunk processing.
3. threadImageProcessing - function that simulates a certain image process action, like filter.