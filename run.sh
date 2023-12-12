#!/bin/bash

# This script compiles all C files in the src/ folder and saves the executables to the out/ folder.
# It then runs the main.c executable.

# Source file name provided as an argument
file_name="main"

# Check if the source file exists
if [ ! -f "src/$file_name.c" ]; then
    echo "Error: main.c file not found in /src folder."
    exit 1
fi

# Create the out/ folder if it doesn't exist
mkdir -p out

# Compile the C file and save the executable to the out/ folder
gcc "src/*.c" -o "out/$file_name"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo -e "Compilation successful. Running the executable...\n"
    # Run the compiled executable
    "./out/$file_name"
else
    echo "Compilation failed."
fi
