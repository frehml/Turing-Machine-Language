//
// Created by frehml on 29/12/2021.
//

#include "Window.h"
#include <iostream>

Window::Window(QWidget *parent) : QMainWindow(parent) {
    gui = new GUI(parent);

    QGraphicsView *view = new QGraphicsView(this);
    setCentralWidget(view);

    resize(1280, 720);

    createOverlay();
}

void Window::createOverlay(){
    textArea = new QTextEdit(this);
    QPalette p;
    p.setColor(QPalette::Base, QColor(41,41,41));
    p.setColor(QPalette::Text, QColor(220, 220, 220));
    textArea->setPalette(p);

    compileButton = new QPushButton(this);
    compileButton->setText("Compile");
    connect(compileButton, &QPushButton::released, this, &Window::compiled);

    example1button = new QPushButton(this);
    example1button->setText("Example 1");
    connect(example1button, &QPushButton::released, this, &Window::example1);

    example2button = new QPushButton(this);
    example2button->setText("Example 2");
    connect(example2button, &QPushButton::released, this, &Window::example2);

    nextButton = new QPushButton(this);
    nextButton->setText("Next Step");
    connect(nextButton, &QPushButton::released, this, &Window::nexted);

    lineText = new QLineEdit(this);
    lineText->setPlaceholderText("input string");

    confirmButton = new QPushButton(this);
    confirmButton->setText("Input");
    connect(confirmButton, &QPushButton::released, this, &Window::inputted);

    QGridLayout *grid = new QGridLayout;

    grid->addWidget(nextButton, 0, 0);
    grid->addWidget(lineText, 0, 1, 1, 3);
    grid->addWidget(confirmButton, 0, 4);

    grid->addWidget(new QGraphicsView(gui), 1, 0, 2, 5);

    grid->addWidget(compileButton, 3, 4);
    grid->addWidget(example1button, 4, 4);
    grid->addWidget(example2button, 5, 4);
    grid->addWidget(textArea, 3, 0, 4, 4);

    centralWidget()->setLayout(grid);
}

void Window::example1() {
    textArea->setText("A*\n"
                      "\t'0' {X >} B\n"
                      "A\n"
                      "\t'Y' {Y >} D\n"
                      "B\n"
                      "\t'0' {0 >} B\n"
                      "B\n"
                      "\t'1' {Y <} C\n"
                      "B\n"
                      "\t'Y' {Y >} B\n"
                      "C\n"
                      "\t'0' {0 <} C\n"
                      "C\n"
                      "\t'X' {X >} A\n"
                      "C\n"
                      "\t'Y' {Y <} C\n"
                      "D\n"
                      "\t'Y' {Y >} D\n"
                      "D\n"
                      "\t'_' {B >} E\n"
                      "E~\n"
                      "\t'A' {E >} E");
}

void Window::example2() {
    textArea->setText("A*\n"
                      "\t'K' 'L' {C >} {D >} T\n"
                      "T\n"
                      "\t'L' 'Q' {Z <} {G >} B\n"
                      "B~\n"
                      "\t'E' 'G' {U >} {D <} A");
}

void Window::inputted(){
    if(compile && lineText->displayText().toStdString() != ""){
        tape = turingmachine.constructTape(lineText->displayText().toStdString());
        tapeHead = 0;
        curState = turingmachine.starting;
        gui->draw(tape, tapeHead);
        input = true;
        compile = false;
    }
}

void Window::compiled(){
    if(textArea->toPlainText().toStdString() != ""){
        LALR1Parser lalr1Parser("src/productionRules/productions-01.txt");

        std::fstream inputFile;
        inputFile.open ("tests/testData/turingmachine.input", std::ofstream::out | std::ofstream::trunc);
        inputFile << textArea->toPlainText().toStdString();
        inputFile.close();

        Lexer lexer("tests/testData/turingmachine.input");
        lalr1Parser.setLexer(lexer);
        auto result = lalr1Parser.parse();
        TuringMachine TM(result);

        turingmachine = TM;

        textArea->setText("");
        compile = true;
    }
}

void Window::nexted(){
    if(input){
        if(turingmachine.simStep(curState, tape, tapeHead))
            gui->draw(tape, tapeHead);
        else{
            gui->result(curState->isAccepting());
            input = false;
            compile = true;
        }
    }
}