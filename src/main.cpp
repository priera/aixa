
//#include "AlsaExamples.h"

#include <QCoreApplication>


int main(int argc, char *argv[]) {
  /*  AlsaExamples examples(argc, argv);
    examples.run(); */

  QCoreApplication app(argc, argv);

  int ret = app.exec();

  return ret;
}

