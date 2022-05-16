//
// Created by dries on 8/12/2021.
//

#ifndef TO_GROUP_MB_TABLE_H
#define TO_GROUP_MB_TABLE_H


#include <utility>
#include <vector>
#include "../CFG/Symbol.h"
#include "../CFG/GrammarRule.h"

enum entryType_ {
    kError, kShift, kReduce, kState, kAccept
};

typedef std::pair<entryType_, int> Action;

typedef std::vector<Action> Row;

typedef std::vector<Row> Table;

class ParsingTable {

    /*
     * table_[i][j] = type, action_of_type.
     * i = index of state
     * j = index of symbol
     */
    Table table_;

    std::vector<Symbol> terminals_;
    std::vector<Symbol> variables_;

    std::vector<GrammarRule> grammarRules_;

public:
    ParsingTable(const size_t amount_of_states, const size_t amount_of_symbols);

    const Table &getTable() const;

    Row &operator[](const int i);

    void setAction(const unsigned long state_id, const unsigned long symbol_id, const Action& action);

    void print();

};


#endif //TO_GROUP_MB_TABLE_H
