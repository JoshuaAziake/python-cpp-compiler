#include "lexer.h"
#include "ast.h"
#include <iostream>
#include <vector>
#include <memory>

void testAST() {
	std::cout << "=== Testing AST Construction  ===\n\n";

	// Test 1: Simple integer
	std::cout << "Test 1: Integer(42)\n";
	auto int_node = std::make_unique<IntegerNode>(42);
	int_node->print();
	std::cout << "\n";

	// Test 2: Binary operation: 5 + 3
	std::cout << "Test 2: 5 + 3\n";
	auto add_node = std::make_unique<BinaryOpNode>(
		BinaryOp::ADD,
		std::make_unique<IntegerNode>(5),
		std::make_unique<IntegerNode>(3)
	);
	add_node->print();
	std::cout << "\n";

	// Test 3: Complex expression: 2 + 3 * 4 
	std::cout << "Test 3: 2 + 3 * 4 (with correct precedence)\n";
	auto complex_expr = std::make_unique<BinaryOpNode>(
		BinaryOp::ADD,
		std::make_unique<IntegerNode>(2),
		std::make_unique<BinaryOpNode>(
			BinaryOp::MUL,
			std::make_unique<IntegerNode>(3),
			std::make_unique<IntegerNode>(4)
		)
	);
	complex_expr->print();
	std::cout << "\n";

	// Test 4: Assignment: x = 10
	std::cout << "Test 4: x = 10\n";
	auto assign_node = std::make_unique<AssignmentNode>(
		"x",
		std::make_unique<IntegerNode>(10)
	);
	assign_node->print();
	std::cout << "\n";

	// Test 5: Assignment with expression: result = 5 + 3
	auto assign_expr = std::make_unique<AssignmentNode>(
		"result",
		std::make_unique<BinaryOpNode>(
			BinaryOp::ADD,
			std::make_unique<IntegerNode>(5),
			std::make_unique<IntegerNode>(3)
		)
	);
	assign_expr->print();
	std::cout << "\n";

	// Test 6: Print statement
	std::cout << "Test 6: print(42)\n";
	auto print_node = std::make_unique<PrintNode>(
		std::make_unique<IntegerNode>(42)
	);
	print_node->print();
	std::cout << "\n";

	// Test 7: Full program
	std::cout << "Test 7: Full program\n";
	std::cout << "x = 5\n";
	std::cout << "y = 10\n";
	std::cout << "z = x + y\n";
	std::cout << "print(z)\n\n";

	auto program = std::make_unique<ProgramNode>();

	program->addStatement(std::make_unique<AssignmentNode>(
		"x",
		std::make_unique<IntegerNode>(5)
	));

	program->addStatement(std::make_unique<AssignmentNode>(
		"y",
		std::make_unique<IntegerNode>(10)
	));

	program->addStatement(std::make_unique<AssignmentNode>(
		"z",
		std::make_unique<BinaryOpNode>(
			BinaryOp::ADD,
			std::make_unique<IdentifierNode>("x"),
			std::make_unique<IdentifierNode>("y")
		)
	));

	program->addStatement(std::make_unique<PrintNode>(
		std::make_unique<IdentifierNode>("z")
	));

	program->print();

}

int main() {
	testAST();
	return 0;

	// testing lexer works:
	//std::string code = "x = 5 + 3";
	//Lexer lexer(code);
	//std::vector<Token> tokens = lexer.tokenize();

	//for (const auto& token : tokens) {
	//	std::cout << token << std::endl;
	//}
	//
	//return 0;
}