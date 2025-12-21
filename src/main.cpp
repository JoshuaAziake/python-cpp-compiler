#include "lexer.h"
#include <iostream>
#include <vector>

int main() {
	std::string code = "x = 5 + 3";
	Lexer lexer(code);
	std::vector<Token> tokens = lexer.tokenize();

	for (const auto& token : tokens) {
		std::cout << token << std::endl;
	}
	
	return 0;
}