#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <ostream>

// Token types for Python subset
enum class TokenType {

	// Literals
	INTEGER,
	FLOAT,
	STRING,
	TRUE,
	FALSE,
	NONE,

	// Identifiers and keywords
	IDENTIFIER,	// variable names

	// Keywords
	IF,
	ELIF,
	ELSE,
	WHILE,
	FOR,
	DEF,
	RETURN,
	CLASS,
	IMPORT,
	FROM,
	IN,
	AND,
	OR,
	NOT,
	PASS,
	BREAK,
	CONTINUE,
	PRINT,

	// Operators
	PLUS,
	MINUS,
	STAR,
	SLASH,
	PERCENT,
	DOUBLE_STAR,
	DOUBLE_SLASH,

	// Comparison operators
	EQUAL_EQUAL,
	NOT_EQUAL,
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,

	// Assignment
	EQUAL,
	PLUS_EQUAL,
	MINUS_EQUAL,
	STAR_EQUAL,
	SLASH_EQUAL,

	// Delimiters
	LPAREN,
	RPAREN,
	LBRACKET,
	RBRACKET,
	LBRACE,
	RBRACE,
	COMMA,
	COLON,
	SEMICOLON,
	DOT,
	ARROW,

	// Special
	NEWLINE,	// \n
	INDENT,		// indentation increase
	DEDENT,		// indentation decrease
	EOF_TOKEN,	// end of file
	INVALID		// for error handling
};

// Token structure
struct Token {
	TokenType type;
	std::string lexeme;
	int line;
	int column;

	Token(TokenType t, const std::string& lex, int ln = 0, int col = 0) : type(t), lexeme(lex), line(ln), column(col) {}
};

// helper function to convert TokenType to string (for debugging)
inline std::string tokenTypeToString(TokenType type) {
	switch (type) {
	case TokenType::INTEGER: return "INTEGER";
	case TokenType::FLOAT: return "FLOAT";
	case TokenType::STRING: return "STRING";
	case TokenType::TRUE: return "TRUE";
	case TokenType::FALSE: return "FALSE";
	case TokenType::NONE: return "NONE";
	case TokenType::IDENTIFIER: return "IDENTIFIER";
	case TokenType::IF: return "IF";
	case TokenType::ELIF: return "ELIF";
	case TokenType::ELSE: return "ELSE";
	case TokenType::WHILE: return "WHILE";
	case TokenType::FOR: return "FOR";
	case TokenType::DEF: return "DEF";
	case TokenType::RETURN: return "RETURN";
	case TokenType::CLASS: return "CLASS";
	case TokenType::IMPORT: return "IMPORT";
	case TokenType::FROM: return "FROM";
	case TokenType::IN: return "IN";
	case TokenType::AND: return "AND";
	case TokenType::OR: return "OR";
	case TokenType::NOT: return "NOT";
	case TokenType::PASS: return "PASS";
	case TokenType::BREAK: return "BREAK";
	case TokenType::CONTINUE: return "CONTINUE";
	case TokenType::PRINT: return "PRINT";
	case TokenType::PLUS: return "PLUS";
	case TokenType::MINUS: return "MINUS";
	case TokenType::STAR: return "STAR";
	case TokenType::SLASH: return "SLASH";
	case TokenType::PERCENT: return "PERCENT";
	case TokenType::DOUBLE_STAR: return "DOUBLE_STAR";
	case TokenType::DOUBLE_SLASH: return "DOUBLE_SLASH";
	case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
	case TokenType::NOT_EQUAL: return "NOT_EQUAL";
	case TokenType::LESS: return "LESS";
	case TokenType::LESS_EQUAL: return "LESS_EQUAL";
	case TokenType::GREATER: return "GREATER";
	case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
	case TokenType::EQUAL: return "EQUAL";
	case TokenType::PLUS_EQUAL: return "PLUS_EQUAL";
	case TokenType::MINUS_EQUAL: return "MINUS_EQUAL";
	case TokenType::STAR_EQUAL: return "STAR_EQUAL";
	case TokenType::SLASH_EQUAL: return "SLASH_EQUAL";
	case TokenType::LPAREN: return "LPAREN";
	case TokenType::RPAREN: return "RPAREN";
	case TokenType::LBRACKET: return "LBRACKET";
	case TokenType::RBRACKET: return "RBRACKET";
	case TokenType::LBRACE: return "LBRACE";
	case TokenType::RBRACE: return "RBRACE";
	case TokenType::COMMA: return "COMMA";
	case TokenType::COLON: return "COLON";
	case TokenType::SEMICOLON: return "SEMICOLON";
	case TokenType::DOT: return "DOT";
	case TokenType::ARROW: return "ARROW";
	case TokenType::NEWLINE: return "NEWLINE";
	case TokenType::INDENT: return "INDENT";
	case TokenType::DEDENT: return "DEDENT";
	case TokenType::EOF_TOKEN: return "EOF";
	case TokenType::INVALID: return "INVALID";
	default: return "UNKNOWN";
	}
}

// overload << operator for easy printing
inline std::ostream& operator<<(std::ostream& os, const Token& token) {
	os << tokenTypeToString(token.type) << "('" << token.lexeme << "')";
	return os;
}

#endif 