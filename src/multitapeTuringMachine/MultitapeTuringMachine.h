#ifndef TO_GROUP_MB_MULTITAPETURINGMACHINE_H
#define TO_GROUP_MB_MULTITAPETURINGMACHINE_H
#include "mtmState.h"
#include "../abstractSyntaxTree/AbstractSyntaxTree.h"

using namespace std;
// Deze class stelt een multitape turing machine voor
class MultitapeTuringMachine {
    vector<mtmState::ptr> states; // Alle states in TM
    mtmState::ptr starting; // Bevat de starting state
    int k; // Aantal tapes

public:
    MultitapeTuringMachine(int k);

    MultitapeTuringMachine(const AbstractSyntaxTree& ast);
    // Creeer een tape voor de gegeven input
    vector<vector<string>> constructTape(const std::string& in);

    // Voeg een state to aan de TM
    void addState(const mtmState::ptr& nState);
    // Stel de start state in
    // TODO: eventueel toevoegen aan addState (hangt mogelijk van tree structure af)
    void setStarting(const mtmState::ptr& sState);
    // Simuleer het TM over een bepaalde input
    bool simulate(const std::string& input);
    // Simuleer het TM over een bepaalde input
    vector<vector<string>> simulate_return(const std::string& input);
    // Simuleer een enkele stap van een TM
    bool simStep(mtmState::ptr& curState, std::vector<vector<string>>& tapes, vector<int>& tapeHeads) const;
    // Getter voor de exporter
    const mtmState::ptr &getStarting() const;
    // Getter voor de exporter
    const vector<mtmState::ptr> &getStates() const;
};
#endif //TO_GROUP_MB_MULTITAPETURINGMACHINE_H
