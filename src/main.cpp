
//#include "AlsaExamples.h"

#include <QApplication>

#include "MainWindow.h"
#include "MainEventFilter.h"

int main(int argc, char *argv[]) {
  /*  AlsaExamples examples(argc, argv);
    examples.run(); */

    QApplication app(argc, argv);

    MainEventFilter mainEventFilter;
    app.installEventFilter(&mainEventFilter);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}

