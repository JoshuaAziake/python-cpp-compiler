#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>
#include <iostream>

// forward declarations
class ASTNode;
class ExprNode;
class StmtNode;

// Base Node
class ASTNode {
public:
	virtual ~ASTNode() = default;


	// for debugging: print tree structure
	virtual void print(int indent = 0) const = 0;

protected:
	// helper to print indentation
	void printIndent(int indent) const {
		for (int i = 0; i < indent; i++) {
			std::cout << " ";
		}
	}
};

// expression nodes (things that evaluate to a val)
class ExprNode : public ASTNode {
public:
	virtual ~ExprNode() = default;
};

// literal integer: 42
class IntegerNode : public ExprNode {
public:
	int value;

	explicit IntegerNode(int val) : value(val) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "Integer(" << value << ")\n";
	}
};

// literal float: 3.14
class FloatNode : public ExprNode {
public:
	double value;

	explicit FloatNode(double val) : value(val) {}

	void print(int indent = 0) const override {
		printIndent(indent);
	}
};

// variable reference: x
class IdentifierNode : public ExprNode {
public:
	std::string name;

	explicit IdentifierNode(const std::string& n) : name(n) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "Identifier(" << name << ")\n";
	}
};

enum class BinaryOp {
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	POWER,
	FLOOR_DIV
};

inline std::string binaryOpToString(BinaryOp op) {
	switch (op) {
		case BinaryOp::ADD: return "+";
		case BinaryOp::SUB: return "-";
		case BinaryOp::MUL: return "*";
		case BinaryOp::DIV: return "/";
		case BinaryOp::MOD: return "%";
		case BinaryOp::POWER: return "**";
		case BinaryOp::FLOOR_DIV: return "//";
		default: return "?";
	}
}

class BinaryOpNode : public ExprNode {
public:
	BinaryOp op;
	std::unique_ptr<ExprNode> left;
	std::unique_ptr<ExprNode> right;

	BinaryOpNode(BinaryOp operation, 
		std::unique_ptr<ExprNode> l, 
		std:: unique_ptr<ExprNode> r)
		: op(operation), left(std::move(l)), right(std::move(r)) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "BinaryOp(" << binaryOpToString(op) << ")\n";
		left->print(indent + 1);
		right->print(indent + 1);
	}
};

enum class UnaryOp {
	NEGATE,
	PLUS,
	NOT
};

inline std::string unaryOpToString(UnaryOp op) {
	switch (op) {
		case UnaryOp::NEGATE: return "-";
		case UnaryOp::PLUS: return "+";
		case UnaryOp::NOT: return "not";
		default: return "?";
	}
}

class UnaryOpNode : public ExprNode {
public:
	UnaryOp op;
	std::unique_ptr<ExprNode> operand;

	UnaryOpNode(UnaryOp operation, std::unique_ptr<ExprNode> expr)
		: op(operation), operand(std::move(expr)) {}

	void print(int indent = 0) const override {
		printIndent(indent); 
		std::cout << "UnaryOp(" << unaryOpToString(op) << ")\n";
		operand->print(indent + 1);
	}
};

// statement nodes (things that perform actions)
class StmtNode : public ASTNode {
public:
	virtual ~StmtNode() = default;
};

// assignment (e.g. x = 5 + 3)
class AssignmentNode : public StmtNode {
public:
	std::string varName;
	std::unique_ptr<ExprNode> value;

	AssignmentNode(const std::string& name, std::unique_ptr<ExprNode> val)
		: varName(name), value(std::move(val)) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "Assignment(" << varName << ")\n";
		value->print(indent + 1);
	}
};

// print statement (e.g. print(x))
class PrintNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> expression;

	explicit PrintNode(std::unique_ptr<ExprNode> expr)
		: expression(std::move(expr)) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "Print\n";
		expression->print(indent + 1);
	}
};

// expression statements, evaluate and discard (e.g. 2 + 3)
class ExprStmtNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> expression;

	explicit ExprStmtNode(std::unique_ptr<ExprNode> expr)
		: expression(std::move(expr)) {}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "ExprStmt\n";
		expression->print(indent + 1);
	}
};

// program: sequence of statements
class ProgramNode : public ASTNode {
public:
	std::vector<std::unique_ptr<StmtNode>> statements;

	void addStatement(std::unique_ptr<StmtNode> stmt) {
		statements.push_back(std::move(stmt));
	}

	void print(int indent = 0) const override {
		printIndent(indent);
		std::cout << "Program\n";
		for (const auto& stmt : statements) {
			stmt->print(indent + 1);
		}
	}
};

#endif