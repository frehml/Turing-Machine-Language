//
// Created by dries on 12/6/21.
//

#include <iostream>
#include "ASTNode.h"

const std::vector<transition> &ASTNode::getTransitions() const {
    return transitions_;
}

void ASTNode::setTransitions(const std::vector<transition> &transitions) {
    transitions_ = transitions;
}

const std::string &ASTNode::getName() const {
    return state_;
}

void ASTNode::setName(const std::string &name) {
    state_ = name;
}

bool ASTNode::isFinal() const {
    return final_;
}

void ASTNode::setFinal(bool final) {
    final_ = final;
}

bool ASTNode::isStarting() const {
    return starting_;
}

void ASTNode::setStarting(bool starting) {
    starting_ = starting;
}

ASTNode::ASTNode(const std::vector<transition> &transitions, const std::string &name, bool final, bool starting)
        : transitions_(transitions), state_(name), final_(final), starting_(starting) {}

void ASTNode::addTransition(const transition &Transition) {
    transitions_.emplace_back(Transition);
}

nlohmann::json ASTNode::toJson() const {
    nlohmann::json to_export;

    to_export["type"] = "ASTNode";

    to_export["Name"] = this->getName();
    to_export["Final_State"] = this->isFinal();
    to_export["Starting_State"] = this->isStarting();

    for (const auto &it: this->getTransitions()) {
        to_export["Productions"].push_back(it.toJson());
    }

    return to_export;
}

const std::vector<std::string> &transition::getCurrentSymbol() const {
    return currentSymbols_;
}

void transition::setCurrentSymbol(const std::vector<std::string> &currentSymbol) {
    transition::currentSymbols_ = currentSymbol;
}

const std::vector<std::string> &transition::getNextSymbol() const {
    return nextSymbols_;
}

void transition::setNextSymbol(const std::vector<std::string> &nextSymbol) {
    transition::nextSymbols_ = nextSymbol;
}

std::vector<bool> transition::getDirection() const {
    return directions_;
}

void transition::setDirection(std::vector<bool> direction) {
    transition::directions_ = direction;
}

const std::string &transition::getNextState() const {
    return nextState_;
}

void transition::setNextState(const std::string &nextState) {
    transition::nextState_ = nextState;
}

transition::transition(const std::vector<std::string> &currentSymbol, const std::vector<std::string> &nextSymbol,
                       std::vector<bool> direction, const std::string &nextState) : currentSymbols_(currentSymbol),
                                                                                    nextSymbols_(nextSymbol),
                                                                                    directions_(direction),
                                                                                    nextState_(nextState) {}

nlohmann::json transition::toJson() const {
    nlohmann::json to_export;

    to_export["currentSymbol_"] = this->getCurrentSymbol();
    to_export["nextSymbol_"] = this->getNextSymbol();
    to_export["direction_"] = this->getDirection();
    to_export["nextState_"] = this->getNextState();

    return to_export;
}
