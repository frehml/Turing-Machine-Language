//
// Created by dries on 12/4/21.
//

#include <queue>
#include <stack>
#include "AbstractParser.h"

States LR1Parser::grammar_sets(const std::vector<GrammarRule> &grammar_rules, const Symbol &start_symbol,
                               std::vector<std::pair<GrammarRule, int>> &goto_set) {

    GrammarRule starting_rule(start_symbol, {});

    for (const GrammarRule &rule: grammar_rules) {
        if (rule.getProductionSymbol() == start_symbol) {
            starting_rule.setResultingSymbols(rule.getResultingSymbols());
            break;
        }
    }

    GrammarRule new_rule = starting_rule.add_dot_symbol();


    State_rules checked_rules;
    State_rules unchecked_rules;

    States states;

    unchecked_rules = this->LR1_set(new_rule, grammar_rules, {});

    states.emplace_back(this->LR1_set(new_rule, grammar_rules, {}));
    State_rules new_unchecked_rules;

    bool found_new_rule{false};
    do {
        found_new_rule = false;

        new_unchecked_rules.clear();
        for (const auto &unchecked_rule: unchecked_rules) {
            if (std::find(checked_rules.begin(), checked_rules.end(), unchecked_rule) == checked_rules.end()) {
                found_new_rule = true;
                checked_rules.emplace_back(unchecked_rule);
                State_rules lr1_set;
                if (unchecked_rule.first.getResultingSymbols().size() != 1) {
                    lr1_set = this->LR1_set(unchecked_rule.first.swap_dot_symbol(), grammar_rules,
                                            unchecked_rule.second);

                } else {
                    continue;
                }

                for (const auto &state: lr1_set) {
                    if (std::find(unchecked_rules.begin(), unchecked_rules.end(), state) == unchecked_rules.end() &&
                        !(Symbol(".", kTerminal) == state.first.getResultingSymbols().back())) {

                        new_unchecked_rules.emplace_back(state.first, state.second);
                    }
                }

                states.emplace_back(lr1_set);
                goto_set.emplace_back(std::make_pair(unchecked_rule.first, states.size() - 1));

            }
        }

        for (const auto &it: new_unchecked_rules)
            unchecked_rules.emplace_back(it);

        std::sort(unchecked_rules.begin(), unchecked_rules.end());
        std::sort(checked_rules.begin(), checked_rules.end());
    } while (unchecked_rules != checked_rules and found_new_rule);

    return states;
}

