#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <vector>
#include <string>
#include <unordered_map>

class Lexer {
public:
	
	// construct lexer with Python source code
	Lexer(const std::string& source);

	// scans through entire source code and returns all tokens 
	std::vector<Token> tokenize();

private:
	std::string source;
	size_t position;
	size_t current_line;
	size_t current_column;

	// Python keyword lookup table
	std::unordered_map<std::string, TokenType> keywords;

	// initialize keywords map
	void initializeKeywords();

	// get current character without advancing
	char currentChar() const;

	// get next character without advancing
	char peekChar() const;

	// get the character n positions ahead without advancing
	char peekChar(size_t n) const;

	// move to the next character
	void advance();

	// skip spaces and tabs
	void skipWhitespace();

	void skipComment();

	bool isAtEnd() const;

	Token scanNumber();

	Token scanIdentifier();

	Token scanString(char quote);

	Token makeToken(TokenType type, const std::string& lexeme);

	Token errorToken(const std::string& message);

	// helper functions
	bool isDigit(char c) const;
	bool isAlpha(char c) const;
	bool isAlphaNumeric(char c) const;
};

#endif