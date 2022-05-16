//
// Created by dries on 12/4/21.
//

#ifndef TO_GROUP_MB_TURINGMACHINE_H
#define TO_GROUP_MB_TURINGMACHINE_H

#include "tmState.h"
#include "../abstractSyntaxTree/AbstractSyntaxTree.h"

// Deze class stelt een turing machine voor
class TuringMachine {
    std::vector<tmState::ptr> states; // Alle states in TM

public:
    tmState::ptr starting; // Bevat de starting state

    TuringMachine() = default;
    TuringMachine(AbstractSyntaxTree &AST);

    // Creeer een tape voor de gegeven input
    static std::vector<std::string> constructTape(const std::string& in);

    // Voeg een state to aan de TM
    void addState(const tmState::ptr& nState);
    // Stel de start state in
    // TODO: eventueel toevoegen aan addState (hangt mogelijk van tree structure af)
    void setStarting(const tmState::ptr& sState);
    // Simuleer het TM over een bepaalde input
    bool simulate(const std::string& input) const;
    // Simuleer een enkele stap van een TM
    bool simStep(tmState::ptr& curState, std::vector<std::string>& tape, int& tapeHead) const;
    // Getter voor de exporter
    const std::vector<tmState::ptr> &getStates() const;
    // Getter voor de exporter
    const tmState::ptr &getStarting() const;

};


#endif //TO_GROUP_MB_TURINGMACHINE_H
