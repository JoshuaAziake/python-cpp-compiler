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
	void printIndent(int indent) const;
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

	explicit IntegerNode(int val);

	void print(int indent = 0) const override;
};

// literal float: 3.14
class FloatNode : public ExprNode {
public:
	double value;

	explicit FloatNode(double val);

	void print(int indent = 0) const override;
};

// boolean literal: True or False
class BooleanNode : public ExprNode {
public:
	bool value;

	explicit BooleanNode(bool val);

	void print(int indent = 0) const override;
};

// variable reference: x
class IdentifierNode : public ExprNode {
public:
	std::string name;

	explicit IdentifierNode(const std::string& n);

	void print(int indent = 0) const override;
};

enum class BinaryOp {
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	POWER,
	FLOOR_DIV,
	EQUAL,
	NOT_EQUAL,
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	AND,
	OR
};

std::string binaryOpToString(BinaryOp op);

class BinaryOpNode : public ExprNode {
public:
	BinaryOp op;
	std::unique_ptr<ExprNode> left;
	std::unique_ptr<ExprNode> right;

	BinaryOpNode(BinaryOp operation,
		std::unique_ptr<ExprNode> l,
		std::unique_ptr<ExprNode> r);

	void print(int indent = 0) const override;
};

enum class UnaryOp {
	NEGATE,
	PLUS,
	NOT
};

std::string unaryOpToString(UnaryOp op);

class UnaryOpNode : public ExprNode {
public:
	UnaryOp op;
	std::unique_ptr<ExprNode> operand;

	UnaryOpNode(UnaryOp operation, std::unique_ptr<ExprNode> expr);

	void print(int indent = 0) const override;
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

	AssignmentNode(const std::string& name, std::unique_ptr<ExprNode> val);

	void print(int indent = 0) const override;
};

// print statement (e.g. print(x))
class PrintNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> expression;

	explicit PrintNode(std::unique_ptr<ExprNode> expr);

	void print(int indent = 0) const override;
};

// expression statements, evaluate and discard (e.g. 2 + 3)
class ExprStmtNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> expression;

	explicit ExprStmtNode(std::unique_ptr<ExprNode> expr);

	void print(int indent = 0) const override;
};

// if statement with optional elif and else clauses
class IfNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> condition;
	std::vector<std::unique_ptr<StmtNode>> thenBlock;
	std::vector<std::unique_ptr<IfNode>> elifClauses;
	std::vector<std::unique_ptr<StmtNode>> elseBlock;

	IfNode(std::unique_ptr<ExprNode> cond, std::vector<std::unique_ptr<StmtNode>> thenStmts);

	void print(int indent = 0) const override;
};

// while loop
class WhileNode : public StmtNode {
public:
	std::unique_ptr<ExprNode> condition;
	std::vector<std::unique_ptr<StmtNode>> body;

	WhileNode(std::unique_ptr<ExprNode> cond, std::vector<std::unique_ptr<StmtNode>> bodyStmts);

	void print(int indent = 0) const override;
};

// program: sequence of statements
class ProgramNode : public ASTNode {
public:
	std::vector<std::unique_ptr<StmtNode>> statements;

	void addStatement(std::unique_ptr<StmtNode> stmt);

	void print(int indent = 0) const override;
};

#endif