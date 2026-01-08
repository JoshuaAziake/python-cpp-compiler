#include "lexer.h"
#include "ast.h"
#include "parser.h"
#include <iostream>

void testParser() {
	std::cout << "=== Testing Parser ===\n\n";

	// test 1: simple expression
	std::cout << "Test 1: 2 + 3\n";
	std::string code1 = "2 + 3";
	Lexer lexer1(code1);
	std::vector<Token> tokens1 = lexer1.tokenize();
	Parser parser1(tokens1);
	auto ast1 = parser1.parse();
	ast1->print();
	std::cout << "\n";

	// test 2: assignment
	std::cout << "Test 2: x = 5 + 3\n";
	std::string code2 = "x = 5 + 3";
	Lexer lexer2(code2);
	std::vector<Token> tokens2 = lexer2.tokenize();
	Parser parser2(tokens2);
	auto ast2 = parser2.parse();
	ast2->print();
	std::cout << "\n";

	// test 3: complex expression with precedence
	std::cout << "Test 3: 2 + 3 * 4\n";
	std::string code3 = "2 + 3 * 4";
	Lexer lexer3(code3);
	std::vector<Token> tokens3 = lexer3.tokenize();
	Parser parser3(tokens3);
	auto ast3 = parser3.parse();
	ast3->print();
	std::cout << "\n";

	// test 4: print statement
	std::cout << "Test 4: print(42)\n";
	std::string code4 = "print(42)";
	Lexer lexer4(code4);
	std::vector<Token> tokens4 = lexer4.tokenize();
	Parser parser4(tokens4);
	auto ast4 = parser4.parse();
	ast4->print();
	std::cout << "\n";

	// test 5: full program
	std::cout << "Test 5: Full program\n";
	std::string code5 = "x = 5\ny = 10\nz = x + y\nprint(z)";
	Lexer lexer5(code5);
	std::vector<Token> tokens5 = lexer5.tokenize();
	Parser parser5(tokens5);
	auto ast5 = parser5.parse();
	ast5->print();
}


//int main() {
//	testParser();
//	return 0;

	// testing lexer works:
	//std::string code = "x = 5 + 3";
	//Lexer lexer(code);
	//std::vector<Token> tokens = lexer.tokenize();

	//for (const auto& token : tokens) {
	//	std::cout << token << std::endl;
	//}
	//
	//return 0;
//}