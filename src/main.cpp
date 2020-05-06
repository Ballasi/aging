#include <ui/MainWindow.h>
#include <QApplication>
#include <logs/Logger.h>

int main(int argc, char **argv) {
  QCoreApplication::setApplicationName("aging");
  QCoreApplication::setOrganizationName("aging-team");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  Logger *log = new Logger(&window, "logs.txt", false);
  log->write("Build ABI : " + QSysInfo::buildAbi());
  log->write("Kernel Type : " + QSysInfo::kernelType());
  log->write("Kernel Version : " + QSysInfo::kernelVersion());
  log->write("Product Type : " + QSysInfo::prettyProductName());
  log->write("Product Version : " + QSysInfo::productVersion());
  QString qtver("Qt Version : ");
  qtver += QT_VERSION_STR;
  log->write(qtver);

  return app.exec();
}
