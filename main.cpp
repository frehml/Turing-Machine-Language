
#include "src/parser/AbstractParser.h"
#include "src/turingMachine/TuringMachine.h"
#include "src/GUI/Window.h"

#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Window w(new QWidget);
    w.show();
    return a.exec();
}
