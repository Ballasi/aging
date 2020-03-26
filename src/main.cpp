#include <ui/MainWindow.h>
#include <QApplication>

int main(int argc, char **argv) {
  QCoreApplication::setApplicationName("aging");
  QCoreApplication::setOrganizationName("aging-team");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();
  return app.exec();
}
