#ifndef TO_GROUP_MB_TMSTATE_H
#define TO_GROUP_MB_TMSTATE_H

#include <vector>
#include <map>
#include <string>
#include <memory>

// Richtingen voor moves van een Turing machine
enum direction {l = -1, r = 1};

class tmState;


// Struct die een transitie van een Turing machine voorsteld
// Bevat een bewegings richting, nieuwe state en nieuw tape character
struct tmTransition {
    std::shared_ptr<tmState> desState;
    direction dir;
    std::string tapeChar;

    // Maak de transitie, false = links; true = rechts
    tmTransition() = default;
    tmTransition(const std::shared_ptr<tmState>& dState, bool dir, const std::string& tChar);
};


// Struct die een transitie van een Turing machine voorstelt
// De state heeft een naam en bevat de transities uit de state
// Een state kan accepting zijn
class tmState {
    std::string name;
    std::map<std::string, tmTransition> transitions;
    bool accepting;

public:
    using ptr = std::shared_ptr<tmState>;

    // Accepting op true voor een accepting state
    tmState(std::string name, bool accepting = false);

    // Voeg een transitie toe aan de state
    void addTransition(std::string tChar, const tmTransition& transition);

    bool isAccepting() const;

    // Nakijken of er een transitie bestaat op het gegeven tape character
    bool transitionPresent(const std::string& tChar);
    // Zoek de transitie op het gegeven tape character
    tmTransition getTransition(const std::string& tChar);

    // Vergelijking gebeurt op basis van naam
    bool operator==(const tmState& other) const;

    // Getter voor exporter
    const std::map<std::string, tmTransition> &getTransitions() const;

    // Getter voor exporter
    const std::string &getName() const;
};


#endif //TO_GROUP_MB_TMSTATE_H
