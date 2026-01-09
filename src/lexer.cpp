#include "lexer.h"
#include <iostream>

Lexer::Lexer(const std::string& source) : source(source), position(0), current_line(1), current_column(1) {
	initializeKeywords();

	indentStack.push_back(0);
	atLineStart = true;
	pendingDedents = 0;
}

void Lexer::initializeKeywords() {
	keywords["if"] = TokenType::IF;
	keywords["else"] = TokenType::ELSE;
	keywords["elif"] = TokenType::ELIF;
	keywords["while"] = TokenType::WHILE;
	keywords["for"] = TokenType::FOR;
	keywords["def"] = TokenType::DEF;
	keywords["return"] = TokenType::RETURN;
	keywords["class"] = TokenType::CLASS;
	keywords["import"] = TokenType::IMPORT;
	keywords["from"] = TokenType::FROM;
	keywords["in"] = TokenType::IN;
	keywords["and"] = TokenType::AND;
	keywords["or"] = TokenType::OR;
	keywords["not"] = TokenType::NOT;
	keywords["True"] = TokenType::TRUE;
	keywords["False"] = TokenType::FALSE;
	keywords["None"] = TokenType::NONE;
	keywords["pass"] = TokenType::PASS;
	keywords["break"] = TokenType::BREAK;
	keywords["continue"] = TokenType::CONTINUE;
	keywords["print"] = TokenType::PRINT;
}

char Lexer::currentChar() const {
	if (isAtEnd()) return '\0';
	return source[position];
}

char Lexer::peekChar() const {
	if (position + 1 >= source.length()) return '\0';
	return source[position + 1];
}

char Lexer::peekChar(size_t n) const {
	if (position + n >= source.length()) return '\0';
	return source[position + n];
}

bool Lexer::isAtEnd() const {
	return position >= source.length();
}

void Lexer::advance() {
	if (!isAtEnd()) {
		if (source[position] == '\n') {
			current_line++;
			current_column = 1;
		}
		else {
			current_column++;
		}
		position++;
	}
}

void Lexer::skipWhitespace() {
	while (!isAtEnd()) {
		char c = currentChar();
		if (c == ' ' || c == '\t' || c == '\r') {
			advance();
		}
		else {
			break;
		}
	}
}

void Lexer::skipComment() {
	while (!isAtEnd() && currentChar() != '\n') {
		advance();
	}
}

Token Lexer::makeToken(TokenType type, const std::string& lexeme) {
	return Token(type, lexeme, current_line, current_column);
}

Token Lexer::errorToken(const std::string& message) {
	return Token(TokenType::INVALID, message, current_line, current_column);
}

bool Lexer::isDigit(char c) const {
	return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
	return isAlpha(c) || isDigit(c);
}

Token Lexer::scanNumber() {
	size_t start = position;
	bool hasDecimal = false;

	// scan integer part
	while (!isAtEnd() && isDigit(currentChar())) {
		advance();
	}

	// check for decimal point
	if (!isAtEnd() && currentChar() == '.' && isDigit(peekChar())) {
		hasDecimal = true;
		advance();

		while (!isAtEnd() && isDigit(currentChar())) {
			advance();
		}
	}

	std::string lexeme = source.substr(start, position - start);
	TokenType type = hasDecimal ? TokenType::FLOAT : TokenType::INTEGER;

	return makeToken(type, lexeme);
}

Token Lexer::scanIdentifier() {
	size_t start = position;

	// assume first character is alpha or underscore
	advance();

	// continue if alphanumeric or underscore
	while (!isAtEnd() && isAlphaNumeric(currentChar())) {
		advance();
	}

	std::string lexeme = source.substr(start, position - start);

	// check if it's a keyword
	auto it = keywords.find(lexeme);
	if (it != keywords.end()) {
		return makeToken(it->second, lexeme);
	}

	// it's an identifier
	return makeToken(TokenType::IDENTIFIER, lexeme);
}

Token Lexer::scanString(char quote) {
	advance(); // skip opening quote

	std::string value;

	while (!isAtEnd() && currentChar() != quote) {
		if (currentChar() == '\\') {
			// handle escape sequences
			advance();
			if (!isAtEnd()) {
				char escaped = currentChar();
				switch (escaped) {
				case 'n': value += '\n'; break;
				case 't': value += '\t'; break;
				case '\\': value += '\\'; break;
				case '"': value += '"'; break;
				case '\'': value += '\''; break;
				default:
					value += '\\';
					value += escaped;
					break;
				}
				advance();
			}
		}
		else {
			value += currentChar();
			advance();
		}
	}

	if (isAtEnd()) {
		return errorToken("Unterminated string");
	}
	advance(); // skip closing quote

	return makeToken(TokenType::STRING, value);
}

