#include "tokenizer.hpp"
#include <iostream>
Tokenizer::Tokenizer(const std::string& filename) : content(filename) {
}
void Tokenizer::tokenize() {
    char c;
    while (content.get(c)) {
        switch (c) {
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
        }
    }
    std::cout << "EOF  null" << std::endl;
}