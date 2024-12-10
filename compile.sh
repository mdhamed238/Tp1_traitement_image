#!/bin/bash

# Check if a filename argument is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# Compile the provided filename
g++ -Wall -std=c++11 $(pkg-config --cflags opencv4) "$1" -o main $(pkg-config --libs opencv4)

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the program with ./main"
else
    echo "Compilation failed."
fi