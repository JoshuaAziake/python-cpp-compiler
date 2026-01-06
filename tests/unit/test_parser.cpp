#include "parser.h"
#include "lexer.h"
#include <iostream>
#include <cassert>
#include <memory>

// helper function to tokenize source code
std::vector<Token> tokenize(const std::string& source) {
	Lexer lexer(source);
	return lexer.tokenize();
}

// helper to check if a node is of specific type
template<typename T>
bool isNodeType(const ASTNode* node) {
	return dynamic_cast<const T*>(node) != nullptr;
}

// test 1: simple integer litteral
void testSimpleInteger() {
	std::cout << "Test 1: Simple integer litteral... ";

	auto tokens = tokenize("42");
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program != nullptr);
	assert(program->statements.size() == 1);
	assert(isNodeType<ExprStmtNode>(program->statements[0].get()));

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	assert(isNodeType<IntegerNode>(exprStmt->expression.get()));

	auto* intNode = dynamic_cast<IntegerNode*>(exprStmt->expression.get());
	assert(intNode->value == 42);

	std::cout << "PASSED\n";
}

// test 2: simple addition
void testSimpleAddition() {
	std::cout << "Test 2: Simple addition (2 + 3)... ";

	auto tokens = tokenize("2 + 3");
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program != nullptr);
	assert(program->statements.size() == 1);
	assert(isNodeType<ExprStmtNode>(program->statements[0].get()));

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	assert(isNodeType<BinaryOpNode>(exprStmt->expression.get()));

	auto* binOp = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());
	assert(binOp->op == BinaryOp::ADD);

	auto* left = dynamic_cast<IntegerNode*>(binOp->left.get());
	auto* right = dynamic_cast<IntegerNode*>(binOp->right.get());
	assert(left->value == 2);
	assert(right->value == 3);

	std::cout << "PASSED\n";
}

// test 3: operator precedence (2 + 3 * 4 should be 2 + (3 * 4))
void testOperatorPrecedence() {
	std::cout << "Test 3: Operator precedence (2 + 3 * 4)... ";

	auto tokens = tokenize("2 + 3 * 4");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* addOp = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());

	assert(addOp->op == BinaryOp::ADD);

	auto* left = dynamic_cast<IntegerNode*>(addOp->left.get());
	assert(left->value == 2);

	auto* mulOp = dynamic_cast<BinaryOpNode*>(addOp->right.get());
	assert(mulOp->op == BinaryOp::MUL);

	auto* mulLeft = dynamic_cast<IntegerNode*>(mulOp->left.get());
	auto* mulRight = dynamic_cast<IntegerNode*>(mulOp->right.get());
	assert(mulLeft->value == 3);
	assert(mulRight->value == 4);

	std::cout << "PASSED\n";
}

// test 4: parentheses override precedence ((2 + 3) * 4)
void testParentheses() {
	std::cout << "Test 4: Parentheses ((2 + 3) * 4)... ";

	auto tokens = tokenize("(2 + 3) * 4");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* mulOp = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());

	assert(mulOp->op == BinaryOp::MUL);

	auto* addOp = dynamic_cast<BinaryOpNode*>(mulOp->left.get());
	assert(addOp->op == BinaryOp::ADD);

	auto* addLeft = dynamic_cast<IntegerNode*>(addOp->left.get());
	auto* addRight = dynamic_cast<IntegerNode*>(addOp->right.get());
	assert(addLeft->value == 2);
	assert(addRight->value == 3);

	auto* right = dynamic_cast<IntegerNode*>(mulOp->right.get());
	assert(right->value == 4);

	std::cout << "PASSED\n";
}

// test 5: simple assignment
void testSimpleAssignment() {
	std::cout << "Test 5: Simple assignment (x = 5)... ";

	auto tokens = tokenize("x = 5");
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program->statements.size() == 1);
	assert(isNodeType<AssignmentNode>(program->statements[0].get()));

	auto* assign = dynamic_cast<AssignmentNode*>(program->statements[0].get());
	assert(assign->varName == "x");
	
	auto* value = dynamic_cast<IntegerNode*>(assign->value.get());
	assert(value->value == 5);

	std::cout << "PASSED\n";
}

// test 6: assignment with expression
void testAssignmentWithExpression() {
	std::cout << "Test 6: Assignment with expression (x = 5 + 3)... ";

	auto tokens = tokenize("x = 5 + 3");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* assign = dynamic_cast<AssignmentNode*>(program->statements[0].get());
	assert(assign->varName == "x");

	auto* binOp = dynamic_cast<BinaryOpNode*>(assign->value.get());
	assert(binOp->op == BinaryOp::ADD);

	std::cout << "PASSED\n";
}

// test 7: variable reference
void testVariableReference() {
	std::cout << "Test 7: Variable reference (x + y)... ";

	auto tokens = tokenize("x + y");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* binOp = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());

	auto* left = dynamic_cast<IdentifierNode*>(binOp->left.get());
	auto* right = dynamic_cast<IdentifierNode*>(binOp->right.get());

	assert(left->name == "x");
	assert(right->name == "y");

	std::cout << "PASSED\n";
}

