//
// Created by dries on 12/4/21.
//

#include "TuringMachine.h"

#include <iostream>

using namespace std;

TuringMachine::TuringMachine(AbstractSyntaxTree &AST) {
    std::vector<tmState::ptr> sV;
    map<string, tmState::ptr> nodes;
    for (auto &node : AST.nodes) {
        auto state = std::make_shared<tmState>(node.getName(), node.isFinal());
        sV.push_back(state);
        nodes[node.getName()] = state;
    }

    for (auto state : sV) {
        this->addState(state);
    }

    this->setStarting(sV[0]);

    for (auto &node : AST.nodes) {
        for (auto &transitions : node.getTransitions()) {
            for (int i = 0; i < transitions.nextSymbols_.size(); i++) {
                nodes[node.getName()]->addTransition(transitions.getCurrentSymbol()[i],
                                                     tmTransition(nodes[transitions.getNextState()],
                                                                  transitions.getDirection()[i],
                                                                  transitions.getNextSymbol()[i]));
            }
        }
    }
}

vector<string> TuringMachine::constructTape(const string &in) {
    vector<string> v;
    for (auto c : in) {
        string curC;
        curC += c;
        v.push_back(curC);
    }
    return v;
}

void TuringMachine::addState(const std::shared_ptr<tmState> &nState) {
    states.push_back(nState);
}

void TuringMachine::setStarting(const tmState::ptr &sState) {
    starting = sState;
}

// TODO: simulatie aanpassen om aparte stappen te kunnen
//       Zo kan de simulatie per stap worden verder gezet

/**
 * Voert een volledige simulatie van een TM uit over een gegeven input string
 * @param input:    String die door TM wordt verwerkt
 * @return  TM accepteert string of niet
 */
bool TuringMachine::simulate(const std::string &input) const {
    // Deze vector bevat de tape. Tape wordt geladen met de input string
    vector<string> tape = constructTape(input);
    bool transition = true;
    // Huidige state en locatie van de tape head
    tmState::ptr curState = starting;
    int tapeHead = 0;

    while (transition) {
        // Zolang er een transitie plaats vond
        // Niewe stap uitvoeren
        // Geen transitie uitgevoerd -> tm vastgelopen (einde simulatie)
        transition = this->simStep(curState, tape, tapeHead);
    }

    return curState->isAccepting();
}

/**
 * Een enkele stap in de simulatie van een Turing machine.
 * Returned bool geeft aan of een transitie heeft kunnen plaats vinden of niet
 * Alle argumenten worden als reference doorgegeven en zo aangepast
 * @param curState: De huidige state van de TM, wordt aangepast naar de nieuwe state
 * @param tape:     De huidige tape van de TM
 * @param tapeHead: De huidige tape head, wordt aangepast naar nieuw tape head
 * @return  bool of er een tranistie plaats vond
 */
bool TuringMachine::simStep(tmState::ptr &curState, std::vector<std::string> &tape, int &tapeHead) const {
    string cTChar = tape[tapeHead]; // Huidig gelezen tape character
    // Kijk of de transitie bestaat voor de huidige state
    bool transition = curState->transitionPresent(cTChar);
    if (transition) {
        // Haal de transitie op
        tmTransition trans = curState->getTransition(cTChar);

        tape[tapeHead] = trans.tapeChar; // Tape character aanpassen
        tapeHead += trans.dir; // Tape head verplaatsen
        if (tapeHead == tape.size() || tapeHead == -1) {
            // Aan uiteinden van tape blank tape character toevoegen
            if (tapeHead < 0)
                tapeHead = 0;
            tape.emplace(tape.begin() + tapeHead, "_");
        }
        curState = trans.desState; // Pas huidige state aan
    }

    return transition;
}
/**
 * Returned vector met states
 * @return
 */
const vector<tmState::ptr> &TuringMachine::getStates() const {
    return states;
}

const tmState::ptr &TuringMachine::getStarting() const {
    return starting;
}
