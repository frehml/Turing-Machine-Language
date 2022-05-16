//
// Created by dries on 8/12/2021.
//

#ifndef TO_GROUP_MB_GRAMMARRULE_H
#define TO_GROUP_MB_GRAMMARRULE_H


#include <string>
#include <vector>
#include <algorithm>
#include "Symbol.h"

class GrammarRule {

    Symbol Production_symbol;

    std::vector<Symbol> Resulting_symbols;
public:
    GrammarRule(const Symbol &productionSymbol, const std::vector<Symbol> &resultingSymbols);

    const Symbol &getProductionSymbol() const;

    void setProductionSymbol(const Symbol &productionSymbol);

    const std::vector<Symbol> &getResultingSymbols() const;

    void setResultingSymbols(const std::vector<Symbol> &resultingSymbols);

    bool operator==(const GrammarRule &other) const;

    bool operator<(const GrammarRule &other) const;

    GrammarRule add_dot_symbol();

    GrammarRule swap_dot_symbol() const;

};


#endif //TO_GROUP_MB_GRAMMARRULE_H