std::vector<std::pair<GrammarRule, Symbols>> LR1Parser::LR1_set(const GrammarRule &current_rule,
                                                                const std::vector<GrammarRule> &grammar_rules,
                                                                const Symbols &previous_follow_set) {
    std::vector<std::pair<GrammarRule, Symbols>> to_export;

    std::vector<Symbol> checked_variables;
    std::vector<Symbol> new_variables;
    Symbol dot_symbol = Symbol(".", kTerminal);
    const std::vector<Symbol> epsilon_production{Symbol("", kTerminal)};


    const auto dot_ptr_start = std::find(current_rule.getResultingSymbols().begin(),
                                         current_rule.getResultingSymbols().end(), dot_symbol);
    if ((dot_ptr_start + 1) != current_rule.getResultingSymbols().end())
        new_variables.emplace_back(*(dot_ptr_start + 1));
    else if ((dot_ptr_start) == current_rule.getResultingSymbols().end()) {
        std::cerr << "DIT IS ECHT EEN PROBLEEM! DOT_PTR_START NIET GEVONDEN!\n";
        return to_export;
    }

    std::vector<Symbol> tmp;

    auto follow_set = previous_follow_set;

    follow_set = this->follow(current_rule.getProductionSymbol(), tmp, grammar_rules);

    Symbols symbols;
    for (const auto &it: follow_set) {
        symbols.insert(it);
    }

    to_export.emplace_back(current_rule, symbols);

    if (*(dot_ptr_start + 1) == this->starting_symbol_)
        to_export.emplace_back(std::make_pair(current_rule, Symbols{Symbol("$", kTerminal)}));

    while (new_variables != checked_variables) {

        checked_variables = new_variables;

        for (const auto &new_variable: new_variables) {
            for (const auto &production: grammar_rules) {
                if (new_variable.getValue() == production.getProductionSymbol().getValue()) {
                    if (production.getResultingSymbols() != epsilon_production) {

                        GrammarRule new_rule = production;
                        std::vector<Symbol> new_production{dot_symbol};
                        for (const auto &copy_result: new_rule.getResultingSymbols())
                            new_production.emplace_back(copy_result);

                        new_rule.setResultingSymbols(new_production);

                        tmp.clear();

                        auto new_rules = grammar_rules;
                        for (auto &it: new_rules) {
                            std::vector<Symbol> resulting_symbols_copy;
                            if (it.getResultingSymbols() == production.getResultingSymbols()) {
                                resulting_symbols_copy = it.getResultingSymbols();
                                long count = std::count(resulting_symbols_copy.begin(), resulting_symbols_copy.end(),
                                                        *(dot_ptr_start + 1));
                                for (int i{1}; i < count; ++i) {
                                    resulting_symbols_copy.erase(
                                            std::find(resulting_symbols_copy.begin(), resulting_symbols_copy.end(),
                                                      *(dot_ptr_start + 1)));
                                }
                                it.setResultingSymbols(resulting_symbols_copy);
                            }
                        }

                        follow_set = previous_follow_set;

                        if (previous_follow_set.empty())
                            follow_set = this->follow(new_rule.getProductionSymbol(), tmp, new_rules);

                        symbols.clear();
                        for (const auto &it: follow_set) {
                            symbols.insert(it);
                        }
                        if (std::find(to_export.begin(), to_export.end(), std::make_pair(new_rule, symbols)) ==
                            to_export.end())
                            to_export.emplace_back(new_rule, symbols);

                        const auto dot_ptr = std::find(new_rule.getResultingSymbols().begin(),
                                                       new_rule.getResultingSymbols().end(), dot_symbol);

                        if ((dot_ptr + 1) != new_rule.getResultingSymbols().end()) {
                            // is het symbool na de . het einde van de productie?
                            Symbol to_evaluate = *(dot_ptr + 1);
                            if (to_evaluate.getType() == kVariable &&

                                std::find(new_variables.begin(), new_variables.end(), to_evaluate) ==
                                new_variables.end()) {
                                new_variables.emplace_back(to_evaluate);
                            }
                        }

                    } else {

                        GrammarRule new_rule = production;
                        std::vector<Symbol> new_production{dot_symbol};
//                        for (const auto &copy_result: new_rule.getResultingSymbols())
//                            new_production.emplace_back(copy_result);

                        new_rule.setResultingSymbols(new_production);

                        tmp.clear();

                        auto new_rules = grammar_rules;
                        for (auto &it: new_rules) {
                            std::vector<Symbol> resulting_symbols_copy;
                            if (it.getResultingSymbols() == production.getResultingSymbols()) {
                                resulting_symbols_copy = it.getResultingSymbols();
                                long count = std::count(resulting_symbols_copy.begin(), resulting_symbols_copy.end(),
                                                        *(dot_ptr_start + 1));
                                for (int i{1}; i < count; ++i) {
                                    resulting_symbols_copy.erase(
                                            std::find(resulting_symbols_copy.begin(), resulting_symbols_copy.end(),
                                                      *(dot_ptr_start + 1)));
                                }
                                it.setResultingSymbols(resulting_symbols_copy);
                            }
                        }

                        follow_set = previous_follow_set;

                        if (previous_follow_set.empty())
                            follow_set = std::set<Symbol>{Symbol("$", kTerminal)};

                        if (std::find(to_export.begin(), to_export.end(), std::make_pair(new_rule, symbols)) ==
                            to_export.end())
                            to_export.emplace_back(new_rule, symbols);

                        const auto dot_ptr = std::find(new_rule.getResultingSymbols().begin(),
                                                       new_rule.getResultingSymbols().end(), dot_symbol);

                        if ((dot_ptr + 1) != new_rule.getResultingSymbols().end()) {
                            // is het symbool na de . het einde van de productie?
                            Symbol to_evaluate = *(dot_ptr + 1);
                            if (to_evaluate.getType() == kVariable &&

                                std::find(new_variables.begin(), new_variables.end(), to_evaluate) ==
                                new_variables.end()) {
                                new_variables.emplace_back(to_evaluate);
                            }
                        }

                    }

                }
            }
        }
    }
    return to_export;
}

