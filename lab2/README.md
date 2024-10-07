# Barrier implementation for image processing

This simple program demonstrates a simple barrier class that synchronises threads to wait
for when all threads have finished their work.

The test is done by simulating image processing - each thread works on an image chunk.
We then wait when all chunks are done to continue on other processes.