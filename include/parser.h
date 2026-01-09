#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "ast.h"
#include <vector>
#include <memory>
#include <stdexcept>

// exception for parse errors
class ParseError : public std::runtime_error {
public:
	ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
public:
	// construct parser with tokens from lexer
	Parser(const std::vector<Token>& tokens);

	// parse tokens into AST
	// returns a ProgramNode containing all statements
	std::unique_ptr<ProgramNode> parse();

private:
	std::vector<Token> tokens;
	size_t current;

	// == token management ==

	// get current token without consuming it
	const Token& currentToken() const;

	// check if current token is of given type
	bool check(TokenType type) const;

	// if current token matches type, consume it and return true
	bool match(TokenType type);

	// if current token matches any type(s), consume and return true
	template<typename... Args>
	bool match(TokenType first, Args... rest);

	// Consume a token of the given type or throw error
	Token consume(TokenType first, const std::string& message);

	// Move to next token
	Token advance();

	// Check if we're at the end of tokens
	bool isAtEnd() const;

	// TBD: add floor division
	// == grammar rules ==
	/*
	* Grammar in EBNF notation:
	* 
	* program -> statement* EOF
	* statement -> assignment | printStmt | exprStmt
	* ifStmt -> "if" expression ":" NEWLINE INDENT statement+ DEDENT
	*				("elif" expression ":" NEWLINE INDENT statement+ DEDENT)*
	*				("else" ":" NEWLINE INDENT statement+ DEDENT)?
	* whileStmt -> "while" expression ":" NEWLINE INDENT statement+ DEDENT
	* assignment -> IDENTIFIER "=" expression
	* printStmt -> "print" "(" expression ")"
	* exprStmt -> expression NEWLINE?
	* 
	* expression -> logicalOr
	* logicalOr -> logicalAnd ( "or" logicalAnd )*
	* logicalAnd -> logicalNot ( "and" logicalNot )*
	* logicalNot -> "not" logicalNot | comparison
	* comparison -> addition (("==" | "!=" | "<" | "<=" | ">" | ">=") addition )*
	* addition -> term ( ("+" | "-") term )*
	* term -> factor ( ("*" | "/" | "%") factor )*
	* factor -> unary ( ("**") unary )*
	* unary -> ("-" | "+") unary | primary
	* primary -> INTEGER | FLOAT | STRING | TRUE | FALSE | NONE 
	* | IDENTIFIER | "(" expression ")"
	*/

	// parse program
	std::unique_ptr<ProgramNode> program();

	// parse statements
	std::unique_ptr<StmtNode> statement();
	std::unique_ptr<StmtNode> assignment();
	std::unique_ptr<StmtNode> printStatement();
	std::unique_ptr<StmtNode> expressionStatement();
	std::unique_ptr<StmtNode> ifStatement();
	std::unique_ptr<StmtNode> whileStatement();
	std::vector<std::unique_ptr<StmtNode>> parseBlock();

	// parse expressions
	std::unique_ptr<ExprNode> expression();
	std::unique_ptr<ExprNode> logicalOr();
	std::unique_ptr<ExprNode> logicalAnd();
	std::unique_ptr<ExprNode> logicalNot();
	std::unique_ptr<ExprNode> comparison();
	std::unique_ptr<ExprNode> addition();
	std::unique_ptr<ExprNode> term();
	std::unique_ptr<ExprNode> factor();
	std::unique_ptr<ExprNode> unary();
	std::unique_ptr<ExprNode> primary();

	// == error handling ==
	void error(const std::string& message);
	void synchronize();
};

template<typename... Args>
bool Parser::match(TokenType first, Args... rest) {
	if (check(first)) {
		advance();
		return true;
	}

	// if there are more types to check, recursively check them
	if constexpr (sizeof...(rest) > 0) {
		return match(rest...);
	}

	return false;
}
#endif