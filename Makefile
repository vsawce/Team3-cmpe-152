#makefile template for C/C++ projects

PROJECTPATH = "$(shell pwd)"

LIB = /usr/local/lib
INCLUDE = /usr/local/include

ifneq ($(wildcard $(shell brew --prefix 2>/dev/null)),) 
    LIB = $(shell brew --prefix)/lib
	INCLUDE = $(shell brew --prefix)/include
endif

#compiler
CXX = g++

#compiler flags
#-g adds debugging information to the executable file
#-Wall turns on most, but not all, compiler warnings
CXXFLAGS = -g -Wall -std=c++17

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
	$(CXX) $(CXXFLAGS) -I$(ANTLR_RUNTIME_INCLUDES) -I$(ANTLR_INCLUDES) -I$(PROJECTPATH) -L$(LIB) -w $(SOURCES) -l $(ANTLR_LIB) -o $(OUT)

antlr:
	$(antlr4) -o ./target/generated-sources/antlr4 -no-listener -visitor -encoding UTF-8 -Dlanguage=Cpp $(ANTLRFILE)

#remove the build target executable and object files
clean:
	rm -rf $(TARGET) $(OBJECTS) $(OUT) $(PROJECTPATH)/target .antlr/ $(wildcard *.dSYM)
