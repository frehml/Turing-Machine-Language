//
// Created by dries on 12/6/21.
//

#include "Lexer.h"

class FileException : public std::exception {
public:
    const char *what() const noexcept override { return "File not found!\n"; }
};

Lexer::Lexer(const std::string &path) {

    std::ifstream file(path);
    std::string input;

    std::map<int, std::vector<char>> read_per_line;

    if (!file.is_open()) {
        throw FileException();
    }

    int index{0};

    while (std::getline(file, input)) {
        read_per_line[index] = split_into_words(input);
        read_per_line[index].emplace_back('\n');
        ++index;
        input.clear();
    }
    read_per_line[index - 1].pop_back();
    this->read_input = read_per_line;
}

Lexer::Lexer() {}

std::vector<char> Lexer::split_into_words(const std::string &input) {
    std::vector<char> to_export;
    std::string buffer;
    bool start{true};
    unsigned int apostrophe_count{0};
    for (const auto &current_letter: input) {
        if ((start or (apostrophe_count % 2 == 0)) and (current_letter == ' ' or current_letter == '\t')) {
            continue;
        } else if (current_letter == '\'') {
            apostrophe_count++;
            to_export.emplace_back(current_letter);
        } else {
            start = false;
            to_export.emplace_back(current_letter);
        }
    }

    return to_export;
}

std::string Lexer::longString() const {
    std::string to_export;
    for (const auto &line: read_input) {
        for (const auto &symbol: line.second) {
            to_export += symbol + ' ';
        }
        if (!line.second.empty()) {
            to_export.pop_back();
        }
        to_export += '\n';
    }
    return to_export;
}

const std::map<int, std::vector<char>> &Lexer::getReadInput() const {
    return read_input;
}
