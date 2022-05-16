//
// Created by dries on 12/17/21.
//

#include "../src/parser/AbstractParser.h"
#include "catch.hpp"
#include "../src/turingMachine/TuringMachine.h"
#include "../src/multitapeTuringMachine/MultitapeTuringMachine.h"
#include "../src/exporter/AbstractExporter.h"
#include "testUtils.h"

#define CATCH_CONFIG_MAIN

TEST_CASE("LALR_TESTS") {

    /*
     * Edit configuration to the correct working directory if this test fails.
     */
    SECTION("INITIALIZATION TEST") {
        LALR1Parser lalr1Parser("tests/testData/test_productions_01.cfgrules");
    }SECTION("NON EXISTING FILE TEST") {
        REQUIRE_THROWS([&]() { LALR1Parser lalr1Parser("this/file/should/not.exist"); }());
    }SECTION("TABLE CONSTRUCTION TEST") {
        LALR1Parser lalr1Parser("tests/testData/test_productions_01.cfgrules");

        ParsingTable table = lalr1Parser.generate_parsing_table();
        REQUIRE(table.getTable().size() == 7);
    }SECTION("PARSE TEST") {
        LALR1Parser lalr1Parser("src/productionRules/productions-01.txt");
        Lexer lexer("tests/testData/test_input02.input");
        lalr1Parser.setLexer(lexer);
        auto result = lalr1Parser.parse();
    }

}

TEST_CASE("DOT_LANGUAGE_EXPORTER_TEST") {

    TuringMachine TM;

    std::vector<tmState::ptr> sV;
    sV.push_back(std::make_shared<tmState>("q0"));
    sV.push_back(std::make_shared<tmState>("q1"));
    sV.push_back(std::make_shared<tmState>("q2"));
    sV.push_back(std::make_shared<tmState>("q3"));
    sV.push_back(std::make_shared<tmState>("q4", true));

    for (auto state: sV) {
        TM.addState(state);
    }
    TM.setStarting(sV[0]);

    sV[0]->addTransition("0", tmTransition(sV[1], true, "X"));
    sV[1]->addTransition("0", tmTransition(sV[1], true, "0"));
    sV[2]->addTransition("0", tmTransition(sV[2], false, "0"));

    sV[1]->addTransition("1", tmTransition(sV[2], false, "Y"));

    sV[2]->addTransition("X", tmTransition(sV[0], true, "X"));

    sV[0]->addTransition("Y", tmTransition(sV[3], true, "Y"));
    sV[1]->addTransition("Y", tmTransition(sV[1], true, "Y"));
    sV[2]->addTransition("Y", tmTransition(sV[2], false, "Y"));
    sV[3]->addTransition("Y", tmTransition(sV[3], true, "Y"));

    sV[3]->addTransition("", tmTransition(sV[4], true, "B"));

    std::string file_path = "tests/testOutput/TM.txt";

    AbstractExporter::toDotLang(TM, file_path);

    SECTION("FILE COMPARE") {
        REQUIRE(FileCompare("tests/testData/test_dot_tm.txt", file_path));
        REQUIRE(FileCompare(file_path, "tests/testData/test_dot_tm.txt"));
    }


}

TEST_CASE("TM_TESTS") {
    TuringMachine TM;

    std::vector<tmState::ptr> sV;
    sV.push_back(std::make_shared<tmState>("q0"));
    sV.push_back(std::make_shared<tmState>("q1"));
    sV.push_back(std::make_shared<tmState>("q2"));
    sV.push_back(std::make_shared<tmState>("q3"));
    sV.push_back(std::make_shared<tmState>("q4", true));

    for (auto state: sV) {
        TM.addState(state);
    }
    TM.setStarting(sV[0]);

    sV[0]->addTransition("0", tmTransition(sV[1], true, "X"));
    sV[1]->addTransition("0", tmTransition(sV[1], true, "0"));
    sV[2]->addTransition("0", tmTransition(sV[2], false, "0"));

    sV[1]->addTransition("1", tmTransition(sV[2], false, "Y"));

    sV[2]->addTransition("X", tmTransition(sV[0], true, "X"));

    sV[0]->addTransition("Y", tmTransition(sV[3], true, "Y"));
    sV[1]->addTransition("Y", tmTransition(sV[1], true, "Y"));
    sV[2]->addTransition("Y", tmTransition(sV[2], false, "Y"));
    sV[3]->addTransition("Y", tmTransition(sV[3], true, "Y"));

    sV[3]->addTransition("_", tmTransition(sV[4], true, "B"));

    SECTION("SIMULATION TEST 1") {
        REQUIRE(TM.simulate("0011") == true);
    }SECTION("SIMULATION TEST 2") {
        REQUIRE(TM.simulate("00011") == false);
    }
}

