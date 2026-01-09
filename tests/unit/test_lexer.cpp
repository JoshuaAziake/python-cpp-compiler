#include "lexer.h"
#include <iostream>
#include <cassert>
#include <vector>

// helper function to check if token matches expected type and lexeme
bool checkToken(const Token& token, TokenType expectedType, const std::string& expectedLexeme) {
	if (token.type != expectedType) {
		std::cerr << "Token type mismatch: expected " << tokenTypeToString(expectedType)
			      << ", got " << tokenTypeToString(token.type) << std::endl;
		return false;
	}
	if (token.lexeme != expectedLexeme) {
		std::cerr << "Token lexeme mismatch: expected '" << expectedLexeme
			<< "', got '" << token.lexeme << "'" << std::endl;
		return false;
	}
	return true;
}

// helper function to create tokens
std::vector<Token> tokenize(const std::string& source) {
	Lexer lexer(source);
	return lexer.tokenize();
}

// test 1: simple integer
void testSimpleInteger() {
	std::cout << "Test 1: Simple integer... ";
	auto tokens = tokenize("42");

	assert(tokens.size() == 2); // INTEGER + EOF
	assert(checkToken(tokens[0], TokenType::INTEGER, "42"));
	assert(checkToken(tokens[1], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 2: simple float
void testSimpleFloat() {
	std::cout << "Test 2: Simple float... ";
	auto tokens = tokenize("3.14");

	assert(tokens.size() == 2); // FLOAT + EOF
	assert(checkToken(tokens[0], TokenType::FLOAT, "3.14"));
	assert(checkToken(tokens[1], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 3: basic arithmetic expression
void testBasicArithmetic() {
	std::cout << "Test 3: Basic arithmetic (2 + 3)... ";
	auto tokens = tokenize("2 + 3");

	assert(tokens.size() == 4); // INTEGER PLUS INTEGER EOF
	assert(checkToken(tokens[0], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[1], TokenType::PLUS, "+"));
	assert(checkToken(tokens[2], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[3], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 4: all arithmetic operators
void testAllOperators() {
	std::cout << "Test 4: All arithmetic operators... ";
	auto tokens = tokenize("5 + 3 - 2 * 4 / 2 % 3 ** 2 // 1");

	assert(tokens.size() == 16); // 8 numbers + 7 operators + EOF
	assert(checkToken(tokens[0], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[1], TokenType::PLUS, "+"));
	assert(checkToken(tokens[2], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[3], TokenType::MINUS, "-"));
	assert(checkToken(tokens[4], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[5], TokenType::STAR, "*"));
	assert(checkToken(tokens[6], TokenType::INTEGER, "4"));
	assert(checkToken(tokens[7], TokenType::SLASH, "/"));
	assert(checkToken(tokens[8], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[9], TokenType::PERCENT, "%"));
	assert(checkToken(tokens[10], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[11], TokenType::DOUBLE_STAR, "**"));
	assert(checkToken(tokens[12], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[13], TokenType::DOUBLE_SLASH, "//"));
	assert(checkToken(tokens[14], TokenType::INTEGER, "1"));
	assert(checkToken(tokens[15], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 5 assignment statement
void testAssignment() {
	std::cout << "Test 5: Assignment (x = 5)... ";
	auto tokens = tokenize("x = 5");

	assert(tokens.size() == 4); // IDENTIFIER EQUAL INTEGER EOF
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::EQUAL, "="));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 6: multi-digit numbers
void testMultiDigitNumbers() {
	std::cout << "Test 6: Multi-digit numbers... ";
	auto tokens = tokenize("123 + 456");

	assert(tokens.size() == 4);
	assert(checkToken(tokens[0], TokenType::INTEGER, "123"));
	assert(checkToken(tokens[1], TokenType::PLUS, "+"));
	assert(checkToken(tokens[2], TokenType::INTEGER, "456"));
	assert(checkToken(tokens[3], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 7: identifiers vs keywords
void testIdentifiersAndKeywords() {
	std::cout << "Test 7: Identifiers vs keywords... ";
	auto tokens = tokenize("x if y print z");

	assert(tokens.size() == 6);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::IF, "if"));
	assert(checkToken(tokens[2], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[3], TokenType::PRINT, "print"));
	assert(checkToken(tokens[4], TokenType::IDENTIFIER, "z"));
	assert(checkToken(tokens[5], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 8: parentheses
void testParentheses() {
	std::cout << "Test 8: Parentheses... ";
	auto tokens = tokenize("(2 + 3) * 4");

	assert(tokens.size() == 8);
	assert(checkToken(tokens[0], TokenType::LPAREN, "("));
	assert(checkToken(tokens[1], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[2], TokenType::PLUS, "+"));
	assert(checkToken(tokens[3], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[4], TokenType::RPAREN, ")"));
	assert(checkToken(tokens[5], TokenType::STAR, "*"));
	assert(checkToken(tokens[6], TokenType::INTEGER, "4"));
	assert(checkToken(tokens[7], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 9: print statement
void testPrintStatement() {
	std::cout << "Test 9: Print statement... ";
	auto tokens = tokenize("print(x)");

	assert(tokens.size() == 5);
	assert(checkToken(tokens[0], TokenType::PRINT, "print"));
	assert(checkToken(tokens[1], TokenType::LPAREN, "("));
	assert(checkToken(tokens[2], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[3], TokenType::RPAREN, ")"));
	assert(checkToken(tokens[4], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 10: multiple statements with newlines
void testMultipleStatements() {
	std::cout << "Test 10: Multiple statements with newlines... ";
	auto tokens = tokenize("x = 5\ny = 10\nz = x + y");

	assert(tokens.size() == 14);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::EQUAL, "="));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::NEWLINE, "\\n"));
	assert(checkToken(tokens[4], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[5], TokenType::EQUAL, "="));
	assert(checkToken(tokens[6], TokenType::INTEGER, "10"));
	assert(checkToken(tokens[7], TokenType::NEWLINE, "\\n"));
	assert(checkToken(tokens[8], TokenType::IDENTIFIER, "z"));
	assert(checkToken(tokens[9], TokenType::EQUAL, "="));
	assert(checkToken(tokens[10], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[11], TokenType::PLUS, "+"));
	assert(checkToken(tokens[12], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[13], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 11: comments are ignored
void testComments() {
	std::cout << "Test 11: Comments... ";
	auto tokens = tokenize("x = 5 #this is a comment\ny = 10");

	assert(tokens.size() == 8);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::EQUAL, "="));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::NEWLINE, "\\n"));
	assert(checkToken(tokens[4], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[5], TokenType::EQUAL, "="));
	assert(checkToken(tokens[6], TokenType::INTEGER, "10"));
	assert(checkToken(tokens[7], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 12: whitespace handling
void testWhitespace() {
	std::cout << "Test 12: Whitespace handling... ";
	auto tokens = tokenize("x	=	5	+	3	");

	assert(tokens.size() == 6);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::EQUAL, "="));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::PLUS, "+"));
	assert(checkToken(tokens[4], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[5], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 13: comparison operators
void testComparisonOperators() {
	std::cout << "Test 13: Comparison operators... ";
	auto tokens = tokenize("x == 5 != 3 < 10 > 2 <= 7 >= 4");

	assert(tokens.size() == 14);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::EQUAL_EQUAL, "=="));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::NOT_EQUAL, "!="));
	assert(checkToken(tokens[4], TokenType::INTEGER, "3"));
	assert(checkToken(tokens[5], TokenType::LESS, "<"));
	assert(checkToken(tokens[6], TokenType::INTEGER, "10"));
	assert(checkToken(tokens[7], TokenType::GREATER, ">"));
	assert(checkToken(tokens[8], TokenType::INTEGER, "2"));
	assert(checkToken(tokens[9], TokenType::LESS_EQUAL , "<="));
	assert(checkToken(tokens[10], TokenType::INTEGER, "7"));
	assert(checkToken(tokens[11], TokenType::GREATER_EQUAL, ">="));
	assert(checkToken(tokens[12], TokenType::INTEGER, "4"));
	assert(checkToken(tokens[13], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 14: boolean keywords
void testBooleanKeywords() {
	std::cout << "Test 14: Boolean keywords... ";
	auto tokens = tokenize("True False and or not");

	assert(tokens.size() == 6);
	assert(checkToken(tokens[0], TokenType::TRUE, "True"));
	assert(checkToken(tokens[1], TokenType::FALSE, "False"));
	assert(checkToken(tokens[2], TokenType::AND, "and"));
	assert(checkToken(tokens[3], TokenType::OR, "or"));
	assert(checkToken(tokens[4], TokenType::NOT, "not"));
	assert(checkToken(tokens[5], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 15: underscore in identifiers
void testUnderscoreIdentifiers() {
	std::cout << "Test 15: Underscore in identifiers... ";
	auto tokens = tokenize("my_var _private __special");

	assert(tokens.size() == 4);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "my_var"));
	assert(checkToken(tokens[1], TokenType::IDENTIFIER, "_private"));
	assert(checkToken(tokens[2], TokenType::IDENTIFIER, "__special"));
	assert(checkToken(tokens[3], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 16: logical operators
void testLogicalOperators() {
	std::cout << "Test 17: Locial operators (and, or, not)... ";
	auto tokens = tokenize("x and y or not z");

	assert(tokens.size() == 7);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::AND, "and"));
	assert(checkToken(tokens[2], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[3], TokenType::OR, "or"));
	assert(checkToken(tokens[4], TokenType::NOT, "not"));
	assert(checkToken(tokens[5], TokenType::IDENTIFIER, "z"));
	assert(checkToken(tokens[6], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 17: mixed comparison and logical operators
void testMixedBooleanOperators() {
	std::cout << "Test 18: Mixed comparison and logical... ";
	auto tokens = tokenize("x > 5 and y <= 10 or not z == True");

	assert(tokens.size() == 13);
	assert(checkToken(tokens[0], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[1], TokenType::GREATER, ">"));
	assert(checkToken(tokens[2], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[3], TokenType::AND, "and"));
	assert(checkToken(tokens[4], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[5], TokenType::LESS_EQUAL, "<="));
	assert(checkToken(tokens[6], TokenType::INTEGER, "10"));
	assert(checkToken(tokens[7], TokenType::OR, "or"));
	assert(checkToken(tokens[8], TokenType::NOT, "not"));
	assert(checkToken(tokens[9], TokenType::IDENTIFIER, "z"));
	assert(checkToken(tokens[10], TokenType::EQUAL_EQUAL, "=="));
	assert(checkToken(tokens[11], TokenType::TRUE, "True"));
	assert(checkToken(tokens[12], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED\n";
}

// test 18: simple indentation
void testSimpleIndentation() {
	std::cout << "Test 18: Simple Indentation... ";
	auto tokens = tokenize("if x:\n	y = 5");

	assert(tokens.size() == 10);
	assert(checkToken(tokens[0], TokenType::IF, "if"));
	assert(checkToken(tokens[1], TokenType::IDENTIFIER, "x"));
	assert(checkToken(tokens[2], TokenType::COLON, ":"));
	assert(checkToken(tokens[3], TokenType::NEWLINE, "\\n"));
	assert(checkToken(tokens[4], TokenType::INDENT, ""));
	assert(checkToken(tokens[5], TokenType::IDENTIFIER, "y"));
	assert(checkToken(tokens[6], TokenType::EQUAL, "="));
	assert(checkToken(tokens[7], TokenType::INTEGER, "5"));
	assert(checkToken(tokens[8], TokenType::DEDENT, ""));
	assert(checkToken(tokens[9], TokenType::EOF_TOKEN, ""));

	std::cout << "PASSED";
}

// test 19: dedentation
void testDedentation() {
	std::cout << "Test 19: Dedentation... ";
	auto tokens = tokenize("if x:\n	y = 5\nz = 10");

	// expect to find INDENT after first line, then dedent before z = 10
	bool foundIndent = false;
	bool foundDedent = false;

	for (const auto& token : tokens) {
		if (token.type == TokenType::INDENT) foundIndent = true;
		if (token.type == TokenType::DEDENT) foundDedent = true;
	}

	assert(foundIndent);
	assert(foundDedent);

	std::cout << "PASSED";
}

// test 20: nested indentation (multiple indent levels)
void testNestedIndentation() {
	std::cout << "Test 20: Nested indentation... ";
	auto tokens = tokenize("if x:\n	if y:\n		z = 5");

	int indentCount = 0;
	for (const auto& token : tokens) {
		if (token.type == TokenType::INDENT) indentCount++;
	}

	assert(indentCount == 2);
	
	std::cout << "PASSED\n";
}

// test 21: multiple dedents at once
void testMultipleDedents() {
	std::cout << "Test 21: Multiple dedents... ";
	auto tokens = tokenize("if x:\n	if y:\n		z = 5\na = 1");

	int indentCount = 0;
	int dedentCount = 0;

	for (const auto& token : tokens) {
		if (token.type == TokenType::INDENT) indentCount++;
		if (token.type == TokenType::DEDENT) dedentCount++;
	}

	assert(indentCount == 2);
	assert(indentCount == 2);
	
	std::cout << "PASSED\n";
}

// test 22: blank lines should not affect indentation
void testBlankLines() {
	std::cout << "Test 22: Blank lines... ";
	auto tokens = tokenize("if x:\n\n	y = 5");

	// should still find indent despite blank line
	bool foundIndent = false;

	for (const auto& token : tokens) {
		if (token.type == TokenType::INDENT) foundIndent = true;
	}

	assert(foundIndent);
	
	std::cout << "PASSED\n";
}

// test 23: maintain indentation within block
void testMaintainIndentation() {
	std::cout << "Test 23: Maintain indentation... ";
	auto tokens = tokenize("if x:\n y = 5\n z = 10;");

	// should have 1 INDENT and 1 DEDENT
	int indentCount = 0;
	int dedentCount = 0;

	for (const auto& token : tokens) {
		if (token.type == TokenType::INDENT) indentCount++;
		if (token.type == TokenType::DEDENT) dedentCount++;
	}

	assert(indentCount == 1);
	assert(dedentCount == 1);

	std::cout << "PASSED\n";
}

int main() {
	std::cout << "\n=== Lexer Unit Tests ===\n\n";

	try {
		testSimpleInteger();
		testSimpleFloat();
		testBasicArithmetic();
		testAllOperators();
		testAssignment();
		testMultiDigitNumbers();
		testIdentifiersAndKeywords();
		testParentheses();
		testPrintStatement();
		testMultipleStatements();
		testComments();
		testWhitespace();
		testComparisonOperators();
		testBooleanKeywords();
		testUnderscoreIdentifiers();
		testLogicalOperators();
		testMixedBooleanOperators();
		testSimpleIndentation();
		testDedentation();
		testNestedIndentation();
		testMultipleDedents();
		testBlankLines();
		testMaintainIndentation();

		std::cout << "\n All lexer tests passed!\n\n";
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "\n Test failed with exception: " << e.what() << "\n\n";
		return 1;
	}
}