Symbols
LR1Parser::first(const Symbol &symbol, std::vector<Symbol> &checked, const std::vector<GrammarRule> &grammar_rules) {

    Symbols to_export;
    auto epsilon = Symbol("", kTerminal);

    if (symbol.getType() == kTerminal) {
        to_export.insert(symbol);
        return to_export;
    } else {
        for (const auto &production: grammar_rules) {
            if (production.getProductionSymbol() == symbol) {
                if (production.getResultingSymbols().size() == 1 &&
                    production.getResultingSymbols()[0] == epsilon) {
                    to_export.insert(epsilon);
                }
                int index{-1};

                Symbols results;
                do {
                    ++index;
                    results.clear();

                    if (production.getResultingSymbols()[index].getType() == kTerminal) {
                        to_export.insert(production.getResultingSymbols()[index]);
                    } else {
                        checked.emplace_back(symbol);
                        if (std::find(checked.begin(), checked.end(), production.getResultingSymbols()[index]) ==
                            checked.end()) {
                            results = this->first(production.getResultingSymbols()[index], checked, grammar_rules);

                            for (const auto &result: results) {
                                if ((index > 0 || !(result == epsilon)) &&

                                    std::find(to_export.begin(), to_export.end(), result) == to_export.end()) {
                                    to_export.insert(result);
                                }
                            }
                        }
                    }
                } while (index < production.getResultingSymbols().size() - 1 &&

                         (!results.empty() &&
                          std::find(results.begin(), results.end(), epsilon) != results.end()));
                if (index == production.getResultingSymbols().size() - 1 &&

                    production.getResultingSymbols().back().getType() ==
                    kVariable) {
                    to_export.insert(epsilon);
                }
            }
        }
    }

    return to_export;
}

LR1Parser::LR1Parser(const Symbol &startingSymbol) : starting_symbol_(startingSymbol) {}

