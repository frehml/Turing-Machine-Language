//
// Created by frehml on 29/12/2021.
//

#include "GUI.h"

GUI::~GUI(){
    delete middle;
    delete pointer;
}

void GUI::result(bool isAccepting){
    delete middle;
    delete pointer;
    middle = new QGraphicsTextItem;
    pointer = new QGraphicsTextItem;
    std::string t;

    if(isAccepting)
        t = "ACCEPTED!";
    else
        t = "DECLINED!";

    middle->setPos(0, 70);
    middle->setPlainText(QString::fromStdString(t));
    middle->setScale(1.5);
    addItem(middle);
}

void GUI::draw(std::vector<std::string> &tape, int &tapeHead){
    delete middle;
    delete pointer;

    middle = new QGraphicsTextItem;
    pointer = new QGraphicsTextItem;

    std::string t;
    std::string h;

    for(int i = 0; i < tape.size(); i++){
        t += tape[i];

        if(i == tapeHead)
            h += "^";

        if(i < tape.size()-1){
            t += "\t";
            h += "\t";
        }

    }

    middle->setPos(0, 0);
    pointer->setPos(0, 20);

    middle->setPlainText(QString::fromStdString(t));
    pointer->setPlainText(QString::fromStdString(h));

    pointer->setScale(1.5);
    middle->setScale(1.5);

    addItem(middle);
    addItem(pointer);
}