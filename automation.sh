#!/bin/bash

# Find the full path to the CEW_Project directory
CEW_PROJECT_DIR=$(find ~ -type d -name "CEW_PROJECT_DIR" 2>/dev/null | head -n 1)

# Check if the CEW_Project directory was found
if [ -z "$CEW_PROJECT_DIR" ]; then
    echo "CEW_Project directory not found."
    exit 1
fi

# Navigate to the CEW_Project directory
cd "$CEW_PROJECT_DIR" || { echo "Failed to navigate to $CEW_PROJECT_DIR"; exit 1; }

# Infinite loop to compile and execute the C code every 30 seconds
while true; do
    # Compile project_file.c with email_sender.c
    gcc project_file.c email_sender.c -o project_file -ljson-c -lcurl
    
    # Check if compilation was successful
    if [ $? -ne 0 ]; then
        echo "Compilation failed."
        exit 1
    fi
    
    # Run the compiled program
    ./project_file
    
    # Wait 30 seconds before the next run
    sleep 30
done