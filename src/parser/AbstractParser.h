//
// Created by dries on 12/4/21.
//

#ifndef TO_GROUP_MB_ABSTRACTPARSER_H
#define TO_GROUP_MB_ABSTRACTPARSER_H

#include <list>
#include <iostream>
#include <vector>
#include <set>
#include "../abstractSyntaxTree/AbstractSyntaxTree.h"
#include "../CFG/GrammarRule.h"
#include "ParsingTable.h"
#include "../lexer/Lexer.h"

typedef std::set<Symbol> Symbols;
typedef std::vector<std::pair<GrammarRule, Symbols>> State_rules;
typedef std::vector<State_rules> States;

class LR1Parser {


    Symbol starting_symbol_ = Symbol("S\'", kVariable);
public:
    LR1Parser(const Symbol &startingSymbol);

    LR1Parser();

    States grammar_sets(const std::vector<GrammarRule> &grammar_rules, const Symbol &start_symbol,
                        std::vector<std::pair<GrammarRule, int>> &goto_set);

    std::vector<std::pair<GrammarRule, Symbols>> LR1_set(const GrammarRule &current_rule,
                                                         const std::vector<GrammarRule> &grammar_rules,
                                                         const Symbols &previous_follow_set);

    Symbols first(const Symbol &symbol, std::vector<Symbol> &checked, const std::vector<GrammarRule> &grammar_rules);

    Symbols follow(const Symbol &symbol, std::vector<Symbol> &checked, const std::vector<GrammarRule> &grammar_rules);
};

class FileNotFoundException : public std::exception {
    const char *what() const throw() {
        return "The Specified File Is Not Found!\n";
    }
};

class SymbolException : public std::exception {
    std::string information_;
public:
    SymbolException(const std::string &input_string, const int line) : information_(
            "Symbol " + input_string + " on line " + std::to_string(line) +
            " was unexpected!\n") {}

    const char *what() const noexcept override {
        return information_.c_str();
    }
};

class UnfoundSymbolException : public std::exception {
    std::string information_;
public:
    UnfoundSymbolException(const std::string &input_string, const int line) : information_(
            "Symbol " + input_string + " on line " + std::to_string(line) +
            " is not a valid symbol!\n") {}

    const char *what() const noexcept override {
        return information_.c_str();
    }
};


class LALR1Parser {
    // https://stackoverflow.com/questions/62075086/what-is-an-lr2-parser-how-does-it-differ-from-an-lr1-parser
    Lexer lexer_;

    LR1Parser lr1_parser_;

    std::vector<GrammarRule> grammar_rules_;

    std::set<Symbol> symbol_list_;

    Symbol starting_symbol_ = Symbol("S\'", kVariable);

    int first_state_index_{0};

public:
    const Lexer getLexer() const;

    void setLexer(const Lexer &lexer);

    const std::set<Symbol> &getSymbolList() const;

    void setSymbolList(const std::set<Symbol> &symbolList);

    const Symbol &getStartingSymbol() const;

    void setStartingSymbol(const Symbol &startingSymbol);

    const LR1Parser &getLr1Parser() const;

    void setLr1Parser(const LR1Parser &lr1Parser);

    const std::vector<GrammarRule> &getGrammarRules() const;

    void setGrammarRules(const std::vector<GrammarRule> &grammarRules);

    LALR1Parser(const Lexer &lexer, const std::vector<GrammarRule> &grammarRules, const Symbol &starting_symbol);

    LALR1Parser();

    LALR1Parser(const std::string &production_rule_file_path);

    const AbstractSyntaxTree parse();

    ParsingTable generate_parsing_table();

    States grammar_sets();

    void test();

    int getSymbolIndex(const Symbol &symbol);

    int getGotoStateIndex(const States &states, const GrammarRule &current_rule);

    int getReduceRuleIndex(const GrammarRule &current_rule, const std::vector<GrammarRule> &grammar_rules);

    int getFirstStateIndex();

    std::vector<std::string> splitIntoProductionRules(const std::string &input);


};

#endif //TO_GROUP_MB_ABSTRACTPARSER_H
