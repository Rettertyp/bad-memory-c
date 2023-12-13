# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Wextra -g

# Source directory
SRC_DIR := src

# Object directory
OBJ_DIR := obj

# Output directory
OUT_DIR := out

# Output executable
TARGET := $(OUT_DIR)/main

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Default target
all: directories $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Rule to compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR) $(OUT_DIR)

# Create the output and object directories
.PHONY: directories
directories: $(OUT_DIR) $(OBJ_DIR)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)