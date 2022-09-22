#makefile template for C/C++ projects

#compiler
CXX = g++

#compiler flags
#-g adds debugging information to the executable file
#-Wall turns on most, but not all, compiler warnings
CXXFLAGS = -g -Wall -std=c++17

#linker flags
#-lm links the math library
LDFLAGS = -lm

#the build target executable
TARGET = main

SOURCES := $(wildcard src/*.cpp) main.cpp

#the object files that must be created in order to link
OBJECTS=$(patsubst %.cpp, %.o, $(SOURCES))

#the default target
all: $(TARGET)

#link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

#compile the source files into object files
main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

#remove the build target executable and object files
clean:
	rm -f $(TARGET) $(OBJECTS)

#run the program
run: $(TARGET)
	./$(TARGET)

#run the program with lldb
lldb: $(TARGET)
	lldb ./$(TARGET)
