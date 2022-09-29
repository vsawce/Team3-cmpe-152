#makefile template for C/C++ projects

PROJECTPATH = $(shell pwd)

ifneq ($(wildcard $(brew --prefix)/),) 
    LIB = $(shell brew --prefix)/lib
	INCLUDE = $(shell brew --prefix)/include
else 
    LIB = /usr/local/lib
	INCLUDE = /usr/local/include
endif

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

SOURCES := $(wildcard Tokens/*.cpp Tokens/*/*.cpp src/*.cpp) main.cpp

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
	rm -rf $(TARGET) $(OBJECTS) $(OUT) $(ANTLR_INCLUDES)

#run the program
run: $(TARGET)
	./$(TARGET)

#run the program with lldb
lldb: $(TARGET)
	lldb ./$(TARGET)

ANTLRFILE = Expr.g4
antlr4=java -jar ${ANTLR_JAR}

ANTLR_RUNTIME_INCLUDES=$(INCLUDE)/antlr4-runtime
ANTLR_INCLUDES=$(PROJECTPATH)/target/generated-sources/antlr4
ANTLR_LIB=antlr4-runtime
PROG=ExprMain.cpp
OUT=prog.out
ANTLR_SOURCES=target/generated-sources/antlr4/*.cpp
SOURCES=$(ANTLR_SOURCES) $(PROG)

compile:
	g++ -std=c++17 -I$(ANTLR_RUNTIME_INCLUDES) -I$(ANTLR_INCLUDES) -I$(PROJECTPATH) -L$(LIB) -w $(SOURCES) -l $(ANTLR_LIB) -o $(OUT)

antlr:
	$(antlr4) -o ./target/generated-sources/antlr4 -no-listener -visitor -encoding UTF-8 -Dlanguage=Cpp $(ANTLRFILE)
