#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

std::string readFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not open file: " + filename);
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
void writeFile(const std::string& filename, const std::string& content) {
	std::ofstream file(filename);
	if (!file) {
		throw std::runtime_error("Could not write to file: " + filename);
	}
	file << content;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <input.py>\n";
		std::cerr << "Example: " << argv[0] << " <program.py>\n";
		return 1;
	}

	std::string inputFile = argv[1];

	try {
		// read Python source code
		std::cout << "Reading Python file: " << inputFile << "\n";
		std::string pythonCode = readFile(inputFile);

		std::cout << "\n=== Python Source Code ===\n";
		std::cout << pythonCode << "\n";

		// Phase 1: Lexical Analysis
		std::cout << "\n=== Phase 1: Lexical Analysis ===\n";
		Lexer lexer(pythonCode);
		std::vector<Token> tokens = lexer.tokenize();
		std::cout << "Generated " << tokens.size() << " tokens\n";

		// Phase 2: Parsing
		std::cout << "\n=== Phase 2: Parsing ===\n";
		Parser parser(tokens);
		auto ast = parser.parse();
		std::cout << "AST Structure:\n";
		ast->print(1);

		// Phase  3: Code Generation
		std::cout << "\n=== Phase 3: Code Generation ===\n";
		CodeGenerator codegen;
		std::string cppCode = codegen.generate(*ast);

		std::cout << "Generated C++ Code:\n";
		std::cout << "----------------------------------------\n";
		std::cout << cppCode;
		std::cout << "----------------------------------------\n";

		// write to output file
		std::string outputFile = "output.cpp";
		writeFile(outputFile, cppCode);
		std::cout << "\nC++ code written to: " << outputFile << "\n";

		// compile C++ code
		std::cout << "\n=== Compiling C++ Code ===\n";
		std::string compileCmd = "g++ -o output " + outputFile;
		int result = system(compileCmd.c_str());

		if (result == 0) {
			std::cout << "Compilation successful!\n";
			std::cout << "\nYou can now run the compiled program with: ./output\n";
		}
		else {
			std::cout << "Compilation failed\n";
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "\nError: " << e.what() << "\n";
		return 1;
	}

	std::cout << "\n Compilation pipeline complete!\n";
	return 0;
}