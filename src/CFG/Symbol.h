//
// Created by dries on 8/12/2021.
//

#ifndef TO_GROUP_MB_SYMBOL_H
#define TO_GROUP_MB_SYMBOL_H

#include <string>

enum type {
    kVariable, kTerminal
};

class Symbol {
    std::string value_;

    type type_;
public:
    const std::string &getValue() const;

    void setValue(const std::string &value);

    type getType() const;

    void setType(const type &new_type);

    Symbol(const std::string &symbol_value, const type &symbol_type);

    bool operator==(const Symbol &other) const;

    bool operator<(const Symbol &other) const;
};


#endif //TO_GROUP_MB_SYMBOL_H
