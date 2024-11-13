# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -Wall -O2
CXXFLAGS = -Wall -O2 -std=c++11
INCLUDES = -I./include -I/usr/include/CL

# OpenCL settings
OPENCL_LIBS = -lOpenCL

# Output binary
TARGET = matrix_test

# Object files
OBJECTS = src/matrix_mul.o src/matrix.o src/test.o

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(OPENCL_LIBS)

# Compilation rules
src/matrix_mul.o: src/matrix_mul.c include/matrix_mul.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/matrix_mul.c -o src/matrix_mul.o

src/matrix.o: src/matrix.cpp include/matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/matrix.cpp -o src/matrix.o

src/test.o: testing/test.cpp include/matrix.hpp include/matrix_mul.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c src/test.cpp -o src/test.o

# Clean
clean:
	rm -f src/*.o $(TARGET)

# Phony targets
.PHONY: all clean