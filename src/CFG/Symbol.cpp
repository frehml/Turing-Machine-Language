//
// Created by dries on 8/12/2021.
//

#include "Symbol.h"

Symbol::Symbol(const std::string &symbol_value, const type &symbol_type) : value_(symbol_value), type_(symbol_type) {}

const std::string &Symbol::getValue() const {
    return value_;
}

void Symbol::setValue(const std::string &value) {
    Symbol::value_ = value;
}

type Symbol::getType() const {
    return type_;
}

bool Symbol::operator==(const Symbol &other) const {
    return (other.getValue() == this->getValue() and
            other.getType() == this->getType());
}

bool Symbol::operator<(const Symbol &other) const {
    return (this->getValue() < other.getValue());
}

void Symbol::setType(const type &new_type) {
    this->type_ = new_type;
}

