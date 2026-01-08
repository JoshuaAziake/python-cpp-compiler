# Makefile for Python-to-C++ Compiler tests
# Compiles and runs unit tests for each component

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../include
LDFLAGS = -static

# Source files from project
PROJECT_DIR = C:/Users/Josh/OneDrive/Desktop/python-cpp-compiler/src
LEXER_SRC = $(PROJECT_DIR)/lexer.cpp
PARSER_SRC = $(PROJECT_DIR)/parser.cpp
CODEGEN_SRC = $(PROJECT_DIR)/codegen.cpp 

# Test executables
TEST_LEXER = unit/test_lexer
TEST_PARSER = unit/test_parser
TEST_CODEGEN = unit/test_codegen 

# Default target: build all tests
all: $(TEST_LEXER) $(TEST_PARSER) $(TEST_CODEGEN)

# Build lexer tests
$(TEST_LEXER): unit/test_lexer.cpp $(LEXER_SRC) $(PROJECT_DIR)/ast.cpp
	@echo "Building lexer tests..."
	$(CXX) $(CXXFLAGS) -o $(TEST_LEXER) unit/test_lexer.cpp $(LEXER_SRC) $(PROJECT_DIR)/ast.cpp $(LDFLAGS)
	@echo "Lexer tests compiled"

# Build parser tests
$(TEST_PARSER): unit/test_parser.cpp $(PARSER_SRC) $(LEXER_SRC)
	@echo "Building parser tests..."
	$(CXX) $(CXXFLAGS) -o $(TEST_PARSER) unit/test_parser.cpp $(PARSER_SRC) $(LEXER_SRC) $(LDFLAGS)
	@echo "Parser tests compiled"

# Build codegen tests
$(TEST_CODEGEN): unit/test_codegen.cpp $(CODEGEN_SRC) $(PARSER_SRC) $(LEXER_SRC)
	@echo "Building code generator tests..."
	$(CXX) $(CXXFLAGS) -o $(TEST_CODEGEN) unit/test_codegen.cpp $(CODEGEN_SRC) $(PARSER_SRC) $(LEXER_SRC) $(LDFLAGS)
	@echo "Code generator tests compiled"

# Run all tests
test: all
	@echo ""
	@echo "=========================================="
	@echo "Running All Unit Tests"
	@echo "=========================================="
	@./$(TEST_LEXER)
	@./$(TEST_PARSER)
	@./$(TEST_CODEGEN)
	@echo ""
	@echo "=========================================="
	@echo "All Tests Complete"
	@echo "=========================================="

# Run only unit Tests
test-unit: $(TEST_LEXER) $(TEST_PARSER) $(TEST_CODEGEN)
	@./$(TEST_LEXER)
	@./$(TEST_PARSER)
	@./$(TEST_CODEGEN)

# Run individual test suites
test-lexer: $(TEST_LEXER)
	@./$(TEST_LEXER)

test-parser: $(TEST_PARSER)
	@./$(TEST_PARSER)

test-codegen: $(TEST_CODEGEN)
	@./$(TEST_CODEGEN)

# Clean up compiled tests
clean:
	@echo "Cleaning up test executables... "
	@rm -f $(TEST_LEXER) $(TEST_PARSER) $(TEST_CODEGEN)
	@echo "Clean complete"

# Rebuild everything
rebuild: clean all

.PHONY: all test test-lexer test-parser test-codegen clean rebuild
