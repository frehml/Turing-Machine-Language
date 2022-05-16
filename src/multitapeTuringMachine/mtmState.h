#ifndef TO_GROUP_MB_MTMSTATE_H
#define TO_GROUP_MB_MTMSTATE_H

#include <vector>
#include <map>
#include <string>
#include <memory>

class mtmState;

// Struct die een transitie van een Turing machine voorsteld
// Bevat een bewegings richting, nieuwe state en nieuw tape character
struct mtmTransition {
    std::shared_ptr<mtmState> desState;
    // left = -1, stay = 0, right = 1
    std::vector<int> dirs;
    std::vector<std::string> tapeChars;

    // Maak de transitie, false = links; true = rechts
    mtmTransition() = default;
    mtmTransition(const std::shared_ptr<mtmState>& dState, const std::vector<int>& dirs, const std::vector<std::string>& toTapeChars);
};


class mtmState {
    std::string name;
    std::map<std::vector<std::string>, mtmTransition> transitions;
    bool accepting;

public:
    using ptr = std::shared_ptr<mtmState>;

    // Accepting op true voor een accepting state
    mtmState(std::string name, bool accepting = false);

    // Voeg een transitie toe aan de state
    void addTransition(std::vector<std::string> readTapeChars, const mtmTransition& transition);

    bool isAccepting() const;

    // Nakijken of er een transitie bestaat op het gegeven tape character
    bool transitionPresent(const std::vector<std::string> readTapeChars);
    // Zoek de transitie op het gegeven tape character
    mtmTransition getTransition(const std::vector<std::string> readTapeChars);

    // Vergelijking gebeurt op basis van naam
    bool operator==(const mtmState& other) const;
    // Getter voor exporter
    const std::string &getName() const;
    // Getter voor exporter
    const std::map<std::vector<std::string>, mtmTransition> &getTransitions() const;
};


#endif //TO_GROUP_MB_MTMSTATE_H
