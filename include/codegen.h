#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <string>
#include <sstream>
#include <unordered_set>

class CodeGenerator {
public:
	CodeGenerator();

	// generate C++ code from an AST
	std::string generate(const ProgramNode& program);

private:
	std::stringstream output;
	int indentLevel;

	// track variable types for proper code generation
	std::unordered_set<std::string> declaredVariables;

	// helper methods for code generation
	void emit(const std::string& code);
	void emitLine(const std::string& code);
	void increaseIndent();
	void decreaseIndent();
	std::string getIndent() const;

	// generate code for different node types
	void generateProgram(const ProgramNode& node);
	void generateStatement(const StmtNode& node);
	void generateExpression(const ExprNode& node);

	// specific statement generators
	void generateAssignment(const AssignmentNode& node);
	void generatePrint(const PrintNode& node);
	void generateExprStmt(const ExprStmtNode& node);

	// specific expression generators
	std::string expressionToString(const ExprNode& node);
	std::string integerToString(const IntegerNode& node);
	std::string floatToString(const FloatNode& node);
	std::string identifierToString(const IdentifierNode& node);
	std::string binaryOpToString(const BinaryOpNode& node);
	std::string unaryOpToString(const UnaryOpNode& node);
	std::string booleanToString(const BooleanNode& node);

	// helper to convert BinaryOp enum to C++ operator string
	std::string getBinaryOperator(BinaryOp op) const;
	std::string getUnaryOperator(UnaryOp op) const;

	// header and includes
	void generateIncludes();
	void generateMainStart();
	void generateMainEnd();
};

#endif