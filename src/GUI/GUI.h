//
// Created by frehml on 29/12/2021.
//

#ifndef TO_GROUP_MB_GUI_H
#define TO_GROUP_MB_GUI_H

#include "../turingMachine/TuringMachine.h"

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QtWidgets>
#include <QTimer>
#include <fstream>
#include <QtCore>
#include <vector>
#include <iostream>

class QAction;
class QActionGroup;

class GUI : public QGraphicsScene  {
Q_OBJECT
public:
    ~GUI();
    explicit GUI(QObject *parent = 0) : QGraphicsScene(parent) {};

    void draw(std::vector<std::string> &tape, int &tapeHead);
    void result(bool accepting);
private:
    QGraphicsTextItem * pointer = new QGraphicsTextItem;
    QGraphicsTextItem * middle = new QGraphicsTextItem;
};


#endif //TO_GROUP_MB_GUI_H
