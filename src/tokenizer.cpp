#include "tokenizer.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <map>



enum class TokenType {
    AND,
    CLASS,
    ELSE,
    FALSE,
    FOR,
    FUN,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    IDENTIFIER,
    EOF_TYPE
};

// Define a map to store the keywords and their associated token types
std::map<std::string, TokenType> keywords;

// Function to initialize the map with the keywords
void initializeKeywords() {
    keywords["and"]    = TokenType::AND;
    keywords["class"]  = TokenType::CLASS;
    keywords["else"]   = TokenType::ELSE;
    keywords["false"]  = TokenType::FALSE;
    keywords["for"]    = TokenType::FOR;
    keywords["fun"]    = TokenType::FUN;
    keywords["if"]     = TokenType::IF;
    keywords["nil"]    = TokenType::NIL;
    keywords["or"]     = TokenType::OR;
    keywords["print"]  = TokenType::PRINT;
    keywords["return"] = TokenType::RETURN;
    keywords["super"]  = TokenType::SUPER;
    keywords["this"]   = TokenType::THIS;
    keywords["true"]   = TokenType::TRUE;
    keywords["var"]    = TokenType::VAR;
    keywords["while"]  = TokenType::WHILE;
}

// Function to convert TokenType enum to a string
std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::AND: return "AND";
        case TokenType::CLASS: return "CLASS";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FOR: return "FOR";
        case TokenType::FUN: return "FUN";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::SUPER: return "SUPER";
        case TokenType::THIS: return "THIS";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::EOF_TYPE: return "EOF";
    }
    return "UNKNOWN";  // Should not reach here
}

Tokenizer::Tokenizer(const std::string& filename) : content(filename) {
    if (!content.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        std::exit(1);
    }
}
bool hasErrors = false;

void Tokenizer::tokenize() {
    char c;
    while (content.get(c)) {
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace.
                break;
            case '\n':
                line++; // Increment line number on newline
                break;
            case '(':
                std::cout << "LEFT_PAREN ( null\n";
                break;
            case ')':
                std::cout << "RIGHT_PAREN ) null\n";
                break;
            case '{':
                std::cout << "LEFT_BRACE { null\n";
                break;
            case '}':
                std::cout << "RIGHT_BRACE } null\n";
                break;
            case '*':
                std::cout << "STAR * null\n";
                break;
            case '+':
                std::cout << "PLUS + null\n";
                break;
            case '-':
                std::cout << "MINUS - null\n";
                break;
            case ',':
                std::cout << "COMMA , null\n";
                break;
            case ';':
                std::cout << "SEMICOLON ; null\n";
                break;
            case '.':
                std::cout << "DOT . null\n";
                break;
            case '=':
                if (match('=')) {
                    std::cout << "EQUAL_EQUAL == null\n";
                } else {
                    std::cout << "EQUAL = null\n";
                }
                break;
            case '!':
                if (match('=')) {
                    std::cout << "BANG_EQUAL != null\n";
                } else {
                    std::cout << "BANG ! null\n";
                }
                break;
            case '<':
                if (match('=')) {
                    std::cout << "LESS_EQUAL <= null\n";
                } else {
                    std::cout << "LESS < null\n";
                }
                break;
            case '>':
                if (match('=')) {
                    std::cout << "GREATER_EQUAL >= null\n";
                } else {
                    std::cout << "GREATER > null\n";
                }
                break;
            case '/':
                if (match('/')) {
                    skipLine(); // Skip comment
                } else {
                    std::cout << "SLASH / null\n";
                }
                break;
            case '"':
                string();
                break;
            default:
            if(isdigit(c))
            {
                content.unget();
                number();
            }
            else if (isalpha(c)||c=='_') {
                    content.unget();
                    identifier();
             }
            else{
                std::cerr << "[line " << line << "] Error: Unexpected character: " << c << std::endl;
                hasErrors = true;
                break;
            }
        }
    }

    std::cout << "EOF  null" << std::endl;

    if (hasErrors) {
        std::exit(65);
    }
}

char Tokenizer::advance() {
    return content.get();
}

bool Tokenizer::match(char expected) {
    if (isAtEnd()) return false;
    char next = content.peek();
    if (next == expected) {
        content.get(); // Consume the character
        return true;
    }
    return false;
}

bool Tokenizer::isAtEnd() {
    return content.eof();
}

void Tokenizer::skipLine() {
    while (content.peek() != '\n' && !isAtEnd()) {
        advance();
    }
}
char Tokenizer::peekNext() {
    content.get(); // Advance to the next character
    char next = content.peek();
    content.unget(); // Rewind to the current position
    return isAtEnd() ? '\0' : next;
}

void Tokenizer::string() {
    size_t startIndex = content.tellg() - 1; // Position of the opening quote
    size_t currentIndex = startIndex;

    while (content.peek() != '"' && !isAtEnd()) {
        if (content.peek() == '\n') {
            line++; // Increment line number on newline
        }
        advance();
        currentIndex++;
    }

    if (isAtEnd()) {
        std::cerr << "[line " << line << "] Error: Unterminated string." << std::endl;
        hasErrors = true; 
        return;
    }

    // Consume the closing quote
    advance();

    // Extract the lexeme (including quotes)
    std::string lexeme(currentIndex - startIndex+2, '\0');
    content.seekg(startIndex);
    content.read(&lexeme[0], lexeme.size());

    // Extract the literal (contents inside quotes)
    std::string literal = lexeme.substr(1, lexeme.size() - 2);

    // Print the string token
    std::cout << "STRING " << lexeme << " " << literal << "\n";
}

void Tokenizer::number() {
    std::string number;
    
    // Capture the integer part
    while (isdigit(content.peek())) {
        number += advance();
    }

    // If there's a decimal point, capture the fractional part
    if (content.peek() == '.') {
        number += advance(); // Consume the dot
        while (isdigit(content.peek())) {
            number += advance();
        }
    }

    // Now, handle formatting and trimming
    std::string numberstr;

    size_t foundat = number.find('.');
    if (foundat == std::string::npos) {
        // No decimal point found, so add ".0"
        numberstr = number + ".0";
    } else {
        // Remove trailing zeros after the decimal point
        int i = number.size() - 1;
        while (i > foundat + 1 && number[i] == '0') {
            i--;
        }
        numberstr = number.substr(0, i + 1);
    }

    // Print the token
    std::cout << "NUMBER " << number << " " << numberstr << std::endl;
}

void Tokenizer::identifier()
{
    std::string identifier;
    while(isalpha(content.peek()))
    {
        identifier+=advance();
    }
    // Capture subsequent valid characters (letters, digits, or underscores)
    while (isalnum(content.peek()) || content.peek() == '_') {
        identifier += advance();
    }
    // Check if the identifier is a keyword
    initializeKeywords();
    auto it = keywords.find(identifier);
    if (it != keywords.end()) {
        // If it's a keyword, output the keyword token type
        TokenType keywordType = it->second;
        std::cout << tokenTypeToString(keywordType)<<" " << identifier << " null\n";
    } else {
        // If it's not a keyword, it's an identifier
        std::cout << "IDENTIFIER " << identifier << " null\n";
    }
}

