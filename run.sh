#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <file_name>"
    exit 1
fi

# Source file name provided as an argument
file_name="$1"

# Check if the source file exists
if [ ! -f "src/$file_name.c" ]; then
    echo "Error: Source file not found in /src folder."
    exit 1
fi

# Create the out/ folder if it doesn't exist
mkdir -p out

# Compile the C file and save the executable to the out/ folder
gcc "src/$file_name.c" -o "out/$file_name"

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo -e "Compilation successful. Running the executable...\n"
    # Run the compiled executable
    "./out/$file_name"
else
    echo "Compilation failed."
fi