Symbols
LR1Parser::follow(const Symbol &symbol, std::vector<Symbol> &checked, const std::vector<GrammarRule> &grammar_rules) {

    Symbols to_export;

    auto epsilon = Symbol("", kTerminal);

    if (this->starting_symbol_ == symbol) {
        to_export.insert(Symbol("$", kTerminal));
        return to_export;
    }

    for (const auto &production: grammar_rules) {
        auto found_vector = std::find(production.getResultingSymbols().begin(), production.getResultingSymbols().end(),
                                      symbol);

        int index{0};

        Symbols results;

        bool used_first_set{false};

        do {
            ++index;
            used_first_set = false;
            if (found_vector != production.getResultingSymbols().end()) {

                if (found_vector + index != production.getResultingSymbols().end()) {
                    // pBq
                    std::vector<Symbol> tmp;
                    results = this->first(*(found_vector + index), tmp, grammar_rules);
                    if (std::find(results.begin(), results.end(), epsilon) == results.end()) {
                        // pBq with epsilon not in first(q) add first(q)
                        used_first_set = true;
                        for (const auto &first_q_element: results) {
                            to_export.insert(first_q_element);
                        }
                    } else {
                        used_first_set = true;
                        for (const auto &first_q_element: results) {
                            if (std::find(to_export.begin(), to_export.end(), first_q_element) == to_export.end() &&

                                !(first_q_element == epsilon))
                                to_export.insert(first_q_element);
                        }
                        if (std::find(checked.begin(), checked.end(), production.getProductionSymbol()) ==
                            checked.end()) {
                            checked.emplace_back(production.getProductionSymbol());
                            auto follow_parent = this->follow(production.getProductionSymbol(), checked, grammar_rules);

                            for (const auto &follow_parent_element: follow_parent) {
                                to_export.insert(follow_parent_element);
                            }
                        }
                        // pBq with epsilon in first(q) add first(q)-epsilon U follow(A)
                    }
                } else {
                    // pB add follow(A)
                    if (std::find(checked.begin(), checked.end(), production.getProductionSymbol()) == checked.end()) {
                        checked.emplace_back(production.getProductionSymbol());
                        auto follow_parent = this->follow(production.getProductionSymbol(), checked, grammar_rules);

                        for (const auto &follow_parent_element: follow_parent) {
                            if (std::find(to_export.begin(), to_export.end(), follow_parent_element) == to_export.end())
                                to_export.insert(follow_parent_element);
                        }
                    }
                }
            }
        } while ((found_vector + index) != production.getResultingSymbols().end()
                 &&
                 (used_first_set &&
                  (std::find(results.begin(), results.end(), epsilon) != results.end())));

    }
    return to_export;
}


LR1Parser::LR1Parser() {}

const Lexer LALR1Parser::getLexer() const {
    return lexer_;
}

void LALR1Parser::setLexer(const Lexer &lexer) {
    lexer_ = lexer;
}

const LR1Parser &LALR1Parser::getLr1Parser() const {
    return lr1_parser_;
}

void LALR1Parser::setLr1Parser(const LR1Parser &lr1Parser) {
    lr1_parser_ = lr1Parser;
}

const std::vector<GrammarRule> &LALR1Parser::getGrammarRules() const {
    return grammar_rules_;
}

void LALR1Parser::setGrammarRules(const std::vector<GrammarRule> &grammarRules) {
    grammar_rules_ = grammarRules;
}

LALR1Parser::LALR1Parser(const Lexer &lexer, const std::vector<GrammarRule> &grammarRules,
                         const Symbol &starting_symbol)
        : lexer_(lexer), lr1_parser_(LR1Parser(starting_symbol)),
          grammar_rules_(grammarRules), starting_symbol_(starting_symbol) {}

ParsingTable LALR1Parser::generate_parsing_table() {

    States lalr1_states = this->grammar_sets();
    ParsingTable table(lalr1_states.size(), this->getSymbolList().size());
    Symbol dot_symbol = Symbol(".", kTerminal);

    unsigned long state_id{0};
    for (const auto &current_state: lalr1_states) {

        for (const auto&[current_rule, current_symbol_set]: current_state) {


            const auto dot_ptr_start = std::find(current_rule.getResultingSymbols().begin(),
                                                 current_rule.getResultingSymbols().end(), dot_symbol);

            if (dot_ptr_start + 1 != current_rule.getResultingSymbols().end() ||
                !(*(dot_ptr_start) == current_rule.getResultingSymbols().back())) {

                int goto_state = this->getGotoStateIndex(lalr1_states, current_rule);

                goto_state = this->getGotoStateIndex(lalr1_states, current_rule);
                int symbol_index = getSymbolIndex(*(dot_ptr_start + 1));

                switch ((dot_ptr_start + 1)->getType()) {
                    case kTerminal:
                        table.setAction(state_id, symbol_index, Action(kShift, goto_state));
                        break;
                    case kVariable:
                        table.setAction(state_id, symbol_index, Action(kState, goto_state));
                        break;
                }
            } else {
                if (current_rule.getProductionSymbol() == this->getStartingSymbol()) {
                    int symbol_index = getSymbolIndex(Symbol("$", kTerminal));
                    table.setAction(state_id, symbol_index, Action(kAccept, 0));
                } else {
                    for (const auto &set_symbol: current_symbol_set) {
                        int symbol_index = getSymbolIndex(set_symbol);
                        table.setAction(state_id, symbol_index, Action(kReduce, this->getReduceRuleIndex(current_rule,
                                                                                                         this->getGrammarRules())));
                    }
                }
            }

        }
        ++state_id;
    }
    return table;
}

