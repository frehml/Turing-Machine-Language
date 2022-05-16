#include "tmState.h"

tmTransition::tmTransition(const std::shared_ptr<tmState>& dState, bool dir, const std::string& tChar)
        : desState(dState), tapeChar(tChar) {
    if (dir)
        this->dir = r;
    else
        this->dir = l;
}

tmState::tmState(std::string name, bool accepting /* false */) : name(name), accepting(accepting) {}

void tmState::addTransition(std::string tChar, const tmTransition &transition) {
    transitions[tChar] = transition;
}

bool tmState::isAccepting() const { return this->accepting; }

bool tmState::transitionPresent(const std::string &tChar) {
    return !(transitions.find(tChar) == transitions.end());
}

tmTransition tmState::getTransition(const std::string &tChar) {
    return transitions[tChar];
}

bool tmState::operator==(const tmState &other) const {
    return this->name == other.name;
}

const std::map<std::string, tmTransition> &tmState::getTransitions() const {
    return transitions;
}

const std::string &tmState::getName() const {
    return name;
}
