#include "parser.h"
#include <iostream>
#include <sstream>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

const Token& Parser::currentToken() const {
	if (current >= tokens.size()) {
		return tokens.back(); // Return EOF token
	}
	return tokens[current];
}

bool Parser::check(TokenType type) const {
	if (isAtEnd()) return type == TokenType::EOF_TOKEN;
	return currentToken().type == type;
}

bool Parser::match(TokenType type) {
	if (check(type)) {
		advance();
		return true;
	}
	return false;
}

Token Parser::advance() {
	if (!isAtEnd()) {
		current++;
	}
	return tokens[current - 1];
}

bool Parser::isAtEnd() const {
	return current >= tokens.size() || currentToken().type == TokenType::EOF_TOKEN;
}

Token Parser::consume(TokenType type, const std::string& message) {
	if (check(type)) {
		return advance();
	}
	error(message);
	throw ParseError(message);
}

std::unique_ptr<ProgramNode> Parser::parse() {
	return program();
}

std::unique_ptr<ProgramNode> Parser::program() {
	auto programNode = std::make_unique<ProgramNode>();

	// skip any leading newlines
	while (match(TokenType::NEWLINE)) {}

	// parse statements until EOF
	while (!isAtEnd()) {
		// skip newlines between statements
		while (match(TokenType::NEWLINE)) {}

		if (!isAtEnd()) {
			try {
				auto stmt = statement();
				if (stmt) {
					programNode->addStatement(std::move(stmt));
				}
			}
			catch (const ParseError& e) {
				// Try to recover and continue parsing
				synchronize();
			}
		}
	}

	return programNode;
}

std::unique_ptr<StmtNode> Parser::statement() {
	// skip newlines at start of statement
	while(match(TokenType::NEWLINE)){}

	if (isAtEnd()) {
		return nullptr;
	}

	// print statement
	if (check(TokenType::PRINT)) {
		return printStatement();
	}

	// assignment or expression statement
	// we need to look ahead to differentiate assignments and expressions
	if (check(TokenType::IDENTIFIER)) {
		// save current position in case we need to backtrack
		size_t savePos = current;

		// look ahead: identifier = ?
		advance(); // move past identifier
		bool isAssignment = check(TokenType::EQUAL);
		current = savePos; // reset to start

		if (isAssignment) {
			return assignment();
		}
		else {
			return expressionStatement();
		}
	}

	// o/w it's an expression statement
	return expressionStatement();
}

std::unique_ptr<StmtNode> Parser::assignment() {
	// parse: IDENTIFIER "=" expression

	Token nameToken = consume(TokenType::IDENTIFIER, "Expected identifier in assignment");
	std::string varName = nameToken.lexeme;

	consume(TokenType::EQUAL, "Expected '=' in assignment");

	auto value = expression();

	// consume optional newline at end of statement
	match(TokenType::NEWLINE);

	return std::make_unique<AssignmentNode>(varName, std::move(value));
}

std::unique_ptr<StmtNode> Parser::printStatement() {
	// Parse: "print" "(" epxression ")"

	consume(TokenType::PRINT, "Expected 'print'");
	consume(TokenType::LPAREN, "Expected '(' after 'print'");

	auto expr = expression();

	consume(TokenType::RPAREN, "Expected ')' after print expression");

	// consume optional newline at end of statement
	match(TokenType::NEWLINE);

	return std::make_unique<PrintNode>(std::move(expr));
}

std::unique_ptr<StmtNode> Parser::expressionStatement() {
	// parse: expression [NEWLINE]

	auto expr = expression();

	// consume optional newline at end of statement
	match(TokenType::NEWLINE);

	return std::make_unique<ExprStmtNode>(std::move(expr));
}

std::unique_ptr<ExprNode> Parser::expression() {
	// expression -> term (("+" | "-") term)*
	// this handles addition and subtraction (lowest precedence)

	auto left = term();

	while (match(TokenType::PLUS, TokenType::MINUS)) {
		TokenType op = tokens[current - 1].type;
		auto right = term();

		BinaryOp binaryOp = (op == TokenType::PLUS) ? BinaryOp::ADD : BinaryOp::SUB;
		left = std::make_unique<BinaryOpNode>(binaryOp, std::move(left), std::move(right));
	}

	return left;
}

