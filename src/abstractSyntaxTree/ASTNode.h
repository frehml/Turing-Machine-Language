//
// Created by dries on 12/6/21.
//

#ifndef TO_GROUP_MB_ASTNODE_H
#define TO_GROUP_MB_ASTNODE_H

#include <set>
#include "../../json/json.hpp"

/*
 * Deze struct simuleert een ingelezen transitie voor de ASTNode klasse.
 */
struct transition {
    /*
     * Elke transition heeft een currentSymbol_, een nextSymbol_, een direction_ eigenschap en een nextState_.
     *  - currentSymbol_: string (currentSymbol_ stelt het huidige tapesymbool voor)
     *  - nextSymbol_: string (nextSymbol_ stelt het weg te schrijven tapesymbool voor)
     *  - direction_: bool (direction_ stelt de richting waar de tapehead zich zal naar verplaatsen voor)
     *  - nextState_: string (nextState_ geeft de naam van het volgende huidige automaatstate)
     */
    std::vector<std::string> currentSymbols_;
    std::vector<std::string> nextSymbols_;
    std::vector<bool> directions_; // FALSE: LINKS     TRUE: RECHTS
    std::string nextState_;

    transition(const std::vector<std::string> &currentSymbol, const std::vector<std::string> &nextSymbol,
               std::vector<bool> direction,
               const std::string &nextState);


    const std::vector<std::string> &getCurrentSymbol() const;

    void setCurrentSymbol(const std::vector<std::string> &currentSymbol);

    const std::vector<std::string> &getNextSymbol() const;

    void setNextSymbol(const std::vector<std::string> &nextSymbol);

    std::vector<bool> getDirection() const;

    void setDirection(std::vector<bool> direction);

    const std::string &getNextState() const;

    void setNextState(const std::string &nextState);

    nlohmann::json toJson() const;
};

/*
 * De ASTNode klasse stelt een knoop voor in de Abstract Syntax Tree.
 *
 */
class ASTNode {

    /*
     * Elke ASTNode heeft transities, een naam, een final eigenschap en een starting eigenschap
     *  - transities: std::set<transition> (set van legale transities vanuit deze node)
     *  - naam: string (naam van de huidige node)
     *  - final: bool (is de huidige ASTNode een finale state)
     *  - starting: bool (is de huidige ASTNode een start state)
     */
    std::vector<transition> transitions_;
    std::string state_;
    bool final_;
    bool starting_;

public:
    ASTNode(const std::vector<transition> &transitions, const std::string &name, bool final, bool starting);

    const std::vector<transition> &getTransitions() const;

    void setTransitions(const std::vector<transition> &transitions);

    const std::string &getName() const;

    void setName(const std::string &name);

    bool isFinal() const;

    void setFinal(bool final);

    bool isStarting() const;

    void setStarting(bool starting);

    void addTransition(const transition &Transition);

    nlohmann::json toJson() const;

};


#endif //TO_GROUP_MB_ASTNODE_H
