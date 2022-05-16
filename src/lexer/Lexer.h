//
// Created by dries on 12/6/21.
//

#ifndef TO_GROUP_MB_LEXER_H
#define TO_GROUP_MB_LEXER_H


#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>

class Lexer {
    std::map<int, std::vector<char>> read_input;

public:
    Lexer(const std::string &path);

    Lexer();

    static std::vector<char> split_into_words(const std::string &input);

    std::string longString() const;

    const std::map<int, std::vector<char>> &getReadInput() const;

};


#endif //TO_GROUP_MB_LEXER_H
