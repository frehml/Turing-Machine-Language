//
// Created by dries on 12/4/21.
//

#ifndef TO_GROUP_MB_ABSTRACTEXPORTER_H
#define TO_GROUP_MB_ABSTRACTEXPORTER_H

#include "../turingMachine/TuringMachine.h"
#include "../turingMachine/tmState.h"
#include "../multitapeTuringMachine/MultitapeTuringMachine.h"
/**
 * the AbstractExporter class will export a given turing machine to a valid dot graph file.
 * official documentation for the dot language: https://www.graphviz.org/documentation/.
 */
class AbstractExporter {
public:
    static void toDotLang(const TuringMachine& tm, const std::string& path);
    static void toDotLang(const MultitapeTuringMachine& tm, const std::string& path);
};


#endif //TO_GROUP_MB_ABSTRACTEXPORTER_H
