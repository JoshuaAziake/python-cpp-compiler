# Python-to-C++ Compiler
**A compiler that translates a subset of Python code into C++ code. This projects focuses on the following phases of compilation: lexical analysis, syntax analysis, and code generation.**

## Supported Features
- Variable declarations and assignments
- Arithmetic expressions with proper operator precedence (Addition, Substraction, Multiplication, Division, Modulo, Exponentiation, Floor Division)
- Unary operators (-, +, not)
- Integer and floating-point literals
- Variable references
- Print statements
- Parenthesized expressions
- Multi-statement programs

## TODO List
- Comparison operators
- Boolean literals and logic
- Control flow
- Function definitions and calls
- Lists and strings
- String literals with escape sequences
- Test suite

## Dependencies
- C++ compiler with C++17 support (g++, clang++)
- CMake 3.10 or higher
- Git

## Building the Compiler
```
# Clone the repository
git clone https://github.com/yourusername/python-cpp-compiler.git
cd python-cpp-compiler

# Create build directory
mkdir build
cd build

# Build with CMake
cmake ..
make

# The compiler executable is now ready
./pycc
```

## Using the Compiler
```
# Compile a Python file to C++
./pycc input.py

# This generates output.cpp, which you can compile and run
g++ -o program output.cpp
./program
```

## Known Limitations
- Limited Python subset (no classes, decorators, comprehensions)
- No type inference (all variables declared as int or double)
- No error recovery in parser
- No optimization passes
- No standard library support

## Resources
This project is based on the free textbook by Robert Nystrom called Crafting Interpreters available online.