States LALR1Parser::grammar_sets() {

    std::vector<std::pair<GrammarRule, int>> goto_set;
    States lr1_states = lr1_parser_.grammar_sets(grammar_rules_, starting_symbol_, goto_set);
    States lalr1_states;

    std::set<std::vector<GrammarRule>> rule_set;
    std::map<GrammarRule, std::set<Symbol>> lalr1_states_map;

    for (const auto &current_state: lr1_states) {
        // create lalr1 production list
        std::vector<GrammarRule> tmp;
        std::vector<GrammarRule> current_state_rules;
        for (const auto &[rule, set]: current_state) {
            tmp.emplace_back(rule);

            // map symbol set to grammar rule
            current_state_rules.emplace_back(rule);
            for (const auto &symbol: set) {
                lalr1_states_map[rule].insert(symbol);
            }
        }
        rule_set.insert(tmp);
    }

    // add symbol set to rule in a state &&
    // add to lalr1_states
    for (const auto &current_state: rule_set) {
        std::vector<std::pair<GrammarRule, std::set<Symbol>>> rule_list;
        for (const auto &current_state_rule: current_state) {
            rule_list.emplace_back(std::make_pair(current_state_rule, lalr1_states_map[current_state_rule]));
        }
        lalr1_states.emplace_back(rule_list);
    }
    int index{0};
    for (const auto &it: lalr1_states) {
        if (it.size() == lr1_states[0].size() and it[0] == lr1_states[0][0]) {
            break;
        }
        ++index;
    }
    this->first_state_index_ = index;
    return lalr1_states;
}

void LALR1Parser::test() {
    std::vector<GrammarRule> rules;

    rules.emplace_back(Symbol("S\'", kVariable), std::vector<Symbol>{Symbol("S", kVariable)});
    rules.emplace_back(Symbol("S", kVariable), std::vector<Symbol>{Symbol("C", kVariable), Symbol("C", kVariable)});
    rules.emplace_back(Symbol("C", kVariable), std::vector<Symbol>{Symbol("c", kTerminal), Symbol("C", kVariable)});
    rules.emplace_back(Symbol("C", kVariable), std::vector<Symbol>{Symbol("d", kTerminal)});

    auto rule = GrammarRule(Symbol("S\'", kVariable),
                            std::vector<Symbol>{Symbol(".", kTerminal), Symbol("S", kVariable)});

    this->setGrammarRules(rules);

    std::set<Symbol> symbol_list{Symbol("S\'", kVariable), Symbol("S", kVariable), Symbol("C", kVariable),
                                 Symbol("c", kTerminal), Symbol("d", kTerminal)};

    LR1Parser a;
    this->setLr1Parser(a);

    this->setSymbolList(symbol_list);

//    this->grammar_sets();

    this->generate_parsing_table();

}

LALR1Parser::LALR1Parser() {}

const std::set<Symbol> &LALR1Parser::getSymbolList() const {
    return symbol_list_;
}

void LALR1Parser::setSymbolList(const std::set<Symbol> &symbolList) {
    symbol_list_ = symbolList;
    symbol_list_.insert(Symbol("$", kTerminal));
}

const Symbol &LALR1Parser::getStartingSymbol() const {
    return starting_symbol_;
}

void LALR1Parser::setStartingSymbol(const Symbol &startingSymbol) {
    starting_symbol_ = startingSymbol;
}