std::unique_ptr<ExprNode> Parser::term() {
	// term -> factor (("*" | "/" | "%") factor )*
	// this handles mult, div, modulo (medium precedence)
	auto left = factor();

	while (match(TokenType::STAR, TokenType::SLASH, TokenType::PERCENT)) {
		TokenType op = tokens[current - 1].type;
		auto right = factor();

		BinaryOp binaryOp;
		switch (op) {
		case TokenType::STAR:
			binaryOp = BinaryOp::MUL;
			break;
		case TokenType::SLASH:
			binaryOp = BinaryOp::DIV;
			break;
		case TokenType::PERCENT:
			binaryOp = BinaryOp::MOD;
			break;
		default:
			error("Unexpected operator in term");
			throw ParseError("Unexpected operator");
		}

		left = std::make_unique<BinaryOpNode>(binaryOp, std::move(left), std::move(right));
	}

	return left;
}

std::unique_ptr<ExprNode> Parser::factor() {
	// factor -> unary (("**") unary)*
	// this handles exponentiation (right-associative, higher precedence than mult/div)

	auto left = unary();

	// right-associative: keep parsing from right to left
	if (match(TokenType::DOUBLE_STAR)) {
		auto right = factor(); // recursive call for right-associativity
		left = std::make_unique<BinaryOpNode>(BinaryOp::POWER, std::move(left), std::move(right));
	}

	return left;
}

std::unique_ptr<ExprNode> Parser::unary() {
	// unary -> ("-" | "+" || "not") unary | primary
	// this handles unary operators (minus, plus, not)

	if (match(TokenType::MINUS)) {
		auto expr = unary();
		return std::make_unique<UnaryOpNode>(UnaryOp::NEGATE, std::move(expr));
	}

	if (match(TokenType::PLUS)) {
		auto expr = unary();
		return std::make_unique<UnaryOpNode>(UnaryOp::PLUS, std::move(expr));
	}

	if (match(TokenType::NOT)) {
		auto expr = unary();
		return std::make_unique<UnaryOpNode>(UnaryOp::NOT, std::move(expr));
	}

	return primary();
}

std::unique_ptr<ExprNode> Parser::primary() {
	// primary -> INTEGER | FLOAT | STRING | TRUE | FALSE | IDENTIFIER | "(" expression ")"

	// Integer literal
	if (check(TokenType::INTEGER)) {
		Token token = advance();
		int value = std::stoi(token.lexeme);
		return std::make_unique<IntegerNode>(value);
	}

	// Float litteral
	if (check(TokenType::FLOAT)) {
		Token token = advance();
		double value = std::stod(token.lexeme);
		return std::make_unique<FloatNode>(value);
	}

	// String literal
	if (check(TokenType::STRING)) {
		Token token = advance();
		// The lexer already processed escape sequences, so token.lexeme is the actual string value
		// TODO: Create a proper StringNode class in the future
		// For now, we'll return a placeholder (0) since we don't have StringNode yet
		return std::make_unique<IntegerNode>(0);
	}

	// Boolean and None literals
	if (check(TokenType::TRUE)) {
		advance();
		// for now, represent True as 1
		return std::make_unique<IntegerNode>(1);
	}

	if (check(TokenType::FALSE)) {
		advance();
		// for now, represent False as 0
		return std::make_unique<IntegerNode>(0);
	}

	if (check(TokenType::NONE)) {
		advance();
		// for now represent None as 0
		return std::make_unique<IntegerNode>(0);
	}

	// identifier (variable reference)
	if (check(TokenType::IDENTIFIER)) {
		Token token = advance();
		return std::make_unique<IdentifierNode>(token.lexeme);
	}

	// parenthesized expression
	if (match(TokenType::LPAREN)) {
		auto expr = expression();
		consume(TokenType::RPAREN, "Expected ')' after expression");
		return expr;
	}

	// if we reach here, something went wrong
	error("Expected expression");
	throw ParseError("Expected expression but found: " + std::string(tokenTypeToString(currentToken().type)));
}

void Parser::error(const std::string& message) {
	const Token& token = currentToken();

	std::ostringstream oss;
	oss << "Parse Error at line " << token.line << ", column " << token.column << ": "
		<< message << " (found '" << tokenTypeToString(token.type) << "')";

	std::cerr << oss.str() << std::endl;
}

void Parser::synchronize() {
	advance();

	// skip tokens until we find something that looks like the start of a new statement
	while (!isAtEnd()) {
		// stop at newline (likely end of statement)
		if (tokens[current - 1].type == TokenType::NEWLINE) {
			return;
		}

		// stop at statement keywords
		switch (currentToken().type) {
		case TokenType::PRINT:
		case TokenType::IF:
		case TokenType::WHILE:
		case TokenType::FOR:
		case TokenType::DEF:
		case TokenType::CLASS:
			return;
		default:
			break;
		}
	
		advance();
	}
}