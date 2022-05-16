#include "mtmState.h"

mtmTransition::mtmTransition(const std::shared_ptr<mtmState>& dState, const std::vector<int>& dirs, const std::vector<std::string>& toTapeChars) {
    this->desState = dState;
    this->tapeChars = toTapeChars;
    this->dirs = dirs;
}

void mtmState::addTransition(std::vector<std::string> readTapeChars, const mtmTransition &transition) {
    transitions[readTapeChars] = transition;
}

bool mtmState::transitionPresent(const std::vector<std::string> readTapeChars) {
    return !(transitions.find(readTapeChars) == transitions.end());
}

mtmTransition mtmState::getTransition(const std::vector<std::string> readTapeChars) {
    return transitions[readTapeChars];
}

mtmState::mtmState(std::string name, bool accepting /* false */) : name(name), accepting(accepting) {}


bool mtmState::isAccepting() const { return this->accepting; }

bool mtmState::operator==(const mtmState &other) const {
    return this->name == other.name;
}

const std::string &mtmState::getName() const {
    return name;
}

const std::map<std::vector<std::string>, mtmTransition> &mtmState::getTransitions() const {
    return transitions;
}
