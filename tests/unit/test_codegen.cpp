#include "codegen.h"
#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <cassert>
#include <string>

// helper function to compile full Python to C++ pipeline
std::string compileToCPP(const std::string & pythonSource) {
	// lexer: python source -> tokens
	Lexer lexer(pythonSource);
	auto tokens = lexer.tokenize();

	// parser: tokens -> AST
	Parser parser(tokens);
	auto ast = parser.parse();

	// codegen: ast -> C++ code
	CodeGenerator codegen;
	return codegen.generate(*ast);
}

// helper to check if generated code contains a substring
// TBD: can be improved
bool contains(const std::string& code, const std::string& substring) {
	return code.find(substring) != std::string::npos;
}

// test 1: simple integer assignment
void testSimpleAssignment() {
	std::cout << "Test 1: Simple assignment (x = 5)... ";

	std::string python = "x = 5";
	std::string cpp = compileToCPP(python);

	// should declare variable and assign
	assert(contains(cpp, "int x"));
	assert(contains(cpp, "x = 5"));

	std::cout << "PASSED\n";
}

// test 2: arithmetic expression
void testArithmeticExpression() {
	std::cout << "Test 2: Arithmetic expression (x = 5 + 3)... ";

	std::string python = "x = 5 + 3";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int x"));
	assert(contains(cpp, "5 + 3"));

	std::cout << "PASSED\n";
}

// test 3: multiple assignments
void testMultipleAssignments() {
	std::cout << "Test 3: Multiple assignments... ";

	std::string python = "x = 5\ny = 10\nz = 15";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int x"));
	assert(contains(cpp, "int y"));
	assert(contains(cpp, "int z"));
	assert(contains(cpp, "x = 5"));
	assert(contains(cpp, "y = 10"));
	assert(contains(cpp, "z = 15"));

	std::cout << "PASSED\n";
}

// test 4: variable reference in expression
void testVariableReference() {
	std::cout << "Test 4: Variable reference (z = x + y)... ";

	std::string python = "x = 5\n y = 10\nz = x + y";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "z = (x + y)"));

	std::cout << "PASSED\n";
}

// test 5: print statement
void testPrintStatement() {
	std::cout << "Test 5: Print statement... ";

	std::string python = "print(42)";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "std::cout"));
	assert(contains(cpp, "42"));
	assert(contains(cpp, "std::endl"));

	std::cout << "PASSED\n";
}

// test 6: print with variable
void testPrintVariable() {
	std::cout << "Test 6: Print with variable... ";

	std::string python = "x = 5\nprint(x)";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "std::cout"));
	assert(contains(cpp, "x"));
	assert(contains(cpp, "std::endl"));

	std::cout << "PASSED\n";
}

// test 7: Operator precedence in output
void testOperatorPrecedence() {
	std::cout << "Test 7: Operator precedence (x = 2 + 3 * 4)... ";

	std::string python = "x = 2 + 3 * 4";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "2 + (3 * 4)") || contains(cpp, "(2 + (3 * 4))"));

	std::cout << "PASSED\n";
}

// test 8: parentheses preserved
void testParentheses() {
	std::cout << "Test 8: Parentheses ((2 + 3) * 4)... ";

	std::string python = "(2 + 3) * 4";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "(2 + 3)"));
	assert(contains(cpp, "* 4"));

	std::cout << "PASSED\n";
}

// test 9: unary minus 
void testUnaryMinus() {
	std::cout << "Test 9: Unary minus (x = -5)... ";

	std::string python = "x = -5";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "-5") || contains(cpp, "(-5)"));

	std::cout << "PASSED\n";
}

// test 10: exponentiation (uses pow) 
void testExponentiation() {
	std::cout << "Test 10: Exponentiation (x = 2 ** 3)... ";

	std::string python = "x = 2 ** 3";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "pow"));
	assert(contains(cpp, "#include <cmath>"));

	std::cout << "PASSED\n";
}

// test 11: floor division // TBD: IMPLEMENT THIS LOGIC
//void testFloorDivision() {
//	std::cout << "Test 11: Floor division (x = 10 // 3)... ";
//
//	std::string python = "x = 10 // 3";
//	std::string cpp = compileToCPP(python);
//
//	assert(contains(cpp, "10 / 3") || contains(cpp, "floor"));
//
//	std::cout << "PASSED\n";
//}

// test 12: modulo operator 
void testModulo() {
	std::cout << "Test 12: Modulo (x = 10 % 3)... ";

	std::string python = "x = 10 % 3";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "10 % 3"));

	std::cout << "PASSED\n";
}

// test 13: full program structure 
void testFullProgramStructure() {
	std::cout << "Test 13: Full program structure... ";

	std::string python = "x = 5\nprint(x)";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "#include <iostream>"));
	assert(contains(cpp, "int main()"));
	assert(contains(cpp, "return 0"));
	assert(contains(cpp, "}"));
	
	std::cout << "PASSED\n";
}

// test 14: complex expression
void testComplexExpression() {
	std::cout << "Test 14: Complex expression... ";

	std::string python = "result = (10 + 5) * 2 - 3 / 4";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "result"));
	assert(contains(cpp, "="));

	std::cout << "PASSED\n";
}

