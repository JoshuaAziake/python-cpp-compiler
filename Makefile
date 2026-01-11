CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
LDFLAGS = -static

# src files
SRC_DIR = src
SOURCES = $(SRC_DIR)/compiler.cpp $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.cpp $(SRC_DIR)/ast.cpp $(SRC_DIR)/codegen.cpp

# output executable
TARGET = pycc

# build the compiler
all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo "Building compiler... "
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)
	@echo "Compiler built successfully!"

# clean
clean:
	@echo "Cleaning..."
	@rm -f $(TARGET) output.cpp output.exe
	@echo "Clean complete"

.PHONY: all clean