// test 8: print stmt
void testPrintStatement() {
	std::cout << "Test 8: Print statement (print(42))... ";

	auto tokens = tokenize("print(42)");
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program->statements.size() == 1);
	assert(isNodeType<PrintNode>(program->statements[0].get()));

	auto* printNode = dynamic_cast<PrintNode*>(program->statements[0].get());
	auto* value = dynamic_cast<IntegerNode*>(printNode->expression.get());
	assert(value->value == 42);

	std::cout << "PASSED\n";
}

// test 9: print with expression
void testPrintWithExpression() {
	std::cout << "Test 9: Print with expression (print(x + 5))... ";

	auto tokens = tokenize("print(x + 5)");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* printNode = dynamic_cast<PrintNode*>(program->statements[0].get());
	auto* binOp = dynamic_cast<BinaryOpNode*>(printNode->expression.get());

	assert(binOp->op == BinaryOp::ADD);

	std::cout << "PASSED\n";
}

// test 10: multiple statements
void testMultipleStatements() {
	std::cout << "Test 10: Multiple statements... ";

	auto tokens = tokenize("x = 5\ny = 10\nz = x + y");
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program->statements.size() == 3);

	auto* assign1 = dynamic_cast<AssignmentNode*>(program->statements[0].get());
	assert(assign1->varName == "x");

	auto* assign2 = dynamic_cast<AssignmentNode*>(program->statements[1].get());
	assert(assign2->varName == "y");

	auto* assign3 = dynamic_cast<AssignmentNode*>(program->statements[2].get());
	assert(assign3->varName == "z");

	std::cout << "PASSED\n";
}

// test 11: unary minus
void testUnaryMinus() {
	std::cout << "Test 11: Unary minus (-5)... ";

	auto tokens = tokenize("-5");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* unaryOp = dynamic_cast<UnaryOpNode*>(exprStmt->expression.get());

	assert(unaryOp->op == UnaryOp::NEGATE);

	auto* value = dynamic_cast<IntegerNode*>(unaryOp->operand.get());
	assert(value->value == 5);

	std::cout << "PASSED\n";
}

// test 12: exponentiation (right-associative)
void testExponentiation() {
	std::cout << "Test 12: Exponentiation ((2 ** 3 ** 2) should be 2 ** (3 ** 2))... ";

	auto tokens = tokenize("2 ** 3 ** 2");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* powerOp1 = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());

	auto* left = dynamic_cast<IntegerNode*>(powerOp1->left.get());
	assert(left->value == 2);

	auto* powerOp2 = dynamic_cast<BinaryOpNode*>(powerOp1->right.get());
	assert(powerOp2->op == BinaryOp::POWER);

	auto* powerLeft = dynamic_cast<IntegerNode*>(powerOp2->left.get());
	auto* powerRight = dynamic_cast<IntegerNode*>(powerOp2->right.get());
	assert(powerLeft->value == 3);
	assert(powerRight->value == 2);

	std::cout << "PASSED\n";
}

// test 13: all operators
void testAllOperators() {
	std::cout << "Test 13: All operators in one expression... ";

	auto tokens = tokenize("10 + 5 - 3 * 2 / 4 % 3");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());

	// should parse without error
	assert(exprStmt != nullptr);

	std::cout << "PASSED\n";
}

// test 14: complex nested expression
void testComplexExpression() {
	std::cout << "Test 14: Complex nested expression... ";

	auto tokens = tokenize("(2 + 3) * (4 - 1) + 5");
	Parser parser(tokens);
	auto program = parser.parse();

	auto* exprStmt = dynamic_cast<ExprStmtNode*>(program->statements[0].get());
	auto* topAdd = dynamic_cast<BinaryOpNode*>(exprStmt->expression.get());

	assert(topAdd->op == BinaryOp::ADD);

	auto* mul = dynamic_cast<BinaryOpNode*>(topAdd->left.get());
	assert(mul->op == BinaryOp::MUL);

	std::cout << "PASSED\n";
}

// test 15: full program
void testFullProgram() {
	std::cout << "Test 15: Full program... ";

	std::string source =
		"x = 5\n"
		"y = 10\n"
		"z = x + y * 2\n"
		"print(z)";

	auto tokens = tokenize(source);
	Parser parser(tokens);
	auto program = parser.parse();

	assert(program->statements.size() == 4);

	// verify all statement types
	assert(isNodeType<AssignmentNode>(program->statements[0].get()));
	assert(isNodeType<AssignmentNode>(program->statements[1].get()));
	assert(isNodeType<AssignmentNode>(program->statements[2].get()));
	assert(isNodeType<PrintNode>(program->statements[3].get()));

	std::cout << "PASSED\n";
}

int main() {
	std::cout << "\n=== Parser Unit Tests ===\n\n";

	try {
		testSimpleInteger();
		testSimpleAddition();
		testOperatorPrecedence();
		testParentheses();
		testSimpleAssignment();
		testAssignmentWithExpression();
		testVariableReference();
		testPrintStatement();
		testPrintWithExpression();
		testMultipleStatements();
		testUnaryMinus();
		testExponentiation();
		testAllOperators();
		testComplexExpression();
		testFullProgram();

		std::cout << "\nAll parser tests passed!\n\n";
		return 0;
	}
	catch (const std::exception& e) {
		std::cerr << "\n Test failed with exception: " << e.what() << "\n\n";
		return 1;
	}
}