std::vector<Token> Lexer::tokenize() {
	std::vector<Token> tokens;

	while (!isAtEnd()) {
		// handle indentation at the start of each line
		if (atLineStart) {
			auto indentTokens = handleIndentation();
			tokens.insert(tokens.end(), indentTokens.begin(), indentTokens.end());
		}
		
		skipWhitespace();
		
		if (isAtEnd()) break;

		char c = currentChar();

		// comments
		if (c == '#') {
			skipComment();
			continue;
		}

		// newlines
		if (c == '\n') {
			tokens.push_back(makeToken(TokenType::NEWLINE, "\\n"));
			advance();
			atLineStart = true;
			continue;
		}

		// numbers
		if (isDigit(c)) {
			tokens.push_back(scanNumber());
			continue;
		}

		// identifiers and keywords
		if (isAlpha(c)) {
			tokens.push_back(scanIdentifier());
			continue;
		}

		// strings
		if (c == '"' || c == '\'') {
			tokens.push_back(scanString(c));
			continue;
		}

		// operators and delimiters
		switch (c) {
			case '+':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::PLUS_EQUAL, "+="));
				}
				else {
					tokens.push_back(makeToken(TokenType::PLUS, "+"));
				}
				break;
			case '-':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::MINUS_EQUAL, "-="));
				}
				else if (currentChar() == '>') {
					advance();
					tokens.push_back(makeToken(TokenType::ARROW, "->"));
				}
				else {
					tokens.push_back(makeToken(TokenType::MINUS, "-"));
				}
				break;
			case '*':
				advance();
				if (currentChar() == '*') {
					advance();
					tokens.push_back(makeToken(TokenType::DOUBLE_STAR, "**"));
				}
				else if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::STAR_EQUAL, "*="));
				}
				else {
					tokens.push_back(makeToken(TokenType::STAR, "*"));
				}
				break;
			case '/':
				advance();
				if (currentChar() == '/') {
					advance();
					tokens.push_back(makeToken(TokenType::DOUBLE_SLASH, "//"));
				}
				else if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::SLASH_EQUAL, "/="));
				}
				else {
					tokens.push_back(makeToken(TokenType::SLASH, "/"));
				}
				break;

			case '%':
				advance();
				tokens.push_back(makeToken(TokenType::PERCENT, "%"));
				break;

			case '=':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::EQUAL_EQUAL, "=="));
				}
				else {
					tokens.push_back(makeToken(TokenType::EQUAL, "="));
				}
				break;

			case '!':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::NOT_EQUAL, "!="));
				}
				else {
					tokens.push_back(errorToken("Unexpected character '!'"));
				}
				break;
			case '<':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::LESS_EQUAL, "<="));
				}
				else {
					tokens.push_back(makeToken(TokenType::LESS, "<"));
				}
				break;

			case '>':
				advance();
				if (currentChar() == '=') {
					advance();
					tokens.push_back(makeToken(TokenType::GREATER_EQUAL, ">="));
				}
				else {
					tokens.push_back(makeToken(TokenType::GREATER, ">"));
				}
				break;

			case '(':
				advance();
				tokens.push_back(makeToken(TokenType::LPAREN, "("));
				break;

			case ')':
				advance();
				tokens.push_back(makeToken(TokenType::RPAREN, ")"));
				break;

			case '[':
				advance();
				tokens.push_back(makeToken(TokenType::LBRACKET, "["));
				break;

			case ']':
				advance();
				tokens.push_back(makeToken(TokenType::RBRACKET, "]"));
				break;
			case '{':
				advance();
				tokens.push_back(makeToken(TokenType::LBRACE, "{"));
				break;

			case '}':
				advance();
				tokens.push_back(makeToken(TokenType::RBRACE, "}"));
				break;

			case ',':
				advance();
				tokens.push_back(makeToken(TokenType::COMMA, ","));
				break;

			case ':':
				advance();
				tokens.push_back(makeToken(TokenType::COLON, ":"));
				break;

			case ';':
				advance();
				tokens.push_back(makeToken(TokenType::SEMICOLON, ";"));
				break;

			case '.':
				advance();
				if (isDigit(currentChar())) {
					position--;  // Back up to re-scan as number
					tokens.push_back(scanNumber());
				}
				else {
					tokens.push_back(makeToken(TokenType::DOT, "."));
				}
				break;
			default:
				tokens.push_back(errorToken(std::string("Unexpected character: ") + c));
				advance();
				break;
		}
	}

	// emit DEDENT tokens for any remaining indentation levels
	while (indentStack.size() > 1) {
		indentStack.pop_back();
		tokens.push_back(makeToken(TokenType::DEDENT, ""));
	}

	tokens.push_back(makeToken(TokenType::EOF_TOKEN, ""));
	return tokens;
}

int Lexer::countIndentation() {
	int spaces = 0;
	size_t startPos = position;

	// count spaces and tabs at the start of the line
	while (!isAtEnd() && (currentChar() == ' ' || currentChar() == '\t')) {
		if (currentChar() == '\t') {
			spaces += 8;
		}
		else {
			spaces += 1;
		}
		advance();
	}

	// if we hit a newline or comment, it's a blank line so we ignore indentation
	if (isAtEnd() || currentChar() == '\n' || currentChar() == '#') {
		position = startPos;
		return -1; // signal to skip this line
	}

	return spaces;
}

std::vector<Token> Lexer::handleIndentation() {
	std::vector<Token> tokens;

	int spaces = countIndentation();

	// if this was a blank line or comment, skip indentation handling
	if (spaces == -1) {
		return tokens;
	}

	int currentIndent = indentStack.back();

	if (spaces > currentIndent) {
		// indentation increased - emit INDENT
		indentStack.push_back(spaces);
		tokens.push_back(makeToken(TokenType::INDENT, ""));
	}
	else if (spaces < currentIndent) {
		// indentation decreased - emit DEDENT(s)
		while (!indentStack.empty() && indentStack.back() > spaces) {
			indentStack.pop_back();
			tokens.push_back(makeToken(TokenType::DEDENT, ""));
		}

		// check for indentation error
		if (indentStack.empty() || indentStack.back() != spaces) {
			tokens.push_back(errorToken("Indentation error: inconsistent indentation"));
		}
	}
	// If spaces == currentIndent, no change in indentation, no tokens needed

	atLineStart = false;
	return tokens;
}