TEST_CASE("AST_TO_TM_TEST") {
    LALR1Parser lalr1Parser("src/productionRules/productions-01.txt");
    Lexer lexer("tests/testData/test_input02.input");
    lalr1Parser.setLexer(lexer);
    auto result = lalr1Parser.parse();
    TuringMachine TM(result);

    SECTION("SIMULATION TEST 1") {
        REQUIRE(TM.simulate("0") == false);
    }SECTION("SIMULATION TEST 2") {
        REQUIRE(TM.simulate("Y_") == true);
    }
}

TEST_CASE("AST_TO_MTM_TEST") {
    LALR1Parser lalr1Parser("src/productionRules/productions-01.txt");
    Lexer lexer("tests/testData/test_input02.input");
    lalr1Parser.setLexer(lexer);
    auto result = lalr1Parser.parse();
    MultitapeTuringMachine mtm(result);
    std::string file_path = "tests/testOutput/TM2.txt";
    AbstractExporter::toDotLang(mtm, file_path);
    // desState == null sometimes.
    SECTION("SIMULATION TEST 1") {
        REQUIRE(mtm.simulate("0011") == true);
    }SECTION("SIMULATION TEST 2") {
        REQUIRE(mtm.simulate("00011") == false);
    } SECTION("EXPORTER TEST 1") {
        REQUIRE(FileCompare("tests/testData/test_dot_tm2.txt", file_path));
        REQUIRE(FileCompare(file_path, "tests/testData/test_dot_tm2.txt"));
    }
}