int LALR1Parser::getSymbolIndex(const Symbol &symbol) {
    unsigned int index{0};
    for (const auto &it: this->getSymbolList()) {
        if (it == symbol) {
            return index;
        }
        ++index;
    }
    return index;
}

int LALR1Parser::getGotoStateIndex(const States &states, const GrammarRule &rule) {
    unsigned long index{0};
    bool found{false};
    for (const auto &state: states) {

        if (rule.swap_dot_symbol() == state[0].first) {
            found = true;
            break;
        }

        ++index;
    }
    if (!found) {
        std::cout << "ERROR!!!!!!\n";
    }
    return index;
}

int LALR1Parser::getReduceRuleIndex(const GrammarRule &current_rule, const std::vector<GrammarRule> &grammar_rules) {
    unsigned long index{0};

    GrammarRule rule_without_dot = current_rule;
    auto new_resulting_symbols = rule_without_dot.getResultingSymbols();
    new_resulting_symbols.pop_back();
    rule_without_dot.setResultingSymbols(new_resulting_symbols);

    if (rule_without_dot.getResultingSymbols().size() == 0)
        rule_without_dot = GrammarRule(rule_without_dot.getProductionSymbol(),
                                       std::vector<Symbol>{Symbol("", kTerminal)});

    for (const auto &rule: grammar_rules) {
        if (rule == rule_without_dot) {
            break;
        }
        ++index;
    }
    return index;
}

