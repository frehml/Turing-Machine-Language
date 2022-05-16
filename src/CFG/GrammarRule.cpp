//
// Created by dries on 8/12/2021.
//

#include "GrammarRule.h"

const Symbol &GrammarRule::getProductionSymbol() const {
    return Production_symbol;
}

void GrammarRule::setProductionSymbol(const Symbol &productionSymbol) {
    Production_symbol = productionSymbol;
}

const std::vector<Symbol> &GrammarRule::getResultingSymbols() const {
    return Resulting_symbols;
}

void GrammarRule::setResultingSymbols(const std::vector<Symbol> &resultingSymbols) {
    Resulting_symbols = resultingSymbols;
}

GrammarRule::GrammarRule(const Symbol &productionSymbol, const std::vector<Symbol> &resultingSymbols)
        : Production_symbol(productionSymbol), Resulting_symbols(resultingSymbols) {}

bool GrammarRule::operator==(const GrammarRule &other) const {
    return (this->getResultingSymbols() == other.getResultingSymbols() and
            this->getProductionSymbol() == other.getProductionSymbol());
}

bool GrammarRule::operator<(const GrammarRule &other) const {
    if(this->getProductionSymbol().getValue()==other.getProductionSymbol().getValue()){
        return (this->getResultingSymbols()<other.getResultingSymbols());
    }
    return (this->getProductionSymbol().getValue()<other.getProductionSymbol().getValue());
}


GrammarRule GrammarRule::add_dot_symbol() {
    GrammarRule new_rule(this->getProductionSymbol(), this->getResultingSymbols());

    std::vector<Symbol> new_production{Symbol(".", kTerminal)};
    for (const auto &copy_result: new_rule.getResultingSymbols())
        new_production.emplace_back(copy_result);

    new_rule.setResultingSymbols(new_production);
    return new_rule;
}

GrammarRule GrammarRule::swap_dot_symbol() const {

    GrammarRule new_rule(this->getProductionSymbol(), this->getResultingSymbols());
    auto new_productions = new_rule.getResultingSymbols();

    auto dot_iterator = std::find(new_productions.begin(), new_productions.end(),
                                  Symbol(".", kTerminal));
    auto tmp = *(dot_iterator);
    *(dot_iterator) = *(dot_iterator + 1);
    *(dot_iterator + 1) = tmp;

    new_rule.setResultingSymbols(new_productions);

    return new_rule;
}