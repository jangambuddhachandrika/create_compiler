#ifndef TOKENIZER_HPP
#define TOKENIZER_HPP

#include <string>
#include <fstream>

class Tokenizer {
private:
    std::ifstream content; // Stream for reading the file content.

public:
    // Constructor to initialize the Tokenizer with a file name.
    explicit Tokenizer(const std::string& filename);

    // Function to tokenize the file content.
    void tokenize();
};

#endif // TOKENIZER_HPP
