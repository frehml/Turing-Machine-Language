//
// Created by frehml on 29/12/2021.
//

#ifndef TO_GROUP_MB_WINDOW_H
#define TO_GROUP_MB_WINDOW_H

#include "../turingMachine/TuringMachine.h"
#include "../parser/AbstractParser.h"
#include "GUI.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QTimer>
#include <fstream>
#include <QtCore>

class Window : public QMainWindow {
Q_OBJECT
public slots:
    void compiled();
    void inputted();
    void nexted();
    void example1();
    void example2();

public:
    TuringMachine turingmachine;
    GUI* gui;

    Window(QWidget *parent);

    bool input = false;
    bool compile = false;
private:
    std::vector<std::string> tape;
    int tapeHead;
    tmState::ptr curState;

    void createOverlay();

    QTextEdit *textArea;
    QPushButton *nextButton;
    QLineEdit* lineText;
    QPushButton *confirmButton;
    QPushButton *compileButton;
    QPushButton *example1button;
    QPushButton *example2button;
    QLabel *label;
};


#endif //TO_GROUP_MB_WINDOW_H
