Assignment #2

Important note for building:
    This builds successfully on GCC 9.3.0 on Ubuntu 20.04.
    We found that it may not build successfully with Clang, or more recent GCC releases.
    As of writing, the reason for this is unknown, and will be investigated.

Using make:
    - make all:     Build main.cpp
    - make:         Shorthand for make all
    - make run:     Run main.cpp
    - make clean:   Remove all build files
    - make lldb:    Debug main.cpp with lldb
