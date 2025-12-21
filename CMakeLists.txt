cmake_minimum_required(VERSION 3.15)
project(PythonCppCompiler VERSION 0.1.0 LANGUAGES CXX)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Source files (we'll add these as we go)
set(SOURCES
    src/main.cpp
    src/lexer.cpp
    src/parser.cpp
    src/ast.cpp
    src/codegen.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Executable
add_executable(pycc ${SOURCES})

# Enable testing
enable_testing()

# Add test directory (we'll create this later)
# add_subdirectory(tests)

# Installation
install(TARGETS pycc DESTINATION bin)