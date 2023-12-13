# compile the program
make

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo -e "\nCompilation successful. Running the executable...\n"
    # Run the compiled executable
    "./out/main.exe"
else
    echo "Compilation failed."
fi