const AbstractSyntaxTree LALR1Parser::parse() {

    typedef std::pair<int, Symbol> StackItem;

    std::stack<StackItem> stack;
    ParsingTable table = this->generate_parsing_table();

    std::map<int, std::vector<char>> input = this->getLexer().getReadInput();
    unsigned int checked_lines{0};
    bool correct_end_line{false};

    stack.push(StackItem(this->getFirstStateIndex(), this->getStartingSymbol()));

    std::map<int, std::vector<std::string>> input_per_line;

    AbstractSyntaxTree tree;

    try {
        do {
            Symbol current_symbol("tmp", kTerminal);
            if (input[checked_lines].empty() and checked_lines < input.size()) {
                correct_end_line = false;
                ++checked_lines;
//                    current_symbol = Symbol("$", kTerminal);
            }
            if (checked_lines == input.size()) {
                checked_lines--;
                current_symbol = Symbol("$", kTerminal);
                // current symbol is $
            }
            if (input[checked_lines].size() == 1 and input[checked_lines][0] == '\n' and !correct_end_line) {
                current_symbol = Symbol("$", kTerminal);
                correct_end_line = true;
            }

            if (!(current_symbol == Symbol("$", kTerminal))) {
                for (const auto &it: this->getSymbolList()) {
                    if (it.getValue() == std::string(1, input[checked_lines][0]) or
                        (it.getValue() == "\\n" and input[checked_lines][0] == '\n')) {
                        current_symbol = it;
                        break;
                    }
                }
            }
            if (current_symbol == Symbol("tmp", kTerminal))
                throw UnfoundSymbolException(current_symbol.getValue(), checked_lines);

            int get_index = this->getSymbolIndex(current_symbol);
            Action current_action = table[stack.top().first][get_index];

            if (current_action.first == kError and input[checked_lines].size() == 1 and
                input[checked_lines][0] == '\n') {
                if (current_symbol.getValue() == "$") {
                    get_index = this->getSymbolIndex(Symbol("\\n", kTerminal));
                } else {
                    get_index = this->getSymbolIndex(Symbol("$", kTerminal));
                }
                current_action = table[stack.top().first][get_index];

            }

            if (current_action.first == kShift) {

                stack.push(StackItem(current_action.second, current_symbol));
                input[checked_lines].erase(input[checked_lines].begin());

            } else if (current_action.first == kReduce) {

                std::pair<int, Symbol> top_stack = stack.top();
                GrammarRule reduce_rule = grammar_rules_[current_action.second];

                reduce_rule = grammar_rules_[current_action.second];

                if (reduce_rule.getResultingSymbols() != std::vector<Symbol>{Symbol("", kTerminal)})
                    for (size_t i{0}; i < reduce_rule.getResultingSymbols().size(); ++i) {
                        stack.pop();
                    }

                top_stack = stack.top();

                stack.push(StackItem(
                        table[top_stack.first][this->getSymbolIndex(
                                reduce_rule.getProductionSymbol())].second,
                        reduce_rule.getProductionSymbol()));

            } else if (current_action.first == kAccept) {
                if (checked_lines == input.size() - 1 and input[checked_lines].empty()) {
//                    std::cout << "ACCEPT!\n";
                    break;
                } else {
                    throw std::exception();
                }
            } else {
                throw SymbolException(current_symbol.getValue(), checked_lines);
            }


            if ((current_symbol.getValue() != "\'" and input_per_line[checked_lines].size() == 0) or
                (current_symbol.getValue() != "\'" and current_symbol.getValue() != "$" and
                 current_symbol.getValue() != "\\n" and
                 input_per_line[checked_lines].size() != 0 and
                 current_symbol.getValue() == stack.top().second.getValue()))

                input_per_line[checked_lines].emplace_back(current_symbol.getValue());

        } while (!input.empty());
    } catch (SymbolException &e) {
        std::cout << e.what();
        throw e;
    }

    std::map<std::string, std::vector<transition>> transition_map;
    std::map<std::string, std::pair<bool, bool>> state_properties; // first = starting, second = ending

    for (size_t i = {0}; i < input_per_line.size() / 2; i++) {
        auto iterator = input_per_line[(i * 2) + 1].begin();
        std::vector<std::string> current_symbols;
        std::vector<std::string> next_symbols;
        std::vector<bool> directions;

        while (iterator != std::find(input_per_line[(i * 2) + 1].begin(), input_per_line[(i * 2) + 1].end(), "{")) {
            current_symbols.emplace_back(*iterator);
            iterator++;
        }
        for (size_t j{0}; j < current_symbols.size(); ++j) {
            iterator++;
            next_symbols.emplace_back(*iterator);
            iterator++;
            directions.emplace_back((*iterator == ">"));
            iterator = std::find(iterator, input_per_line[(i * 2) + 1].end(), "{");
        }

        transition tmp(current_symbols, next_symbols, directions, input_per_line[(i * 2) + 1].back());
        bool filled{false};
        if (state_properties.find(*input_per_line[i * 2].begin()) != state_properties.end()) {
            if (state_properties[*input_per_line[i * 2].begin()] != std::make_pair(
                    input_per_line[i * 2].end() !=
                    std::find(input_per_line[i * 2].begin(), input_per_line[i * 2].end(), "*"),
                    input_per_line[i * 2].end() !=
                    std::find(input_per_line[i * 2].begin(), input_per_line[i * 2].end(), "~"))) {
                filled = true;
            }
        }
        if (!filled)
            state_properties[*input_per_line[i * 2].begin()] = std::make_pair(
                    input_per_line[i * 2].end() !=
                    std::find(input_per_line[i * 2].begin(), input_per_line[i * 2].end(), "*"),
                    input_per_line[i * 2].end() !=
                    std::find(input_per_line[i * 2].begin(), input_per_line[i * 2].end(), "~"));
        transition_map[*input_per_line[i * 2].begin()].emplace_back(tmp);


        // when there is already a Q3 or *  use the first one.

    }

    for (const auto&[state_name, transition_list]: transition_map) {
        ASTNode tmp(transition_list, state_name, state_properties[state_name].second,
                    state_properties[state_name].first);
        tree.addNode(tmp);
    }

    return tree;
}

