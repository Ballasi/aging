#include "ui/MainWindow.h"
#include <QApplication>

#include "logic/Universe.hpp"


int main(int argc, char **argv) {

  QApplication app(argc,argv);

  MainWindow window;
  window.show();
  return app.exec();

}
