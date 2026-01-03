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

	// get token n positions ahead
	const Token& peekToken(size_t n = 1) const;

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

	// == grammar rules ==
	/*
	* Grammar in EBNF notation:
	* 
	* program -> statement* EOF
	* statement -> assignment | printStmt | exprStmt
	* assignment -> IDENTIFIER "=" expression
	* printStmt -> "print" "(" expression ")"
	* exprStmt -> expression NEWLINE?
	* 
	* expression -> term ( ("+" | "-") term )*
	* term -> factor ( ("*" | "/" | "%") factor )*
	* factor -> unary ( ("**") unary )*
	* unary -> ("-" | "+" | "not") factor | power
	* power -> primary
	* primary -> INTEGER | FLOAT | STRING | TRUE | FALSE | NONE 
	* | IDENTIFIER | "(" expression ")"
	*/

	// parse entire program
	std::unique_ptr<ProgramNode> program();

	// parse a single statement
	std::unique_ptr<StmtNode> statement();

	// parse assignment statement
	std::unique_ptr<StmtNode> assignment();

	// parse print statement
	std::unique_ptr<StmtNode> printStatement();

	// parse expression statement
	std::unique_ptr<StmtNode> expressionStatement();

	// parse expression (addition/subtraction)
	std::unique_ptr<ExprNode> expression();

	// parse term (multiplication/division/modulo)
	std::unique_ptr<ExprNode> term();

	// parse factor (exponentiation)
	std::unique_ptr<ExprNode> factor();

	// parse unary (unary operators)
	std::unique_ptr<ExprNode> unary();

	// parse primary (literals, identifiers, parentheses)
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