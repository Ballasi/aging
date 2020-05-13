#include <ui/MainWindow.h>
#include <QApplication>
#include <logs/Logger.h>
#include <QSettings>
#include <QString>
#include <QColor>

void set_theme(QApplication* app) {
  QSettings settings("aging.conf", QSettings::NativeFormat);

  bool isDarkTheme = false;
  isDarkTheme = settings.value("isDarkTheme", isDarkTheme).toBool();

  QString color_theme;
  switch (2) {
    case 1:
      color_theme = "#8bb158";
      break;
    case 2:
      color_theme = "#8b58b1";
      break;
    case 3:
      color_theme = "#b18b58";
      break;
    case 4:
      color_theme = "#b1588b";
      break;
    case 5:
      color_theme = "#588bb1";
      break;
    case 6:
      color_theme = "#58b18b";
      break;
    default:
      color_theme = "#8bb158";
      break;
  }
  if (isDarkTheme) {
    app->setStyleSheet(
        " * {color: #E0E0E0;" // color-text
        "selection-color: #2f2f2f;" // color-text selected
        "background: #2f2f2f;" // bg
        "selection-background-color: "+ color_theme +";}"); // bg selected
  } else {
    app->setStyleSheet(
        " * {color: #202020;" // color-text
        "selection-color: #E0E0E0;" // color-text selected
        "background: #E0E0E0;" // bg
        "selection-background-color: "+ color_theme +";}"); // bg selected
  }
}

int main(int argc, char **argv) {
  QCoreApplication::setApplicationName("aging");
  QCoreApplication::setOrganizationName("aging-team");

  QApplication app(argc, argv);

  set_theme(&app);

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
