# Define the compiler
CXX = g++

# Define the flags
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17

# Define the target executable
TARGET = out

# Define the source files
SRCS = main.cpp

# Default target: build the executable
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Clean up generated files
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
