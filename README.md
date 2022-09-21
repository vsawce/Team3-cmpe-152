# Assignment #2

## Important note for building
~~This builds successfully on GCC 9.3.0 on Ubuntu 20.04.~~
<br>~~We found that it may not build successfully with Clang, or more recent GCC releases.
As of writing, the reason for this is unknown, and will be investigated.~~
<br>We now find that unless specified in the compilation flags, some compilers may not use our desired C++ standard level. Clang for instance, defaults to C++98. To fix this, in Line 9 of the Makefile we added an additional flag, `-std=c++17`. The project should now build successfully with any recent release of GCC or Clang.

## Using make
- make all:     Build main.cpp
- make:         Shorthand for make all
- make run:     Run main.cpp
- make clean:   Remove all build files
- make lldb:    Debug main.cpp with lldb
