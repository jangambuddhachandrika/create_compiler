#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <fstream>
#include <string>

class Tokenizer {
public:
    explicit Tokenizer(const std::string& filename);
    void tokenize();

private:
    std::ifstream content;
    int line = 1; // Current line number

    // Utility functions
    char advance();
    bool match(char expected);
    bool isAtEnd();
    void skipLine();
    void string();
    void number();
    char peekNext();
    void identifier();
};

#endif
