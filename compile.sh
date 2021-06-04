#!/usr/bin/bash

g++ kernel_conv.cpp -o kconv \
    -I/usr/local/include/opencv4 \
    -L/usr/local/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_videoio \
        -lopencv_highgui