// test 15: realistic program
void testRealisticProgram() {
	std::cout << "Test 15: full realistic program... ";

	std::string python =
		"x = 5\n"
		"y = 10\n"
		"z = x + y * 2\n"
		"print(z)";

	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int x"));
	assert(contains(cpp, "int y"));
	assert(contains(cpp, "int z"));

	assert(contains(cpp, "x = 5"));
	assert(contains(cpp, "y = 10"));

	assert(contains(cpp, "std::cout"));
	assert(contains(cpp, "z"));

	std::cout << "PASSED\n";
}

// test 16: boolean literals
void testBooleanLiterals() {
	std::cout << "Test 16: Boolean literals (x = True)... ";

	std::string python = "x = True\ny = False";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int x"));
	assert(contains(cpp, "true"));
	assert(contains(cpp, "int y"));
	assert(contains(cpp, "false"));

	std::cout << "PASSED\n";
}

// test 17: simple comparison
void testSimpleComparison() {
	std::cout << "Test 17: Simple comparison (x == 5)... ";

	std::string python = "result = x == 5";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "result"));
	assert(contains(cpp, "x == 5") || contains(cpp, "(x == 5)"));

	std::cout << "PASSED\n";
}

// test 18: all comparison operators
void testAllComparisonOperators() {
	std::cout << "Test 18: All comparison operators... ";

	std::string python =
		"a = x == 5\n"
		"b = x != 5\n"
		"c = x < 5\n"
		"d = x <= 5\n"
		"e = x > 5\n"
		"f = x >= 5\n";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "=="));
	assert(contains(cpp, "!="));
	assert(contains(cpp, "<"));
	assert(contains(cpp, "<="));
	assert(contains(cpp, ">"));
	assert(contains(cpp, ">="));

	std::cout << "PASSED\n";
}

// test 19: logical AND operator
void testLogicalAnd() {
	std::cout << "Test 19: Logical AND (x and y)... ";

	std::string python = "result = x and y";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "&&"));
	assert(contains(cpp, "x"));
	assert(contains(cpp, "y"));

	std::cout << "PASSED\n";
}

// test 20: logical OR operator
void testLogicalOr() {
	std::cout << "Test 20: Logical OR (x or y)... ";

	std::string python = "result = x or y";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "||"));
	assert(contains(cpp, "x"));
	assert(contains(cpp, "y"));

	std::cout << "PASSED\n";
}

// test 21: logical NOT operator
void testLogicalNot() {
	std::cout << "Test 21: Logical NOT (not x)... ";

	std::string python = "result = not x";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "!"));
	assert(contains(cpp, "x"));

	std::cout << "PASSED\n";
}

// test 22: comparison in print
void testComparisonInPrint() {
	std::cout << "Test 22: Comparison in print statement... ";

	std::string python = "print(x > 5)";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "std::cout"));
	assert(contains(cpp, "x > 5") || contains(cpp, "(x > 5)"));
	assert(contains(cpp, "std::endl"));

	std::cout << "PASSED\n";
}

// test 23: boolean expression with arithmetic
void testBooleanWithArithmetic() {
	std::cout << "Test 23: Boolean with arithmetic (x + 5 > 10)... ";

	std::string python = "result = x + 5 > 10";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "result"));
	assert(contains(cpp, "+"));
	assert(contains(cpp, ">"));

	std::cout << "PASSED\n";
}

// test 24: complex boolean expression
void testComplexBooleanExpression() {
	std::cout << "Test 24: Complex boolean expression... ";
	
	std::string python = "result = x > 5 and y < 10 or not z";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "result"));
	assert(contains(cpp, "&&"));
	assert(contains(cpp, "||"));
	assert(contains(cpp, "!"));

	std::cout << "PASSED\n";
}

// test 25: chained comparisons
void testChainedComparisons() {
	std::cout << "Test 25: Multiple comparisons... ";
	
	std::string python = "a = x < y\nb = y < z";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int a"));
	assert(contains(cpp, "int b"));
	assert(contains(cpp, "<"));

	std::cout << "PASSED\n";
}


// test 26: boolean in assignment and print
void testBooleanInAssignmentAndPrint() {
	std::cout << "Test 26: Boolean in assignment and print... ";

	std::string python = "x = 5\ny = 10\nresult = x < y\nprint(result)";
	std::string cpp = compileToCPP(python);

	assert(contains(cpp, "int x"));
	assert(contains(cpp, "int y"));
	assert(contains(cpp, "int result"));
	assert(contains(cpp, "<"));
	assert(contains(cpp, "std::cout"));

	std::cout << "PASSED\n";
}
int main() {
	std::cout << "\n=== Code Generator Unit Tests ===\n\n";

	try {
		testSimpleAssignment();
		testArithmeticExpression();
		testMultipleAssignments();
		testVariableReference();
		testPrintStatement();
		testPrintVariable();
		testOperatorPrecedence();
		testParentheses();
		testUnaryMinus();
		testExponentiation();
		//testFloorDivision(); TBD
		testModulo();
		testFullProgramStructure();
		testComplexExpression();
		testRealisticProgram();
		testBooleanLiterals();
		testSimpleComparison();
		testAllComparisonOperators();
		testLogicalAnd();
		testLogicalOr();
		testLogicalNot();
		testComparisonInPrint();
		testBooleanWithArithmetic();
		testComplexBooleanExpression();
		testChainedComparisons();
		testBooleanInAssignmentAndPrint();

		std::cout << "\nAll code generator tests passed!\n\n";
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "\nTest failed with exception: " << e.what() << "\n\n";
		return 1;
	}
}