LALR1Parser::LALR1Parser(const std::string &production_rule_file_path) {
    std::ifstream file(production_rule_file_path);
    std::string input;

    if (!file.is_open()) {
        throw FileNotFoundException();
    }
    std::vector<GrammarRule> rules;
    std::set<std::string> variable_names;
    std::set<Symbol> symbol_collection;

    while (std::getline(file, input)) {

        std::vector<std::string> split_line = this->splitIntoProductionRules(input);
        bool first_seen{false};

        std::vector<std::string> buffer;

        std::string production_symbol_name;


        for (const auto &element_of_slit_line: split_line) {
            if (!first_seen) {

                production_symbol_name = element_of_slit_line;
                variable_names.insert(production_symbol_name);
                first_seen = true;

            } else if (element_of_slit_line == "|") {

                // all chars since last | or since -> are resulting symbols of production split_line[0].
                std::vector<Symbol> resulting_symbols;
                for (const auto &symbol: buffer) {
                    if (symbol == "e") {
                        resulting_symbols.emplace_back(Symbol("", kTerminal));
                    } else if (!symbol.empty())
                        resulting_symbols.emplace_back(Symbol(symbol, kTerminal));
                }
                buffer.clear();
                Symbol production_symbol(production_symbol_name, kVariable);
                GrammarRule new_rule(production_symbol, resulting_symbols);
                rules.emplace_back(new_rule);

            } else if (element_of_slit_line == "->") {

                first_seen = true;
                buffer.clear();

            } else {
                buffer.emplace_back(element_of_slit_line);
                // append current symbol to the seen list.
            }

        }
        if (!buffer.empty()) {
            std::vector<Symbol> resulting_symbols;
            for (const auto &symbol: buffer) {
                if (symbol == "e") {
                    resulting_symbols.emplace_back(Symbol("", kTerminal));
                } else if (!symbol.empty())
                    resulting_symbols.emplace_back(Symbol(symbol, kTerminal));
            }
            buffer.clear();
            Symbol production_symbol(production_symbol_name, kVariable);
            GrammarRule new_rule(production_symbol, resulting_symbols);
            rules.emplace_back(new_rule);
        }

        input.clear();
    }

    // change symbols that are variables to type = kVariable and not kTerminal
    for (auto &rule: rules) {
        std::vector<Symbol> old_rules = rule.getResultingSymbols();
        for (auto &symbol: old_rules) {
            if (variable_names.find(symbol.getValue()) != variable_names.end()) {
                symbol.setType(kVariable);
            }
            symbol_collection.insert(symbol);
        }
        symbol_collection.insert(rule.getProductionSymbol());
        rule.setResultingSymbols(old_rules);
    }

    this->setStartingSymbol(rules[0].getProductionSymbol());
    this->setLr1Parser(LR1Parser(this->getStartingSymbol()));
    this->setSymbolList(symbol_collection);
    this->setGrammarRules(rules);

}

std::vector<std::string> LALR1Parser::splitIntoProductionRules(const std::string &input) {

    std::vector<std::string> to_export;
    std::string buffer;

    for (auto current_letter = input.begin(); current_letter != input.end(); current_letter++) {
        if (*current_letter == ' ' and current_letter != input.begin() and current_letter != input.end()) {

            auto backup = current_letter;
            backup++;
            if (buffer == "\'" and *backup == '\'') {
                to_export.emplace_back(buffer);
                to_export.emplace_back(" ");
                buffer.clear();
            } else {
                if (!buffer.empty())
                    to_export.emplace_back(buffer);
                buffer.clear();
            }

        } else if (*current_letter == ' ') {
            if (!buffer.empty())
                to_export.emplace_back(buffer);
            buffer.clear();
        } else if (*current_letter == '|') {
            if (!buffer.empty())
                to_export.emplace_back(buffer);
            buffer.clear();
            buffer.push_back(*current_letter);
            to_export.emplace_back(buffer);
            buffer.clear();
        } else {
            buffer.push_back(*current_letter);
        }
    }

    if (!buffer.empty()) {
        to_export.push_back(buffer);
    }
    return to_export;

}

int LALR1Parser::getFirstStateIndex() {
    return this->first_state_index_;
}
