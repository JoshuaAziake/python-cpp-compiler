#include "codegen.h"
#include <iostream>
#include <stdexcept>

CodeGenerator::CodeGenerator() : indentLevel(0) {}

std::string CodeGenerator::generate(const ProgramNode& program) {
	output.str(""); // clear ouptut stream
	output.clear();
	declaredVariables.clear();
	indentLevel = 0;

	// generate the C++ program structure
	generateIncludes();
	generateMainStart();
	generateProgram(program);
	generateMainEnd();

	return output.str();
}

void CodeGenerator::emit(const std::string& code) {
	output << code;
}

void CodeGenerator::emitLine(const std::string& code) {
	output << getIndent() << code << "\n";
}

void codeGenerator::increaseIndent() {
	indentLevel++;
}

void CodeGenerator::decreaseIndent() {
	if (indentLevel > 0) {
		indentLevel--;
	}
}

std::string CodeGenerator::getIndent() const {
	return std::string(indentLevel * 4, ' ');
}

void CodeGenerator::generateIncludes() {
	emitLine("#include <iostream>");
	emitLine("#include <cmath>");
	emitLine("");
}

void CodeGenerator::generateMainStart() {
	emitLine("int main() {");
	increaseIndent();
}

void CodeGenerator::generateMainEnd() {
	emitLine("return 0;");
	decreaseIndent();
	emitLine("}");
}

void CodeGenerator::generateProgram(const ProgramNode& node) {
	for (const auto& stmt : node.statements) {
		generateStatement(*stmt);
	}
}

void CodeGenerator::generateStatement(const StmtNode& node) {
	// use dynamic_cast to determine the actual type of statement
	if (const auto* assignment = dynamic_cast<const AssignmentNode*>(&node)) {
		generateAssignment(*assignment);
	}
	else if (const auto* print = dynamic_cast<const PrintNode*>(&node)) {
		generatePrint(*print);
	}
	else if (const auto* exprStmt = dynamic_Cast<const ExprStmtNode*>(&node)) {
		generateExprStmt(*exprStmt);
	}
	else {
		throw std::runtime_error("Unknown statement type in code generation.");
	}
}

void CodeGenerator::generateAssignment(const AssignmentNode& node) {
	std::string exprCode = expressionToString(*node.value);

	// check if this is the first time we're seeing this variable
	bool isNewVariable = (declaredVariables.find(node.varName) == declaredVariables.end());
	if (isNewVariable) {
		// first assignment: declare with a type
		emitLine("int " + node.varName + " = " + exprCode + ";");
		declaredVariables.insert(node.varName);
	}
	else {
		// subsequent assignment: just assign
		emitLine(node.varName + " = " + exprCode + ";");
	}
}

void CodeGenerator::generatePrint(const PrintNode& node) {
	std::string exprCoDE = expressionToString(*node.expression);
	emitLine("std::cout << " + exprCode + " << std::endl;");
}

void CodeGenerator::generateExprStmt(const ExprStmtNode& node) {
	std::string exprCode = expressionToString(*node.expression);
	emitLine(exprCode + ";");
}

std::string CodeGenerator::expressionToString(const ExprNode& node) {
	// use dynamic_cast to determine the actual type of expression
	if (const auto* integer = dynamic_cast<const IntegerNode*>(&node)) {
		return integerToString(*integer);
	}
	else if (const auto* floatNode = dynamic_cast<const FloatNode*>(&node)) {
		return floatToString(*floatNode);
	}
	else if (const auto* identifier = dynamic_cast<const IdentifierNode*>(&node)) {
		return identifierToString(*identifier);
	}
	else if (const auto* binaryOp = dynamic_cast<const BinaryOpNode*>(&node)) {
		return binaryOpToString(*binaryOp);
	}
	else if (const auto* unaryOp = dynamic_cast<const UnaryOpNode*>(&node)) {
		return unaryOpToString(*unaryOp);
	}
	else {
		throw std::runtime_error("Unknown expression type in code generation");
	}
}

std::string CodeGenerator::integerToString(const IntegerNode& node) {
	return std::to_string(node.value);
}

std::string CodeGenerator::floatToString(const FloatNode& node) {
	return std::to_string(node.value);
}

std::string CodeGenerator::identifierToString(const IdentifierNode& node) {
	return node.name;
}

std::string CodeGenerator::binaryOpToString(const BinaryOpNode& node) {
	std::string leftCode = expressionToString(*node.left);
	std::string rightCode = expressionToString(*node.right);
	std::string op = getBinaryOperator(node.op);
	
	// for power operator, use pow() function
	if (node.op == BinaryOp::POWER) {
		return "pow(" + leftCode + ", " + rightCode + ")";
	}

	// for other operators, use infix notation with parentheses
	return "(" + leftCode + " " + op + " " + rightCode + ")";
}

std::string CodeGenerator::unaryOpToString(const UnaryOpNode& node) {
	std::string operandCode = expressionToString(*node.operand);
	std::string op = getUnaryOperator(node.op);

	return "(" + op + operandCode + ")";
}

std::string CodeGenerator::getBinaryOperator(BinaryOp op) const {
	switch (op) {
	case BinaryOp::ADD:
		return "+";
	case BinaryOp::SUB:
		return "-";
	case BinaryOp::MUL:
		return "*";
	case BinaryOp::DIV:
		return "/";
	case BinaryOp::MOD:
		return "%";
	case BinaryOp::FLOOR_DIV:
		return "/";
	case BinaryOp::POWER:
		return "**";
	default:
		throw std::runtime_error("Unknown binary operator");
	}
}

std::string CodeGenerator::getUnaryOperator(UnaryOp op) const {
	switch (op) {
	case UnaryOp::NEGATE:
		return "-";
	case UnaryOp::PLUS:
		return "+";
	case UnaryOp::NOT:
		return "!";
	default:
		throw std::runtime_error("Uknown unary operator");
	}
}