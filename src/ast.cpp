#include "ast.h"

// ASTNode - Base class
void ASTNode::printIndent(int indent) const {
	for (int i = 0; i < indent; i++) {
		std::cout << " ";
	}
}

// IntegerNode
IntegerNode::IntegerNode(int val) : value(val) {}

void IntegerNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Integer(" << value << ")\n";
}

// FloatNode
FloatNode::FloatNode(double val) : value(val) {}

void FloatNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Float(" << value << ")\n";
}

// BooleanNode
BooleanNode::BooleanNode(bool val) : value(val) {}

void BooleanNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Boolean(" << (value ? "True" : "False") << ")\n";
}

// IdentifierNode
IdentifierNode::IdentifierNode(const std::string& n) : name(n) {}

void IdentifierNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Identifier(" << name << ")\n";
}

// BinaryOp utilities
inline std::string binaryOpToString(BinaryOp op) {
	switch (op) {
	// arithmetic operators
	case BinaryOp::ADD: return "+";
	case BinaryOp::SUB: return "-";
	case BinaryOp::MUL: return "*";
	case BinaryOp::DIV: return "/";
	case BinaryOp::MOD: return "%";
	case BinaryOp::POWER: return "**";
	case BinaryOp::FLOOR_DIV: return "//";
	// comparison operators
	case BinaryOp::EQUAL: return "==";
	case BinaryOp::NOT_EQUAL: return "!=";
	case BinaryOp::LESS: return "<";
	case BinaryOp::LESS_EQUAL: return "<=";
	case BinaryOp::GREATER: return ">";
	case BinaryOp::GREATER_EQUAL: return ">=";
	// logical operators
	case BinaryOp::AND: return "and";
	case BinaryOp::OR: return "or";
	default: return "?";
	}
}

// BinaryOpNode
BinaryOpNode::BinaryOpNode(BinaryOp operation, std::unique_ptr<ExprNode> l,
	std::unique_ptr<ExprNode> r) : op(operation), left(std::move(l)), right(std::move(r)) {}

void BinaryOpNode::print(int indent) const {
	printIndent(indent);
	std::cout << "BinaryOp(" << binaryOpToString(op) << ")\n";
	left->print(indent + 1);
	right->print(indent + 1);
}

// UnaryOp utilities
std::string unaryOpToString(UnaryOp op) {
	switch (op) {
	case UnaryOp::NEGATE: return "-";
	case UnaryOp::PLUS: return "+";
	case UnaryOp::NOT: return "not";
	default: return "?";
	}
}

// UnaryOpNode
UnaryOpNode::UnaryOpNode(UnaryOp operation, std::unique_ptr<ExprNode> expr)
	: op(operation), operand(std::move(expr)) {}

void UnaryOpNode::print(int indent) const {
	printIndent(indent);
	std::cout << "UnaryOp(" << unaryOpToString(op) << ")\n";
	operand->print(indent + 1);
}

// AssignmentNode
AssignmentNode::AssignmentNode(const std::string& name, std::unique_ptr<ExprNode> val)
	: varName(name), value(std::move(val)) {}

void AssignmentNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Assignment(" << varName << ")\n";
	value->print(indent + 1);
}

// PrintNode
PrintNode::PrintNode(std::unique_ptr<ExprNode> expr) : expression(std::move(expr)) {}

void PrintNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Print\n";
	expression->print(indent + 1);
}

// ExprStmtNode
ExprStmtNode::ExprStmtNode(std::unique_ptr<ExprNode> expr) : expression(std::move(expr)) {}

void ExprStmtNode::print(int indent) const {
	printIndent(indent);
	std::cout << "ExprStmt\n";
	expression->print(indent + 1);
}

// ProgramNode
void ProgramNode::addStatement(std::unique_ptr<StmtNode> stmt) {
	statements.push_back(std::move(stmt));
}

void ProgramNode::print(int indent) const {
	printIndent(indent);
	std::cout << "Program\n";
	for (const auto& stmt : statements) {
		stmt->print(indent + 1);
	}
}