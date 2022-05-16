cmake_minimum_required(VERSION 3.17.3)
project(TO_Group_MB)

set(CMAKE_CXX_STANDARD 17)

# Set custom compilation flags:
## -Wall && -Wextra information: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
## -pthread information: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html#Preprocessor-Options
## -openmp API specification: https://www.openmp.org/wp-content/uploads/OpenMP-API-Specification-5.0.pdf

## -O[0,1,2,3] for release information: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
### REMOVE THIS OPTION WHEN DEBUGGING!

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pthread")

# Print compiler flags

message("C++ compiler flags: ${CMAKE_CXX_FLAGS}")

#Qt5 setup

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

#set(Qt5Core_DIR "C:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Core")
#set(Qt5Widgets_DIR "C:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Widgets")
#set(Qt5Gui_DIR "C:\\Qt\\5.15.2\\mingw81_64\\lib\\cmake\\Qt5Gui")

set(Qt5Core_DIR "~/Qt/5.12.12/clang_64/lib/cmake/Qt5Core")
set(Qt5Widgets_DIR "~/Qt/5.12.12/clang_64/lib/cmake/Qt5Widgets")
set(Qt5Gui_DIR "~/Qt/5.12.12/clang_64/lib/cmake/Qt5Gui")

find_package( Qt5Core REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Gui REQUIRED )

set(SOURCE_FILES json/json.hpp
        src/parser/AbstractParser.cpp src/parser/AbstractParser.h
        src/parser/ParsingTable.cpp src/parser/ParsingTable.h
        src/turingMachine/TuringMachine.cpp src/turingMachine/TuringMachine.h
        src/exporter/AbstractExporter.cpp src/exporter/AbstractExporter.h
        src/lexer/Lexer.cpp src/lexer/Lexer.h
        src/abstractSyntaxTree/AbstractSyntaxTree.cpp src/abstractSyntaxTree/AbstractSyntaxTree.h
        src/abstractSyntaxTree/ASTNode.cpp src/abstractSyntaxTree/ASTNode.h
        src/CFG/GrammarRule.cpp src/CFG/GrammarRule.h src/CFG/Symbol.cpp src/CFG/Symbol.h
        src/turingMachine/tmState.cpp src/turingMachine/tmState.h
        src/multitapeTuringMachine/mtmState.h src/multitapeTuringMachine/mtmState.cpp
        src/multitapeTuringMachine/MultitapeTuringMachine.h src/multitapeTuringMachine/MultitapeTuringMachine.cpp)

set(GUI
        src/GUI/Window.cpp src/GUI/Window.h
        src/GUI/GUI.cpp src/GUI/GUI.h)

set(TEST_FILES tests/test.cpp
        tests/catch.cpp tests/catch.hpp
        tests/testUtils.cpp tests/testUtils.h
        src/turingMachine/TuringMachine.h src/turingMachine/TuringMachine.cpp
        )

add_executable(TO_Group_MB main.cpp ${SOURCE_FILES} ${GUI})

add_executable(TO_Group_MB_TESTS tests/test.cpp ${TEST_FILES} ${SOURCE_FILES})

qt5_use_modules(TO_Group_MB Core Widgets Gui)
