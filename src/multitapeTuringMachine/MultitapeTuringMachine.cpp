#include "MultitapeTuringMachine.h"

using namespace std;

MultitapeTuringMachine::MultitapeTuringMachine(int k) {
    this->k = k;
}

vector<vector<string>> MultitapeTuringMachine::constructTape(const string &in) {
    // amount of blanks to initialise on each tape
    int tape_length = 10;
    vector<vector<string>> tapes;
    // initialise k tapes
    for (int i = 0; i < k; i++) {
        vector<string> v;
        tapes.push_back(v);
    }
    // initialise input tapes with blanks "_" (makes it easy to design TMs)
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < tape_length; j++) {
            tapes[i].push_back("_");
        }
    }
    // initialise input on the first tape
    int i = 0;
    for (auto c: in) {
        string curC;
        curC += c;
        if (i >= tape_length) {
            tapes[0].push_back(curC);
        } else {
            tapes[0].at(i) = curC;
        }
        i++;
    }
    return tapes;
}

void MultitapeTuringMachine::addState(const std::shared_ptr<mtmState> &nState) {
    states.push_back(nState);
}

void MultitapeTuringMachine::setStarting(const mtmState::ptr &sState) {
    starting = sState;
}

// TODO: simulatie aanpassen om aparte stappen te kunnen
//       Zo kan de simulatie per stap worden verder gezet

/**
 * Voert een volledige simulatie van een TM uit over een gegeven input string
 * @param input:    String die door TM wordt verwerkt
 * @return  TM accepteert string of niet
 */
bool MultitapeTuringMachine::simulate(const std::string &input) {
    // Deze vector bevat de tape. Tape wordt geladen met de input string
    vector<vector<string>> tapes = constructTape(input);
    bool transition = true;
    // Huidige state en locatie van de tape head
    mtmState::ptr curState = starting;
    // Each tape has its own tape head, we start with the head at position 1 (not 0 since that is '#')
    vector<int> tapeHeads;
    for (int i = 0; i < this->k; i++) {
        tapeHeads.push_back(0);
    }
    while (transition) {
        // Zolang er een transitie plaats vond
        // Niewe stap uitvoeren
        // Geen transitie uitgevoerd -> tm vastgelopen (einde simulatie)
        transition = this->simStep(curState, tapes, tapeHeads);
    }
    return curState->isAccepting();
}

/**
 * Voert een volledige simulatie van een TM uit over een gegeven input string
 * @param input:    String die door TM wordt verwerkt
 * @return  tapes van de TM met het resultaat van de uitvoering
 */
vector<vector<string>> MultitapeTuringMachine::simulate_return(const std::string &input) {
    // Deze vector bevat de tape. Tape wordt geladen met de input string
    vector<vector<string>> tapes = constructTape(input);
    bool transition = true;
    // Huidige state en locatie van de tape head
    mtmState::ptr curState = starting;
    // Each tape has its own tape head, we start with the head at position 1 (not 0 since that is '#')
    vector<int> tapeHeads;
    for (int i = 0; i < this->k; i++) {
        tapeHeads.push_back(0);
    }
    while (transition) {
        // Zolang er een transitie plaats vond
        // Niewe stap uitvoeren
        // Geen transitie uitgevoerd -> tm vastgelopen (einde simulatie)
        transition = this->simStep(curState, tapes, tapeHeads);
    }
    return tapes;
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
bool
MultitapeTuringMachine::simStep(mtmState::ptr &curState, vector<vector<string>> &tapes, vector<int> &tapeHeads) const {
    // Currently read tape characters
    std::vector<std::string> readTapeChars;
    for (int i = 0; i < this->k; i++) {
        string cTChar = tapes[i].at(tapeHeads[i]);
        readTapeChars.push_back(cTChar);
    }
    // Kijk of de transitie bestaat voor de huidige state
    bool transition = curState->transitionPresent(readTapeChars);
    if (transition) {
        // Haal de transitie op
        mtmTransition trans = curState->getTransition(readTapeChars);
        for (int i = 0; i < this->k; i++) {
            // Tape characters aanpassen
            string newChar = trans.tapeChars[i];
            tapes[i].at(tapeHeads[i]) = newChar;
            // Tape heads verplaatsen
            tapeHeads[i] += trans.dirs[i];
            if (tapeHeads[i] == tapes[i].size() || tapeHeads[i] == -1) {
                // Aan uiteinden van tape blank tape character toevoegen (_)
                if (tapeHeads[i] < 0)
                    tapeHeads[i] = 0;
                tapes[i].emplace(tapes[i].begin() + tapeHeads[i], "_");
            }
        }
        curState = trans.desState; // Pas huidige state aan
    }
    return transition;
}

MultitapeTuringMachine::MultitapeTuringMachine(const AbstractSyntaxTree &AST) {
    map<string, mtmState::ptr> nodes;
    this->k = AST.getNodes()[0].getTransitions()[0].getDirection().size();
    std::vector<mtmState::ptr> sV;

    for (auto &node: AST.getNodes()) {
        auto state = std::make_shared<mtmState>(node.getName(), node.isFinal());
        sV.push_back(state);
        nodes[node.getName()] = state;
    }

    for (auto state: sV) {
        this->addState(state);
    }
    this->setStarting(sV[0]);

    for (auto &node: AST.getNodes()) {
        for (auto &transitions: node.getTransitions()) {

            std::vector<int> dirs;
            for (const auto it: transitions.getDirection()) {
                dirs.emplace_back(-1 * (it == false) + 1 * (it == true));
            }

            nodes[node.getName()]->addTransition(transitions.getCurrentSymbol(),
                                                 mtmTransition(nodes[transitions.getNextState()],
                                                               dirs,
                                                               transitions.getNextSymbol()));


        }
    }
}

const vector<mtmState::ptr> &MultitapeTuringMachine::getStates() const {
    return states;
}

const mtmState::ptr &MultitapeTuringMachine::getStarting() const {
    return starting;
}