# Kernel Convolution

A simple implementation of a kernel convolution image filter in C++. Tested for Sobel filters, low pass filters and sharpening filters. These are implemented and must be changed in the call to the convolution function in the code. Handling values outside the pixel range is different for each filter, certain lines needed for specific filters are noted as needing to be commented out as needed.

The code is written to work on a live camera feed or video file, and can be changed from the VideoCapture command.

Note that this code is not an optimal implementation and there will probably be many better implementations of kernel convolution elsewhere, I just wanted to try and write it myself in C++.

To make the C++ compile flags simpler, I have put the g++ compile command with the C++ libraries, this may need to be altered for your location and version of OpenCV, dependent on the location of the opencv.hpp and corresponding library files, it's also probably useful to ensure the #include commands for the opencv files are correct for your system.
