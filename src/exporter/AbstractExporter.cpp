//
// Created by dries on 12/4/21.
//

#include <set>
#include <fstream>
#include "AbstractExporter.h"

/**
 * This static function exports a turing machine to a dot language approved format.
 * @param tm The turing machine
 * @param path file path
 */
void AbstractExporter::toDotLang(const TuringMachine &tm, const std::string &path) {


    std::ofstream file(path);
    if (file.is_open()) {

        file << "digraph D { \n";
        std::string transitions;
        std::string starting_node_info;
        starting_node_info += tm.getStarting()->getName() + "[fontcolor=green]\n";
        for (const auto &state: tm.getStates()) {
            if (state->isAccepting()) {
                starting_node_info += state->getName() + "[shape=doublecircle]\n";
            } else {
                starting_node_info += state->getName() + "[shape=circle]\n";
            }

            for (const auto&[name, transition]: state->getTransitions()) {
                std::string direct;
                if (transition.dir == 1)
                    direct = ">";
                else
                    direct = "<";
                std::string output_string_state =
                        state->getName() + "-> " + transition.desState->getName() + "[ label=\"" + name + " " + direct +
                        "\"]\n";
                transitions += output_string_state;
            }
        }

        file << starting_node_info << transitions << "}";
        file.close();

    }
}

void AbstractExporter::toDotLang(const MultitapeTuringMachine &tm, const string &path) {
    std::ofstream file(path);
    if (file.is_open()) {

        file << "digraph D { \n";
        std::string transitions;
        std::string starting_node_info;

        starting_node_info += tm.getStarting()->getName() + "[fontcolor=green]\n";
        for (const auto &state: tm.getStates()) {
            if (state->isAccepting()) {
                starting_node_info += state->getName() + "[shape=doublecircle]\n";
            } else {
                starting_node_info += state->getName() + "[shape=circle]\n";
            }

            std::string output_string_state = state->getName() + "-> {";
            for (const auto&[name, transition]: state->getTransitions()) {
                output_string_state += transition.desState->getName() + ", ";
            }
            if (output_string_state.back() != '{') {
                output_string_state.pop_back();
                output_string_state.pop_back();
            }
            output_string_state += "}\n";
            transitions += output_string_state;
        }

        file << starting_node_info << transitions << "}";
        file.close();

    }
}