TEST_CASE("MTM_2tape_TESTS") {
    MultitapeTuringMachine MTM = MultitapeTuringMachine(2);

    std::vector<mtmState::ptr> sV;
    // 2-tape Turing Machine: a binary palindrome tester
    sV.push_back(std::make_shared<mtmState>("qCopy"));
    sV.push_back(std::make_shared<mtmState>("qReturn"));
    sV.push_back(std::make_shared<mtmState>("qTest"));
    sV.push_back(std::make_shared<mtmState>("qAccept", true));

    for (auto state: sV) {
        MTM.addState(state);
    }
    MTM.setStarting(sV[0]);
    // Use "_" as a blank symbol
    std::vector<std::string> readTapeChars{"0", "_"};
    std::vector<int> dirs{1, 1};
    std::vector<std::string> toTapeChars{"0", "0"};
    // COPY STATE
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[0], dirs, toTapeChars));
    readTapeChars = {"1", "_"}, dirs = {1, 1}, toTapeChars = {"1", "1"};
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[0], dirs, toTapeChars));
    readTapeChars = {"_", "_"}, dirs = {0, -1}, toTapeChars = {"_", "_"};
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    // RETURN STATE
    readTapeChars = {"_", "0"}, dirs = {0, -1}, toTapeChars = {"_", "0"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    readTapeChars = {"_", "1"}, dirs = {0, -1}, toTapeChars = {"_", "1"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    readTapeChars = {"_", "_"}, dirs = {-1, 1}, toTapeChars = {"_", "_"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    // TEST STATE
    readTapeChars = {"0", "0"}, dirs = {-1, 1}, toTapeChars = {"0", "0"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    readTapeChars = {"1", "1"}, dirs = {-1, 1}, toTapeChars = {"1", "1"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    readTapeChars = {"_", "_"}, dirs = {0, 0}, toTapeChars = {"_", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    SECTION("SIMULATION TEST 1") {
        REQUIRE(MTM.simulate("1001") == true);
    }SECTION("SIMULATION TEST 2") {
        REQUIRE(MTM.simulate("10011") == false);
    }SECTION("SIMULATION TEST 3") {
        REQUIRE(MTM.simulate("0010110100") == true);
    }SECTION("SIMULATION TEST 4") {
        REQUIRE(MTM.simulate("001011010011") == false);
    }
}

TEST_CASE("MTM_3tape_TESTS") {
    MultitapeTuringMachine MTM = MultitapeTuringMachine(3);

    std::vector<mtmState::ptr> sV;
    // 2-tape Turing Machine: a binary palindrome tester
    sV.push_back(std::make_shared<mtmState>("q0"));
    sV.push_back(std::make_shared<mtmState>("q1"));
    sV.push_back(std::make_shared<mtmState>("q2"));
    sV.push_back(std::make_shared<mtmState>("q3"));
    sV.push_back(std::make_shared<mtmState>("q4"));
    sV.push_back(std::make_shared<mtmState>("q5", true));

    for (auto state: sV) {
        MTM.addState(state);
    }
    MTM.setStarting(sV[0]);
    // Use "_" as a blank symbol
    std::vector<std::string> readTapeChars{"0", "_", "_"};
    std::vector<int> dirs{1, 0, 0};
    std::vector<std::string> toTapeChars{"0", "_", "_"};
    // q0
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[0], dirs, toTapeChars));
    readTapeChars = {"1", "_", "_"}, dirs = {1, 0, 0}, toTapeChars = {"1", "_", "_"};
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[0], dirs, toTapeChars));
    readTapeChars = {"+", "_", "_"}, dirs = {1, 0, 0}, toTapeChars = {"_", "_", "_"};
    sV[0]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    // q1
    readTapeChars = {"0", "_", "_"}, dirs = {1, 1, 0}, toTapeChars = {"_", "0", "_"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    readTapeChars = {"1", "_", "_"}, dirs = {1, 1, 0}, toTapeChars = {"_", "1", "_"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[1], dirs, toTapeChars));
    readTapeChars = {"_", "_", "_"}, dirs = {-1, -1, 0}, toTapeChars = {"_", "_", "_"};
    sV[1]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    // q2
    readTapeChars = {"_", "0", "_"}, dirs = {-1, 0, 0}, toTapeChars = {"_", "0", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    readTapeChars = {"_", "1", "_"}, dirs = {-1, 0, 0}, toTapeChars = {"_", "1", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[2], dirs, toTapeChars));
    readTapeChars = {"1", "0", "_"}, dirs = {0, 0, 0}, toTapeChars = {"1", "0", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"1", "1", "_"}, dirs = {0, 0, 0}, toTapeChars = {"1", "1", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "1", "_"}, dirs = {0, 0, 0}, toTapeChars = {"0", "1", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "0", "_"}, dirs = {0, 0, 0}, toTapeChars = {"0", "0", "_"};
    sV[2]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    // q3
    readTapeChars = {"1", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "0", "1"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "1", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "1", "1"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "0", "0"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"1", "1", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "1", "0"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"_", "_", "_"}, dirs = {0, 0, 0}, toTapeChars = {"_", "_", "_"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[5], dirs, toTapeChars));
    readTapeChars = {"1", "_", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "_", "1"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "_", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "_", "0"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"_", "1", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"_", "1", "1"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"_", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"_", "0", "0"};
    sV[3]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "0", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    // q4
    readTapeChars = {"0", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "0", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"0", "1", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "1", "0"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"1", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "0", "0"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"1", "1", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "1", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"_", "0", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"_", "0", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"1", "_", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"_", "1", "0"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"1", "_", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"1", "_", "0"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[4], dirs, toTapeChars));
    readTapeChars = {"0", "_", "_"}, dirs = {-1, -1, -1}, toTapeChars = {"0", "_", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[3], dirs, toTapeChars));
    readTapeChars = {"_", "_", "_"}, dirs = {0, 0, 0}, toTapeChars = {"_", "_", "1"};
    sV[4]->addTransition(readTapeChars, mtmTransition(sV[5], dirs, toTapeChars));
    SECTION("SIMULATION TEST 1") {
        vector<string> result = MTM.simulate_return("10+10")[2];
        REQUIRE(result.at(0) == "1");
        REQUIRE(result.at(1) == "0");
        REQUIRE(result.at(2) == "0");
    }SECTION("SIMULATION TEST 2") {
        vector<string> result = MTM.simulate_return("1011+10")[2];
        REQUIRE(result.at(1) == "1");
        REQUIRE(result.at(2) == "1");
        REQUIRE(result.at(3) == "0");
        REQUIRE(result.at(4) == "1");
    }
}
