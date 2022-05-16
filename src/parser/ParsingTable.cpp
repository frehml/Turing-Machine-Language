//
// Created by dries on 8/12/2021.
//

#include <iostream>
#include "ParsingTable.h"

ParsingTable::ParsingTable(const size_t amount_of_states, const size_t amount_of_symbols) {
    Action tmp(kError, 0);
    Row tmp2;
    for (unsigned long i{0}; i < amount_of_symbols; ++i)
        tmp2.emplace_back(tmp);
    for (size_t i{0}; i < amount_of_states; ++i) {
        this->table_.emplace_back(tmp2);
    }
}

const Table &ParsingTable::getTable() const {
    return table_;
}

Row &ParsingTable::operator[](const int i) {
    return this->table_[i];
}

void ParsingTable::setAction(const unsigned long state_id, const unsigned long symbol_id, const Action &action) {
    if (this->table_[state_id][symbol_id] != Action(kError, 0)) {
        std::cout<<"ALREADY FILLED!";
    }
    this->table_[state_id][symbol_id] = action;
}

void ParsingTable::print() {
    int state_index{0};
    for (const auto &it: this->table_) {
        std::cout << state_index << " : ";
        for (const auto &it2: it) {
            switch (it2.first) {
                case kError:
                    std::cout << " ";
                    break;
                case kShift:
                    std::cout << "S" << it2.second << " ";
                    break;
                case kReduce:
                    std::cout << "R" << it2.second << " ";
                    break;
                case kAccept:
                    std::cout << "Acc ";
                    break;
                case kState:
                    std::cout << it2.second << " ";
                    break;
            }
        }
        std::cout << std::endl;
        state_index++;
    }
}
